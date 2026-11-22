# BÁO CÁO BÀI TẬP DỰ ÁN: ĐỀ TÀI 11 - HỆ THỐNG QUẢN LÝ BIỂN SỐ XE

## 1. GIỚI THIỆU ĐỀ TÀI
*   1.1. Bối cảnh đề tài 
Hiện nay, số lượng phương tiện giao thông đăng ký mới mỗi ngày rất lớn và được phân cấp quản lý theo từng Tỉnh/Thành phố trực thuộc trung ương và Quận/Huyện/Xã. Trong thực tế, khi cần tra cứu thông tin của một biển số xe hoặc thống kê danh sách xe theo từng đơn vị hành chính, tốc độ tìm kiếm là yếu tố quan trọng nhất. Nếu chỉ lưu trữ dữ liệu bằng mảng thông thường hay danh sách liên kết đơn thuần, mỗi lần tra cứu chương trình phải duyệt tuần tự từ đầu đến cuối (tốn chi phí O(n)), làm chậm hệ thống khi quy mô dữ liệu tăng lên hàng chục nghìn bản ghi. Vì vậy, việc áp dụng **Bảng băm (Hash Table)** để đưa tốc độ truy xuất về mức gần như tức thời (O(1)) là giải pháp rất phù hợp cho bài toán này.

*   1.2. Mục tiêu dự án 
    *   Xây dựng hoàn chỉnh ứng dụng quản lý biển số xe chạy trên môi trường dòng lệnh (Console) bằng ngôn ngữ **C++ (chuẩn C++11/C++17)**.
    *   Tự tay cài đặt cấu trúc **Bảng băm (`LicensePlateHashTable`)** kết hợp xử lý đụng độ bằng **Danh sách liên kết đơn (`PlateNode` - Separate Chaining)** thông qua con trỏ thuần, tuyệt đối không dùng các thư viện container có sẵn của STL như `std::map`, `std::unordered_map`, `std::set` hay `std::list`.
    *   Đảm bảo chương trình hoạt động ổn định, kiểm tra chặt chẽ tính hợp lệ khi người dùng nhập liệu, lưu và đọc dữ liệu tự động từ file `.txt`, đồng thời quản lý bộ nhớ động (Heap) an toàn để không xảy ra rò rỉ bộ nhớ (Memory Leak).

*   1.3. Phạm vi thực hiện 
    *   **Các chức năng chính:** Thêm (đăng ký) biển số xe mới, tra cứu thông tin một biển số, liệt kê danh sách các mã Xã duy nhất (không trùng lặp) thuộc một Tỉnh, và in ra toàn bộ danh sách biển số đã cấp phát của một Xã cụ thể.
    *   **Lưu trữ & Đánh giá hiệu năng:** Tự động nạp dữ liệu từ file `data/sample_plates.txt` khi khởi động và lưu lại khi thoát chương trình (quy mô dữ liệu mẫu từ 100 đến 1.000 biển số), kết hợp đo thời gian thực thi thực tế bằng thư viện `<chrono>`.

---

## 2. PHÂN TÍCH BÀI TOÁN
*   2.1. Yêu cầu chức năng và phi chức năng
    *   **Yêu cầu chức năng:**
        1. **Đăng ký biển số mới (`insertPlate`):** Nhập chuỗi biển số xe, họ tên chủ xe và ngày đăng ký. Hệ thống kiểm tra xem biển số có đúng cú pháp và đã tồn tại trong bảng băm chưa; nếu hợp lệ thì cấp phát node mới và thêm vào hệ thống.
        2. **Tra cứu một biển số (`searchPlate`):** Nhập biển số cần tìm, hệ thống tính giá trị băm để nhảy thẳng tới bucket tương ứng và hiển thị đầy đủ thông tin xe nếu tìm thấy.
        3. **Hiển thị danh sách mã Xã của một Tỉnh (`displayWardsOfProvince`):** Nhập mã tỉnh (`ab`), chương trình duyệt bảng băm để lọc và in ra các mã xã (`cd`) trực thuộc tỉnh đó (mỗi mã xã chỉ hiển thị 1 lần).
        4. **Hiển thị danh sách biển số của một Xã (`displayPlatesOfWard`):** Nhập cả mã Tỉnh (`ab`) và mã Xã (`cd`), chương trình liệt kê chi tiết toàn bộ các xe đã đăng ký tại xã đó.
        5. **Đọc và ghi file tự động (`loadFromFile` / `saveToFile`):** Khi mở chương trình sẽ tự nạp dữ liệu từ `data/sample_plates.txt` vào bảng băm; khi thoát chương trình sẽ ghi ngược toàn bộ dữ liệu hiện hành ra file để lưu trữ lâu dài.
    *   **Yêu cầu phi chức năng:**
        * **Tốc độ xử lý:** Các thao tác thêm, tìm kiếm, xóa biển số đạt độ phức tạp trung bình O(1).
        * **Ràng buộc kỹ thuật:** Viết 100% bằng con trỏ C++, cấm sử dụng các container có sẵn trong thư viện STL (`std::unordered_map`, `std::map`, `std::set`, `std::list`).
        * **Quản lý bộ nhớ:** Mọi node cấp phát động bằng `new` đều phải được giải phóng sạch sẽ bằng `delete` khi xóa hoặc khi kết thúc chương trình.
        * **Biên dịch sạch:** Biên dịch qua `g++` hoặc `clang++` với cờ `-Wall -Wextra -O2` không báo lỗi và không có cảnh báo nguy hiểm.

