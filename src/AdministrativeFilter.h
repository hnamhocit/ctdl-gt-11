#ifndef ADMINISTRATIVE_FILTER_H
#define ADMINISTRATIVE_FILTER_H

#include <iostream>
#include <string>

// Định nghĩa cấu trúc PlateNode đầy đủ để truy cập được các trường plateNumber và next
struct PlateNode {
    std::string plateNumber;
    PlateNode* next;
};

class AdministrativeFilter {
public:
    // Đếm số lượng biển số thuộc mã tỉnh (ab)
    int countPlatesInProvince(PlateNode* buckets[], int bucketSize, const std::string& provinceCode) {
        int count = 0;
        for (int i = 0; i < bucketSize; ++i) {
            PlateNode* current = buckets[i];
            while (current != nullptr) {
                if (current->plateNumber.length() >= 2 && current->plateNumber.substr(0, 2) == provinceCode) {
                    count++;
                }
                current = current->next;
            }
        }
        return count;
    }

    // Hiển thị các biển số thuộc mã xã/huyện (cd)
    void displayPlatesOfWard(PlateNode* buckets[], int bucketSize, const std::string& wardCode) {
        std::cout << "--- Danh sach bien so thuoc ma xa/huyen [" << wardCode << "] ---\n";
        bool found = false;
        for (int i = 0; i < bucketSize; ++i) {
            PlateNode* current = buckets[i];
            while (current != nullptr) {
                if (current->plateNumber.length() >= 5 && current->plateNumber.substr(3, 2) == wardCode) {
                    std::cout << "- " << current->plateNumber << "\n";
                    found = true;
                }
                current = current->next;
            }
        }
        if (!found) {
            std::cout << "Khong tim thay bien so nao.\n";
        }
    }

    // Liệt kê mã xã duy nhất (distinct) thuộc tỉnh (khử trùng lặp bằng mảng tạm)
    void displayWardsOfProvince(PlateNode* buckets[], int bucketSize, const std::string& provinceCode) {
        std::cout << "--- Danh sach cac ma xa/huyen duy nhat thuoc tinh [" << provinceCode << "] ---\n";
        
        int maxCapacity = 1000;
        std::string* tempDistinctWards = new std::string[maxCapacity];
        int distinctCount = 0;

        for (int i = 0; i < bucketSize; ++i) {
            PlateNode* current = buckets[i];
            while (current != nullptr) {
                if (current->plateNumber.length() >= 5 && current->plateNumber.substr(0, 2) == provinceCode) {
                    std::string ward = current->plateNumber.substr(3, 2);
                    
                    bool exists = false;
                    for (int k = 0; k < distinctCount; ++k) {
                        if (tempDistinctWards[k] == ward) {
                            exists = true;
                            break;
                        }
                    }

                    if (!exists && distinctCount < maxCapacity) {
                        tempDistinctWards[distinctCount++] = ward;
                    }
                }
                current = current->next;
            }
        }

        for (int i = 0; i < distinctCount; ++i) {
            std::cout << "- Ma xa/huyen: " << tempDistinctWards[i] << "\n";
        }
        if (distinctCount == 0) {
            std::cout << "Khong co du lieu.\n";
        }

        delete[] tempDistinctWards;
    }
};

#endif // ADMINISTRATIVE_FILTER_H