#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <string>
#include <functional>
#include <iostream>

class Benchmark {
public:
    /**
     * @brief Measures execution time of a function in microseconds
     * @param func Function to measure
     * @param label Description for output
     * @return Execution time in microseconds
     */
    static uint64_t measureTime(std::function<void()> func, const std::string& label = "") noexcept {
        if (!func) {
            if (!label.empty()) {
                std::cerr << "[BENCHMARK ERROR] Null function provided\n";
            }
            return 0;
        }

        auto start = std::chrono::steady_clock::now();
        func();
        auto end = std::chrono::steady_clock::now();
        
        auto duration = static_cast<uint64_t>(
            std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        );
        
        if (!label.empty()) {
            std::cout << "[BENCHMARK] " << label << ": " 
                     << duration << " μs\n";
        }
        
        return duration;
    }

    /**
     * @brief Measures average execution time over multiple runs
     * @param func Function to measure
     * @param iterations Number of runs
     * @param label Description for output
     * @return Average execution time in microseconds
     */
    static uint64_t measureAverageTime(std::function<void()> func,
                                     int iterations = 100,
                                     int warmup = 10,
                                     const std::string& label = "") noexcept {
        if (iterations <= 0) {
            if (!label.empty()) {
                std::cerr << "[BENCHMARK ERROR] Invalid iteration count: " << iterations << "\n";
            }
            return 0;
        }

        if (warmup < 0) {
            if (!label.empty()) {
                std::cerr << "[BENCHMARK WARNING] Negative warmup count, using 0\n";
            }
            warmup = 0;
        }
        
        // Warmup runs to account for cache effects
        for (int i = 0; i < warmup; ++i) {
            func();
        }

        uint64_t totalTime = 0;
        
        for (int i = 0; i < iterations; ++i) {
            totalTime += measureTime(func);
        }
        
        uint64_t averageTime = totalTime / iterations;
        
        if (!label.empty()) {
            std::cout << "[BENCHMARK] " << label << " (avg " 
                     << iterations << " runs): " << averageTime << " μs\n";
        }
        
        return averageTime;
    }
};

#endif // BENCHMARK_H
