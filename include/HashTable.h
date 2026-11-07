#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Plate.h"

/**
 * @brief Lớp Bảng Băm LicensePlateHashTable quản lý cơ sở dữ liệu biển số xe.
 * 
 * RÀNG BUỘC HỌC THUẬT BẮT BUỘC (ACADEMIC DIRECTIVES):
 * - Không sử dụng bất kỳ thư viện STL chứa sẵn nào (<unordered_map>, <map>, <vector>, <list>).
 * - Cài đặt hoàn toàn bằng con trỏ thuần: mảng con trỏ `PlateNode* buckets[10007]`.
 * - Kỹ thuật giải quyết đụng độ: Separate Chaining (Danh sách liên kết đơn).
 * - Thuật toán băm chuỗi: DJB2 (hash * 33 + c) với giá trị khởi tạo 5381.
 * - Kích thước bảng băm: TABLE_SIZE = 10007 (số nguyên tố lớn hạn chế chu kỳ và xung đột).
 */
class LicensePlateHashTable {
public:
    static const int TABLE_SIZE = 10007; // Số nguyên tố lớn để chia dư phân bố đều

private:
    PlateNode* buckets[TABLE_SIZE];      // Mảng con trỏ quản lý đầu danh sách liên kết
    int totalCount;                      // Tổng số lượng biển số xe đang lưu trữ

public:
    /**
     * @brief Khởi tạo Bảng Băm rỗng. Mọi phần tử trong mảng buckets được gán nullptr.
     * Độ phức tạp thời gian: O(TABLE_SIZE)
     * Độ phức tạp không gian: O(1)
     */
    LicensePlateHashTable();

    /**
     * @brief Destructor: Tự động thu hồi 100% vùng nhớ Heap của toàn bộ PlateNode.
     * Ngăn chặn hoàn toàn rò rỉ bộ nhớ (Zero Memory Leak).
     * Độ phức tạp thời gian: O(TABLE_SIZE + N)
     * Độ phức tạp không gian: O(1)
     */
    ~LicensePlateHashTable();

    // Ngăn sao chép bảng băm để tránh lỗi giải phóng vùng nhớ hai lần (double free)
    LicensePlateHashTable(const LicensePlateHashTable&) = delete;
    LicensePlateHashTable& operator=(const LicensePlateHashTable&) = delete;

    /**
     * @brief Giải thuật băm chuỗi DJB2 của Daniel J. Bernstein.
     * Công thức: hash_i = (hash_{i-1} * 33) + c = ((hash_{i-1} << 5) + hash_{i-1}) + c
     * @param plate Chuỗi biển số xe đầu vào (ví dụ: "59-X1-12345")
     * @return int Chỉ số bucket trong khoảng [0, TABLE_SIZE - 1]
     * 
     * Độ phức tạp thời gian: O(L) với L là chiều dài biển số (11 ký tự -> O(1))
     * Độ phức tạp không gian: O(1)
     */
    int hashFunction(const char* plate) const;

    /**
     * @brief Chèn một bản ghi biển số xe mới vào Bảng Băm.
     * - Kiểm tra xem biển số đã tồn tại hay chưa (chống trùng lặp).
     * - Nếu chưa tồn tại: Cấp phát động một PlateNode mới trên Heap và chèn vào đầu danh sách (chèn đầu O(1)).
     * @param newPlate Bản ghi PlateNode chứa thông tin biển số cần chèn
     * @return true nếu chèn thành công, false nếu biển số đã tồn tại hoặc dữ liệu rỗng
     * 
     * Độ phức tạp thời gian: O(1) trung bình (alpha < 0.5), O(N) trong trường hợp xấu nhất
     * Độ phức tạp không gian: O(1) cấp phát một Node mới
     */
    bool insertPlate(const PlateNode& newPlate);

    /**
     * @brief Tra cứu thông tin biển số xe trong Bảng Băm theo cơ chế O(1).
     * Băm chuỗi ra chỉ số bucket, sau đó duyệt danh sách liên kết tại bucket đó.
     * @param plate Chuỗi biển số xe cần tìm kiếm
     * @return PlateNode* Con trỏ tới Node nếu tìm thấy, nullptr nếu không tồn tại
     * 
     * Độ phức tạp thời gian: O(1) trung bình, O(N) trường hợp xấu nhất
     * Độ phức tạp không gian: O(1)
     */
    PlateNode* searchPlate(const char* plate) const;

    /**
     * @brief Xóa một bản ghi biển số xe khỏi Bảng Băm và giải phóng vùng nhớ Heap của Node đó.
     * Tìm kiếm và thực hiện thao tác ngắt liên kết con trỏ trước khi gọi delete.
     * @param plate Chuỗi biển số xe cần xóa
     * @return true nếu tìm thấy và xóa thành công, false nếu không tồn tại
     * 
     * Độ phức tạp thời gian: O(1) trung bình, O(N) trường hợp xấu nhất
     * Độ phức tạp không gian: O(1)
     */
    bool removePlate(const char* plate);

    /**
     * @brief Thu hồi toàn bộ bộ nhớ Heap của tất cả các node trong toàn bộ buckets.
     * Đưa số lượng phần tử về 0 và gán toàn bộ mảng buckets thành nullptr.
     * 
     * Độ phức tạp thời gian: O(TABLE_SIZE + N) với N là tổng số bản ghi
     * Độ phức tạp không gian: O(1)
     */
    void clear();

    /**
     * @brief Lấy tổng số lượng bản ghi biển số xe hiện có trong hệ thống.
     * @return int Tổng số biển số
     * Độ phức tạp: O(1)
     */
    int getTotalCount() const;

    /**
     * @brief Tính hệ số tải (Load Factor) alpha = totalCount / TABLE_SIZE.
     * Dùng để đánh giá độ dày đặc và xác suất đụng độ của bảng băm.
     * @return double Giá trị hệ số tải
     * Độ phức tạp: O(1)
     */
    double getLoadFactor() const;

    /**
     * @brief Lấy kích thước mảng bucket (TABLE_SIZE = 10007).
     * Cung cấp cho các module mở rộng như FileIO và AdministrativeFilter duyệt bảng.
     * @return int Số lượng bucket
     * Độ phức tạp: O(1)
     */
    int getBucketSize() const;

    /**
     * @brief Lấy con trỏ đầu danh sách liên kết tại bucket thứ index.
     * @param index Chỉ số bucket [0, TABLE_SIZE - 1]
     * @return PlateNode* Con trỏ đầu danh sách liên kết tại bucket
     * Độ phức tạp: O(1)
     */
    PlateNode* getBucket(int index) const;
};

#endif // HASHTABLE_H
