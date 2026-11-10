#include "../include/AdministrativeFilter.h"
#include <iostream>
#include <cstring>

// 1. Tiện ích: Đếm tổng số lượng biển số xe thuộc tỉnh
int AdministrativeFilter::countPlatesInProvince(const LicensePlateHashTable& table, const char* provinceCode) {
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

// 2. Chức năng 4: Lọc và hiển thị danh sách biển số thuộc Tỉnh và Xã cụ thể
void AdministrativeFilter::displayPlatesOfWard(const LicensePlateHashTable& table, const char* provinceCode, const char* wardCode) {
    std::cout << "--- Danh sach bien so [Tinh " << provinceCode << " - Xa " << wardCode << "] ---\n";
    bool found = false;
    int bucketSize = table.getBucketSize();

    for (int i = 0; i < bucketSize; ++i) {
        PlateNode* current = table.getBucket(i);
        while (current != nullptr) {
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
        std::cout << "Thong bao: Khong tim thay bien so nao phu hop.\n";
    }
}

// 3. Chức năng 3 (PROJ-9): Lọc và hiển thị các mã xã DUY NHẤT (Khử trùng lặp)
void AdministrativeFilter::displayWardsOfProvince(const LicensePlateHashTable& table, const char* provinceCode) {
    std::cout << "--- Danh sach cac ma Xa thuoc Tinh [" << provinceCode << "] ---\n";

    const int MAX_CAPACITY = 1000;
    // Mảng động cấp phát trên Heap để khử trùng lặp (không dùng STL)
    char (*tempDistinctWards)[4] = new char[MAX_CAPACITY][4];
    int distinctCount = 0;
    int bucketSize = table.getBucketSize();

    // Duyệt qua toàn bộ các buckets và danh sách liên kết
    for (int i = 0; i < bucketSize; ++i) {
        PlateNode* current = table.getBucket(i);
        while (current != nullptr) {
            // Lọc node có provinceCode bằng với provinceCode truyền vào
            if (std::strncmp(current->provinceCode, provinceCode, 2) == 0) {
                
                // Thuật toán kiểm tra khử trùng lặp
                bool exists = false;
                for (int k = 0; k < distinctCount; ++k) {
                    if (std::strncmp(tempDistinctWards[k], current->wardCode, 2) == 0) {
                        exists = true;
                        break;
                    }
                }

                // Nếu chưa tồn tại thì lưu vào mảng distinct
                if (!exists && distinctCount < MAX_CAPACITY) {
                    std::strncpy(tempDistinctWards[distinctCount], current->wardCode, 2);
                    tempDistinctWards[distinctCount][2] = '\0';
                    distinctCount++;
                }
            }
            current = current->next;
        }
    }

    // Đánh giá Acceptance Criteria: Thông báo rõ nếu chưa có phương tiện đăng ký
    if (distinctCount == 0) {
        std::cout << "Thong bao: Tinh [" << provinceCode << "] chua co phuong tien nao dang ky.\n";
    } else {
        for (int i = 0; i < distinctCount; ++i) {
            std::cout << "- Ma Xa: " << tempDistinctWards[i] << "\n";
        }
        std::cout << "Tong so ma xa duy nhat: " << distinctCount << "\n";
    }

    // Giải phóng bộ nhớHeap an toàn
    delete[] tempDistinctWards;
}