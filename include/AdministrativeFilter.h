#ifndef ADMINISTRATIVE_FILTER_H
#define ADMINISTRATIVE_FILTER_H

#include "HashTable.h"

/**
 * @brief Module AdministrativeFilter: Thực thi các chức năng lọc dữ liệu theo địa giới hành chính.
 * - Chức năng 3: Lọc và thống kê danh sách mã xã duy nhất (distinct) thuộc một tỉnh.
 * - Chức năng 4: Lọc và hiển thị danh sách biển số xe đã đăng ký tại một xã thuộc tỉnh.
 * - Tiện ích bổ trợ: Đếm tổng số lượng biển số xe đã đăng ký tại một tỉnh.
 */
class AdministrativeFilter {
public:
    /**
     * @brief Đếm tổng số lượng biển số xe đang lưu hành thuộc về một tỉnh cụ thể.
     * Duyệt qua toàn bộ TABLE_SIZE buckets và danh sách liên kết.
     * @param table Tham chiếu tới Bảng Băm dữ liệu
     * @param provinceCode Chuỗi mã tỉnh cần đếm (ví dụ: "59")
     * @return int Số lượng biển số xe thuộc tỉnh
     * 
     * Độ phức tạp thời gian: O(M + N) với M là TABLE_SIZE, N là tổng số bản ghi
     * Độ phức tạp không gian: O(1)
     */
    int countPlatesInProvince(const LicensePlateHashTable& table, const char* provinceCode);

    /**
     * @brief Hiển thị toàn bộ các biển số xe thuộc về một xã của một tỉnh.
     * @param table Tham chiếu tới Bảng Băm
     * @param provinceCode Chuỗi mã tỉnh (ví dụ: "59")
     * @param wardCode Chuỗi mã xã (ví dụ: "X1")
     * 
     * Độ phức tạp thời gian: O(M + N)
     * Độ phức tạp không gian: O(1)
     */
    void displayPlatesOfWard(const LicensePlateHashTable& table, const char* provinceCode, const char* wardCode);

    /**
     * @brief Lọc và hiển thị các mã xã DUY NHẤT (Distinct) đã có biển số đăng ký thuộc một tỉnh.
     * Sử dụng mảng động phụ trợ char tempDistinctWards[MAX_CAPACITY][4] để loại bỏ trùng lặp
     * mà KHÔNG sử dụng std::set hay std::unordered_set.
     * @param table Tham chiếu tới Bảng Băm
     * @param provinceCode Chuỗi mã tỉnh (ví dụ: "59")
     * 
     * Độ phức tạp thời gian: O(M + N * K) với K là số lượng xã duy nhất (K <= MAX_CAPACITY)
     * Độ phức tạp không gian: O(MAX_CAPACITY) được cấp phát và giải phóng sạch trên Heap
     */
    void displayWardsOfProvince(const LicensePlateHashTable& table, const char* provinceCode);
};

#endif // ADMINISTRATIVE_FILTER_H
