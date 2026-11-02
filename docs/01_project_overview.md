# 01. Tổng Quan Dự Án & Yêu Cầu Kỹ Thuật (Project Overview)

> **Môn học**: Cấu trúc Dữ liệu và Giải thuật (CTDL&GT)  
> **Đề tài 11**: Hệ thống quản lý biển số xe (Hash Table & Linked List)  
> **Ngôn ngữ**: C++ (Chuẩn C++17 / C++11)  
> **Nhóm thực hiện**: Nhóm 11 (6 thành viên)

---

## 1. Bối Cảnh & Mục Tiêu Đề Tài

Trong bối cảnh chuyển đổi số và quản lý giao thông thông minh, việc tra cứu, quản lý và thống kê hàng triệu biển số xe cơ giới đòi hỏi hệ thống lưu trữ phải đạt tốc độ truy xuất gần như tức thời ($O(1)$). Nếu sử dụng danh sách tuyến tính hoặc mảng thông thường, thao tác tìm kiếm sẽ tốn $O(n)$, gây nghẽn cổ chai khi dữ liệu lớn.

Đề tài 11 yêu cầu xây dựng một ứng dụng C++ độc lập, tự cài đặt cấu trúc dữ liệu **Bảng băm (Hash Table)** kết hợp với **Danh sách liên kết đơn (Singly Linked List - Separate Chaining)** để định danh, tra cứu và phân loại biển số xe theo địa bàn hành chính.

---

## 2. Các Ràng Buộc Kỹ Thuật Tiên Quyết (Non-Negotiable Constraints)

Các ràng buộc này là tiêu chí sống còn khi chấm điểm và vấn đáp:

1. **Ngôn ngữ lập trình**: 100% C++. Tuyệt đối không dùng Java, Python, C# hay JavaScript.
2. **Cấu trúc dữ liệu cốt lõi**:
   - **Tự cài đặt (Handcrafted)** từ con trỏ thuần: Cấu trúc `PlateNode`, Danh sách liên kết đơn, và Bảng băm `LicensePlateHashTable`.
   - **CẤM HOÀN TOÀN**: Sử dụng thư viện container có sẵn như `std::unordered_map`, `std::map`, `std::unordered_set`, `std::set`, hoặc `std::list` để lưu trữ dữ liệu biển số. (Vi phạm sẽ bị đánh giá trượt phần CTDL cốt lõi).
3. **Quản lý bộ nhớ**: Mọi node được cấp phát động (`new`) phải được giải phóng sạch sẽ (`delete`) khi xóa hoặc khi hủy bảng (`destructor`), tuyệt đối không để rò rỉ bộ nhớ (Memory Leak).
4. **Biên dịch sạch**: Biên dịch với `g++` hoặc `clang++` cờ `-Wall -Wextra -O2` không có lỗi và không có cảnh báo nghiêm trọng.

---

## 3. Quy Cách Chuẩn Hóa Biển Số Xe (`ab-cd-ghxyz`)

Biển số xe trong hệ thống phải tuân thủ nghiêm ngặt định dạng chuỗi:

$$\text{ab-cd-ghxyz}$$

| Thành phần | Định dạng quy ước | Chiều dài | Ý nghĩa & Quy tắc kiểm tra | Ví dụ |
| :--- | :--- | :---: | :--- | :--- |
| **`ab`** | 2 chữ số (`0-9`) | 2 | Mã định danh Tỉnh / Thành phố trực thuộc trung ương | `59` (TP.HCM), `29` (Hà Nội), `61` (Bình Dương) |
| **`-`** | Dấu gạch nối đơn | 1 | Ký tự phân cách phần 1 | `-` |
| **`cd`** | 2 ký tự chữ/số (`A-Z`, `0-9`) | 2 | Mã phân cấp đơn vị hành chính cấp Quận/Huyện/Xã | `X1`, `T2`, `AB`, `C3` |
| **`-`** | Dấu gạch nối đơn | 1 | Ký tự phân cách phần 2 | `-` |
| **`ghxyz`** | 5 chữ số (`0-9`) | 5 | Số thứ tự phương tiện đăng ký (dãy 5 số liên tiếp) | `12345`, `04567`, `99999` |

- **Tổng độ dài chuỗi chuẩn**: Đúng $11$ ký tự (không tính ký tự kết thúc chuỗi `\0`).
- **Ví dụ biển số hợp lệ**: `59-X1-12345`, `29-A1-99988`, `61-B2-04567`, `43-C1-00001`.
- **Ví dụ biển số không hợp lệ**: `59X112345` (thiếu gạch nối), `5A-X1-12345` (mã tỉnh có chữ), `59-X1-1234` (thiếu số thứ tự), `59-X1-123456` (dư số thứ tự).

