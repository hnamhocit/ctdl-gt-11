# Project Coding Guidelines & AI Agent Rules: Đề tài 11 - Quản lý Biển Số Xe (CTDL&GT)

> **Mục tiêu**: Đây là bộ quy tắc chỉ đạo (Directives) bắt buộc dành cho bất kỳ Trí tuệ Nhân tạo (AI Agent / Copilot / Gemini / Claude / Cursor) nào tham gia hỗ trợ dự án Đề tài 11 môn Cấu trúc Dữ liệu và Giải thuật.

---

## 1. Ràng Buộc Bắt Buộc Về Cấu Trúc Dữ Liệu (Non-Negotiable Invariants)

1. **TUYỆT ĐỐI KHÔNG DÙNG CONTAINER CÓ SẴN CHO LƯU TRỮ CỐT LÕI**:
   - **CẤM**: `#include <unordered_map>`, `#include <map>`, `#include <unordered_set>`, `#include <set>`, `#include <list>`.
   - Bảng băm phải được tự cài đặt hoàn toàn bằng con trỏ thuần: Mảng các con trỏ `PlateNode* buckets[TABLE_SIZE]`.
   - Kỹ thuật giải quyết đụng độ bắt buộc là **Separate Chaining** (Danh sách liên kết đơn nối trực tiếp tại mỗi bucket).

2. **Cấu Trúc `PlateNode`**:
   - Sử dụng mảng ký tự tĩnh `char[]` cho các trường dữ liệu cố định trong Node (`fullPlate[16]`, `provinceCode[4]`, `wardCode[4]`, `serialNum[8]`, `registeredDate[12]`, `ownerName[50]`, `PlateNode* next`).
   - Khởi tạo rõ ràng con trỏ `next = nullptr`.

3. **Kích thước Bảng Băm & Giải Thuật Băm**:
   - Kích thước mặc định: `TABLE_SIZE = 10007` (số nguyên tố).
   - Hàm băm chuỗi: Cài đặt giải thuật **DJB2** (`hash = ((hash << 5) + hash) + c`) hoặc Polynomial Rolling Hash.

---

## 2. Tiêu Chuẩn Ngôn Ngữ & Biên Dịch (C++ Standards)

- **Ngôn ngữ**: C++17 (tương thích C++11 đến C++20).
- **Trình biên dịch**: `g++` hoặc `clang++`.
- **Cờ biên dịch bắt buộc**: `-std=c++17 -Wall -Wextra -Wpedantic -O2`.
- Mã nguồn sinh ra phải **biên dịch sạch 100%**, không có warning hoặc error.

---

## 3. Quản Lý Bộ Nhớ & Ngăn Chặn Rò Rỉ (Memory Safety)

- Mọi thao tác cấp phát `new PlateNode()` phải có cơ chế thu hồi tương ứng.
- Phải hiện thực đầy đủ Destructor `~LicensePlateHashTable()` và hàm `clear()`.
- Trong hàm `clear()`, duyệt qua toàn bộ $10007$ buckets, duyệt từng phần tử trong danh sách liên kết và `delete` từng node, sau đó gán lại `nullptr`.

---

## 4. Quy Cách Dữ Liệu Biển Số Xe (`ab-cd-ghxyz`)

Mọi hàm xử lý, kiểm tra (Validator) và sinh dữ liệu mẫu phải tuân thủ chuẩn:
- `ab`: Đúng 2 ký tự số (`0-9`).
- `cd`: Đúng 2 ký tự chữ/số (`A-Z`, `0-9`). Chữ cái phải được viết hoa.
- `ghxyz`: Đúng 5 ký tự số (`0-9`).
- Ký tự phân cách: Dấu gạch nối `-` tại vị trí 2 và 5 (tổng chiều dài 11 ký tự).

---

## 5. Phong Cách Lập Trình & Sẵn Sàng Cho Vấn Đáp (Oral Defense Ready)

1. **Comment giải thích tường minh**:
   - Mỗi hàm phải có docstring nêu rõ: Mục đích, Tham số đầu vào, Giá trị trả về, Độ phức tạp thời gian $O(\dots)$ và không gian $O(\dots)$.
   - Các thao tác con trỏ phức tạp (chèn đầu, ngắt liên kết khi xóa, duyệt bucket) phải được chú thích từng bước để sinh viên có thể giải thích trôi chảy khi giảng viên chỉ vào code.
2. **Tách biệt Header (.h) và Implementation (.cpp)**:
   - Khai báo class/struct trong thư mục `include/`.
   - Hiện thực chi tiết phương thức trong thư mục `src/`.
   - Giữ hàm `main()` tinh gọn, tập trung vào điều hướng UI và khởi tạo.

---

## 6. Tuân Thủ Phân Công Nhóm & Quy Trình Git

- Tôn trọng phân công 6 thành viên trong [docs/04_team_roles_and_git_workflow.md](file:///home/hnamhocit/Workspace/cpp/ctdl-gt-11/docs/04_team_roles_and_git_workflow.md):
  - Nam: HashTable core
  - Dư Quốc Hưng: File I/O & dataset
  - Nguyễn Ngọc Hưng: Bộ lọc hành chính
  - Thái: UI & Validator
  - Việt: Test & Benchmark
  - Ly: Docs & Báo cáo
- Tên branch luôn theo format: `feature/PROJ-<id>-<short-name>`.
- Commit message theo chuẩn Conventional Commits (`feat:`, `fix:`, `docs:`, `test:`).
