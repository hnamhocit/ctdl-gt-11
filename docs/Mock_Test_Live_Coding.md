#### BỘ ĐỀ ÔN TẬP VẤN ĐÁP & LIVE CODING (ĐỀ TÀI 11 - BẢNG BĂM BIỂN SỐ XE)

**Mục đích**: Tài liệu luyện tập nội bộ cho 6 thành viên trước buổi bảo vệ đồ án.

#### PHẦN 1: 10 CÂU HỎI VẤN ĐÁP TRỌNG TÂM

1. **Tại sao chọn hàm băm DJB2 (hash * 33 + c) thay vì cộng tổng mã ASCII các ký tự?** 

  * **Trả lời:** Cộng mã ASCII khiến các chuỗi đảo vị trí ký tự (như 59-X1-12345 và 59-X1-54321) ra cùng một giá trị băm, gây đụng độ rất cao. DJB2 kết hợp phép dịch bit và cộng dồn giúp phân biệt thứ tự từng ký tự.
2. **Tại sao kích thước bảng băm TABLE_SIZE lại chọn là 10007 mà không phải 10000?** 

  * **Trả lời:** 10007 là một số nguyên tố. Phép chia lấy dư % 10007 giúp phân tán chỉ số đều hơn và tránh tạo ra các chu kỳ lặp lại khi dữ liệu đầu vào có quy luật.
3. **Hệ số tải 

αalpha
𝛼
⁠ là gì và trong đồ án này nó đạt bao nhiêu với 1.000 biển số?** 

  * **Trả lời:** Hệ số tải 
𝛼

=totalCount

/TABLE_SIZE
⁠. Với 1.000 bản ghi trên 10.007 ngăn chứa, 
𝛼

≈0.099
⁠ (rất nhỏ), giúp độ dài trung bình mỗi danh sách liên kết 

<1is less than 1
<1
⁠ và tốc độ tìm kiếm đạt 
𝑂

(

1

)
⁠.
4. **Tại sao trong struct PlateNode lại dùng mảng char[] cố định thay vì std::string?** 

  * **Trả lời:** Mỗi trường dùng một mảng `char[]` có kích thước cố định nằm ngay trong node. Khi lưu, FileIO ghi từng trường ra TXT, phân cách bằng dấu `|`.
5. **Khi xảy ra đụng độ (2 biển số trùng chỉ số băm), hệ thống chèn nút mới vào đầu hay cuối danh sách liên kết? Tại sao?** 

  * **Trả lời:** Sau khi duyệt bucket để kiểm tra trùng, chương trình nối node mới với đầu cũ rồi cập nhật đầu bucket. Riêng bước nối đầu mất `O(1)`.
6. **Giải thích cách hàm clear() giải phóng bộ nhớ mà không bị lỗi treo con trỏ (Dangling Pointer)?** 

  * **Trả lời:** Dùng vòng lặp duyệt qua 10.007 buckets; tại mỗi bucket dùng con trỏ tạm temp giữ nút hiện tại, cho current nhảy sang current->next trước rồi mới gọi delete temp, sau cùng gán buckets[i] = nullptr.
7. **Nếu xóa một biển số nằm ngay đầu danh sách liên kết tại buckets[index], thao tác nối con trỏ khác gì so với xóa nút ở giữa?** 

  * **Trả lời:** Nếu xóa nút đầu (prev == nullptr), phải cập nhật trực tiếp buckets[index] = current->next. Nếu xóa nút giữa, cập nhật prev->next = current->next rồi mới delete current.
8. **Làm sao hàm lọc danh sách mã xã của một tỉnh (displayWardsOfProvince) loại bỏ được các mã xã trùng nhau khi không được dùng std::set hay std::unordered_set?** 

  * **Trả lời:** Duyệt các node khớp mã tỉnh. Với mỗi node, dò mã xã trong mảng tạm và chỉ thêm mã mới nếu mảng còn chỗ. Mảng hiện giữ tối đa 1.000 mã xã.
9. **Khi dữ liệu tăng lên tới 100.000 xe (gấp 10 lần TABLE_SIZE), chương trình có bị lỗi tràn mảng không?** 

  * **Trả lời:** Mỗi bucket giữ đầu một danh sách liên kết. Khi thêm bản ghi, danh sách có thể dài thêm nếu còn bộ nhớ. Danh sách dài hơn có thể làm tra cứu chậm hơn.
10. **Tỷ lệ sử dụng AI trong dự án này là bao nhiêu và làm sao chứng minh cả 6 thành viên tự làm chủ code?** 

  * **Trả lời:** AI chỉ dùng để gợi ý công thức DJB2, sinh 1.000 dữ liệu mẫu và gợi ý test case (đã ghi rõ trong Mục 6). Toàn bộ code con trỏ thuần đều do các thành viên tự viết lại, biên dịch sạch với -Wall -Wextra -Wpedantic và sẵn sàng sửa trực tiếp khi vấn đáp.

