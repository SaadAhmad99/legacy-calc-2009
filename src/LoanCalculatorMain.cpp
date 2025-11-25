#include "LoanCalculator.h"
#include <iostream>
#include <fstream>
#include <sstream>


void loadConfigDefaults(const std::string &filename, LoanCalculator &calc)
{
std::ifstream file(filename);
if(!file.is_open()) return;


std::string line;
while(std::getline(file, line))
{
size_t pos = line.find('=');
if(pos == std::string::npos) continue;


std::string key = line.substr(0, pos);
long double value = std::stold(line.substr(pos + 1));


if(key == "DEFAULT_INTEREST") calc.setInterest(value);
if(key == "DEFAULT_PERIOD") calc.setPeriodTotal(static_cast<int>(value));
if(key == "DEFAULT_OPENING_FEE") calc.setOpeningFee(value);
}
}


int main()
{
try
{
LoanCalculator calculator;


// Load default values from config file
loadConfigDefaults("config.txt", calculator);


long double amount, interest, payment;
int period;


std::cout << "Enter Loan Amount: ";
std::cin >> amount;
calculator.setAmount(amount);


std::cout << "Enter Interest Rate (%): ";
std::cin >> interest;
calculator.setInterest(interest);


std::cout << "Enter Loan Period (months): ";
std::cin >> period;
calculator.setPeriodTotal(period);


payment = calculator.calculatePayment();
calculator.setPayment(payment);


std::cout << "===== Loan Summary =====";
std::cout << calculator.toString();
std::cout << "Monthly Payment: " << payment << " ";
std::cout << "Effective Interest Rate: " << calculator.calculateEffectiveInterestRate() << "% ";
}
catch(const std::exception &e)
{
std::cout << "Error: " << e.what() << std::endl;
}

return 0;
}