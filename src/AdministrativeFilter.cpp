#include "../include/AdministrativeFilter.h"
#include <iostream>
#include <iomanip>
#include <cstring>

// 1. Tiện ích: Đếm tổng số lượng biển số xe thuộc tỉnh
int AdministrativeFilter::countPlatesInProvince(const LicensePlateHashTable& table, const char* provinceCode) {
    if (provinceCode == nullptr) return 0;

    int count = 0;
    int bucketSize = table.getBucketSize();
    
    for (int i = 0; i < bucketSize; ++i) {
        PlateNode* current = table.getBucket(i);
        while (current != nullptr) {
            if (std::strncmp(current->provinceCode, provinceCode, 2) == 0) {
                count++;
            }
            current = current->next;
        }
    }
    return count;
}

// 2. Chức năng 4 (PROJ-15): Lọc và hiển thị danh sách biển số thuộc Tỉnh và Xã theo dạng BẢNG
void AdministrativeFilter::displayPlatesOfWard(const LicensePlateHashTable& table, const char* provinceCode, const char* wardCode) {
    if (provinceCode == nullptr || wardCode == nullptr) {
        std::cout << "[Loi] Ma tinh hoac ma xa khong hop le.\n";
        return;
    }

    std::cout << "\n==================================================================================\n";
    std::cout << "  DANH SACH BIEN SO XE DANG KY TAI TINH [" << provinceCode << "] - XA [" << wardCode << "]\n";
    std::cout << "==================================================================================\n";
    std::cout << std::left 
              << std::setw(6)  << "STT" 
              << std::setw(16) << "Bien So Xe" 
              << std::setw(35) << "Chu So Huu" 
              << std::setw(15) << "Ngay Dang Ky" << "\n";
    std::cout << "----------------------------------------------------------------------------------\n";

    int stt = 0;
    int bucketSize = table.getBucketSize();

    for (int i = 0; i < bucketSize; ++i) {
        PlateNode* current = table.getBucket(i);
        while (current != nullptr) {
            if (std::strncmp(current->provinceCode, provinceCode, 2) == 0 &&
                std::strncmp(current->wardCode, wardCode, 2) == 0) {
                stt++;
                std::cout << std::left 
                          << std::setw(6)  << stt
                          << std::setw(16) << current->fullPlate 
                          << std::setw(35) << current->ownerName 
                          << std::setw(15) << current->registeredDate << "\n";
            }
            current = current->next;
        }
    }

    if (stt == 0) {
        std::cout << "  [Thong bao] Chua co phuong tien nao dang ky tai Tinh " << provinceCode << " - Xa " << wardCode << ".\n";
    } else {
        std::cout << "----------------------------------------------------------------------------------\n";
        std::cout << "  Tong cong: " << stt << " phuong tien.\n";
    }
    std::cout << "==================================================================================\n\n";
}

// 3. Chức năng 3 (PROJ-9): Lọc và hiển thị các mã xã DUY NHẤT (Khử trùng lặp)
void AdministrativeFilter::displayWardsOfProvince(const LicensePlateHashTable& table, const char* provinceCode) {
    if (provinceCode == nullptr) {
        std::cout << "[Loi] Ma tinh khong hop le.\n";
        return;
    }

    std::cout << "--- Danh sach cac ma Xa thuoc Tinh [" << provinceCode << "] ---\n";

    const int MAX_CAPACITY = 1000;
    char (*tempDistinctWards)[4] = new char[MAX_CAPACITY][4];
    int distinctCount = 0;
    int bucketSize = table.getBucketSize();

    for (int i = 0; i < bucketSize; ++i) {
        PlateNode* current = table.getBucket(i);
        while (current != nullptr) {
            if (std::strncmp(current->provinceCode, provinceCode, 2) == 0) {
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

    if (distinctCount == 0) {
        std::cout << "Thong bao: Tinh [" << provinceCode << "] chua co phuong tien nao dang ky.\n";
    } else {
        for (int i = 0; i < distinctCount; ++i) {
            std::cout << "- Ma Xa: " << tempDistinctWards[i] << "\n";
        }
        std::cout << "Tong so ma xa duy nhat: " << distinctCount << "\n";
    }

    delete[] tempDistinctWards;
}