#### PHẦN 2: 5 BÀI TẬP LIVE CODING MẪU (KÈM ĐÁP ÁN C++17)

#### Bài 1: Đếm số ngăn chứa (buckets) đang trống trong bảng băm

* **Yêu cầu:** Viết hàm int countEmptyBuckets() const trả về số lượng bucket bằng nullptr.
* **Đáp án mẫu:**

cpp

int LicensePlateHashTable::countEmptyBuckets() const {
    int emptyCount = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (buckets[i] == nullptr) {
            emptyCount++;
        }
    }
    return emptyCount;
}

Hãy thận trọng khi sử dụng mã.

Sao chép văn bản mã vào bảng nhớ tạm.

#### Bài 2: Tìm độ dài lớn nhất của một chuỗi đụng độ (Max Chain Length)

* **Yêu cầu:** Viết hàm int getMaxChainLength() const để kiểm tra xem bucket bị trùng nhiều nhất đang chứa bao nhiêu nút.
* **Đáp án mẫu:**

cpp

int LicensePlateHashTable::getMaxChainLength() const {
    int maxLen = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int currentLen = 0;
        PlateNode* curr = buckets[i];
        while (curr != nullptr) {
            currentLen++;
            curr = curr->next;
        }
        if (currentLen > maxLen) {
            maxLen = currentLen;
        }
    }
    return maxLen;
}

Hãy thận trọng khi sử dụng mã.

Sao chép văn bản mã vào bảng nhớ tạm.

#### Bài 3: Cập nhật tên chủ xe của một biển số đã tồn tại

* **Yêu cầu:** Viết hàm bool updateOwnerName(const char* plate, const char* newOwner) tìm biển số và cập nhật ownerName, trả về false nếu không tìm thấy.
* **Đáp án mẫu:**

cpp

bool LicensePlateHashTable::updateOwnerName(const char* plate, const char* newOwner) {
    PlateNode* target = searchPlate(plate);
    if (target == nullptr || newOwner == nullptr) {
        return false;
    }
    std::strncpy(target->ownerName, newOwner, sizeof(target->ownerName) - 1);
    target->ownerName[sizeof(target->ownerName) - 1] = '\0';
    return true;
}

Hãy thận trọng khi sử dụng mã.

Sao chép văn bản mã vào bảng nhớ tạm.

#### Bài 4: Đếm tổng số xe thuộc một mã tỉnh bất kỳ

* **Yêu cầu:** Viết hàm int countPlatesByProvince(const char* provCode) const duyệt bảng băm và đếm số nút có provinceCode trùng khớp.
* **Đáp án mẫu:**

cpp

int LicensePlateHashTable::countPlatesByProvince(const char* provCode) const {
    if (provCode == nullptr) return 0;
    int count = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        PlateNode* curr = buckets[i];
        while (curr != nullptr) {
            if (std::strcmp(curr->provinceCode, provCode) == 0) {
                count++;
            }
            curr = curr->next;
        }
    }
    return count;
}

Hãy thận trọng khi sử dụng mã.

Sao chép văn bản mã vào bảng nhớ tạm.

#### Bài 5: Thử thách Debugging (Sửa lỗi truy cập vùng nhớ sau khi giải phóng)

* **Đề bài:** Đoạn code xóa nút đầu danh sách dưới đây bị sai ở đâu? Hãy sửa lại.

cpp

// Code lỗi:
if (prev == nullptr) {
    delete current;
    buckets[index] = current->next; // LỖI: Truy cập current->next sau khi đã delete current!
}

Hãy thận trọng khi sử dụng mã.

Sao chép văn bản mã vào bảng nhớ tạm.

* **Giải thích & Sửa đúng:** Lệnh delete current đã thu hồi vùng nhớ của nút đầu, việc đọc current->next ngay sau đó gây lỗi Use-After-Free. Phải nối con trỏ trước rồi mới xóa:

cpp

if (prev == nullptr) {
    buckets[index] = current->next;
    delete current;
}

Hãy thận trọng khi sử dụng mã.

Sao chép văn bản mã vào bảng nhớ tạm.

## Bài tập bổ sung từ cẩm nang vấn đáp

- **Lọc theo đuôi biển số.** Duyệt mọi bucket và từng node, lấy hai chữ số cuối của `fullPlate` rồi in bản ghi có đuôi `68` hoặc `86`. Giải thích chi phí quét $O(M + N)$.
- **Thay hàm băm bằng SDBM.** Cập nhật giá trị băm với công thức `c + (hash << 6) + (hash << 16) - hash` trên kiểu số nguyên không dấu. Nếu bảng đã có dữ liệu, phải băm lại các node trước khi tra cứu bằng hàm mới.
- **Xóa một biển số.** Giải thích cách `removePlate()` xử lý node đầu bucket và node ở giữa hoặc cuối danh sách. Sau khi nối lại danh sách, giải phóng node và giảm số bản ghi. [Mã nguồn HashTable.cpp](../src/HashTable.cpp)
