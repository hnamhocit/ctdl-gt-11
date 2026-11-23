#ifndef PLATE_H
#define PLATE_H

#include <cstring>
#include <cstddef>
#include <iostream>

/** Một bản ghi biển số, ví dụ "59-X1-12345" với tỉnh "59" và xã "X1". */
struct PlateNode {
    char fullPlate[16];      // Chuỗi biển số chuẩn: "59-X1-12345\0" (tối đa 15 ký tự)
    char provinceCode[4];    // Mã tỉnh: "59\0" (2 số + '\0')
    char wardCode[4];        // Mã xã: "X1\0" (2 chữ/số + '\0')
    char serialNum[8];       // Số thứ tự xe: "12345\0" (5 số + '\0')
    char registeredDate[12]; // Ngày đăng ký: "2026-10-30\0" (yyyy-mm-dd + '\0')
    char ownerName[50];      // Tên chủ sở hữu phương tiện (UTF-8, tối đa 49 byte + '\0')
    PlateNode* next;         // Con trỏ trỏ tới Node kế tiếp trong cùng một Bucket (Separate Chaining)

    /** Tạo bản ghi rỗng với next = nullptr. O(1). */
    PlateNode() : next(nullptr) {
        fullPlate[0] = '\0';
        provinceCode[0] = '\0';
        wardCode[0] = '\0';
        serialNum[0] = '\0';
        registeredDate[0] = '\0';
        ownerName[0] = '\0';
    }

    /** Tạo bản ghi từ sáu trường đầu vào. O(L) với L là tổng độ dài các chuỗi, bộ nhớ phụ O(1). */
    PlateNode(const char* plate, const char* prov, const char* ward,
              const char* serial, const char* date, const char* owner)
        : next(nullptr) {
        copySafe(fullPlate, sizeof(fullPlate), plate);
        copySafe(provinceCode, sizeof(provinceCode), prov);
        copySafe(wardCode, sizeof(wardCode), ward);
        copySafe(serialNum, sizeof(serialNum), serial);
        copySafe(registeredDate, sizeof(registeredDate), date);
        copySafe(ownerName, sizeof(ownerName), owner);
    }

    /** Sao chép dữ liệu từ other và đặt next = nullptr. O(1). */
    PlateNode(const PlateNode& other) : next(nullptr) {
        std::memcpy(fullPlate, other.fullPlate, sizeof(fullPlate));
        std::memcpy(provinceCode, other.provinceCode, sizeof(provinceCode));
        std::memcpy(wardCode, other.wardCode, sizeof(wardCode));
        std::memcpy(serialNum, other.serialNum, sizeof(serialNum));
        std::memcpy(registeredDate, other.registeredDate, sizeof(registeredDate));
        std::memcpy(ownerName, other.ownerName, sizeof(ownerName));
    }

    /** Gán dữ liệu từ other, giữ nguyên next và trả về *this. O(1). */
    PlateNode& operator=(const PlateNode& other) {
        if (this != &other) {
            std::memcpy(fullPlate, other.fullPlate, sizeof(fullPlate));
            std::memcpy(provinceCode, other.provinceCode, sizeof(provinceCode));
            std::memcpy(wardCode, other.wardCode, sizeof(wardCode));
            std::memcpy(serialNum, other.serialNum, sizeof(serialNum));
            std::memcpy(registeredDate, other.registeredDate, sizeof(registeredDate));
            std::memcpy(ownerName, other.ownerName, sizeof(ownerName));
        }
        return *this;
    }

private:
    /** Chép src vào dest, chừa chỗ cho '\0' theo destSize. Thời gian O(L), bộ nhớ phụ O(1). */
    static void copySafe(char* dest, std::size_t destSize, const char* src) {
        if (!dest || destSize == 0) return;
        if (!src) {
            dest[0] = '\0';
            return;
        }
        std::size_t len = std::strlen(src);
        if (len >= destSize) {
            len = destSize - 1;
        }
        std::memcpy(dest, src, len);
        dest[len] = '\0';
    }
};

/**
 * Tách rawInput (ví dụ "59-X1-12345") vào outNode.
 * Trả true nếu đúng độ dài và vị trí dấu gạch. Thời gian O(L), bộ nhớ phụ O(1).
 */
bool parseLicensePlate(const char* rawInput, PlateNode& outNode);

/** In bản ghi node ra console. O(1) thời gian và bộ nhớ phụ. */
void printPlateNode(const PlateNode* node);

/** Trả true nếu plateA và plateB trùng khớp, có phân biệt hoa thường. O(1) với biển số chuẩn. */
bool isSamePlate(const char* plateA, const char* plateB);

#endif // PLATE_H
