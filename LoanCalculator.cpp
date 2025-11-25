
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <string>

#include "LoanCalculator.h"

using namespace std;

LoanCalculator::LoanCalculator() :
  amountSet_(false),
  initialPayment_(0.0),
  interestSet_(false),
  paymentSet_(false),
  periodTotalSet_(false),
  periodElapsedSet_(false),
  openingFee_(0.0),
  openingPercent_(0.0)
{
}

//
// The actual calculation methods
//

/**
 * Loan balance after n payments have been made:
 *   B_n = A*(1+i)^n - (P/i)*((1+i)^n - 1)
 */
long double LoanCalculator::calculateLoanBalance()
{
  if(!amountSet_ || !interestSet_ || !periodElapsedSet_ || !paymentSet_)
  {
    throw invalid_argument("Must set loan amount, interest, elapsed period and payment for this calculation");
  }
  // If periodic interest is zero, balance is simple linear:
  if (interestPeriodic_ == 0.0L)
  {
    long double totalPaid = payment_ * (long double)periodElapsed_;
    long double principal = amount_ - initialPayment_;
    long double balance = principal - totalPaid;
    return balance > 0.0L ? balance : 0.0L;
  }
  long double onePlusI_pow_n = std::pow((1.0L + interestPeriodic_), (long double)periodElapsed_);
  long double balance = (amount_ * onePlusI_pow_n) -
                        (payment_ / interestPeriodic_) * (onePlusI_pow_n - 1.0L);
  return balance;
}


/**
 * Payment amount on a loan:
 *   P = i*A / (1 - (1+i)^-N)
 */
long double LoanCalculator::calculatePayment()
{
  if(!amountSet_ || !interestSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set loan amount, interest, and total period for this calculation");
  }
  if (periodTotal_ <= 0){
    throw invalid_argument("Total period must be > 0");
  }
  long double totalAmount = amount_ - initialPayment_;
  totalAmount = totalAmount + openingFee_ + (totalAmount * (openingPercent_/100.0L));
  // Zero interest case: simple division
  if (interestPeriodic_ == 0.0L) {
    return totalAmount / (long double)periodTotal_;
  }
  long double denom = 1.0L - std::pow((1.0L + interestPeriodic_), - (long double)periodTotal_);
  if (denom == 0.0L) {
    throw invalid_argument("Denominator in payment calculation is zero (check interest and period)");
  }
  return (interestPeriodic_ * totalAmount) / denom;
}
/**
 * Number of payments on a loan:
 *   N = -log(1-i*A/P) / log(1+i)
 *      (You can use any logarithm base, as long as both logs use the same base.)
 *      Aunt Sally offers to lend you $3500 at 6% for that new home theater system you want.
 *      If you pay her back $100 a month, how long will it take?
 *      Solution:  6% per year is 0.5% per month, or 0.005. P = 100 and A = 3500. N = 38.57
 */
long double LoanCalculator::calculateNumberPayments()
{
  if(!amountSet_ || !interestSet_ || !paymentSet_){
    throw invalid_argument("Must set loan amount, interest, and payment for this calculation");
  }
  if (payment_ <= 0.0L){
    throw invalid_argument("Payment must be > 0");
  }
  long double principal = amount_ - initialPayment_;

  // If interest periodic is zero, N = principal / payment (rounded up)
  if (interestPeriodic_ == 0.0L){
    if (payment_ <= 0.0L) throw invalid_argument("Payment must be > 0 for zero-interest loan");
    return principal / payment_;
  }
  long double ratio = 1.0L - (interestPeriodic_ * principal / payment_);
  if (ratio <= 0.0L){
    throw invalid_argument("Invalid parameters: payment is too small to cover interest; ratio <= 0");
  }
  long double denom = std::log(1.0L + interestPeriodic_);
  if (denom == 0.0L){
    throw invalid_argument("Logarithm denominator is zero in number-of-payments calculation");
  }
  long double N = - std::log(ratio) / denom;
  return N;
}
/**
 * Original loan amount:
 *   A = (P/i)*(1 - (1+i)^-N)
 */