---

## 4. Bốn Chức Năng Cốt Lõi & Yêu Cầu File I/O

### 4.1. Chức năng 1: Đăng ký biển số mới
- **Đầu vào**: Chuỗi biển số `ab-cd-ghxyz`, tên chủ xe, ngày đăng ký (tùy chọn mở rộng).
- **Quy trình xử lý**:
  1. Kiểm tra tính hợp lệ cú pháp (Validate định dạng chuẩn).
  2. Băm chuỗi để tra cứu trong bảng băm xem biển số đã tồn tại hay chưa.
  3. Nếu đã tồn tại: Báo lỗi "Biển số đã được đăng ký", không cho phép ghi đè.
  4. Nếu chưa tồn tại: Cấp phát dynamic node mới, chèn vào đầu (hoặc cuối) danh sách liên kết tại bucket tương ứng với độ phức tạp $O(1)$.
  5. Tăng biến đếm tổng số bản ghi `totalCount`.

### 4.2. Chức năng 2: Tra cứu một biển số
- **Đầu vào**: Chuỗi biển số cần tìm (VD: `59-X1-12345`).
- **Quy trình xử lý**:
  1. Tính chỉ số băm `hashIndex = hashFunction(plate)`.
  2. Truy cập trực tiếp bucket tương ứng trong $O(1)$.
  3. Duyệt danh sách liên kết tại bucket đó để so khớp chuỗi `fullPlate`.
  4. Trả về con trỏ `PlateNode*` chứa đầy đủ thông tin hoặc `nullptr` nếu không tồn tại.

### 4.3. Chức năng 3: Hiển thị danh sách mã Xã của một Tỉnh
- **Đầu vào**: Mã tỉnh `ab` (VD: `59`).
- **Quy trình xử lý**:
  1. Duyệt toàn bộ các bucket của bảng băm.
  2. Lọc ra tất cả các bản ghi có `provinceCode == ab`.
  3. Thu thập các mã xã `cd` duy nhất (**loại bỏ trùng lặp / distinct**).
  4. Xuất danh sách các mã xã trực thuộc tỉnh ra màn hình.

### 4.4. Chức năng 4: Hiển thị danh sách biển số của một Xã
- **Đầu vào**: Cặp mã Tỉnh `ab` và mã Xã `cd` (VD: Tỉnh `59`, Xã `X1`).
- **Quy trình xử lý**:
  1. Duyệt các node trong bảng băm khớp đồng thời `provinceCode == ab` và `wardCode == cd`.
  2. In danh sách chi tiết tất cả các biển số xe đã được cấp phát cho xã đó theo dạng bảng rõ ràng.

### 4.5. Chức năng Phụ trợ: Lưu trữ & Nạp Dữ Liệu File TXT
- Hệ thống tự động nạp dữ liệu từ file `data/sample_plates.txt` khi khởi động ứng dụng.
- Tự động lưu toàn bộ dữ liệu hiện hành ngược lại file TXT khi thoát ứng dụng hoặc khi người dùng chọn lệnh lưu.
- Bộ dữ liệu mẫu phải có ít nhất **100 đến 1.000 bản ghi** hợp lệ để phục vụ demo và kiểm thử hiệu năng.

---

## 5. Cơ Cấu Đánh Giá & Rubric Điểm (Thang Điểm 10)

Tổng điểm môn học được tính theo công thức:

$$\text{Điểm} = 0.20 \times D_{\text{quy\_trình}} + 0.10 \times D_{\text{phối\_hợp\_nhóm}} + 0.30 \times D_{\text{sản\_phẩm}} + 0.10 \times D_{\text{đóng\_góp\_cá\_nhân}} + 0.30 \times D_{\text{vấn\_đáp\_cá\_nhân}}$$

- **Điểm Nhóm (60%)**: Quy trình (20%) + Phối hợp nhóm (10%) + Sản phẩm & Báo cáo (30%).
- **Điểm Cá Nhân (40%)**: Đóng góp thực tế (10%) + Vấn đáp & Live Coding cá nhân (30%).

> [!IMPORTANT]
> Điểm vấn đáp được chấm hoàn toàn độc lập. Mọi thành viên phải nắm chắc 100% dòng code trong dự án và có khả năng sửa code tại chỗ theo yêu cầu của giảng viên.
