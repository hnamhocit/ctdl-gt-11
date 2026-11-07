#include "../include/AdministrativeFilter.h"
#include <iostream>
#include <cstring>

int AdministrativeFilter::countPlatesInProvince(const LicensePlateHashTable& table, const char* provinceCode) {
    int count = 0;
    int bucketSize = table.getBucketSize();
    
    for (int i = 0; i < bucketSize; ++i) {
        PlateNode* current = table.getBucket(i);
        while (current != nullptr) {
            // So sánh 2 ký tự mã tỉnh trên mảng char[]
            if (std::strncmp(current->provinceCode, provinceCode, 2) == 0) {
                count++;
            }
            current = current->next;
        }
    }
    return count;
}

void AdministrativeFilter::displayPlatesOfWard(const LicensePlateHashTable& table, const char* provinceCode, const char* wardCode) {
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
                          << " | Chu xe: " << current->ownerName << "\n";
                found = true;
            }
            current = current->next;
        }
    }
    if (!found) {
        std::cout << "Khong tim thay bien so phu hop.\n";
    }
}

void AdministrativeFilter::displayWardsOfProvince(const LicensePlateHashTable& table, const char* provinceCode) {
    std::cout << "--- Danh sach ma xa duy nhat thuoc Tinh [" << provinceCode << "] ---\n";
    
    const int MAX_CAPACITY = 1000;
    // Mảng động tạm chứa các mã xã (mỗi mã 2 ký tự + '\0')
    char (*tempDistinctWards)[3] = new char[MAX_CAPACITY][3];
    int distinctCount = 0;
    int bucketSize = table.getBucketSize();

    for (int i = 0; i < bucketSize; ++i) {
        PlateNode* current = table.getBucket(i);
        while (current != nullptr) {
            if (std::strncmp(current->provinceCode, provinceCode, 2) == 0) {
                // Kiểm tra mã xã đã có trong mảng tạm chưa
                bool exists = false;
                for (int k = 0; k < distinctCount; ++k) {
                    if (std::strncmp(tempDistinctWards[k], current->wardCode, 2) == 0) {
                        exists = true;
                        break;
                    }
                }

                if (!exists && distinctCount < MAX_CAPACITY) {
                    std::strncpy(tempDistinctWards[distinctCount], current->wardCode, 2);
                    tempDistinctWards[distinctCount][2] = '\0';
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

    delete[] tempDistinctWards; // Giải phóng bộ nhớ đúng chuẩn C++
}