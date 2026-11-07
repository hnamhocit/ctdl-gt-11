#include "HashTable.h"

/**
 * @brief Khởi tạo Bảng Băm rỗng.
 * Mọi con trỏ bucket được gán nullptr và totalCount = 0.
 * 
 * Độ phức tạp thời gian: O(TABLE_SIZE)
 * Độ phức tạp không gian: O(1)
 */
LicensePlateHashTable::LicensePlateHashTable() : totalCount(0) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        buckets[i] = nullptr;
    }
}

/**
 * @brief Destructor: Thu hồi toàn bộ bộ nhớ Heap của các PlateNode.
 * 
 * Độ phức tạp thời gian: O(TABLE_SIZE + N)
 * Độ phức tạp không gian: O(1)
 */
LicensePlateHashTable::~LicensePlateHashTable() {
    clear();
}

/**
 * @brief Giải thuật băm chuỗi DJB2 do Daniel J. Bernstein thiết kế.
 * Khởi tạo: hash = 5381.
 * Với mỗi ký tự c: hash = hash * 33 + c = ((hash << 5) + hash) + c.
 * Phép toán nhân với 33 kết hợp phép dịch bit giúp giá trị băm phân bố đều trên dải [0, TABLE_SIZE - 1].
 * 
 * @param plate Chuỗi biển số xe đầu vào (ví dụ: "59-X1-12345")
 * @return int Chỉ số bucket tương ứng
 * 
 * Độ phức tạp thời gian: O(L) với L là độ dài chuỗi (L=11 -> O(1))
 * Độ phức tạp không gian: O(1)
 */
int LicensePlateHashTable::hashFunction(const char* plate) const {
    if (!plate || plate[0] == '\0') return 0;

    unsigned long hash = 5381;
    int c;
    while ((c = static_cast<unsigned char>(*plate++))) {
        // (hash << 5) tương đương hash * 32, cộng thêm hash thành hash * 33
        hash = ((hash << 5) + hash) + static_cast<unsigned long>(c);
    }

    return static_cast<int>(hash % TABLE_SIZE);
}

/**
 * @brief Chèn một bản ghi biển số xe mới vào Bảng Băm (Separate Chaining).
 * - Bước 1: Tính toán bucket index qua hàm băm DJB2.
 * - Bước 2: Duyệt danh sách liên kết tại bucket đó để kiểm tra trùng lặp khóa fullPlate.
 * - Bước 3: Nếu chưa trùng, cấp phát PlateNode mới trên Heap và chèn vào đầu danh sách (O(1)).
 * 
 * @param newPlate Đối tượng PlateNode chứa dữ liệu biển số
 * @return true nếu chèn thành công, false nếu biển số bị trùng hoặc không hợp lệ
 * 
 * Độ phức tạp thời gian: O(1) trung bình (khi alpha < 0.5), O(N) trường hợp xấu nhất
 * Độ phức tạp không gian: O(1) cấp phát 1 node Heap
 */
bool LicensePlateHashTable::insertPlate(const PlateNode& newPlate) {
    if (newPlate.fullPlate[0] == '\0') return false;

    int index = hashFunction(newPlate.fullPlate);

    // Bước 2: Kiểm tra chống trùng lặp (Uniqueness check)
    PlateNode* current = buckets[index];
    while (current != nullptr) {
        if (std::strcmp(current->fullPlate, newPlate.fullPlate) == 0) {
            return false; // Biển số đã tồn tại trong hệ thống
        }
        current = current->next;
    }

    // Bước 3: Cấp phát động node mới và thực hiện chèn đầu (Insert at head)
    PlateNode* newNode = new PlateNode(newPlate);
    newNode->next = buckets[index];
    buckets[index] = newNode;
    totalCount++;

    return true;
}

