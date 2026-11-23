#include "Plate.h"
#include <iostream>
#include <iomanip>
#include <cctype>

bool parseLicensePlate(const char* rawInput, PlateNode& outNode) {
    if (!rawInput) return false;
    
    std::size_t len = std::strlen(rawInput);
    if (len != 11) return false;

    // Kiểm tra cấu trúc ab-cd-ghxyz
    if (rawInput[2] != '-' || rawInput[5] != '-') return false;

    std::memcpy(outNode.fullPlate, rawInput, 11);
    outNode.fullPlate[11] = '\0';

    // Hai ký tự đầu là mã tỉnh, ví dụ "59".
    outNode.provinceCode[0] = rawInput[0];
    outNode.provinceCode[1] = rawInput[1];
    outNode.provinceCode[2] = '\0';

    // Hai ký tự sau dấu gạch đầu là mã xã, ví dụ "X1".
    outNode.wardCode[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(rawInput[3])));
    outNode.wardCode[1] = static_cast<char>(std::toupper(static_cast<unsigned char>(rawInput[4])));
    outNode.wardCode[2] = '\0';

    // Năm ký tự cuối là số thứ tự, ví dụ "12345".
    std::memcpy(outNode.serialNum, rawInput + 6, 5);
    outNode.serialNum[5] = '\0';

    return true;
}

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

bool isSamePlate(const char* plateA, const char* plateB) {
    if (!plateA || !plateB) return false;
    return std::strcmp(plateA, plateB) == 0;
}
