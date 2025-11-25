#ifndef LOANCALCULATOR_H_INCLUDED
#define LOANCALCULATOR_H_INCLUDED

/*
Formulas from: http://oakroadsystems.com/math/loan.htm

Loan balance after n payments have been made:
  B_n = A*(1+i)^n - (P/i)*((1+i)^n - 1)


Payment amount on a loan:
  P = i*A / (1 - (1+i)^-N)


Number of payments on a loan: (You can use any logarithm base, as long as both logs use the same base.)
                              Aunt Sally offers to lend you $3500 at 6% for that new home theater system you want.
                              If you pay her back $100 a month, how long will it take?
                              Solution:  6% per year is 0.5% per month, or 0.005. P = 100 and A = 3500. N = 38.57
  N = -log(1-i*A/P) / log(1+i)


Original loan amount:
  A = (P/i)*(1 - (1+i)^-N)


Interest Rate:
  i = (((1 + P/A)^(1/q) - 1 )^q - 1)  NOTICE: This is an approximate not an exact solution
  where q = log(1+1/N) / log(2)


Variables:
A   	the loan amount (the principal sum) or initial investment
B_n or Bn   	(pronounced B sub n) the balance after n payments have been made. After the last payment has been made, B_N is zero.)
i   	the interest rate per period, not per year
      (For instance, if the loan payments are made monthly and the interest rate is 9%, then i = 9%/12 = 0.75% = 0.0075.)
n   	the number of time periods elapsed at any given point
N   	the total number of payments for the entire loan or investment
P   	the amount of each equal payment
*/

#include <string>

class LoanCalculator
{
public:
  LoanCalculator();
  ~LoanCalculator() {}

  //
  // Setters and Getters
  //

  /**
   * Total loan amount A
   */
  inline void setAmount(long double A) { amount_ = A; amountSet_ = true; }
  inline long double getAmount() const { return amount_; }

  /**
   * Initial down payment
   */
  inline void setInitialPayment(long double initialA)  { initialPayment_ = initialA; }
  inline long double getInitialPayment() const         { return initialPayment_; }

  /**
   * Yearly interest rate i as in 6.75
   * Internally .0675/12 will be used
   * If 6.75 is passed to setInterest()
   *    getInterest() will return 6.75
   *    getPeriodicInterest() will return .0675/12.0
   */
  void setInterest(float i) { interest_ = i; interestPeriodic_ = i/100.0/12.0; interestSet_ = true; }
  inline long double getInterest() const         { return interest_; }
  inline long double getPeriodicInterest() const { return interestPeriodic_; }

  void setPayment(long double P)        { payment_ = P; paymentSet_ = true; }
  inline long double getPayment() const { return payment_; }

  void setPeriodTotal(int N)        { periodTotal_ = N; periodTotalSet_ = true; }
  inline int getPeriodTotal() const { return periodTotal_; }

  void setPeriodElapsed(int n)         { periodElapsed_ = n; periodElapsedSet_ = true; }
  inline int getPeriodElapsed() const  { return periodElapsed_; }

  inline void setOpeningFee(long double fee) { openingFee_ = fee; }
  inline long double getOpeningFee() const   { return openingFee_; }

  inline void setOpeningPercent(long double percent) { openingPercent_ = percent; }
  inline long double getOpeningPercent() const       { return openingPercent_; }

  inline void reset() {
    amount_ = initialPayment_ = interest_ = interestPeriodic_ = payment_ = openingFee_ = openingPercent_ = 0.0;
    periodTotal_ = periodElapsed_ = 0;
    amountSet_ = interestSet_ = paymentSet_ = periodTotalSet_ = periodElapsedSet_ = false;
  }

  //
  // The actual calculation methods
  //

  long double calculateLoanBalance();
  long double calculatePayment();
  long double calculateNumberPayments();
  long double calculateLoanAmount();
  long double calculateInterestRate();
  // The effective interest rate, once fees have been applied
  long double calculateEffectiveInterestRate();

  std::string toString();

private:
  long double amount_;        // loan amount
  bool amountSet_;

  long double initialPayment_;     // initial down payment

  long double interest_;          // interest rate, something like 6.75
  long double interestPeriodic_;  // this will be .0675/12
  bool interestSet_;

  long double payment_;       // payment amount
  bool paymentSet_;

  int periodTotal_;     // total payment periods
  bool periodTotalSet_;

  int periodElapsed_;   // number of elapsed payment periods
  bool periodElapsedSet_;

  // These two are used if loans charge a fee opening fee or percentage
  long double openingFee_;
  long double openingPercent_;

};

#endif // LOANCALCULATOR_H_INCLUDED