long double LoanCalculator::calculateLoanAmount()
{
  if(!paymentSet_ || !interestSet_ || !periodTotalSet_){
    throw invalid_argument("Must set payment, interest, and total period for this calculation");
  }
  if (periodTotal_ <= 0){
    throw invalid_argument("Total period must be > 0");
  }
  // Zero interest case: A = P * N
  if (interestPeriodic_ == 0.0L){
    return payment_ * (long double)periodTotal_;
  }
  long double factor = 1.0L - std::pow((1.0L + interestPeriodic_), - (long double)periodTotal_);
  return (payment_ / interestPeriodic_) * factor;
}
/**
 * Interest Rate:
 *   i = (((1 + P/A)^(1/q) - 1 )^q - 1)  NOTICE: This is an approximate not an exact solution
 *   where q = log(1+1/N) / log(2)
*/

long double LoanCalculator::calculateInterestRate()
{
  if(!amountSet_ || !paymentSet_ || !periodTotalSet_){
    throw invalid_argument("Must set amount, payment, and total period for this calculation");
  }
  long double principal = amount_ - initialPayment_;
  if (principal <= 0.0L){
    throw invalid_argument("Principal must be > 0 to compute interest rate");
  }
  if (payment_ <= 0.0L){
    throw invalid_argument("Payment must be > 0 to compute interest rate");
  }
  // q approximation
  long double q = std::log(1.0L + 1.0L/(long double)periodTotal_) / std::log(2.0L);
  long double base = 1.0L + payment_ / principal;
  if (base <= 0.0L){
    throw invalid_argument("Invalid values for interest-rate approximation (base<=0)");
  }
  long double monthlyInterest = std::pow(std::pow(base, 1.0L/q) - 1.0L, q) - 1.0L;
  // Convert to yearly percentage:
  return monthlyInterest * 12.0L * 100.0L;
}

long double LoanCalculator::calculateEffectiveInterestRate()
{
  if(!amountSet_ || !periodTotalSet_){
    throw invalid_argument("Must set amount and total period for this calculation");
  }
  long double payment = calculatePayment();
  long double totalAmount = amount_ - initialPayment_;
  if (totalAmount <= 0.0L){
    throw invalid_argument("Total amount must be > 0 for effective interest rate calculation");
  }
  long double q = std::log(1.0L + 1.0L/(long double)periodTotal_) / std::log(2.0L);
  long double base = 1.0L + payment/totalAmount;
  if (base <= 0.0L){
    throw invalid_argument("Invalid values for effective-interest calculation (base<=0)");
  }
  long double monthlyInterest = std::pow(std::pow(base, 1.0L/q) - 1.0L, q) - 1.0L;
  return monthlyInterest * 12.0L * 100.0L;
}

std::string LoanCalculator::toString()
{
  stringstream ss;
  //ss << "LoanCalculator set values:\n";
  if(amountSet_)
  {
    ss << "Initial Amount:      " << (long double)amount_ << "\n";
  }

  if(initialPayment_ != 0.0)
  {
    ss << "Initial Payment:     " << (long double)initialPayment_ << "\n";
    ss << "Actual Loan Amount:  " << (long double)(amount_ - initialPayment_) << "\n";
  }

  if(interestSet_)
  {
    ss << "Yearly Interest:     " << (long double)interest_ << "%\n";
    //ss << "Yearly Interest:     " << interest_
    //   << "\nMonthly Interest:    " << interestPeriodic_ << "\n";
  }

  if(paymentSet_)
  {
    ss << "Monthly payment:     " <<(long double) payment_ << "\n";
  }

  if(periodTotalSet_)
  {
    ss << "Loan Period:         " << periodTotal_ << " months\n";
  }

  if(periodElapsedSet_)
  {
    ss << "Elapsed Period:      " << periodElapsed_ << " months\n";
  }

  if(openingFee_ != 0.0)
  {
      ss << "Opening Fee:       " <<(long double) openingFee_ << "\n";
  }

  if(openingPercent_ != 0.0)
  {
    ss << "Opening Fee %:       " <<(long double) openingPercent_ << "% = "
       << (long double)openingPercent_/100*(amount_ - initialPayment_) << "\n";
  }
  return ss.str();
}
