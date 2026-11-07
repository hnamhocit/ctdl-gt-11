#include "AdministrativeFilter.h"
#include <iostream>
#include <cstring>

/**
 * @brief Đếm tổng số biển số xe thuộc tỉnh.
 * Duyệt qua tất cả các bucket trong bảng băm và đếm các node có provinceCode khớp.
 * 
 * Độ phức tạp thời gian: O(M + N) với M = TABLE_SIZE, N = tổng số node.
 * Độ phức tạp không gian: O(1).
 */
int AdministrativeFilter::countPlatesInProvince(const LicensePlateHashTable& table, const char* provinceCode) {
    if (!provinceCode) return 0;
    int count = 0;
    int bucketSize = table.getBucketSize();

    for (int i = 0; i < bucketSize; ++i) {
        PlateNode* current = table.getBucket(i);
        while (current != nullptr) {
            // So sánh 2 ký tự mã tỉnh
            if (std::strncmp(current->provinceCode, provinceCode, 2) == 0) {
                count++;
            }
            current = current->next;
        }
    }
    return count;
}

/**
 * @brief Hiển thị toàn bộ các biển số xe đã đăng ký tại một xã thuộc một tỉnh.
 * Duyệt qua toàn bộ bảng băm, lọc các node khớp cả provinceCode và wardCode.
 * 
 * Độ phức tạp thời gian: O(M + N).
 * Độ phức tạp không gian: O(1).
 */
void AdministrativeFilter::displayPlatesOfWard(const LicensePlateHashTable& table, const char* provinceCode, const char* wardCode) {
    if (!provinceCode || !wardCode) return;
    std::cout << "--- Danh sach bien so [Tinh " << provinceCode << " - Xa " << wardCode << "] ---\n";
    bool found = false;
    int bucketSize = table.getBucketSize();

    for (int i = 0; i < bucketSize; ++i) {
        PlateNode* current = table.getBucket(i);
        while (current != nullptr) {
            // Lọc chính xác KHỚP CẢ MÃ TỈNH VÀ MÃ XÃ
            if (std::strncmp(current->provinceCode, provinceCode, 2) == 0 &&
                std::strncmp(current->wardCode, wardCode, 2) == 0) {
                std::cout << "- Bien so: " << current->fullPlate 
                          << " | Chu xe: " << current->ownerName
                          << " | Ngay dang ky: " << current->registeredDate << "\n";
                found = true;
            }
            current = current->next;
        }
    }
    if (!found) {
        std::cout << "Khong tim thay bien so phu hop.\n";
    }
}

/**
 * @brief Lọc và in danh sách mã xã duy nhất (Distinct) thuộc một tỉnh.
 * Sử dụng mảng động tạm thời char[MAX_CAPACITY][4] và thuật toán kiểm tra tuyến tính.
 * Thu hồi bộ nhớ mảng động ngay sau khi hoàn thành.
 * 
 * Độ phức tạp thời gian: O(M + N * K) với K là số xã duy nhất đã ghi nhận.
 * Độ phức tạp không gian: O(MAX_CAPACITY * 4) byte cấp phát động.
 */
void AdministrativeFilter::displayWardsOfProvince(const LicensePlateHashTable& table, const char* provinceCode) {
    if (!provinceCode) return;
    std::cout << "--- Danh sach ma xa duy nhat thuoc Tinh [" << provinceCode << "] ---\n";

    const int MAX_CAPACITY = 1000;
    // Mảng động tạm chứa các mã xã (mỗi mã tối đa 3 ký tự + '\0')
    char (*tempDistinctWards)[4] = new char[MAX_CAPACITY][4];
    int distinctCount = 0;
    int bucketSize = table.getBucketSize();

    for (int i = 0; i < bucketSize; ++i) {
        PlateNode* current = table.getBucket(i);
        while (current != nullptr) {
            if (std::strncmp(current->provinceCode, provinceCode, 2) == 0) {
                // Kiểm tra mã xã đã có trong mảng tạm chưa
                bool exists = false;
                for (int k = 0; k < distinctCount; ++k) {
                    if (std::strcmp(tempDistinctWards[k], current->wardCode) == 0) {
                        exists = true;
                        break;
                    }
                }

                if (!exists && distinctCount < MAX_CAPACITY) {
                    std::size_t wardLen = std::strlen(current->wardCode);
                    if (wardLen > 3) wardLen = 3;
                    std::memcpy(tempDistinctWards[distinctCount], current->wardCode, wardLen);
                    tempDistinctWards[distinctCount][wardLen] = '\0';
                    distinctCount++;
                }
            }
            current = current->next;
        }
    }

    for (int i = 0; i < distinctCount; ++i) {
        std::cout << "- Ma Xa: " << tempDistinctWards[i] << "\n";
    }
    if (distinctCount == 0) {
        std::cout << "Khong co du lieu.\n";
    }

    delete[] tempDistinctWards; // Giải phóng bộ nhớ đúng chuẩn C++, Zero Memory Leak
}
