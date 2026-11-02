# 03. Đặc Tả Các Module Mã Nguồn (Module Specifications)

Hệ thống được thiết kế theo hướng module hóa cao (Modular Architecture), tách biệt rành mạch giữa cấu trúc dữ liệu, tầng logic nghiệp vụ, tầng vào/ra file và tầng giao diện điều khiển.

---

## 1. Cấu Trúc Phân Tách Thư Mục & Tệp Nguồn

```text
ctdl-gt-11/
├── include/                     # Thư mục Header files (.h)
│   ├── Plate.h                  # Định nghĩa Struct PlateNode & các tiện ích chuỗi
│   ├── HashTable.h              # Định nghĩa Class LicensePlateHashTable
│   ├── Validator.h              # Hàm kiểm tra định dạng ab-cd-ghxyz, ngày, tên
│   ├── FileIO.h                 # Module đọc/ghi tệp TXT
│   ├── AdministrativeFilter.h   # Module lọc mã xã và danh sách biển số theo địa bàn
│   ├── MenuUI.h                 # Giao diện dòng lệnh console và điều hướng menu
│   └── Benchmark.h              # Tiện ích đo lường thời gian thực thi (chrono)
├── src/                         # Thư mục Hiện thực mã nguồn (.cpp)
│   ├── main.cpp                 # Entrypoint chính của chương trình
│   ├── Plate.cpp
│   ├── HashTable.cpp
│   ├── Validator.cpp
│   ├── FileIO.cpp
│   ├── AdministrativeFilter.cpp
│   ├── MenuUI.cpp
│   └── Benchmark.cpp
├── data/                        # Dữ liệu mẫu
│   └── sample_plates.txt        # Tệp dữ liệu ban đầu (ít nhất 100 - 1000 biển số)
├── docs/                        # Tài liệu hướng dẫn & đặc tả kỹ thuật
├── tests/                       # Kịch bản kiểm thử tự động
├── Makefile                     # Kịch bản biên dịch Make
├── CMakeLists.txt               # Kịch bản biên dịch CMake
└── README.md                    # Hướng dẫn tổng quan & cài đặt
```

---

## 2. Đặc Tả Chi Tiết Từng Module

### 2.1. Module `Plate` (`include/Plate.h`, `src/Plate.cpp`)
- **Trách nhiệm**: Quản lý cấu trúc dữ liệu của một bản ghi biển số xe.
- **Hàm chính**:
  ```cpp
  // Tách chuỗi chuẩn 'ab-cd-ghxyz' thành các trường riêng biệt
  bool parseLicensePlate(const char* rawInput, PlateNode& outNode);

  // In thông tin 1 Node ra màn hình theo định dạng bảng đẹp
  void printPlateNode(const PlateNode* node);

  // So sánh hai biển số xem có trùng khớp hay không
  bool isSamePlate(const char* plateA, const char* plateB);
  ```

---

### 2.2. Module `Validator` (`include/Validator.h`, `src/Validator.cpp`)
- **Trách nhiệm**: Kiểm tra tính hợp lệ cú pháp đầu vào của biển số, mã tỉnh, mã xã.
- **Hàm chính**:
  ```cpp
  // Kiểm tra chuỗi có đúng dạng ab-cd-ghxyz (ab: 2 số, cd: 2 chữ/số, ghxyz: 5 số)
  bool isValidLicensePlateFormat(const char* plate);

  // Kiểm tra mã tỉnh ab có đúng 2 ký tự số không (00 - 99)
  bool isValidProvinceCode(const char* provCode);

  // Kiểm tra mã xã cd có đúng 2 ký tự chữ/số không
  bool isValidWardCode(const char* wardCode);

  // Chuẩn hóa chuỗi (chuyển chữ thường sang chữ in hoa, loại bỏ khoảng trắng thừa)
  void normalizePlateString(char* plate);
  ```

---

