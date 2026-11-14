#include "Benchmark.h"

#include <chrono>
#include <iostream>

std::uint64_t Benchmark::measureTime(
    const std::function<void()>& func, const std::string& label) {
    if (!func) {
        if (!label.empty()) {
            std::cerr << "[BENCHMARK ERROR] Function rong.\n";
        }
        return 0;
    }

    const auto start = std::chrono::steady_clock::now();
    func();
    const auto finish = std::chrono::steady_clock::now();
    const auto elapsed = static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());

    if (!label.empty()) {
        std::cout << "[BENCHMARK] " << label << ": " << elapsed << " us\n";
    }
    return elapsed;
}

std::uint64_t Benchmark::measureAverageTime(
    const std::function<void()>& func, int iterations, int warmup,
    const std::string& label) {
    if (!func || iterations <= 0 || warmup < 0) {
        if (!label.empty()) {
            std::cerr << "[BENCHMARK ERROR] Function/iterations/warmup khong hop le.\n";
        }
        return 0;
    }

    for (int i = 0; i < warmup; ++i) {
        func();
    }

    std::uint64_t total = 0;
    for (int i = 0; i < iterations; ++i) {
        total += measureTime(func);
    }

    const std::uint64_t average = total / static_cast<std::uint64_t>(iterations);
    if (!label.empty()) {
        std::cout << "[BENCHMARK] " << label << " (avg " << iterations
                  << " runs): " << average << " us\n";
    }
    return average;
}
