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
                std::cout << "\n--- [CHUC NANG 3] DANH SACH MA XA THEO TINH ---\n";

                std::string provCode;

                std::cout << "Nhap ma Tinh (2 chu so, VD: 59): ";
                std::cin >> provCode;

                if (Validator::isValidProvinceCode(provCode.c_str())) {
                    AdministrativeFilter filter;
                    filter.displayWardsOfProvince(hashTable, provCode.c_str());
                } else {
                    std::cout << "Ma tinh khong hop le! Vui long nhap dung 2 chu so (00 - 99).\n";
                }

                break;
            }

            case 4: {
                std::cout << "\n--- [CHUC NANG 4] DANH SACH BIEN SO THEO XA ---\n";

                std::string provCode, wardCode;

                std::cout << "Nhap ma Tinh (VD: 59): ";
                std::cin >> provCode;

                std::cout << "Nhap ma Xa (VD: X1): ";
                std::cin >> wardCode;

                if (Validator::isValidProvinceCode(provCode.c_str()) &&
                    Validator::isValidWardCode(wardCode.c_str())) {

                    AdministrativeFilter filter;
                    filter.displayPlatesOfWard(
                        hashTable,
                        provCode.c_str(),
                        wardCode.c_str()
                    );
                } else {
                    std::cout << "Ma Tinh hoac ma Xa nhap vao khong dung dinh dang!\n";
                }

                break;
            }

            case 5: {
                std::cout << "\n========================================================\n";
                std::cout << "       [CHUC NANG 5] NAP / LUU DU LIEU TU TEP TXT        \n";
                std::cout << "========================================================\n";
                std::cout << " 1. Nap du lieu tu tep TXT (Load from file)\n";
                std::cout << " 2. Luu du lieu ra tep TXT (Save to file)\n";
                std::cout << " 0. Quay lai Menu chinh\n";
                std::cout << "--------------------------------------------------------\n";

                int subChoice = getValidChoice(0, 2);

                if (subChoice == 1) {
                    std::cout << "\n--- NAP DU LIEU TU FILE ---\n";

                    std::string filePath;
                    bool loadSuccess = false;

                    while (!loadSuccess) {
                        std::cout << "Nhap duong dan tep TXT (VD: data/input.txt) hoac go '0' de huy: ";
                        std::cin >> filePath;

                        if (filePath == "0") {
                            std::cout << "Da huy thao tac nap file.\n";
                            break;
                        }

                        std::ifstream checkFile(filePath);

                        if (!checkFile.is_open()) {
                            std::cout << "Loi (File Not Found): Khong tim thay tep '"
                                      << filePath
                                      << "' hoac khong the mo tep! Vui long kiem tra lai duong dan.\n\n";
                        } else {
                            checkFile.close();

                            bool loaded = loadFromFile(
                                hashTable,
                                filePath.c_str()
                            );

                            if (loaded) {
                                std::cout << "Nap du lieu tu tep '"
                                          << filePath
                                          << "' vao Bang bam thanh cong!\n";
                            } else {
                                std::cout << "Co loi xay ra trong qua trinh doc du lieu tu tep!\n";
                            }

                            loadSuccess = true;
                        }
                    }
                } else if (subChoice == 2) {
                    std::cout << "\n--- LUU DU LIEU RA FILE ---\n";

                    std::string filePath;

                    std::cout << "Nhap duong dan tep TXT de luu (VD: data/output.txt): ";
                    std::cin >> filePath;

                    bool saved = saveToFile(
                        hashTable,
                        filePath.c_str()
                    );

                    if (saved) {
                        std::cout << "Luu du lieu tu Bang bam ra tep '"
                                  << filePath
                                  << "' thanh cong!\n";
                    } else {
                        std::cout << "Co loi xay ra trong qua trinh ghi du lieu ra tep '"
                                  << filePath
                                  << "'!\n";
                    }
                }

                break;
            }

            case 0:
                std::cout << "\nCam on ban da su dung he thong. Tam biet!\n";
                isRunning = false;
                break;
        }
    }
}

}