*   2.2. Cấu trúc dữ liệu đầu vào và đầu ra 
    *   **Định dạng chuẩn của biển số xe (`ab-cd-ghxyz`):** Mỗi biển số trong chương trình có độ dài cố định **11 ký tự** (chưa tính ký tự kết thúc chuỗi `\0`):

| Thành phần | Quy định ký tự | Độ dài | Ý nghĩa | Ví dụ hợp lệ |
| :--- | :--- | :---: | :--- | :--- |
| **`ab`** | 2 chữ số (`0`–`9`) | 2 | Mã Tỉnh / Thành phố trực thuộc TW | `59`, `29`, `61` |
| **`-`** | Dấu gạch nối | 1 | Ký tự ngăn cách phần 1 | `-` |
| **`cd`** | 2 ký tự chữ in hoa hoặc số (`A`–`Z`, `0`–`9`) | 2 | Mã Quận / Huyện / Xã | `X1`, `A1`, `B2` |
| **`-`** | Dấu gạch nối | 1 | Ký tự ngăn cách phần 2 | `-` |
| **`ghxyz`** | 5 chữ số (`0`–`9`) | 5 | Số thứ tự đăng ký của xe | `12345`, `04567` |

    *   **Định dạng lưu trữ trong file `data/sample_plates.txt`:** Dữ liệu trong file text được lưu mỗi xe trên 1 dòng, các trường thông tin ngăn cách nhau bởi dấu gạch đứng `|`:
```text
fullPlate|provinceCode|wardCode|serialNum|registeredDate|ownerName
59-X1-12345|59|X1|12345|2026-10-30|Nguyen Van A
29-A1-99988|29|A1|99988|2026-10-31|Tran Thi B
```

*   2.3. Các trường hợp đặc biệt và xử lý ngoại lệ 
    *   **Nhập sai định dạng hoặc gõ chữ thường:** Nếu người dùng nhập `59-x1-12345` (chữ thường), module `Validator` sẽ tự động chuyển sang chữ in hoa (`59-X1-12345`) và cắt bỏ khoảng trắng thừa. Nếu nhập thiếu dấu gạch nối (`59X112345`), sai số lượng chữ số (`59-X1-1234`) hoặc mã tỉnh có chữ (`5A-X1-12345`), chương trình sẽ báo lỗi cụ thể và yêu cầu nhập lại.
    *   **Đăng ký trùng biển số:** Khi thêm một biển số đã có sẵn trong bảng băm, hàm `insertPlate` sẽ phát hiện trùng khóa `fullPlate` tại bucket tương ứng, lập tức từ chối thêm mới và thông báo cho người dùng chứ không ghi đè lên dữ liệu cũ.
    *   **Tìm kiếm không ra kết quả hoặc nhập mã tỉnh/xã trống:** Chương trình trả về `nullptr` hoặc thông báo "Không tìm thấy dữ liệu phù hợp", tránh tình trạng truy cập vào con trỏ rỗng gây văng chương trình (Crash / Segmentation Fault).
    *   **File dữ liệu bị lỗi hoặc chưa tồn tại:** Khi mở file `sample_plates.txt`, nếu không tìm thấy file hoặc gặp dòng bị sai định dạng, chương trình sẽ bỏ qua dòng lỗi an toàn và thông báo trên màn hình.

---


## 3. THIẾT KẾ GIẢI PHÁP
*   3.1. Mô hình dữ liệu và sơ đồ cấu trúc hệ thống
Nhóm chọn giải pháp **Bảng băm kết hợp Nối kết trực tiếp (Separate Chaining)**. Bảng băm gồm một mảng tĩnh có kích thước `TABLE_SIZE = 10007` phần tử con trỏ `PlateNode* buckets[TABLE_SIZE]`. Mỗi phần tử `buckets[i]` là con trỏ đầu (`head`) của một danh sách liên kết đơn chứa tất cả các biển số bị băm trùng vào vị trí `i`.

