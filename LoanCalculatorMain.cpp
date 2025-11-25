#include<fstream>
#include<iomanip>
#include <stdlib.h>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include <QApplication>

#include <LoanCalcQtMainWindow.h>
#include <CmdLineParser.h>
#include <LoanCalculator.h>

using namespace std;

enum CALC_TYPE
{
  CALC_UNKNOWN=0,
  CALC_BALANCE=100,
  CALC_PAYMENT,
  CALC_NUMPAYMENTS,
  CALC_AMOUNT,
  CALC_INTEREST
};

const string ARG_CALC_BALANCE      = "-cb";
const string ARG_CALC_PAYMENT      = "-cp";
const string ARG_CALC_NUMPAYMENTS  = "-cn";
const string ARG_CALC_AMOUNT       = "-ca";
const string ARG_CALC_INTEREST     = "-ci";

const string ARG_PAYMENT           = "-p";
const string ARG_PERIOD_TOTAL      = "-N";
const string ARG_PERIOD_ELAPSED    = "-n";
const string ARG_AMOUNT            = "-a";
const string ARG_INITIAL_PAYMENT   = "-ai";
const string ARG_INTEREST          = "-i";
const string ARG_OPENFEE           = "-of";
const string ARG_OPENPERCENT       = "-op";

void loadCmdLine(CmdLineParser &clp)
{
  clp.setMainHelpText("A simple loan calculator");
  clp.setMainHelpTextEnd("With no options set, a GUI will be launched");

  // Calculation types
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_BALANCE,
         "Calculate the loan balance after making several payments, given:\n"
         "\t\t loan amount, interest, monthly payment and number of monthly payments made so far",
         false, CALC_BALANCE));
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_PAYMENT,
         "Calculate the monthly loan payment, given: loan amount, loan period, and interest",
         false, CALC_PAYMENT));
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_NUMPAYMENTS,
         "Calculate the number of payments needed to pay a loan, given: loan amount, monthly payment, interest",
         false, CALC_NUMPAYMENTS));
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_AMOUNT,
         "Calculate the initial loan amount, given: monthly payment, loan period, and interest",
         false, CALC_AMOUNT));
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_INTEREST,
         "Calculate the loan interest, given: loan amount, loan period, and monthly payment",
         false, CALC_INTEREST));
  clp.setMutExclUsageText("Calculations");

  // Different values
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_PAYMENT, "Set the monthly loan payment. Ej: 325.67"));
  clp.addCmdLineOption(new CmdLineOptionInt(   ARG_PERIOD_TOTAL, "Set the total loan period in months. Ej: 60"));
  clp.addCmdLineOption(new CmdLineOptionInt(   ARG_PERIOD_ELAPSED, "Set the elapsed period in months. Ej: 32"));
  clp.addCmdLineOption(new CmdLineOptionInt(   ARG_AMOUNT, "Set the initial amount. Ej: 19300"));
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_INITIAL_PAYMENT,
         "Set the initial payment, loan will be for (initial amount - initial payment) Ej: 1000, Default 0.0"));
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_INTEREST, "Set the yearly interest rate. Ej: 6.75"));
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_OPENFEE, "Set fees for opening the loan. Ej: 100, Default 0.0"));
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_OPENPERCENT,
         "Set fees for opening the loan, charged as a percentage. Ej: 2.75%, Default 0.0%"));
  
  clp.setMinNumberArgs(3);
}

//
// Simple Command line parser
//
CALC_TYPE parseCommandLine(int argc, char **argv, CmdLineParser &clp, LoanCalculator &calculator)
{
  CALC_TYPE ct(CALC_UNKNOWN);

  if(!clp.parseCmdLine(argc, argv))
  {
    clp.printUsage();
    return ct;
  }

  calculator.setAmount(
       ((CmdLineOptionInt*)   clp.getCmdLineOption(ARG_AMOUNT))->getValue());
  calculator.setInitialPayment(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_INITIAL_PAYMENT))->getValue());
  calculator.setInterest(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_INTEREST))->getValue());
  calculator.setPayment(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_PAYMENT))->getValue());
  calculator.setPeriodTotal(
       ((CmdLineOptionInt*)   clp.getCmdLineOption(ARG_PERIOD_TOTAL))->getValue());
  calculator.setPeriodElapsed(
       ((CmdLineOptionInt*)   clp.getCmdLineOption(ARG_PERIOD_ELAPSED))->getValue());
  calculator.setOpeningFee(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_OPENFEE))->getValue());
  calculator.setOpeningPercent(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_OPENPERCENT))->getValue());

  CmdLineOption *option(clp.getMutExclOption());
  if(option != NULL) // cant be NULL, else the parser mutExcl checking didnt work
  {
    ct = (CALC_TYPE) ((CmdLineOptionFlag*) option)->getValueKey();
  }

  return ct;
}

