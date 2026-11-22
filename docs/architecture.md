# Kiến trúc hệ thống quản lý biển số xe

Đề tài 11 yêu cầu đăng ký biển số, tra cứu theo biển số đầy đủ, liệt kê mã xã của một tỉnh và liệt kê biển số của một xã. Chương trình dùng C++17 và có dữ liệu mẫu trong [data/](../data/).

## Dữ liệu và các module

Biển số có dạng `ab-cd-ghxyz`. `ab` gồm hai chữ số, `cd` gồm hai ký tự thuộc `A–Z` hoặc `0–9`, `ghxyz` gồm năm chữ số. Mã xã được xác định cùng mã tỉnh khi lọc biển số. Tệp TXT dùng thứ tự trường `fullPlate|provinceCode|wardCode|serialNum|registeredDate|ownerName`.

| Module | Trách nhiệm |
|---|---|
| [Plate](../include/Plate.h) | `PlateNode` lưu biển số, mã tỉnh, mã xã, số thứ tự, ngày đăng ký và tên chủ xe bằng các mảng `char[]`. Con trỏ `next` nối các node trong cùng bucket. |
| [HashTable](../include/HashTable.h) | Quản lý bucket, chống chèn trùng, tra cứu, xóa và thu hồi bộ nhớ. |
| [Validator](../include/Validator.h) | Chuẩn hóa và kiểm tra đầu vào tương tác. |
| [AdministrativeFilter](../include/AdministrativeFilter.h) | Quét bảng để lọc tỉnh, xã và đếm bản ghi theo tỉnh. |
| [FileIO](../include/FileIO.h) | Đọc, kiểm tra và bỏ qua dòng TXT lỗi hoặc trùng. Ghi bản ghi và sinh dữ liệu mẫu. |
| [MenuUI](../include/MenuUI.h) | Đọc lựa chọn người dùng và gọi các module xử lý. |
| [Benchmark](../include/Benchmark.h) | Đo thời gian thực thi. |

Tại commit `c508041`, [main.cpp](../src/main.cpp) nạp file mẫu vào bảng băm và lưu khi thoát. [MenuUI.cpp](../src/MenuUI.cpp) có vòng lặp giao diện riêng, hiện chưa được gọi từ `main()`.

## Bảng băm và độ phức tạp

Bảng có `M = 10007` con trỏ bucket. DJB2 bắt đầu từ `5381`, cập nhật `hash = hash * 33 + c` cho mỗi ký tự, rồi lấy phần dư theo `M`. Các khóa cùng chỉ số bucket nằm trong một danh sách liên kết đơn. Khi chèn, chương trình dò trùng bằng khóa đầy đủ rồi nối node mới vào đầu danh sách. Hàm hủy gọi `clear()` để giải phóng từng node và đặt lại các bucket.

Gọi `N` là số bản ghi, `P` là số bản ghi của tỉnh đang lọc và `K` là số mã xã được giữ trong mảng tạm.

| Thao tác | Thời gian | Bộ nhớ phụ |
|---|---|---|
| Chèn, tra cứu, xóa theo biển số | Trung bình `O(1 + N/M)` khi khóa phân bố đều, xấu nhất `O(N)` | `O(1)` |
| Quét biển số theo xã, đếm theo tỉnh hoặc giải phóng bảng | `O(M + N)` | `O(1)` |
| Liệt kê mã xã duy nhất của một tỉnh | `O(M + N + P × K)` do dò tuyến tính trong mảng tạm | Mảng cố định 1.000 mã xã |

Tổng bộ nhớ của bảng là `O(M + N)`. Giới hạn 1.000 mã trong bộ lọc hiện tại thấp hơn 1.296 mã xã mà định dạng cho phép.
