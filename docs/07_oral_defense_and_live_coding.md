# 07. Cẩm Nang Vấn Đáp & Kịch Bản Live Coding (Oral Defense & Live Coding)

> **Mục tiêu**: Chuẩn bị cho 100% thành viên Nhóm 11 đạt điểm Mức 5 (8.5 - 10 điểm) ở phần Đánh giá Vấn đáp Cá nhân (chiếm 30% tổng điểm học phần).

---

## 1. Top 10 Câu Hỏi Vấn Đáp Thường Gặp & Câu Trả Lời Mẫu

### Câu 1: Tại sao nhóm chọn Bảng băm kết hợp Danh sách liên kết thay vì Cây nhị phân tìm kiếm (AVL/Red-Black Tree) hay Mảng động (`std::vector`)?
- **Trả lời**:  
  - So với **Mảng động**: Tìm kiếm trên mảng chưa sắp xếp tốn $O(n)$, nếu sắp xếp thì tìm kiếm nhị phân tốn $O(\log n)$ nhưng thao tác chèn/xóa tốn $O(n)$ do phải dịch chuyển các phần tử.
  - So với **Cây AVL/Red-Black**: Cây cân bằng cho thời gian tìm kiếm $O(\log n)$. Tuy nhiên bài toán quản lý biển số xe là bài toán **tra cứu chính xác (Exact Match Lookup)** theo khóa định danh duy nhất. Bảng băm đem lại thời gian tra cứu lý tưởng trung bình là $O(1)$, vượt trội hoàn toàn so với $O(\log n)$.
  - Sử dụng **Danh sách liên kết (Chaining)** thay vì dò tuyến tính (Open Addressing) giúp tránh triệt để hiện tượng dồn cụm và việc cấp phát động node diễn ra linh hoạt khi có dữ liệu mới.

### Câu 2: Giải thuật băm DJB2 hoạt động như thế nào? Con số 5381 và 33 có ý nghĩa gì?
- **Trả lời**:  
  - Thuật toán băm DJB2 của Daniel J. Bernstein duyệt qua từng ký tự của chuỗi biển số và cập nhật giá trị băm theo công thức: `hash = ((hash << 5) + hash) + c` (tương đương `hash * 33 + c`).
  - Số `5381` là một số nguyên tố khởi tạo được chứng minh qua thực nghiệm giúp giảm thiểu va chạm ban đầu.
  - Phép nhân với `33` (được tối ưu thành dịch trái 5 bit cộng chính nó) tạo ra độ phân tán bit cao (Avalanche effect), biến các chuỗi dù chỉ khác nhau 1 ký tự cuối (ví dụ `...12345` và `...12346`) thành hai chỉ số băm hoàn toàn cách xa nhau.

### Câu 3: Kích thước bảng băm `TABLE_SIZE = 10007` được chọn dựa trên nguyên lý nào?
- **Trả lời**:  
  - `10007` là một số nguyên tố (Prime number). Phép toán `% TABLE_SIZE` với số chia là số nguyên tố sẽ phân bố đều giá trị băm và triệt tiêu tính tuần hoàn của dữ liệu chuỗi.
  - Với tập dữ liệu mục tiêu khoảng 1.000 đến 3.000 xe, hệ số tải $\alpha = N / M \approx 0.1 - 0.3$. Độ dài trung bình của các bucket chỉ khoảng $0.1$ node, do đó xác suất va chạm rất thấp, đảm bảo thời gian truy xuất thực tế đạt xấp xỉ $O(1)$.

