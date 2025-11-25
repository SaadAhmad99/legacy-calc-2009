#include "gtest/gtest.h"
#include "../src/LoanCalculator.h"

// 1. Normal EMI calculation
TEST(LoanCalculatorTest, NormalEMI) {
    LoanCalculator loan;
    loan.setAmount(100000);
    loan.setInterest(12);
    loan.setPeriodTotal(360);
    long double emi = loan.calculatePayment();
    EXPECT_NEAR(emi, 1028.61, 0.5);
}

// 2. Invalid input handling
TEST(LoanCalculatorTest, InvalidInput) {
    LoanCalculator loan;
    EXPECT_THROW(loan.setAmount(-1000), std::invalid_argument);
    EXPECT_THROW(loan.setInterest(-5), std::invalid_argument);
    EXPECT_THROW(loan.setPeriodTotal(-10), std::invalid_argument);
}

// 3. Large tenure
TEST(LoanCalculatorTest, LargeTenure) {
    LoanCalculator loan;
    loan.setAmount(500000);
    loan.setInterest(10);
    loan.setPeriodTotal(10000);
    long double emi = loan.calculatePayment();
    EXPECT_GT(emi, 0);
}
