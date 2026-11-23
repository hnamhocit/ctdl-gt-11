#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Plate.h"

/** Bảng băm DJB2 dùng danh sách liên kết đơn để xử lý đụng độ. */
class LicensePlateHashTable {
public:
    static const int TABLE_SIZE = 10007; // Số nguyên tố lớn để chia dư phân bố đều

private:
    PlateNode* buckets[TABLE_SIZE];      // Mảng con trỏ quản lý đầu danh sách liên kết
    int totalCount;                      // Tổng số lượng biển số xe đang lưu trữ

public:
    /** Tạo bảng rỗng. Thời gian O(TABLE_SIZE), bộ nhớ phụ O(1). */
    LicensePlateHashTable();

    /** Giải phóng các node qua clear(). Thời gian O(TABLE_SIZE + N), bộ nhớ phụ O(1). */
    ~LicensePlateHashTable();

    // Ngăn sao chép bảng băm để tránh lỗi giải phóng vùng nhớ hai lần (double free)
    LicensePlateHashTable(const LicensePlateHashTable&) = delete;
    LicensePlateHashTable& operator=(const LicensePlateHashTable&) = delete;

    /**
     * Tính chỉ số bucket từ plate bằng DJB2 (bắt đầu từ 5381, cập nhật hash * 33 + c).
     * Thời gian O(L) với L là độ dài plate, bộ nhớ phụ O(1).
     */
    int hashFunction(const char* plate) const;

    /**
     * Chèn newPlate vào đầu bucket. Trả false nếu biển số rỗng hoặc trùng.
     * Thời gian O(1 + K), bộ nhớ phụ O(1), với K là số node trong bucket.
     */
    bool insertPlate(const PlateNode& newPlate);

    /**
     * Tìm plate trong bucket tương ứng. Trả con trỏ tới node hoặc nullptr.
     * Thời gian O(1 + K), bộ nhớ phụ O(1), với K là số node trong bucket.
     */
    PlateNode* searchPlate(const char* plate) const;

    /**
     * Xóa node ứng với plate. Trả true nếu xóa thành công.
     * Thời gian O(1 + K), bộ nhớ phụ O(1), với K là số node trong bucket.
     */
    bool removePlate(const char* plate);

    /** Xóa N node và đặt lại các bucket. Thời gian O(TABLE_SIZE + N), bộ nhớ phụ O(1). */
    void clear();

    /** Trả số bản ghi hiện có. O(1) thời gian và bộ nhớ phụ. */
    int getTotalCount() const;

    /** Trả hệ số tải totalCount / TABLE_SIZE. O(1) thời gian và bộ nhớ phụ. */
    double getLoadFactor() const;

    /** Trả số bucket. O(1) thời gian và bộ nhớ phụ. */
    int getBucketSize() const;

    /** Trả node đầu của bucket index, hoặc nullptr nếu rỗng hay ngoài phạm vi. O(1). */
    PlateNode* getBucket(int index) const;
};

#endif // HASHTABLE_H
