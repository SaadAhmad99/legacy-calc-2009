#ifndef LOANCALCULATOR_H_INCLUDED
#define LOANCALCULATOR_H_INCLUDED

#include <string>
#include <stdexcept>

/**
 * @brief Simple loan calculator for common loan calculations (EMI, balance, etc).
 *
 * Uses yearly interest in percent (e.g. 6.5) which is converted internally
 * to a monthly periodic interest.
 */
class LoanCalculator
{
public:
    LoanCalculator();

    void setAmount(long double A);
    long double getAmount() const;

    void setInitialPayment(long double initialA);
    long double getInitialPayment() const;

    void setInterest(long double i);
    long double getInterest() const;

    void setPayment(long double P);
    long double getPayment() const;

    void setPeriodTotal(int N);
    int getPeriodTotal() const;

    void setPeriodElapsed(int n);
    int getPeriodElapsed() const;

    void setOpeningFee(long double fee);
    long double getOpeningFee() const;

    void setOpeningPercent(long double percent);
    long double getOpeningPercent() const;

    void reset();

    long double calculateLoanBalance();
    long double calculatePayment();
    long double calculateNumberPayments();
    long double calculateLoanAmount();
    long double calculateInterestRate();
    long double calculateEffectiveInterestRate();

    std::string toString();

private:
    long double amount_;
    long double initialPayment_;
    long double interest_;
    long double interestPeriodic_;
    long double payment_;
    int periodTotal_;
    int periodElapsed_;
    long double openingFee_;
    long double openingPercent_;

    // helper validator
    void validatePositive(long double value, const std::string &msg);
};

#endif // LOANCALCULATOR_H_INCLUDED