//
// Main program
//
int main(int argc, char **argv)
{
  LoanCalculator calculator;
  // Attempt to read defaults from config.txt (optional)
std::ifstream cfg("config.txt");
if (cfg) {
    long double cfg_amount;           // use long double variable here
    long double cfg_interest;
    int cfg_period;
    long double cfg_openfee;
    long double cfg_openpercent;

    // read values in a known order, handle missing/invalid lines
    if (cfg >> cfg_amount) {
        // set value on calculator (use setter)
        calculator.setAmount((float)cfg_amount); // cast if your setter uses float; if you changed to long double use that type
    }
    if (cfg >> cfg_interest) {
        calculator.setInterest((float)cfg_interest);
    }
    if (cfg >> cfg_period) {
        calculator.setPeriodTotal(cfg_period);
    }
    if (cfg >> cfg_openfee) {
        calculator.setOpeningFee((float)cfg_openfee);
    }
    if (cfg >> cfg_openpercent) {
        calculator.setOpeningPercent((float)cfg_openpercent);
    }
    // If desired: print a message that config loaded
}

  // If no arguments are given, then launch the GUI
  if(argc == 1)
  {
    QApplication app(argc, argv);

    LoanCalcQtMainWindow mainWindow(&calculator);
    mainWindow.show();

    return app.exec();
  }

  //
  // Parse the command line arguments
  //
 // Parse the command line arguments
CmdLineParser clp;
loadCmdLine(clp);

CALC_TYPE ct = CALC_UNKNOWN;
try {
    ct = parseCommandLine(argc, argv, clp, calculator);
} catch (const std::exception &ex) {
    std::cerr << "Error parsing command line: " << ex.what() << std::endl;
    clp.printUsage();
    return 1;
}

// -----------------------
// Input validation
// -----------------------
if (calculator.getAmount() <= 0.0L) {
    cerr << "Error: Loan amount must be > 0\n";
    return 1;
}
if (calculator.getInterest() < 0.0L) {
    cerr << "Error: Interest rate cannot be negative\n";
    return 1;
}
if (calculator.getPeriodTotal() <= 0) {
    cerr << "Error: Loan period must be > 0 months\n";
    return 1;
}
if (calculator.getPeriodElapsed() < 0 ||
    calculator.getPeriodElapsed() > calculator.getPeriodTotal()) {
    cerr << "Error: Elapsed months must be between 0 and total period\n";
    return 1;
}
if (calculator.getPayment() < 0.0L) {
    cerr << "Error: Payment cannot be negative\n";
    return 1;
}
if (calculator.getOpeningPercent() < 0.0L || calculator.getOpeningPercent() > 100.0L) {
    cerr << "Error: Opening percent must be between 0 and 100\n";
    return 1;
}
if (calculator.getOpeningFee() < 0.0L) {
    cerr << "Error: Opening fee cannot be negative\n";
    return 1;
}

// Perform calculations and print results (guarded with a try to catch runtime errors)
try {
    cout << fixed << setprecision(2);

    if (ct == CALC_BALANCE) {
        auto bal = calculator.calculateLoanBalance();
        cout << "Loan Balance = " << bal << endl;
    }
    else if (ct == CALC_PAYMENT) {
        auto payment = calculator.calculatePayment();
        cout << "Monthly Payment    = " << payment << "\n"
             << "Total amt paid     = " << (payment * calculator.getPeriodTotal()) << endl;

        if (calculator.getOpeningPercent() != 0.0L || calculator.getOpeningFee() != 0.0L) {
            auto eff = calculator.calculateEffectiveInterestRate();
            cout << "Effective Yearly Interest (with fees) = " << eff << " %" << endl;
        }
    }
    else if (ct == CALC_NUMPAYMENTS) {
        auto n = calculator.calculateNumberPayments();
        cout << "Number of payments = " << n << endl;
    }
    else if (ct == CALC_AMOUNT) {
        auto amt = calculator.calculateLoanAmount();
        cout << "Initial Loan amount = " << amt << endl;
    }
    else if (ct == CALC_INTEREST) {
        auto ir = calculator.calculateInterestRate();
        cout << "Yearly Interest Rate = " << ir << " %" << endl;
    }
    else if (ct == CALC_UNKNOWN) {
        // most likely the case that help was selected
        return 1;
    }
    else {
        cerr << "Unrecognized calculation type, exiting" << endl;
        return 1;
    }

    // print the values set on the calculator
    cout << calculator.toString() << endl;
} catch (const std::exception &e) {
    cerr << "Error executing loan calculator: " << e.what() << endl;
    return 1;
}
