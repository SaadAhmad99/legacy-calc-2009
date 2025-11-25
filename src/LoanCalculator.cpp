#include "LoanCalculator.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// Constructor
LoanCalculator::LoanCalculator() 
    : amount_(0), initialPayment_(0), interest_(0), interestPeriodic_(0),
      payment_(0), periodTotal_(0), periodElapsed_(0),
      openingFee_(0), openingPercent_(0) {}

// Validate positive number
void LoanCalculator::validatePositive(long double value, const std::string &msg)
{
    if(value < 0)
        throw std::invalid_argument(msg);
}

// Setters & Getters
void LoanCalculator::setAmount(long double A)
{
    validatePositive(A, "Amount cannot be negative");
    amount_ = A;
}

long double LoanCalculator::getAmount() const { return amount_; }

void LoanCalculator::setInitialPayment(long double initialA)
{
    validatePositive(initialA, "Initial payment cannot be negative");
    initialPayment_ = initialA;
}

long double LoanCalculator::getInitialPayment() const { return initialPayment_; }

void LoanCalculator::setInterest(long double i)
{
    validatePositive(i, "Interest cannot be negative");
    interest_ = i;
    interestPeriodic_ = (i / 100.0L) / 12.0L; // monthly rate
}

long double LoanCalculator::getInterest() const { return interest_; }

void LoanCalculator::setPayment(long double P)
{
    validatePositive(P, "Payment cannot be negative");
    payment_ = P;
}

long double LoanCalculator::getPayment() const { return payment_; }

void LoanCalculator::setPeriodTotal(int N)
{
    if(N <= 0) throw std::invalid_argument("Period must be positive");
    periodTotal_ = N;
}

int LoanCalculator::getPeriodTotal() const { return periodTotal_; }

void LoanCalculator::setPeriodElapsed(int n)
{
    if(n < 0) throw std::invalid_argument("Elapsed period cannot be negative");
    periodElapsed_ = n;
}

int LoanCalculator::getPeriodElapsed() const { return periodElapsed_; }

void LoanCalculator::setOpeningFee(long double fee)
{
    validatePositive(fee, "Opening fee cannot be negative");
    openingFee_ = fee;
}

long double LoanCalculator::getOpeningFee() const { return openingFee_; }

void LoanCalculator::setOpeningPercent(long double percent)
{
    validatePositive(percent, "Opening percent cannot be negative");
    openingPercent_ = percent;
}

long double LoanCalculator::getOpeningPercent() const { return openingPercent_; }

void LoanCalculator::reset()
{
    amount_ = initialPayment_ = interest_ = interestPeriodic_ = payment_ = openingFee_ = openingPercent_ = 0;
    periodTotal_ = periodElapsed_ = 0;
}

// ===================== Calculation Functions =====================

// EMI calculation
long double LoanCalculator::calculatePayment()
{
    if(amount_ <= 0 || periodTotal_ <= 0)
        throw std::invalid_argument("Missing required parameters");

    long double principal = amount_ - initialPayment_;
    principal += openingFee_ + (principal * openingPercent_ / 100.0L);

    long double monthlyRate = (interest_ > 0) ? (interest_ / 100.0L) / 12.0L : 0.0L;

    if(monthlyRate == 0.0L)
        return principal / periodTotal_;

    long double powTerm = powl(1.0L + monthlyRate, periodTotal_);
    long double emi = (monthlyRate * principal * powTerm) / (powTerm - 1.0L);

    return emi;
}

// Remaining balance after payments
long double LoanCalculator::calculateLoanBalance()
{
    long double principal = amount_ - initialPayment_;
    principal += openingFee_ + (principal * openingPercent_ / 100.0L);

    if(interestPeriodic_ == 0.0L)
        return principal - payment_ * periodElapsed_;

    return (principal * powl(1 + interestPeriodic_, periodElapsed_)) -
           (payment_ / interestPeriodic_) * (powl(1 + interestPeriodic_, periodElapsed_) - 1);
}

// Number of payments needed
long double LoanCalculator::calculateNumberPayments()
{
    if(payment_ <= 0)
        throw std::invalid_argument("Payment must be positive");

    if(interestPeriodic_ == 0.0L)
        return (amount_ - initialPayment_ + openingFee_ + (amount_ - initialPayment_) * openingPercent_ / 100.0L) / payment_;

    long double principal = amount_ - initialPayment_;
    principal += openingFee_ + (principal * openingPercent_ / 100.0L);

    return -logl(1 - (interestPeriodic_ * principal / payment_)) /
           logl(1 + interestPeriodic_);
}

// Loan amount from payment
long double LoanCalculator::calculateLoanAmount()
{
    if(interestPeriodic_ == 0.0L)
        return payment_ * periodTotal_;

    return (payment_ / interestPeriodic_) * (1 - powl(1 + interestPeriodic_, -periodTotal_));
}

long double LoanCalculator::calculateInterestRate()
{
    return interest_;
}

// Effective interest (approx.)
long double LoanCalculator::calculateEffectiveInterestRate()
{
    long double payment = calculatePayment();
    long double principal = amount_ - initialPayment_;
    principal += openingFee_ + (principal * openingPercent_ / 100.0L);
    return ((payment * periodTotal_) - principal) / principal * 100.0L;
}

// Summary
std::string LoanCalculator::toString()
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);

    ss << "Loan Summary:\n";
    ss << "Amount: " << amount_ << "\n";
    ss << "Interest: " << interest_ << "%\n";
    ss << "Payment: " << payment_ << "\n";
    ss << "Period: " << periodTotal_ << " months\n";

    return ss.str();
}
