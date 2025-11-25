#include "LoanCalculator.h"
#include <gtest/gtest.h>
#include <limits>

// Normal EMI calculation
TEST(LoanCalculatorTest, NormalEMICalculation) {
    LoanCalculator calc;
    calc.setAmount(100000.0L);
    calc.setInterest(6.0L); // yearly %
    calc.setPeriodTotal(120); // 10 years
    long double emi = calc.calculatePayment();
    // expected EMI approximate: ~1110.21 (depends on formula) — check with tolerance
    EXPECT_NEAR((double)emi, 1110.21, 0.5);
}

// Invalid input handling
TEST(LoanCalculatorTest, InvalidInputsThrow) {
    LoanCalculator calc;
    EXPECT_THROW(calc.setAmount(-50.0L), std::invalid_argument);
    EXPECT_THROW(calc.setInterest(-1.0L), std::invalid_argument);
    EXPECT_THROW(calc.setPayment(-10.0L), std::invalid_argument);
    EXPECT_THROW(calc.setPeriodTotal(0), std::invalid_argument);
}

// Large tenure calculations without overflow
TEST(LoanCalculatorTest, LargeTenureNoOverflow) {
    LoanCalculator calc;
    calc.setAmount(1e12L);            // large principal
    calc.setInterest(8.0L);           // reasonable interest
    calc.setPeriodTotal(100 * 12);    // 100 years -> 1200 months
    // should compute without producing inf/nan and EMI should be finite and > 0
    long double emi = calc.calculatePayment();
    ASSERT_FALSE(std::isnan((double)emi));
    ASSERT_FALSE(std::isinf((double)emi));
    EXPECT_GT((double)emi, 0.0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