~~~mermaid
graph LR
    subgraph HashTable ["LicensePlateHashTable (TABLE_SIZE = 10007)"]
        B0["buckets"]
        B1["buckets"]
        B2["buckets"]
        BM["buckets"]
    end

    subgraph Chains ["Danh sách liên kết đơn (Separate Chaining)"]
        B1 --> N1["PlateNode<br/>59-X1-12345"]
        N1 --> N2["PlateNode<br/>29-A1-99988"]
        N2 --> NULL1["nullptr"]

        B2 --> N3["PlateNode<br/>61-B2-04567"]
        N3 --> NULL2["nullptr"]

        B0 --> NULL0["nullptr"]
        BM --> NULLM["nullptr"]
    end
~~~

*   3.2. Cấu trúc dữ liệu cốt lõi 
    *   **Cấu trúc `PlateNode` (`include/Plate.h`):** Tách sẵn mã tỉnh, mã xã và số thứ tự ngay lúc nhập vào struct `PlateNode` giúp chức năng lọc theo Tỉnh/Xã sau này chỉ cần so sánh trực tiếp `provinceCode` và `wardCode` mà không tốn công cắt chuỗi lại nhiều lần. Dùng mảng ký tự cố định `char[]` thay vì `std::string` để kích thước mỗi Node gọn gàng trên bộ nhớ Heap:

~~~cpp
struct PlateNode {
    char fullPlate[16];      
    char provinceCode[4];    
    char wardCode[4];        
    char serialNum[8];       
    char registeredDate[12]; 
    char ownerName[50];      
    PlateNode* next;        

    PlateNode() : next(nullptr) {
        fullPlate[0] = '\0';
        provinceCode[0] = '\0';
        wardCode[0] = '\0';
        serialNum[0] = '\0';
        registeredDate[0] = '\0';
        ownerName[0] = '\0';
    }
};
~~~

    *   **Lớp `LicensePlateHashTable` (`include/HashTable.h`):**

~~~cpp
class LicensePlateHashTable {
private:
    static const int TABLE_SIZE = 10007; // Kích thước bảng băm (số nguyên tố)
    PlateNode* buckets[TABLE_SIZE];      // Mảng các con trỏ đầu danh sách liên kết
    int totalCount;                      // Đếm tổng số biển số hiện có

public:
    LicensePlateHashTable();
    ~LicensePlateHashTable();

    int hashFunction(const char* plate) const;
    bool insertPlate(const PlateNode& newPlate);
    PlateNode* searchPlate(const char* plate) const;
    bool removePlate(const char* plate);

    void displayWardsOfProvince(const char* provCode) const;
    void displayPlatesOfWard(const char* provCode, const char* wardCode) const;
    int countPlatesByProvince(const char* provCode) const;

    int getTotalCount() const;
    void clear();
    double getLoadFactor() const;
};
~~~

*   3.3. Giải thuật băm 
Nhóm sử dụng thuật toán băm chuỗi **DJB2** (do Daniel J. Bernstein đề xuất) kết hợp chia lấy dư cho số nguyên tố `TABLE_SIZE = 10007`:
    *   **Công thức:** $\text{hash}_0 = 5381$, $\text{hash}_i = ((\text{hash}_{i-1} \ll 5) + \text{hash}_{i-1}) + c_i$, và $\text{index} = \text{hash}_n \bmod 10007$.
    *   **Lý do chọn:** Số khởi tạo `5381` và hệ số nhân `33` giúp các biển số giống phần đầu (như `59-X1-12345` và `59-X1-12346`) tạo ra mã băm khác hẳn nhau, giảm đụng độ. Phép dịch trái `(hash << 5) + hash` chạy nhanh hơn phép nhân thông thường. Số nguyên tố `10007` giúp rải đều dữ liệu, giữ hệ số tải $\alpha \in [0.1, 0.5]$ cho tập 1.000–5.000 biển số.

