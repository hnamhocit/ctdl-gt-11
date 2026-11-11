#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <cstdint>
#include <functional>
#include <string>

class Benchmark {
public:
    /**
     * @brief Đo thời gian thực thi một hàm theo microsecond.
     * @param func Hàm cần đo.
     * @param label Nhãn mô tả kết quả (có thể để trống).
     * @return Thời gian thực thi, đơn vị microsecond; trả về 0 nếu hàm rỗng.
     * Time: O(T) với T là thời gian chạy của func; Space: O(1).
     */
    static std::uint64_t measureTime(
        const std::function<void()>& func,
        const std::string& label = "");

    /**
     * @brief Đo thời gian trung bình qua nhiều lần chạy, có warm-up.
     * @param func Hàm cần đo.
     * @param iterations Số lần đo chính thức, phải lớn hơn 0.
     * @param warmup Số lần chạy làm nóng, không tính vào kết quả.
     * @param label Nhãn mô tả kết quả (có thể để trống).
     * @return Thời gian trung bình microsecond; trả về 0 nếu tham số không hợp lệ.
     * Time: O((iterations + warmup) * T); Space: O(1).
     */
    static std::uint64_t measureAverageTime(
        const std::function<void()>& func,
        int iterations = 100,
        int warmup = 10,
        const std::string& label = "");
};

#endif // BENCHMARK_H
