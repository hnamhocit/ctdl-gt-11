#include "Plate.h"
#include <iostream>
#include <iomanip>
#include <cctype>

/**
 * @brief Phân tích chuỗi biển số xe chuẩn ab-cd-ghxyz thành các trường trong PlateNode.
 * @param rawInput Chuỗi biển số gốc dạng "ab-cd-ghxyz" (ví dụ: "59-X1-12345")
 * @param outNode Node đích nhận các trường tách được
 * @return true nếu đúng định dạng cơ bản 11 ký tự, ngược lại false
 * 
 * Độ phức tạp thời gian: O(1) do độ dài biển số cố định 11 ký tự.
 * Độ phức tạp không gian: O(1).
 */
bool parseLicensePlate(const char* rawInput, PlateNode& outNode) {
    if (!rawInput) return false;
    
    std::size_t len = std::strlen(rawInput);
    if (len != 11) return false;

    // Kiểm tra cấu trúc ab-cd-ghxyz
    if (rawInput[2] != '-' || rawInput[5] != '-') return false;

    // Lưu toàn bộ biển số
    std::memcpy(outNode.fullPlate, rawInput, 11);
    outNode.fullPlate[11] = '\0';

    // Mã tỉnh: 2 ký tự đầu
    outNode.provinceCode[0] = rawInput[0];
    outNode.provinceCode[1] = rawInput[1];
    outNode.provinceCode[2] = '\0';

    // Mã xã: 2 ký tự kế sau dấu '-' đầu
    outNode.wardCode[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(rawInput[3])));
    outNode.wardCode[1] = static_cast<char>(std::toupper(static_cast<unsigned char>(rawInput[4])));
    outNode.wardCode[2] = '\0';

    // Số serial: 5 ký tự cuối
    std::memcpy(outNode.serialNum, rawInput + 6, 5);
    outNode.serialNum[5] = '\0';

    return true;
}

/**
 * @brief In thông tin một Node biển số xe ra màn hình console dạng bảng rõ ràng.
 * @param node Con trỏ tới PlateNode cần in
 * 
 * Độ phức tạp thời gian: O(1).
 * Độ phức tạp không gian: O(1).
 */
void printPlateNode(const PlateNode* node) {
    if (!node) {
        std::cout << "[Trong: Node rong]\n";
        return;
    }
    std::cout << "| " << std::left << std::setw(13) << node->fullPlate
              << "| " << std::setw(6) << node->provinceCode
              << "| " << std::setw(6) << node->wardCode
              << "| " << std::setw(8) << node->serialNum
              << "| " << std::setw(12) << node->registeredDate
              << "| " << std::setw(25) << node->ownerName
              << " |\n";
}

/**
 * @brief So sánh hai chuỗi biển số xe.
 * @param plateA Biển số thứ nhất
 * @param plateB Biển số thứ hai
 * @return true nếu trùng khớp hoàn toàn, false nếu khác hoặc một trong hai là nullptr
 * 
 * Độ phức tạp thời gian: O(1) vì độ dài biển số tối đa 15 ký tự.
 * Độ phức tạp không gian: O(1).
 */
bool isSamePlate(const char* plateA, const char* plateB) {
    if (!plateA || !plateB) return false;
    return std::strcmp(plateA, plateB) == 0;
}