### Câu 4: Hãy chỉ ra đoạn mã giải quyết xung đột (Collision Resolution) trong code?
- **Trả lời**:  
  - Sinh viên mở tệp [HashTable.cpp](file:///home/hnamhocit/Workspace/cpp/ctdl-gt-11/src/HashTable.cpp) tại hàm `insertPlate`:
  - Khi tính được `index = hashFunction(newPlate.fullPlate)`, nếu `buckets[index] != nullptr`, chương trình duyệt qua danh sách liên kết tại `buckets[index]` để kiểm tra trùng lặp khóa. Nếu không trùng, node mới được liên kết vào đầu danh sách: `newNode->next = buckets[index]; buckets[index] = newNode;` với độ phức tạp $O(1)$.

### Câu 5: Hệ thống làm thế nào để đảm bảo không bị rò rỉ bộ nhớ (Memory Leak)?
- **Trả lời**:  
  - Trong hàm `clear()` và `~LicensePlateHashTable()` (Destructor), chương trình duyệt qua từng bucket từ $0$ đến $\text{TABLE\_SIZE} - 1$. Với mỗi bucket có danh sách liên kết, dùng vòng lặp `while (current != nullptr)` lưu con trỏ tạm `temp = current`, cho `current = current->next`, sau đó gọi `delete temp`.
  - Nhóm đã kiểm tra kiểm thử rò rỉ bộ nhớ bằng Valgrind với kết quả `0 errors / 0 bytes leaked`.

### Câu 6: Độ phức tạp của chức năng "Hiển thị danh sách mã Xã của Tỉnh" là bao nhiêu?
- **Trả lời**:  
  - Độ phức tạp thời gian là $O(M + N)$, trong đó $M = 10007$ (số bucket phải duyệt qua) và $N$ là tổng số xe hiện có.
  - Để tránh in trùng mã xã, nhóm lưu các mã xã đã gặp vào một mảng tạm và kiểm tra trùng trước khi in, tốn thêm $O(K^2)$ với $K$ là số lượng xã của tỉnh đó ($K \le 30$, là hằng số nhỏ).

### Câu 7: Nếu một bucket có nhiều node trùng mã băm, làm sao phân biệt được biển số cần tìm?
- **Trả lời**:  
  - Băm chỉ đưa ta đến đúng bucket chứa danh sách liên kết. Trong bucket đó, ta duyệt từng `PlateNode` và dùng hàm so sánh chuỗi `strcmp(current->fullPlate, targetPlate) == 0` để tìm đúng bản ghi cần tra cứu.

### Câu 8: Tại sao lại dùng mảng tĩnh `char[]` trong struct `PlateNode` thay vì `std::string`?
- **Trả lời**:  
  - Giúp kiểm soát chính xác kích thước vùng nhớ của từng Node trong bộ nhớ Heap.
  - Tránh chi phí phụ trợ (overhead) và phân mảnh bộ nhớ của `std::string` (thường kèm con trỏ động quản lý buffer riêng).
  - Phù hợp với tư duy lập trình C++ hệ thống và chuẩn mực học phần Cấu trúc Dữ liệu & Giải thuật.

### Câu 9: Trình bày quy trình nạp dữ liệu từ file TXT? Nếu gặp dòng lỗi thì xử lý ra sao?
- **Trả lời**:  
  - Đọc từng dòng bằng `std::ifstream` và `std::getline`. Phân tách các trường theo dấu gạch đứng `|`.
  - Chuyển dữ liệu qua module `Validator` để kiểm tra độ dài và cú pháp. Nếu dòng đó đúng định dạng thì gọi `insertPlate`. Nếu dòng dữ liệu bị lỗi/bẩn thì bỏ qua dòng đó, ghi log cảnh báo và tiếp tục nạp các dòng tiếp theo mà không làm crash chương trình.

### Câu 10: Nếu muốn mở rộng để hỗ trợ 1 triệu xe thì thiết kế hiện tại cần thay đổi gì?
- **Trả lời**:  
  - Cần tăng kích thước bảng băm lên số nguyên tố lớn hơn (ví dụ: `2000003`) hoặc hiện thực cơ chế **Tự động băm lại (Dynamic Rehashing)**: khi hệ số tải $\alpha > 0.75$, tự động cấp phát bảng mới gấp đôi kích thước và di chuyển toàn bộ các node sang bảng mới.

---

## 2. Kịch Bản Live Coding Giảng Viên Yêu Cầu Sửa Code Trực Tiếp

### Kịch bản 1: Thêm hàm đếm tổng số xe thuộc một tỉnh
**Yêu cầu của GV**: *"Hãy viết thêm một hàm đếm xem tỉnh `59` có tổng cộng bao nhiêu xe đang đăng ký và in ra màn hình?"*  
**Cách xử lý**:
```cpp
int LicensePlateHashTable::countPlatesByProvince(const char* provCode) const {
    if (!provCode) return 0;
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
```

---

### Kịch bản 2: Lọc các biển số xe có đuôi "Lộc Phát" (68 hoặc 86)
**Yêu cầu của GV**: *"Hãy sửa chức năng tra cứu để in ra tất cả các xe có 2 chữ số cuối cùng là 68 hoặc 86?"*  
**Cách xử lý**:
```cpp
void LicensePlateHashTable::displayLuckyPlates() const {
    std::cout << "\n=== DANH SÁCH BIỂN SỐ LỘC PHÁT (ĐUÔI 68 / 86) ===\n";
    for (int i = 0; i < TABLE_SIZE; ++i) {
        PlateNode* curr = buckets[i];
        while (curr != nullptr) {
            int len = std::strlen(curr->fullPlate);
            if (len >= 2) {
                const char* tail = curr->fullPlate + (len - 2);
                if (std::strcmp(tail, "68") == 0 || std::strcmp(tail, "86") == 0) {
                    std::cout << "-> " << curr->fullPlate 
                              << " | Chủ xe: " << curr->ownerName << "\n";
                }
            }
            curr = curr->next;
        }
    }
}
```

---

### Kịch bản 3: Đổi hàm băm sang thuật toán khác (SDBM)
**Yêu cầu của GV**: *"Bây giờ hãy đổi hàm băm sang thuật toán SDBM để so sánh?"*  
**Cách xử lý**:
```cpp
// Thuật toán băm SDBM: hash = c + (hash << 6) + (hash << 16) - hash
int LicensePlateHashTable::hashFunctionSDBM(const char* plate) const {
    unsigned long hash = 0;
    int c;
    while ((c = static_cast<unsigned char>(*plate++))) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return static_cast<int>(hash % TABLE_SIZE);
}
```

---

### Kịch bản 4: Xóa một biển số xe khỏi bảng băm (`removePlate`)
**Yêu cầu của GV**: *"Hãy viết hàm xóa một biển số xe đã đăng ký khỏi hệ thống?"*  
**Cách xử lý**:
```cpp
bool LicensePlateHashTable::removePlate(const char* plate) {
    int index = hashFunction(plate);
    PlateNode* curr = buckets[index];
    PlateNode* prev = nullptr;

    while (curr != nullptr) {
        if (std::strcmp(curr->fullPlate, plate) == 0) {
            if (prev == nullptr) {
                // Xóa node đầu bucket
                buckets[index] = curr->next;
            } else {
                // Xóa node ở giữa hoặc cuối
                prev->next = curr->next;
            }
            delete curr;
            totalCount--;
            return true; // Xóa thành công
        }
        prev = curr;
        curr = curr->next;
    }
    return false; // Không tìm thấy để xóa
}
```