~~~cpp
int LicensePlateHashTable::hashFunction(const char* plate) const {
    if (!plate || plate == '\0') return 0;

    unsigned long hash = 5381;
    int c;
    while ((c = static_cast<unsigned char>(*plate++))) {
hash = ((hash << 5) + hash) + c; 
return static_cast<int>(hash % TABLE_SIZE);
}
~~~
*   3.4. Thiết kế chức năng hệ thống theo module
  * Lưu đồ hàm insertPlate (Thêm biển số mới):
~~~mermaid
flowchart TD
A([Bắt đầu: insertPlate]) --> B[Tính index = hashFunction newPlate.fullPlate]
B --> C[Gán current = buckets index]
C --> D{current != nullptr?}
D -- Có --> E{strcmp current->fullPlate, newPlate.fullPlate == 0?}
E -- Đúng --> F[Báo trùng biển số -> Trả về false]
F --> Z([Kết thúc])
E -- Sai --> G[current = current->next]
G --> D
D -- Không --> H[Tạo node mới: node = new PlateNode newPlate]
H --> I[Chèn đầu DSLK: node->next = buckets index]
I --> J[buckets index = node; totalCount++]
J --> K[Trả về true]
K --> Z
~~~
  * Lưu đồ hàm searchPlate (Tra cứu một biển số):
~~~mermaid
flowchart TD
A([Bắt đầu: searchPlate]) --> B[Tính index = hashFunction plate]
B --> C[Gán current = buckets index]
C --> D{current != nullptr?}
D -- Không --> E[Không tìm thấy -> Trả về nullptr]
E --> Z([Kết thúc])
D -- Có --> F{strcmp current->fullPlate, plate == 0?}
F -- Đúng --> G[Trả về con trỏ current]
G --> Z
F -- Sai --> H[current = current->next]
H --> D
~~~
  * Lưu đồ hàm displayWardsOfProvince (Lọc danh sách mã Xã của một Tỉnh):
~~~mermaid
flowchart TD
A([Bắt đầu: displayWardsOfProvince]) --> B[Tạo mảng seenWards rỗng để lưu mã xã đã gặp]
B --> C[Duyệt i từ 0 đến TABLE_SIZE - 1]
C --> D[Gán current = buckets i]
D --> E{current != nullptr?}
E -- Không --> I[Tăng i++]
I --> J{i < TABLE_SIZE?}
J -- Có --> D
J -- Không --> K[In danh sách mã xã trong seenWards ra màn hình]
K --> Z([Kết thúc])
E -- Có --> F{strcmp current->provinceCode, provCode == 0?}
F -- Đúng --> G{current->wardCode chưa có trong seenWards?}
G -- Đúng --> H[Thêm current->wardCode vào seenWards]
H --> L[current = current->next]
G -- Sai --> L
F -- Sai --> L
L --> E
~~~
*   3.5. Phân tích độ phức tạp thuật toán lý thuyết
Ký hiệu: n là tổng số biển số xe (totalCount), m = 10007 (TABLE_SIZE), L = 11 (độ dài biển số chuẩn), và α = n / m là hệ số tải trung bình:

Tên hàm / Thao tác | Tốt nhất (Best) | Trung bình (Average) | Xấu nhất (Worst) | Bộ nhớ phụ trợ | Giải thích
| --- | --- | --- | --- | --- | --- |
hashFunction (DJB2) | O(1) | O(1) | O(1) | O(1) | Luôn duyệt đúng 11 ký tự của biển số chuẩn nên chi phí không đổi.
insertPlate | O(1) | O(1 + α) | O(n) | O(1) | Băm mất O(1), kiểm tra trùng trong bucket mất O(1 + α), chèn đầu danh sách mất O(1). Xấu nhất O(n) nếu tất cả n xe bị băm vào cùng 1 bucket.
searchPlate | O(1) | O(1 + α) | O(n) | O(1) | Nhảy thẳng tới buckets[index] trong O(1), duyệt trung bình α node. Xấu nhất O(n) khi bảng biến thành 1 danh sách liên kết dài.
removePlate | O(1) | O(1 + α) | O(n) | O(1) | Tìm node tại bucket tương ứng và cập nhật lại con trỏ prev->next trong O(1).
displayWardsOfProvince | O(m + n) | O(m + n) | O(m + n) | O(W) | Phải duyệt qua toàn bộ m buckets và n node để gom mã xã của tỉnh (W là số xã của tỉnh đó, W ≪ n).
displayPlatesOfWard | O(m + n) | O(m + n) | O(m + n) | O(1) |  Quét qua m buckets và n node để in các xe trùng cả mã tỉnh và mã xã.
loadFromFile / saveToFile | O(n) / O(m + n) | O(n) / O(m + n) | O(n²) / O(m + n) | O(1) | saveToFile duyệt hết m buckets và ghi n dòng mất O(m + n). loadFromFile gọi n lần insertPlate nên trung bình mất O(n).
Tổng bộ nhớ bảng băm | — | — | — | O(m + n) | Mảng tĩnh 10007 con trỏ (khoảng 80 KB) cộng với n struct PlateNode cấp phát động.


## 4. CÀI ĐẶT VÀ HIỆN THỰC

*   4.1. Môi trường phát triển và công cụ sử dụng
	* Ngôn ngữ lập trình: C++ (chuẩn C++17 / C++11).
	* Trình biên dịch: g++ (GCC) hoặc clang++ kèm các cờ kiểm tra lỗi -Wall -Wextra -O2.
	* Công cụ build tự động: Makefile và CMakeLists.txt.
	* Môi trường lập trình & Quản lý code nhóm: Visual Studio Code, Git và GitHub (chia nhánh theo từng thành viên/tính năng và tạo Pull Request để kiểm tra chéo trước khi gộp code).
*   4.2. Cấu trúc mã nguồn mô-đun
Để 6 thành viên dễ chia việc và tránh xung đột code (merge conflict), nhóm chia chương trình thành các file header (.h) trong thư mục include/ và file cài đặt (.cpp) trong thư mục src/:
~~~text
ctdl-gt-11/
├── include/                     # Các file khai báo cấu trúc dữ liệu và nguyên mẫu hàm
│   ├── Plate.h                  # Khai báo struct PlateNode và các hàm xử lý chuỗi biển số
│   ├── HashTable.h              # Khai báo lớp LicensePlateHashTable và hàm băm DJB2
│   ├── Validator.h              # Kiểm tra định dạng ab-cd-ghxyz và chuẩn hóa chữ in hoa
│   ├── FileIO.h                 # Đọc/ghi dữ liệu file sample_plates.txt và tạo dữ liệu mẫu
│   ├── AdministrativeFilter.h   # Lọc danh sách mã xã (distinct) và danh sách xe theo xã
│   ├── MenuUI.h                 # Hiển thị menu dòng lệnh và điều hướng lựa chọn người dùng
│   └── Benchmark.h              # Đo thời gian chạy thực tế bằng thư viện <chrono>
├── src/                         # Các file viết code xử lý chi tiết
│   ├── main.cpp                 # Hàm main() khởi tạo hệ thống, nạp file và chạy menu
│   ├── Plate.cpp
│   ├── HashTable.cpp
│   ├── Validator.cpp
│   ├── FileIO.cpp
│   ├── AdministrativeFilter.cpp
│   ├── MenuUI.cpp
│   └── Benchmark.cpp
├── data/
│   └── sample_plates.txt        # File dữ liệu mẫu lưu thông tin biển số xe
├── Makefile                     # File cấu hình biên dịch nhanh bằng lệnh make
└── CMakeLists.txt               # File cấu hình build bằng CMake
~~~
*   4.3. Hệ thống build chương trình tự động
	* Dự án sử dụng Makefile và CMakeLists.txt với bộ cờ biên dịch -std=c++17 -Wall -Wextra -O2 -Iinclude để tự động hóa quá trình biên dịch toàn bộ các tệp .cpp trong src/ thành tệp thực thi duy nhất trong thư mục out/, giúp phát hiện sớm các cảnh báo về con trỏ hoặc kiểu dữ liệu.
*   4.4. Cơ chế quản lý vùng nhớ Heap và giải phóng con trỏ
	* Khởi tạo sạch con trỏ: Trong constructor LicensePlateHashTable(), toàn bộ 10007 phần tử trong mảng buckets được gán về nullptr và totalCount = 0 để tránh lỗi trỏ vào vùng nhớ rác.
	* Giải phóng khi xóa từng biển số (removePlate): Dùng con trỏ prev đứng trước current để nối lại prev->next = current->next (hoặc cập nhật buckets[index] = current->next), sau đó gọi delete current và giảm totalCount đi 1.
	* Giải phóng toàn bộ khi thoát chương trình (clear & Destructor): Hàm clear() duyệt qua từng bucket từ 0 đến 10006, dùng con trỏ tạm temp giữ địa chỉ node hiện tại rồi dịch current = current->next trước khi gọi delete temp. Hàm hủy ~LicensePlateHashTable() tự động gọi clear() khi chương trình kết thúc để tránh Memory Leak.
~~~cpp
void LicensePlateHashTable::clear() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        PlateNode* current = buckets[i];
        while (current != nullptr) {
             PlateNode* temp = current;
             current = current->next;
             delete temp;
        }
        buckets[i] = nullptr;
     }
     totalCount = 0;
}

LicensePlateHashTable::~LicensePlateHashTable() {
clear();
}
~~~