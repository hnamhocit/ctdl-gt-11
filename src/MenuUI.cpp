#include "MenuUI.h"
#include "Validator.h"
#include "HashTable.h"
#include "FileIO.h"
#include "AdministrativeFilter.h"
#include "Plate.h"

#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <cstring>

namespace LicensePlateManager {

void MenuUI::displayMenu() {
    std::cout << "\n========================================================\n";
    std::cout << " HỆ THỐNG QUẢN LÝ BIỂN SỐ XE - NHÓM 11 (C++17)\n";
    std::cout << "========================================================\n";
    std::cout << " 1. Đăng ký biển số xe mới\n";
    std::cout << " 2. Tra cứu thông tin biển số xe\n";
    std::cout << " 3. Hiển thị danh sách mã Xã theo mã Tỉnh\n";
    std::cout << " 4. Hiển thị danh sách biển số xe theo Xã\n";
    std::cout << " 5. Nạp / Lưu dữ liệu từ tệp TXT\n";
    std::cout << " 0. Thoát chương trình\n";
    std::cout << "========================================================\n";
}

int MenuUI::getValidChoice(int minVal, int maxVal) {
    int choice;

    while (true) {
        std::cout << "Nhap lua chon cua ban (" << minVal << " - " << maxVal << "): ";

        if (std::cin >> choice) {
            if (choice >= minVal && choice <= maxVal) {
                return choice;
            }

            std::cout << "Lua chon khong nam trong pham vi menu. Vui long nhap lai!\n";
        } else {
            std::cout << "Loi: Ban vua nhap ky tu khong phai so! Vui long thu lai.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string MenuUI::inputAndValidateLicensePlate() {
    std::string rawInput;

    while (true) {
        std::cout << "Nhap bien so xe (VD: 59-X1-12345): ";
        std::cin >> rawInput;

        std::string cleanPlate = Validator::normalizePlateString(rawInput);
        ValidationResult result = Validator::validateDetailed(cleanPlate);

        if (result.isValid) {
            std::cout << result.errorMessage << "\n";
            return cleanPlate;
        }

        std::cout << result.errorMessage << " Vui long nhap lai!\n\n";
    }
}

void MenuUI::run() {
    LicensePlateHashTable hashTable;
    run(hashTable);
}

void MenuUI::run(LicensePlateHashTable& hashTable) {
    bool isRunning = true;

    while (isRunning) {
        displayMenu();

        int choice = getValidChoice(0, 5);

        switch (choice) {
            case 1: {
                std::cout << "\n--- [CHUC NANG 1] DANG KY BIEN SO MOI ---\n";

                std::string validPlate = inputAndValidateLicensePlate();

                std::string ownerName, regDate;

                std::cout << "Nhap ho ten chu xe: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, ownerName);

                std::cout << "Nhap ngay dang ky (VD: 2026-11-20): ";
                std::cin >> regDate;

                PlateNode newNode{};

                parseLicensePlate(validPlate.c_str(), newNode);

                std::strncpy(
                    newNode.ownerName,
                    ownerName.c_str(),
                    sizeof(newNode.ownerName) - 1
                );

                std::strncpy(
                    newNode.registeredDate,
                    regDate.c_str(),
                    sizeof(newNode.registeredDate) - 1
                );

                bool inserted = hashTable.insertPlate(newNode);

                if (inserted) {
                    std::cout << "Da dang ky va chen bien so "
                              << validPlate
                              << " vao Bang bam thanh cong!\n";
                } else {
                    std::cout << "Bien so "
                              << validPlate
                              << " da ton tai trong he thong!\n";
                }

                break;
            }

            case 2: {
                std::cout << "\n--- [CHUC NANG 2] TRA CUU BIEN SO XE ---\n";

                std::string searchPlate = inputAndValidateLicensePlate();

                PlateNode* node = hashTable.searchPlate(searchPlate.c_str());

                if (node != nullptr) {
                    std::cout << "TIM THAY THONG TIN BIEN SO XE:\n";
                    std::cout << "   - Bien so: " << node->fullPlate << "\n";
                    std::cout << "   - Chu xe: " << node->ownerName << "\n";
                    std::cout << "   - Ngay dang ky: " << node->registeredDate << "\n";
                } else {
                    std::cout << "Khong tim thay thong tin cho bien so xe '"
                              << searchPlate
                              << "' trong he thong!\n";
                }

                break;
            }

            case 3: {
                std::cout << "\n--- [CHUC NANG 3