# Kiến trúc hệ thống quản lý biển số xe

Đề tài 11 yêu cầu đăng ký biển số, tra cứu theo biển số đầy đủ, liệt kê mã xã của một tỉnh và liệt kê biển số của một xã. Chương trình dùng C++17 và có dữ liệu mẫu trong [data/](../data/).

## Dữ liệu và các module

Biển số có dạng `ab-cd-ghxyz`. `ab` gồm hai chữ số, `cd` gồm hai ký tự thuộc `A–Z` hoặc `0–9`, `ghxyz` gồm năm chữ số. Ví dụ `59-X1-12345` có mã tỉnh `59`, mã xã `X1` và số thứ tự `12345`. Mã xã được xác định cùng mã tỉnh khi lọc biển số. Tệp TXT dùng thứ tự trường `fullPlate|provinceCode|wardCode|serialNum|registeredDate|ownerName`.

| Module | Trách nhiệm |
|---|---|
| [Plate](../include/Plate.h) | `PlateNode` lưu biển số, mã tỉnh, mã xã, số thứ tự, ngày đăng ký và tên chủ xe bằng các mảng `char[]`. Con trỏ `next` nối các node trong cùng bucket. |
| [HashTable](../include/HashTable.h) | Quản lý bucket, chống chèn trùng, tra cứu, xóa và thu hồi bộ nhớ. |
| [Validator](../include/Validator.h) | Chuẩn hóa và kiểm tra đầu vào tương tác. |
| [AdministrativeFilter](../include/AdministrativeFilter.h) | Quét bảng để lọc tỉnh, xã và đếm bản ghi theo tỉnh. |
| [FileIO](../include/FileIO.h) | Đọc, kiểm tra và bỏ qua dòng TXT lỗi hoặc trùng. Ghi bản ghi và sinh dữ liệu mẫu. |
| [MenuUI](../include/MenuUI.h) | Đọc lựa chọn người dùng và gọi các module xử lý. |
| [Benchmark](../include/Benchmark.h) | Đo thời gian thực thi. |

[main.cpp](../src/main.cpp) nạp tệp mẫu, truyền bảng băm vào [MenuUI](../src/MenuUI.cpp) và lưu lại vào tệp mẫu khi số bản ghi thay đổi trong phiên.

Bộ sinh mẫu trong [FileIO.cpp](../src/FileIO.cpp) dùng 8 mã tỉnh và 8 mã xã mỗi tỉnh, tạo 64 cặp. Mỗi cặp có 100.000 số thứ tự từ `00000` đến `99999`, nên có tối đa 6.400.000 biển số khác nhau. Sau mỗi vòng qua 64 cặp, số thứ tự tăng thêm 7.919 theo modulo 100.000. Hai số nguyên tố cùng nhau nên số thứ tự chỉ lặp lại sau khi đã đi qua đủ 100.000 giá trị.

## Bảng băm và độ phức tạp

Bảng có `M = 10007` con trỏ bucket. DJB2 bắt đầu từ `5381`, cập nhật `hash = hash * 33 + c` cho mỗi ký tự, rồi lấy phần dư theo `M`. Các khóa cùng chỉ số bucket nằm trong một danh sách liên kết đơn. Khi chèn, chương trình dò trùng bằng khóa đầy đủ rồi nối node mới vào đầu danh sách. Hàm hủy gọi `clear()` để giải phóng từng node và đặt lại các bucket.

Gọi `N` là số bản ghi, `P` là số bản ghi của tỉnh đang lọc và `K` là số mã xã được giữ trong mảng tạm.

| Thao tác | Thời gian | Bộ nhớ phụ |
|---|---|---|
| Chèn, tra cứu, xóa theo biển số | Trung bình `O(1 + N/M)` khi khóa phân bố đều, xấu nhất `O(N)` | `O(1)` |
| Quét biển số theo xã, đếm theo tỉnh hoặc giải phóng bảng | `O(M + N)` | `O(1)` |
| Liệt kê mã xã duy nhất của một tỉnh | `O(M + N + P × K)` do dò tuyến tính trong mảng tạm | Mảng cố định 1.000 mã xã |

Tổng bộ nhớ của bảng là `O(M + N)`. Giới hạn 1.000 mã trong bộ lọc hiện tại thấp hơn 1.296 mã xã mà định dạng cho phép.
