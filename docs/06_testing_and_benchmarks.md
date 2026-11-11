# 06. Kiểm thử & Đo đạc hiệu năng (PROJ-11 / PROJ-17)

**Người phụ trách:** Huỳnh Quốc Việt  
**Phạm vi:** Kiểm thử `Validator`, `LicensePlateHashTable`; đo thời gian chèn/tra cứu và hướng dẫn kiểm tra bộ nhớ.

## 1. Bộ 20 Unit Test tự động

Mã kiểm thử độc lập: `tests/test_proj11.cpp`.

| Mã | Hạng mục | Dữ liệu / thao tác | Kết quả mong đợi |
|---|---|---|---|
| TC-01 | Biển số hợp lệ | `59-X1-12345` | Hợp lệ |
| TC-02 | Chữ thường | `59-x1-12345` qua normalize | Chuẩn hóa thành `59-X1-12345`, hợp lệ |
| TC-03 | Khoảng trắng | ` 59-X1-12345 ` | Chuẩn hóa, bỏ khoảng trắng |
| TC-04 | Sai độ dài | `59-X1-1234` | `INVALID_LENGTH` |
| TC-05 | Thiếu dấu gạch nối | `59X1-12345` | `MISSING_HYPHEN` |
| TC-06 | Mã tỉnh có chữ | `5A-X1-12345` | `INVALID_PROVINCE_CODE` |
| TC-07 | Mã tỉnh có ký tự lạ | `5.-X1-12345` | `INVALID_PROVINCE_CODE` |
| TC-08 | Mã xã có ký tự lạ | `59-X@-12345` | `INVALID_COMMUNE_CODE` |
| TC-09 | Serial có chữ | `59-X1-12A45` | `INVALID_SERIAL_NUMBER` |
| TC-10 | Serial quá dài | `59-X1-123456` | `INVALID_LENGTH` |
| TC-11 | Chuỗi rỗng | `""` | `INVALID_LENGTH` |
| TC-12 | Chỉ có khoảng trắng | `"           "` | `INVALID_LENGTH` |
| TC-13 | Khởi tạo bảng băm | Tạo bảng mới | Count = 0, alpha = 0 |
| TC-14 | Chèn bản ghi | `59-X1-12345` | Chèn thành công |
| TC-15 | Chèn trùng | Chèn lại `59-X1-12345` | Từ chối, count không tăng |
| TC-16 | Tra cứu có dữ liệu | `59-X1-12345` | Trả về node |
| TC-17 | Tra cứu không có dữ liệu | `99-Z9-99999` | Trả về `nullptr` |
| TC-18 | Chèn khóa rỗng | `PlateNode()` | Từ chối |
| TC-19 | Xóa bản ghi | Xóa `59-X1-12345` | Xóa thành công, count giảm |
| TC-20 | Xóa toàn bộ | Gọi `clear()` | Count = 0, không tìm thấy node cũ |

Chương trình in `PASS` hoặc `FAIL` cho từng mã và trả mã thoát khác 0 nếu có test thất bại. Các ca kiểm thử phản ánh hành vi hiện tại của Validator; hàm kiểm tra định dạng không tự chuẩn hóa đầu vào, vì vậy ca chữ thường phải gọi `normalizePlateString()` trước.

### Chạy Unit Test

```bash
make test
```

Riêng bộ 20 test:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude \
  src/Plate.cpp src/HashTable.cpp src/Validator.cpp \
  tests/test_proj11.cpp -o bin/test_proj11
./bin/test_proj11
```

## 2. Benchmark bảng băm

Mã nguồn: `tests/benchmark_hash_table.cpp`.

- Tập dữ liệu: 100, 1.000 và 10.000 biển số duy nhất, được sinh theo mẫu `ab-cd-ghxyz`.
- Đo chèn: tổng thời gian chèn toàn bộ tập dữ liệu và thời gian trung bình trên một bản ghi.
- Đo tra cứu: 100.000 lần tra cứu khóa đã tồn tại.
- Đồng hồ: `std::chrono::steady_clock` (đơn vị quy đổi microsecond/nanosecond).
- Hệ số tải: `alpha = số phần tử / 10007`.

### Chạy benchmark

```bash
make benchmark
```

Chương trình in trực tiếp bảng số liệu của lần chạy. Kết quả phụ thuộc CPU, hệ điều hành, compiler và tải hệ thống; không điền số liệu dự kiến thay cho kết quả đo thực tế.

### Kết quả tham khảo đã chạy trong môi trường Linux x86_64 / g++ 14.2.0

| N | Alpha | Tổng chèn (us) | Chèn TB (ns/bản ghi) | Tra cứu TB (ns) |
|---:|---:|---:|---:|---:|
| 100 | 0,009993 | 25,788 | 257,880 | 9,457620 |
| 1.000 | 0,099930 | 134,462 | 134,462 | 10,424170 |
| 10.000 | 0,999300 | 1.432,144 | 143,214 | 9,336240 |

Đây là một lần đo trong môi trường thực thi hiện tại, không phải cam kết hiệu năng trên mọi máy. Nên chạy lặp lại trên máy của nhóm trước khi chốt số liệu báo cáo.

| Số bản ghi | Alpha |
|---:|---:|
| 100 | 100 / 10007 ≈ 0,0100 |
| 1.000 | 1000 / 10007 ≈ 0,0999 |
| 10.000 | 10000 / 10007 ≈ 0,9993 |

## 3. Kiểm tra an toàn bộ nhớ bằng Valgrind

Chạy trên Linux (Ubuntu/WSL):

```bash
sudo apt update
sudo apt install valgrind
make valgrind
```

Lệnh Valgrind được cấu hình:

```bash
valgrind --leak-check=full --show-leak-kinds=all \
  --track-origins=yes --error-exitcode=1 ./bin/test_proj11
```

Chỉ kết luận không có lỗi/rò rỉ bộ nhớ khi log thực tế xác nhận `ERROR SUMMARY: 0 errors` và không còn khối `definitely lost`. Môi trường thực thi hiện tại chưa cài Valgrind, vì vậy hạng mục này vẫn cần chạy trên Ubuntu/WSL của nhóm.

## 4. Module Benchmark dùng chung

`include/Benchmark.h` khai báo và `src/Benchmark.cpp` hiện thực:

- `Benchmark::measureTime(...)`: đo một lần, trả microsecond.
- `Benchmark::measureAverageTime(...)`: chạy warm-up, đo nhiều lần và trả trung bình microsecond.

Benchmark chuyên biệt ở `tests/benchmark_hash_table.cpp` dùng nanosecond dạng số thực để tránh làm tròn mất độ phân giải ở các thao tác rất nhanh.

## 5. Ghi nhận kết quả thực nghiệm

Đã ghi nhận kết quả `make test` và `make benchmark` trong `docs/testing_report.md`. Valgrind chưa được cài trên môi trường hiện tại; cần chạy lệnh ở mục 3 trên Ubuntu/WSL và bổ sung log vào báo cáo trước khi xác nhận tiêu chí Valgrind hoàn tất.
