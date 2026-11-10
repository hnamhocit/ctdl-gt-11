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
    static long measureTime(std::function<void()> func, const std::string& label = "") {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        
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
    static long measureAverageTime(std::function<void()> func, 
                                 int iterations = 100, 
                                 const std::string& label = "") {
        long totalTime = 0;
        
        for (int i = 0; i < iterations; ++i) {
            totalTime += measureTime(func);
        }
        
        long averageTime = totalTime / iterations;
        
        if (!label.empty()) {
            std::cout << "[BENCHMARK] " << label << " (avg " 
                     << iterations << " runs): " << averageTime << " μs\n";
        }
        
        return averageTime;
    }
};

#endif // BENCHMARK_H
