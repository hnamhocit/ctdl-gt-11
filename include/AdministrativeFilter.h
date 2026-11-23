#ifndef ADMINISTRATIVE_FILTER_H
#define ADMINISTRATIVE_FILTER_H

#include "HashTable.h"

/** Lọc biển số theo mã tỉnh và mã xã. M là số bucket, N là số bản ghi. */
class AdministrativeFilter {
public:
    /** Đếm biển số trong table có provinceCode, ví dụ "59". Thời gian O(M + N), bộ nhớ phụ O(1). */
    int countPlatesInProvince(const LicensePlateHashTable& table, const char* provinceCode);

    /**
     * In biển số trong table khớp cả provinceCode và wardCode, ví dụ tỉnh "59", xã "X1".
     * Thời gian O(M + N), bộ nhớ phụ O(1).
     */
    void displayPlatesOfWard(const LicensePlateHashTable& table, const char* provinceCode, const char* wardCode);

    /**
     * In mỗi mã xã của provinceCode trong table một lần.
     * Thời gian O(M + N × K) với K là số mã xã đã ghi nhận, bộ nhớ phụ theo dung lượng mảng mã xã.
     */
    void displayWardsOfProvince(const LicensePlateHashTable& table, const char* provinceCode);

    /**
     * In số xe và tỷ lệ theo tỉnh trong table.
     * Thời gian O(M + N × P) với P là số mã tỉnh đã ghi nhận, bộ nhớ phụ theo dung lượng mảng mã tỉnh.
     */
    void displayProvinceStatistics(const LicensePlateHashTable& table);
};

#endif // ADMINISTRATIVE_FILTER_H