/**
 * @brief Tra cứu thông tin biển số xe trong Bảng Băm.
 * - Bước 1: Tính bucket index bằng hashFunction trong O(1).
 * - Bước 2: Truy xuất trực tiếp buckets[index], duyệt DSLK so sánh chuỗi fullPlate.
 * 
 * @param plate Chuỗi biển số cần tìm
 * @return PlateNode* Con trỏ tới bản ghi tìm thấy, nullptr nếu không tồn tại
 * 
 * Độ phức tạp thời gian: O(1) trung bình, O(N) trường hợp xấu nhất
 * Độ phức tạp không gian: O(1)
 */
PlateNode* LicensePlateHashTable::searchPlate(const char* plate) const {
    if (!plate || plate[0] == '\0') return nullptr;

    int index = hashFunction(plate);
    PlateNode* current = buckets[index];

    while (current != nullptr) {
        if (std::strcmp(current->fullPlate, plate) == 0) {
            return current;
        }
        current = current->next;
    }

    return nullptr;
}

/**
 * @brief Xóa một bản ghi biển số xe khỏi Bảng Băm và giải phóng bộ nhớ Heap.
 * - Sử dụng kỹ thuật duyệt danh sách với 2 con trỏ (current và previous).
 * - Ngắt liên kết con trỏ trước khi gọi delete để đảm bảo toàn vẹn cấu trúc danh sách.
 * 
 * @param plate Chuỗi biển số cần xóa
 * @return true nếu tìm thấy và xóa thành công, false nếu không tồn tại
 * 
 * Độ phức tạp thời gian: O(1) trung bình, O(N) trường hợp xấu nhất
 * Độ phức tạp không gian: O(1)
 */
bool LicensePlateHashTable::removePlate(const char* plate) {
    if (!plate || plate[0] == '\0') return false;

    int index = hashFunction(plate);
    PlateNode* current = buckets[index];
    PlateNode* previous = nullptr;

    while (current != nullptr) {
        if (std::strcmp(current->fullPlate, plate) == 0) {
            // Ngắt liên kết con trỏ
            if (previous == nullptr) {
                // Node cần xóa nằm ngay đầu bucket
                buckets[index] = current->next;
            } else {
                // Node cần xóa nằm ở giữa hoặc cuối DSLK
                previous->next = current->next;
            }

            delete current; // Thu hồi bộ nhớ Heap của Node
            totalCount--;
            return true;
        }
        previous = current;
        current = current->next;
    }

    return false;
}

/**
 * @brief Thu hồi toàn bộ vùng nhớ Heap của tất cả các Node trong Bảng Băm.
 * Duyệt tuần tự qua TABLE_SIZE buckets, xóa từng node trong từng danh sách liên kết.
 * 
 * Độ phức tạp thời gian: O(TABLE_SIZE + N) với N là tổng số node
 * Độ phức tạp không gian: O(1)
 */
void LicensePlateHashTable::clear() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        PlateNode* current = buckets[i];
        while (current != nullptr) {
            PlateNode* temp = current;
            current = current->next;
            delete temp; // Giải phóng từng node đơn lẻ
        }
        buckets[i] = nullptr;
    }
    totalCount = 0;
}

/**
 * @brief Lấy tổng số lượng bản ghi biển số hiện có trong Bảng Băm.
 * Độ phức tạp: O(1).
 */
int LicensePlateHashTable::getTotalCount() const {
    return totalCount;
}

/**
 * @brief Tính hệ số tải alpha = totalCount / TABLE_SIZE.
 * Độ phức tạp: O(1).
 */
double LicensePlateHashTable::getLoadFactor() const {
    return static_cast<double>(totalCount) / static_cast<double>(TABLE_SIZE);
}

/**
 * @brief Trả về số lượng bucket của Bảng Băm (TABLE_SIZE = 10007).
 * Độ phức tạp: O(1).
 */
int LicensePlateHashTable::getBucketSize() const {
    return TABLE_SIZE;
}

/**
 * @brief Trả về con trỏ đầu danh sách liên kết tại bucket thứ index.
 * Độ phức tạp: O(1).
 */
PlateNode* LicensePlateHashTable::getBucket(int index) const {
    if (index < 0 || index >= TABLE_SIZE) return nullptr;
    return buckets[index];
}
