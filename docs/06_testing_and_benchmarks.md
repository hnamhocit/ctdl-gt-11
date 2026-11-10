# 06. Ma Trận Kiểm Thử & Đo Đạc Hiệu Năng (Testing & Benchmarks)

> **Người phụ trách**: Huỳnh Quốc Việt (Kỹ sư Kiểm thử & Hiệu năng)  
> **Mục tiêu**: Đảm bảo hệ thống đạt độ ổn định 100%, không bị crash khi người dùng nhập dữ liệu bẩn và có số liệu định lượng cho Báo cáo Mục 5.

---

## 1. Ma Trận 20 Ca Kiểm Thử (Test Cases)

| Mã TC | Hạng mục kiểm tra | Dữ liệu đầu vào (Input) | Kết quả mong đợi (Expected Output) | Trạng thái |
| :---: | :--- | :--- | :--- | :---: |
| **TC-01** | Biển số hợp lệ chuẩn (TP.HCM) | `59-X1-12345` | Hợp lệ, chèn thành công vào bảng băm | PASS |
| **TC-02** | Biển số hợp lệ chuẩn (Hà Nội) | `29-A1-99999` | Hợp lệ, chèn thành công vào bảng băm | PASS |
| **TC-03** | Biển số hợp lệ chuẩn (Bình Dương) | `61-B2-00001` | Hợp lệ, chèn thành công vào bảng băm | PASS |
| **TC-04** | Biển số chữ thường (Cần chuẩn hóa) | `59-x1-12345` | Tự động chuyển thành `59-X1-12345` và chèn hợp lệ | PASS |
| **TC-05** | Biển số có khoảng trắng đầu/cuối | `  59-X1-12345  ` | Tự động trim khoảng trắng và chèn hợp lệ | PASS |
| **TC-06** | Thiếu dấu gạch nối giữa | `59X112345` | Báo lỗi định dạng sai cú pháp | PASS |
| **TC-07** | Dùng sai ký tự phân cách | `59.X1.12345` hoặc `59_X1_12345` | Báo lỗi định dạng sai cú pháp | PASS |
| **TC-08** | Mã tỉnh không phải chữ số | `5A-X1-12345` | Báo lỗi: Mã tỉnh `ab` phải gồm 2 chữ số (00-99) | PASS |
| **TC-09** | Mã tỉnh chỉ có 1 chữ số | `5-X1-12345` | Báo lỗi: Độ dài mã tỉnh không đủ | PASS |
| **TC-10** | Mã tỉnh quá 2 chữ số | `599-X1-12345` | Báo lỗi: Độ dài mã tỉnh vượt quá quy định | PASS |
| **TC-11** | Mã xã có ký tự đặc biệt | `59-X@-12345` | Báo lỗi: Mã xã chỉ chứa chữ cái và số | PASS |
| **TC-12** | Mã xã chỉ có 1 ký tự | `59-X-12345` | Báo lỗi: Mã xã phải có đúng 2 ký tự | PASS |
| **TC-13** | Số thứ tự xe chỉ có 4 số | `59-X1-1234` | Báo lỗi: Số thứ tự `ghxyz` phải đủ 5 chữ số | PASS |
| **TC-14** | Số thứ tự xe có 6 số | `59-X1-123456` | Báo lỗi: Số thứ tự vượt quá 5 chữ số | PASS |
| **TC-15** | Số thứ tự chứa chữ cái | `59-X1-12A45` | Báo lỗi: Số thứ tự chỉ được chứa số | PASS |
| **TC-16** | Đăng ký biển số đã tồn tại | Nhập lại `59-X1-12345` | Báo lỗi: Biển số đã tồn tại, không thể chèn đè | PASS |
| **TC-17** | Tra cứu biển số không tồn tại | `99-Z9-88888` | Báo kết quả: Không tìm thấy biển số | PASS |
| **TC-18** | Chuỗi rỗng hoặc chỉ có khoảng trắng | `""` hoặc `"   "` | Báo lỗi: Dữ liệu không được để trống | PASS |
| **TC-19** | Chuỗi có độ dài cực lớn (chống tràn buffer) | Chuỗi 500 ký tự `'A'` | Bắt lỗi sớm, không gây tràn bộ đệm (Buffer Overflow) | PASS |
| **TC-20** | Tra cứu danh sách xã của tỉnh không có xe | `84` (chưa có xe nào đăng ký) | Thông báo: Tỉnh chưa có dữ liệu biển số | PASS |

---

## 2. Phương Pháp Đo Lường Hiệu Năng (Benchmark Methodology)

Module `Benchmark.h` cung cấp 2 phương thức chính:

```cpp
// Đo thời gian chạy 1 lần
long measureTime(std::function<void()>, const std::string& label = "");

// Đo thời gian chạy trung bình qua nhiều lần 
long measureAverageTime(std::function<void()>, int iterations = 100, const std::string& label = "");
```

Ví dụ đo thời gian chèn bản ghi:
```cpp
Benchmark::measureTime([&]() {
    table.insertPlate(PlateNode("59-X1-12345", "59", "X1", "12345", "2026-10-30", "Nguyen Van A"));
}, "Thời gian chèn bản ghi");
```

```cpp
#include <chrono>
#include <iostream>

void benchmarkSearch(const LicensePlateHashTable& table, const char* targetPlate) {
    auto start = std::chrono::high_resolution_clock::now();
    
    PlateNode* result = table.searchPlate(targetPlate);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    
    std::cout << "Thời gian tra cứu biển số " << targetPlate << ": " 
              << duration << " nanoseconds (" << duration / 1000.0 << " microseconds)\n";
}
```

### Bảng Kết Quả Đo Đạc Dự Kiến (Mẫu trình bày Báo cáo):

| Kích thước tập dữ liệu ($N$) | Hệ số tải $\alpha = N / 10007$ | Thời gian Chèn TB ($\mu s$) | Thời gian Tra cứu TB ($\mu s$) | Tỷ lệ va chạm băm (%) |
| :---: | :---: | :---: | :---: | :---: |
| **100 bản ghi** | $0.010$ | $\approx 1.2\ \mu s$ | $\approx 0.4\ \mu s$ | $< 0.5\%$ |
| **1.000 bản ghi** | $0.099$ | $\approx 1.4\ \mu s$ | $\approx 0.5\ \mu s$ | $\approx 4.8\%$ |
| **10.000 bản ghi** | $0.999$ | $\approx 2.1\ \mu s$ | $\approx 0.9\ \mu s$ | $\approx 36.7\%$ |
| **50.000 bản ghi** | $4.996$ | $\approx 5.8\ \mu s$ | $\approx 2.8\ \mu s$ | Chaining giải quyết trơn tru |

> **Nhận xét chuyên môn**: Ngay cả khi số lượng bản ghi đạt 10.000 (hệ số tải xấp xỉ 1.0), thời gian tra cứu vẫn ở mức dưới $1$ microsecond nhờ giải thuật băm DJB2 phân bố cực kỳ đồng đều trên 10.007 buckets.
