#ifndef MENU_UI_H
#define MENU_UI_H

#include <string>
#include "HashTable.h"

namespace LicensePlateManager {

// Lớp quản lý giao diện menu của chương trình
class MenuUI {
public:
    // Khởi chạy menu chính với bảng băm độc lập
    static void run();

    // Khởi chạy menu chính dùng chung bảng băm đã nạp dữ liệu từ main
    static void run(LicensePlateHashTable& hashTable);

    // Hiển thị danh sách 6 chức năng
    static void displayMenu();

    // Kiểm tra dữ liệu nhập vào và giới hạn lựa chọn
    static int getValidChoice(int minVal, int maxVal);

    // Nhập biển số và kiểm tra tính hợp lệ
    static std::string inputAndValidateLicensePlate();
};

}

#endif // MENU_UI_H