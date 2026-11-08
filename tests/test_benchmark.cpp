#include <iostream>
#include <chrono>
#include "../include/HashTable.h"

// Module đo đạc hiệu năng thực thi do Huỳnh Quốc Việt thực hiện
void testPerformanceBenchmark(LicensePlateHashTable& table, const char* searchPlate) {
    auto start = std::chrono::high_resolution_clock::now();

    // Thực hiện hàm tra cứu biển số xe trong O(1)
    PlateNode* result = table.searchPlate(searchPlate);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;

    std::cout << "[BENCHMARK - HUYNH QUOC VIET] Tra cuu bien so: " << searchPlate << std::endl;
    std::cout << "-> Thoi gian thuc thi: " << duration.count() << " us (microseconds)" << std::endl;
}
