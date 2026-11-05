#ifndef ADMINISTRATIVE_FILTER_H
#define ADMINISTRATIVE_FILTER_H

// Cấu trúc PlateNode chuẩn dùng mảng char[] theo 02_architecture
struct PlateNode {
    char fullPlate[12];
    char provinceCode[3];
    char wardCode[3];
    char serialNum[6];
    char registeredDate[11];
    char ownerName[50];
    PlateNode* next;
};

class LicensePlateHashTable {
public:
    int getBucketSize() const;
    PlateNode* getBucket(int index) const;
};

class AdministrativeFilter {
public:
    int countPlatesInProvince(const LicensePlateHashTable& table, const char* provinceCode);
    void displayPlatesOfWard(const LicensePlateHashTable& table, const char* provinceCode, const char* wardCode);
    void displayWardsOfProvince(const LicensePlateHashTable& table, const char* provinceCode);
};

#endif // ADMINISTRATIVE_FILTER_H