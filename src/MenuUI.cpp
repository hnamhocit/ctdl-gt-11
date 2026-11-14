#include "MenuUI.h"
#include "Validator.h"

#include <iostream>
#include <limits>
#include <string>

namespace LicensePlateManager {

// Hiển thị menu chính gồm 6 chức năng
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

// Nhập lựa chọn và kiểm tra dữ liệu hợp lệ
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
            // Xử lý khi người dùng nhập ký tự không phải số
            std::cout << "Loi: Ban vua nhap ky tu khong phai so! Vui long thu lai.\n";

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// Nhập biển số và kiểm tra bằng module Validator
std::string MenuUI::inputAndValidateLicensePlate() {
    std::string rawInput;

    while (true) {
        std::cout << "Nhap bien so xe (VD: 59-X1-12345): ";
        std::cin >> rawInput;

        // Chuẩn hóa biển số trước khi kiểm tra
        std::string cleanPlate = Validator::normalizePlateString(rawInput);

        // Kiểm tra định dạng và nhận thông báo lỗi
        ValidationResult result = Validator::validateDetailed(cleanPlate);

        if (result.isValid) {
            std::cout << result.errorMessage << "\n";
            return cleanPlate;
        }

        std::cout << result.errorMessage << " Vui long nhap lai!\n\n";
    }
}

// Khởi chạy vòng lặp menu chính
void MenuUI::run() {
    bool isRunning = true;

    while (isRunning) {
        displayMenu();

        int choice = getValidChoice(0, 5);

        switch (choice) {
            case 1: {
                std::cout << "\n--- [CHUC NANG 1] DANG KY BIEN SO MOI ---\n";

                std::string validPlate = inputAndValidateLicensePlate();

                std::cout << "-> Da ghi nhan bien so hop le: "
                          << validPlate << "\n";
                break;
            }

            case 2:
                std::cout << "\n--- [CHUC NANG 2] TRA CUU BIEN SO XE ---\n";
                // Sẽ kết nối module HashTable ở Sprint 2
                break;

            case 3:
                std::cout << "\n--- [CHUC NANG 3] DANH SACH MA XA THEO TINH ---\n";
                // Sẽ kết nối module AdministrativeFilter ở Sprint 2
                break;

            case 4:
                std::cout << "\n--- [CHUC NANG 4] DANH SACH BIEN SO THEO XA ---\n";
                break;

            case 5:
                std::cout << "\n--- [CHUC NANG 5] NAP / LUU FILE TXT ---\n";
                break;

            case 0:
                std::cout << "\nCam on ban da su dung he thong. Tam biet!\n";
                isRunning = false;
                break;
        }
    }
}

} // namespace LicensePlateManager