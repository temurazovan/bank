#include <iostream>
#include <fstream>
#include <vector>
#include <map>

const std::vector<int> g_denominations = {100, 200, 500, 1000, 2000, 5000};

bool checkBills(std::fstream &file) {
    int bill = 0;
    bool found = false;
    while (!file.eof()) {
        file >> bill;

        for (int i: g_denominations) {
            if (bill == i) {
                found = true;
            }
        }

        if (!found) {
            return false;
        }
    }
    return true;
}

int billsSum(int sum, std::fstream &file) {
    while (!file.eof()) {
        int tempBill = 0;
        file >> tempBill;
        sum += tempBill;
    }
    return sum;
}

std::map<int, int> billsCounter(std::fstream &file) {
    std::vector<int> buffer;
    std::map<int, int> denomination{
        {100, 0},
        {200, 0},
        {500, 0},
        {1000, 0},
        {2000, 0},
        {5000, 0}
    };

    while (!file.eof()) {
        int tempBill = 0;
        file >> tempBill;
        buffer.push_back(tempBill);
    }

    for (int elements: buffer) {
        denomination[elements]++;
    }

    return denomination;
}

std::vector<int> missingBills(std::map<int, int> bank) {
    std::vector<int> vec;
    for (const auto &[denomination, number]: bank) {
        if (bank[denomination] == 0) {
            vec.push_back(denomination);
        }
    }

    return vec;
}

void printBank(std::fstream &file, int bills, std::map<int, int> bank) {
    file.seekg(0, std::ios::beg);
    bills = billsSum(bills, file);
    file.seekg(0, std::ios::beg);
    bank = billsCounter(file);
    std::cout << "Sum of bills in the ATM: " << bills << std::endl;
    for (const auto &[denomination, number]: bank) {
        std::cout << denomination << "\t" << number << std::endl;
    }
}

void depositing(std::fstream &file, int money) {
    std::map<int, int> denomination{
        {100, 0},
        {200, 0},
        {500, 0},
        {1000, 0},
        {2000, 0},
        {5000, 0}
    };

    denomination[5000] = money / 5000;
    money = money - denomination[5000] * 5000;
    denomination[2000] = money / 2000;
    money = money - denomination[2000] * 2000;
    denomination[1000] = money / 1000;
    money = money - denomination[1000] * 1000;
    denomination[500] = money / 500;
    money = money - denomination[500] * 500;
    denomination[200] = money / 200;
    money = money - denomination[200] * 200;
    denomination[100] = money / 100;

    for (const auto &number: denomination) {
        int key = number.first;
        int value = number.second;

        for (int i = 0; i < value; i++) {
            file << '\n' << key;
        }
    }
}

int main() {
    std::fstream file("../ATM.txt", std::ios::app | std::ios::in | std::ios::out);
    std::ofstream ATM_state("../file.bin");
    char input;
    int bills = 0;
    std::map<int, int> bank;

    if (!std::filesystem::exists("../ATM.txt")) {
        std::cout << "File not found";
        return 0;
    }
    if (!std::filesystem::exists("../file.bin")) {
        std::cout << "ATM data not saved \n";
        return 0;
    } else {
        printBank(file, bills, bank);
    }

    file.seekg(0, std::ios::beg);

    if (!checkBills(file)) {
        std::cout << "incorrect value" << std::endl;
    }


    while (input != 'q') {
        file.clear();
        file.seekp(0, std::ios::end);
        std::cout << "Select an action:" << std::endl;
        std::cin >> input;
        std::vector<int> missedBills;
        int money;

        if (input == '+') {
            if (!missedBills.empty()) {
                for (int element: missedBills) {
                    file << element << std::endl;
                }
            }
            std::cout << "Enter the amount to deposit:" << std::endl;
            while (std::cin >> money) {
                if (money % 100 != 0 || money <= 0 || money >= 5000000) {
                    std::cout << "Money out of range" << std::endl;
                } else {
                    break;
                }
            }
            depositing(file, money);
            printBank(file, bills, bank);
        }
        if (input == '-') {

        }
    }

    file.close();
    ATM_state.close();

    return 0;
}
