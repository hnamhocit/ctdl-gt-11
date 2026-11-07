#ifndef PLATE_H
#define PLATE_H

#include <cstring>
#include <cstddef>
#include <iostream>

/**
 * @brief Cấu trúc dữ liệu đại diện cho một bản ghi biển số xe trong hệ thống.
 * Sử dụng mảng ký tự tĩnh char[] thuần để tối ưu bộ nhớ Heap, tránh phân mảnh bộ nhớ
 * và phục vụ việc ghi/đọc tệp tin trực tiếp theo chuẩn kiến trúc CTDL&GT.
 */
struct PlateNode {
    char fullPlate[16];      // Chuỗi biển số chuẩn: "59-X1-12345\0" (tối đa 15 ký tự)
    char provinceCode[4];    // Mã tỉnh: "59\0" (2 số + '\0')
    char wardCode[4];        // Mã xã: "X1\0" (2 chữ/số + '\0')
    char serialNum[8];       // Số thứ tự xe: "12345\0" (5 số + '\0')
    char registeredDate[12]; // Ngày đăng ký: "2026-10-30\0" (yyyy-mm-dd + '\0')
    char ownerName[50];      // Tên chủ sở hữu phương tiện (UTF-8, tối đa 49 byte + '\0')
    PlateNode* next;         // Con trỏ trỏ tới Node kế tiếp trong cùng một Bucket (Separate Chaining)

    /**
     * @brief Constructor mặc định: khởi tạo chuỗi rỗng và con trỏ next = nullptr.
     */
    PlateNode() : next(nullptr) {
        fullPlate[0] = '\0';
        provinceCode[0] = '\0';
        wardCode[0] = '\0';
        serialNum[0] = '\0';
        registeredDate[0] = '\0';
        ownerName[0] = '\0';
    }

    /**
     * @brief Constructor đầy đủ tham số để tạo bản ghi nhanh chóng.
     */
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

    /**
     * @brief Copy Constructor: sao chép dữ liệu, con trỏ next luôn khởi tạo nullptr.
     */
    PlateNode(const PlateNode& other) : next(nullptr) {
        std::memcpy(fullPlate, other.fullPlate, sizeof(fullPlate));
        std::memcpy(provinceCode, other.provinceCode, sizeof(provinceCode));
        std::memcpy(wardCode, other.wardCode, sizeof(wardCode));
        std::memcpy(serialNum, other.serialNum, sizeof(serialNum));
        std::memcpy(registeredDate, other.registeredDate, sizeof(registeredDate));
        std::memcpy(ownerName, other.ownerName, sizeof(ownerName));
    }

    /**
     * @brief Toán tử gán (Copy Assignment): gán dữ liệu trường, không đè con trỏ next.
     */
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
    /**
     * @brief Tiện ích chép chuỗi an toàn chống tràn bộ đệm.
     */
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
 * @brief Phân tích chuỗi biển số định dạng ab-cd-ghxyz và trích xuất ra các trường trong PlateNode.
 * @param rawInput Chuỗi biển số gốc
 * @param outNode Node đích nhận dữ liệu
 * @return true nếu tách thành công và hợp lệ cú pháp cơ bản
 * Độ phức tạp thời gian: O(1)
 * Độ phức tạp không gian: O(1)
 */
bool parseLicensePlate(const char* rawInput, PlateNode& outNode);

/**
 * @brief In thông tin một Node biển số xe ra màn hình theo dạng bảng đẹp.
 * @param node Con trỏ tới PlateNode cần in
 * Độ phức tạp thời gian: O(1)
 * Độ phức tạp không gian: O(1)
 */
void printPlateNode(const PlateNode* node);

/**
 * @brief So sánh hai chuỗi biển số xe (phân biệt hoa thường hoặc chuẩn hóa).
 * @param plateA Biển số A
 * @param plateB Biển số B
 * @return true nếu hai biển số giống nhau
 * Độ phức tạp thời gian: O(1)
 * Độ phức tạp không gian: O(1)
 */
bool isSamePlate(const char* plateA, const char* plateB);

#endif // PLATE_H