### 2.3. Module `HashTable` (`include/HashTable.h`, `src/HashTable.cpp`)
- **Trách nhiệm**: Cài đặt Bảng băm con trỏ thuần, hàm băm DJB2, giải quyết xung đột bằng danh sách liên kết đơn.
- **Hàm chính**:
  ```cpp
  int hashFunction(const char* plate) const;
  bool insertPlate(const PlateNode& newPlate);
  PlateNode* searchPlate(const char* plate) const;
  bool removePlate(const char* plate);
  void clear();
  int getTotalCount() const;
  double getLoadFactor() const;
  ```

---

### 2.4. Module `AdministrativeFilter` (`include/AdministrativeFilter.h`, `src/AdministrativeFilter.cpp`)
- **Trách nhiệm**: Hiện thực Chức năng 3 (Lọc danh sách xã của tỉnh) và Chức năng 4 (Lọc biển số của một xã).
- **Hàm chính**:
  ```cpp
  // Lấy và hiển thị toàn bộ mã xã 'cd' duy nhất (distinct) thuộc tỉnh 'ab'
  void displayWardsOfProvince(const LicensePlateHashTable& table, const char* provCode);

  // Hiển thị toàn bộ danh sách các xe đã đăng ký tại xã 'cd' thuộc tỉnh 'ab'
  void displayPlatesOfWard(const LicensePlateHashTable& table, const char* provCode, const char* wardCode);

  // Đếm tổng số biển số đang lưu hành tại một tỉnh
  int countPlatesInProvince(const LicensePlateHashTable& table, const char* provCode);
  ```

---

### 2.5. Module `FileIO` (`include/FileIO.h`, `src/FileIO.cpp`)
- **Trách nhiệm**: Lưu trữ bền vững và nạp dữ liệu từ file định dạng văn bản `.txt`.
- **Định dạng file TXT quy ước**:
  ```text
  fullPlate|provinceCode|wardCode|serialNum|registeredDate|ownerName
  59-X1-12345|59|X1|12345|2026-10-30|Nguyen Van A
  29-A1-99988|29|A1|99988|2026-10-31|Tran Thi B
  ```
- **Hàm chính**:
  ```cpp
  // Đọc danh sách biển số từ file TXT vào bảng băm khi khởi động
  bool loadFromFile(LicensePlateHashTable& table, const char* filepath);

  // Ghi toàn bộ dữ liệu từ bảng băm ra file TXT khi thoát chương trình
  bool saveToFile(const LicensePlateHashTable& table, const char* filepath);

  // Sinh tệp dữ liệu ngẫu nhiên phục vụ kiểm thử (100 - 10,000 bản ghi)
  bool generateSampleDataset(const char* filepath, int count);
  ```

---

### 2.6. Module `MenuUI` (`include/MenuUI.h`, `src/MenuUI.cpp`)
- **Trách nhiệm**: Render giao diện Menu Console dạng văn bản tương tác, bắt phím, phân luồng chức năng và hiển thị thông báo lỗi rõ ràng.
- **Hàm chính**:
  ```cpp
  void printBanner();
  void displayMainMenu();
  void handleRegister(LicensePlateHashTable& table);
  void handleSearch(const LicensePlateHashTable& table);
  void handleDisplayWards(const LicensePlateHashTable& table);
  void handleDisplayPlatesOfWard(const LicensePlateHashTable& table);
  void handleStatistics(const LicensePlateHashTable& table);
  void handleExportImport(LicensePlateHashTable& table);
  ```

---

### 2.7. Module `Benchmark` (`include/Benchmark.h`, `src/Benchmark.cpp`)
- **Trách nhiệm**: Đo lường và xuất thời gian thực thi (tính bằng microsecond / millisecond) của các thao tác băm, chèn và tra cứu để phục vụ số liệu trong Báo cáo Mục 5 & Mục 7.
- **Hàm chính**:
  ```cpp
  void runBenchmarkSuite(LicensePlateHashTable& table, int testSize);
  ```
