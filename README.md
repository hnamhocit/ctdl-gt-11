# Hệ Thống Quản Lý Biển Số Xe (License Plate Management System)

> **Học phần**: Cấu trúc Dữ liệu và Giải thuật (CTDL&GT)  
> **Đề tài số 11**: Quản lý biển số xe bằng Bảng Băm (Hash Table) và Danh Sách Liên Kết (Linked List)  
> **Ngôn ngữ**: C++ (Chuẩn C++17 / C++11)  
> **Nhóm thực hiện**: Nhóm 11 (6 thành viên)

[![C++17](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![Build](https://img.shields.io/badge/Build-g%2B%2B%20%7C%20Make%20%7C%20CMake-brightgreen.svg)]()
[![License](https://img.shields.io/badge/Academic-Course%20Project-orange.svg)]()
[![Zero STL Map](https://img.shields.io/badge/Core%20DS-Raw%20Pointers%20%26%20Chaining-red.svg)]()

---

## 1. Hướng Dẫn Biên Dịch & Chạy Chương Trình (Quick Start)

Dự án hỗ trợ đầy đủ các phương thức biên dịch tiêu chuẩn trên Linux, macOS và Windows:

### Cách 1: Sử dụng Makefile (Khuyên dùng - Nhanh nhất)
```bash
# Biên dịch toàn bộ mã nguồn vào bin/main
make

# Biên dịch và chạy chương trình ngay lập tức
make run

# Dọn dẹp tệp tin build trung gian
make clean
```

### Cách 2: Sử dụng lệnh `g++` trực tiếp
```bash
# Biên dịch sạch với chuẩn C++17 và các cờ cảnh báo tối ưu
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude src/main.cpp -o bin/main

# Chạy file thực thi
./bin/main

# (Khi đã có nhiều file .cpp trong src/, biên dịch tất cả bằng):
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude src/*.cpp -o bin/main
```

### Cách 3: Sử dụng CMake (Chuẩn công nghiệp đa nền tảng)
```bash
mkdir -p build && cd build
cmake ..
make -j4
./bin/main
```

### Cách 4: Kiểm tra an toàn bộ nhớ (Valgrind trên Linux)
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/main
```

---

## 2. Giới Thiệu & Bối Cảnh Đề Tài

Hệ thống quản lý biển số xe là ứng dụng C++ độc lập hiệu năng cao, được thiết kế để giải quyết bài toán định danh và tra cứu phương tiện giao thông tức thời với độ phức tạp lý thuyết đạt $O(1)$.

### 📌 Các Ràng Buộc Kỹ Thuật Tiên Quyết:
- **Ngôn ngữ bắt buộc**: 100% C++ thuần.
- **Cấu trúc dữ liệu cốt lõi**: **Bảng Băm (Hash Table)** giải quyết đụng độ bằng **Danh sách liên kết đơn (Separate Chaining)** được tự cài đặt hoàn toàn bằng con trỏ thuần (`PlateNode*`, `new`, `delete`).
- **Nghiêm cấm lạm dụng STL**: Tuyệt đối không dùng `std::unordered_map`, `std::map`, `std::unordered_set`, `std::set`, hoặc `std::list` cho kho lưu trữ biển số cốt lõi.
- **An toàn bộ nhớ**: Thu hồi $100\%$ các node được cấp phát động, không gây rò rỉ bộ nhớ (0 memory leak khi kiểm thử với Valgrind).

---

## 3. Quy Cách Chuẩn Hóa Biển Số Xe (`ab-cd-ghxyz`)

Tất cả biển số trong hệ thống tuân thủ nghiêm ngặt theo mẫu chuỗi 11 ký tự:

$$\mathbf{ab - cd - ghxyz}$$

- **`ab`**: Đúng 2 ký tự số (`00` - `99`) — Mã định danh Tỉnh/Thành phố trực thuộc trung ương (VD: `59` = TP.HCM, `29` = Hà Nội, `61` = Bình Dương).
- **`-`**: Dấu gạch nối phân cách đơn vị.
- **`cd`**: Đúng 2 ký tự chữ/số viết hoa (`A-Z`, `0-9`) — Mã đơn vị hành chính cấp Quận/Huyện/Xã (VD: `X1`, `T2`, `AB`).
- **`-`**: Dấu gạch nối phân cách.
- **`ghxyz`**: Đúng 5 ký tự số liên tiếp (`00000` - `99999`) — Số thứ tự đăng ký định danh phương tiện.

*Ví dụ hợp lệ*: `59-X1-12345`, `29-A1-99988`, `61-B2-04567`.

---

## 4. Bốn Chức Năng Cốt Lõi & Lưu Trữ File

1. **Đăng ký biển số mới**: Kiểm tra tính hợp lệ cú pháp (`ab-cd-ghxyz`), tính mã băm kiểm tra trùng lặp trong $O(1)$, cấp phát động `PlateNode` và chèn vào danh sách liên kết tại bucket tương ứng.
2. **Tra cứu một biển số**: Băm chuỗi biển số cần tìm, nhảy thẳng đến bucket đích trong $O(1)$, duyệt danh sách liên kết cục bộ để trích xuất thông tin chủ xe và ngày đăng ký.
3. **Hiển thị danh sách mã Xã của một Tỉnh**: Nhập mã tỉnh `ab`, lọc toàn bộ bảng băm để xuất ra danh sách các mã xã `cd` duy nhất (khử trùng lặp).
4. **Hiển thị danh sách biển số của một Xã**: Nhập cặp mã Tỉnh `ab` và mã Xã `cd`, lọc và xuất toàn bộ các biển số đã đăng ký tại địa bàn xã.
5. **Nạp & Lưu File TXT**: Hỗ trợ tải dữ liệu từ tệp TXT trong thư mục `data/` khi khởi chạy và lưu ngược lại file khi thoát chương trình.

---

## 5. Cấu Trúc Thư Mục Dự Án

```text
ctdl-gt-11/
├── include/                     # File tiêu đề (.h)
│   ├── Plate.h                  # Cấu trúc PlateNode & parse chuỗi
│   ├── HashTable.h              # Bảng băm & giải quyết đụng độ
│   ├── Validator.h              # Kiểm tra định dạng ab-cd-ghxyz
│   ├── FileIO.h                 # Đọc/ghi file TXT & sinh dataset
│   ├── AdministrativeFilter.h   # Lọc xã theo tỉnh & biển số theo xã
│   ├── MenuUI.h                 # Giao diện dòng lệnh console
│   └── Benchmark.h              # Đo thời gian thực thi (microsecond)
├── src/                         # Hiện thực mã nguồn (.cpp)
│   └── main.cpp                 # Starter empty template cho nhóm
├── docs/                        # Tài liệu kỹ thuật AI-First & Team
│   ├── README.md                # Danh mục tài liệu
│   ├── 01_project_overview.md   # Yêu cầu đề tài & rubric điểm
│   ├── 02_architecture_and_data_structures.md # Bảng băm & Big-O
│   ├── 03_module_specifications.md            # Đặc tả module C++
│   ├── 04_team_roles_and_git_workflow.md      # Phân công & Git Jira
│   ├── 05_ai_guidelines_and_report_sec6.md    # Dùng AI & Báo cáo Mục 6
│   ├── 06_testing_and_benchmarks.md           # 20 Test Cases & Đo lường
│   └── 07_oral_defense_and_live_coding.md     # Cẩm nang vấn đáp & sửa code
├── data/                        # Tệp dữ liệu (thành viên tự tạo khi làm task)
├── bin/                         # Chứa tệp thực thi sau khi build (bin/main)
├── .gitvn/                      # Bảng Kanban & Quản trị tiến độ GitVN
│   └── tasks.json               # 18 tasks phân công chi tiết 6 thành viên & 3 sprints
├── .agent/rules/                # Quy tắc dành cho Antigravity AI Agent
├── Makefile                     # Kịch bản biên dịch Make tự động
├── CMakeLists.txt               # Kịch bản biên dịch CMake đa nền tảng
├── AGENTS.md                    # Luật AI Agent cốt lõi
├── GEMINI.md                    # Hướng dẫn Gemini / Antigravity
├── .cursorrules                 # Cấu hình luật cho Cursor IDE
├── MEMBER_WORKFLOW.md           # Quy trình Git nội bộ nhóm
└── README.md                    # Tài liệu giới thiệu chính
```

---

## 6. Phân Công Thành Viên Nhóm 11 (6 Thành Viên)

| Thành viên | Trách nhiệm chính | Module phụ trách |
| :--- | :--- | :--- |
| **Nguyễn Hoàng Nam** | Trưởng nhóm, Thiết kế Bảng Băm & Thuật toán DJB2 Chaining | `Plate.h`, `HashTable.h`, `HashTable.cpp` |
| **Dư Quốc Hưng** | Quản lý File TXT I/O, Sinh Dataset 1.000 mẫu UTF-8 | `FileIO.h`, `FileIO.cpp`, `data/` |
| **Nguyễn Ngọc Hưng** | Thuật toán lọc đơn vị hành chính (Tỉnh / Xã, Distinct) | `AdministrativeFilter.h`, `AdministrativeFilter.cpp` |
| **Trịnh Quốc Thái** | Xây dựng Console Menu UI & Module Validator ngoại lệ | `MenuUI.h`, `MenuUI.cpp`, `Validator.h`, `Validator.cpp` |
| **Huỳnh Quốc Việt** | Kỹ sư QA, Thiết kế 20 Test Cases & Đo Benchmark | `Benchmark.h`, `Benchmark.cpp`, `tests/` |
| **Huỳnh Thị Khánh Ly** | Soạn thảo Báo cáo dự án (8 mục), Quản lý Mục 6 AI & Slide | `docs/`, `Bao_Cao_De_Tai_11.docx`, Slides |

---

## 7. Quản Lý Tiến Độ Bằng GitVN (Kanban & Sprints)

Toàn bộ tiến độ và phân công công việc của 6 thành viên được theo dõi trực tiếp trong dự án qua **GitVN** tại tệp [.gitvn/tasks.json](file:///home/hnamhocit/Workspace/cpp/ctdl-gt-11/.gitvn/tasks.json).

### 📋 Cách Xem & Nhận Nhiệm Vụ (Task):
- **Xem bảng trực quan**: Mở giao diện/tiện ích mở rộng **GitVN** trong IDE để xem bảng Kanban theo các cột `To Do`, `In Progress`, `Review`, `Done`.
- **Xem trực tiếp file cấu hình**: Mở tệp [.gitvn/tasks.json](file:///home/hnamhocit/Workspace/cpp/ctdl-gt-11/.gitvn/tasks.json) để tra cứu ID task được giao (`PROJ-1` đến `PROJ-18`), mục tiêu và tiêu chí nghiệm thu (Acceptance Criteria).

### ⏱️ Lộ Trình 3 Sprints (Thời hạn 16 ngày):
- **Sprint 1 (02/11 - 07/11/2026) - Kiến Trúc Cốt Lõi & Khởi Động** (Active):
  - `PROJ-1`, `PROJ-2`: Cấu trúc `PlateNode`, Bảng băm `LicensePlateHashTable`, băm DJB2 & Chaining (*Nguyễn Hoàng Nam*).
  - `PROJ-3`: Module `Validator` kiểm tra định dạng `ab-cd-ghxyz` (*Trịnh Quốc Thái*).
  - `PROJ-4`: Cấu trúc tệp dữ liệu TXT và module `FileIO` cơ bản (*Dư Quốc Hưng*).
  - `PROJ-5`: Khung giao diện dòng lệnh Console Menu `MenuUI` (*Trịnh Quốc Thái*).
  - `PROJ-6`: Dàn ý Báo cáo đồ án 8 chương & Mẫu ghi chép Mục 6 AI (*Huỳnh Thị Khánh Ly*).
- **Sprint 2 (08/11 - 13/11/2026) - Nghiệp Vụ, File I/O & Tích Hợp Hệ Thống** (Planned):
  - `PROJ-7`: Hoàn thiện Đăng ký mới & Tra cứu $O(1)$ (*Nguyễn Hoàng Nam*).
  - `PROJ-8`, `PROJ-9`: Nạp/lưu file TXT & sinh bộ 1.000 bản ghi mẫu (*Dư Quốc Hưng*).
  - `PROJ-10`, `PROJ-11`: Lọc danh sách xã của tỉnh & lọc danh sách biển số của xã (*Nguyễn Ngọc Hưng*).
  - `PROJ-12`: Tích hợp điều hướng Console Menu hoàn chỉnh (*Trịnh Quốc Thái*).
  - `PROJ-13`: Soạn thảo Mục 1 đến Mục 4 của Báo cáo đồ án (*Huỳnh Thị Khánh Ly*).
- **Sprint 3 (14/11 - 17/11/2026) - Kiểm Thử, Review Toàn Diện & Chuẩn Bị Vấn Đáp (4 Ngày Cuối)** (Planned):
  - `PROJ-14`, `PROJ-15`: 20 ca kiểm thử biên (Edge Cases), đo Benchmark chrono & kiểm tra rò rỉ Valgrind (*Huỳnh Quốc Việt*).
  - `PROJ-16`, `PROJ-17`: Hoàn thiện Mục 6 AI, tổng hợp Báo cáo 8 chương & Slide thuyết trình (*Huỳnh Thị Khánh Ly*).
  - `PROJ-18`: Code Review toàn hệ thống, tối ưu giải phóng bộ nhớ & Mock Interview Live Coding (*Nguyễn Hoàng Nam*).

---

## 8. Quy Trình Git & Jira / GitVN Workflow

Nhóm tuân thủ nghiêm ngặt nguyên tắc: **1 Task = 1 Branch = 1 Pull Request** theo tài liệu [MEMBER_WORKFLOW.md](file:///home/hnamhocit/Workspace/cpp/ctdl-gt-11/MEMBER_WORKFLOW.md).

1. Nhận Task trên GitVN (`PROJ-xxx`) $\rightarrow$ Chuyển trạng thái sang `In Progress`.
2. Tạo branch từ `develop`:
   ```bash
   git checkout develop
   git pull origin develop
   git checkout -b feature/PROJ-<id>-<ten-ngan>
   ```
3. Lập trình và commit theo chuẩn Conventional Commits:
   ```bash
   git commit -m "feat(hashtable): add DJB2 hashing and chaining search"
   ```
4. Push lên GitHub và tạo Pull Request vào nhánh `develop`.
5. Chuyển task sang `Review` và chờ trưởng nhóm duyệt merge.

---

## 9. Cẩm Nang Vấn Đáp & Tài Liệu AI-First

- Mọi thành viên phải đọc kỹ [docs/07_oral_defense_and_live_coding.md](file:///home/hnamhocit/Workspace/cpp/ctdl-gt-11/docs/07_oral_defense_and_live_coding.md) để nắm chắc 10 câu hỏi vấn đáp cốt lõi và 5 kịch bản live coding (sửa code tại chỗ).
- Chi tiết về khai thác AI và nhật ký prompt cho Mục 6 của Báo cáo được quy định tại [docs/05_ai_guidelines_and_report_sec6.md](file:///home/hnamhocit/Workspace/cpp/ctdl-gt-11/docs/05_ai_guidelines_and_report_sec6.md).
- Quy tắc điều hướng dành riêng cho AI Agent được lưu tại [AGENTS.md](file:///home/hnamhocit/Workspace/cpp/ctdl-gt-11/AGENTS.md), [GEMINI.md](file:///home/hnamhocit/Workspace/cpp/ctdl-gt-11/GEMINI.md), và [.cursorrules](file:///home/hnamhocit/Workspace/cpp/ctdl-gt-11/.cursorrules).
