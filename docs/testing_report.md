# BÁO CÁO KIỂM THỬ HỆ THỐNG & BENCHMARK HIỆU NĂNG
**Người thực hiện:** Huỳnh Quốc Việt
**Phân công:** Kiểm thử hệ thống (Testing) & Đo đạc hiệu năng

## 1. Bảng 20 Ca kiểm thử (Test Cases)
| STT | Mã Test | Chức năng | Dữ liệu đầu vào | Kết quả mong đợi | Trạng thái |
|---|---|---|---|---|---|
| 1 | TEST_REG_01 | Đăng ký hợp lệ | 59-X1-12345 | Thêm thành công vào Bảng băm | Pass |
| 2 | TEST_REG_02 | Đăng ký trùng | 59-X1-12345 | Báo lỗi biển số đã tồn tại | Pass |
| 3 | TEST_REG_03 | Sai định dạng | 59-X1-1234 | Báo lỗi thiếu chữ số | Pass |
| 4 | TEST_REG_04 | Sai định dạng | 59_X1_12345 | Báo lỗi sai dấu phân cách | Pass |
| 5 | TEST_REG_05 | Sai định dạng | ABC-X1-12345 | Báo lỗi mã tỉnh phải là số | Pass |
| 6 | TEST_SEARCH_01 | Tra cứu có dữ liệu | 59-X1-12345 | Trả về thông tin trong O(1) | Pass |
| 7 | TEST_SEARCH_02 | Tra cứu không có | 99-Z9-99999 | Thông báo không tìm thấy | Pass |
| 8 | TEST_SEARCH_03 | Tra cứu chuỗi rỗng | "" | Báo lỗi chưa nhập dữ liệu | Pass |
| 9 | TEST_PROV_01 | Lọc Xã theo Tỉnh | Mã tỉnh 59 | Liệt kê danh sách các mã Xã | Pass |
| 10 | TEST_PROV_02 | Tỉnh chưa có dữ liệu| Mã tỉnh 99 | Thông báo chưa có dữ liệu | Pass |
| 11 | TEST_PROV_03 | Nhập sai Mã Tỉnh | Mã tỉnh 599 | Báo lỗi độ dài mã tỉnh | Pass |
| 12 | TEST_WARD_01 | Lọc biển theo Xã | Tỉnh 59, Xã X1 | Xuất toàn bộ biển số của Xã | Pass |
| 13 | TEST_WARD_02 | Xã chưa có biển | Tỉnh 59, Xã Z9 | Thông báo chưa có biển số | Pass |
| 14 | TEST_WARD_03 | Nhập sai Mã Xã | Tỉnh 59, Xã X12 | Báo lỗi độ dài mã xã | Pass |
| 15 | TEST_FILE_01 | Tự đọc File | Khởi chạy ứng dụng | Nạp thành công data.txt | Pass |
| 16 | TEST_FILE_02 | Tự ghi File | Thoát ứng dụng | Lưu Bảng băm vào data.txt | Pass |
| 17 | TEST_FILE_03 | Thiếu File data | Xóa file data.txt | Tự khởi tạo file rỗng mới | Pass |
| 18 | TEST_PERF_01 | Benchmark 100 dòng | Tra cứu 1 biển | Thời gian < 0.01 ms | Pass |
| 19 | TEST_PERF_02 | Benchmark 1.000 dòng| Tra cứu 1 biển | Thời gian < 0.01 ms | Pass |
| 20 | TEST_PERF_03 | Benchmark 10.000 dòng| Tra cứu 1 biển | Thời gian < 0.02 ms (O(1)) | Pass |
