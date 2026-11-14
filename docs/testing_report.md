# BÁO CÁO KIỂM THỬ & BENCHMARK — HUỲNH QUỐC VIỆT

## Phạm vi công việc
- PROJ-11: 20 Unit Test cho Validator và Hash Table.
- PROJ-17: Benchmark 100, 1.000, 10.000 bản ghi và kiểm tra Valgrind.

## Mã nguồn bàn giao
- `tests/test_proj11.cpp`
- `tests/benchmark_hash_table.cpp`
- `include/Benchmark.h`
- `src/Benchmark.cpp`
- `docs/06_testing_and_benchmarks.md`

## Cách thực thi
```bash
make test
make benchmark
make valgrind
```

## Kết quả
Các trạng thái và số liệu bên dưới được ghi từ lần chạy thực tế trong môi trường kiểm thử nêu tại báo cáo.

| Hạng mục | Trạng thái | Bằng chứng |
|---|---|---|
| 20 Unit Test | **20/20 PASS** | `make test` |
| Benchmark 100 / 1.000 / 10.000 | **Đã chạy** | `make benchmark` |
| CMake + CTest | **1/1 test PASS** | `cmake --build build -j2` và `ctest --test-dir build --output-on-failure` |
| Valgrind | **Chưa thực hiện** | Môi trường hiện tại chưa cài Valgrind (`command -v valgrind` không tìm thấy) |

## Môi trường đo
- Hệ điều hành: Linux x86_64.
- Compiler: g++ 14.2.0.
- Chuẩn: C++17; cờ `-Wall -Wextra -Wpedantic -O2`.
- Tra cứu: 100.000 lần cho mỗi kích thước dữ liệu.
- Đây là một lần chạy tham khảo; thời gian có thể thay đổi theo máy và tải hệ thống.

## Số liệu benchmark thực tế của lần chạy
| N | Alpha | Tổng thời gian chèn (us) | Chèn TB (ns/bản ghi) | Tra cứu TB (ns) |
|---:|---:|---:|---:|---:|
| 100 | 0,009993 | 25,788 | 257,880 | 9,457620 |
| 1.000 | 0,099930 | 134,462 | 134,462 | 10,424170 |
| 10.000 | 0,999300 | 1.432,144 | 143,214 | 9,336240 |

**Lưu ý:** thời gian chèn trung bình giảm khi N tăng trong lần chạy này do chi phí khởi động/đo và nhiễu hệ thống chi phối tập nhỏ; không nên suy luận tuyến tính từ một lần chạy. Cần chạy lặp nhiều lần trên máy báo cáo để có số liệu ổn định hơn.
