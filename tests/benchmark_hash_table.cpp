#include "Benchmark.h"
#include "HashTable.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

namespace {
std::string makePlate(int index) {
    char buffer[16];
    const int province = (index / 100000) % 100;
    const int ward = (index / 1000) % 100;
    const int serial = index % 100000;
    const char wardChars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char first = wardChars[(ward / 36) % 36];
    const char second = wardChars[ward % 36];
    std::snprintf(buffer, sizeof(buffer), "%02d-%c%c-%05d",
                  province, first, second, serial);
    return std::string(buffer);
}

void printBenchmarkRow(int count) {
    LicensePlateHashTable table;

    const auto insertStart = std::chrono::steady_clock::now();
    for (int i = 0; i < count; ++i) {
        const std::string plate = makePlate(i);
        PlateNode node;
        if (!parseLicensePlate(plate.c_str(), node) || !table.insertPlate(node)) {
            std::cerr << "Loi tao/chen du lieu tai index " << i << '\n';
            std::exit(1);
        }
    }
    const auto insertEnd = std::chrono::steady_clock::now();

    const std::string target = makePlate(count - 1);
    constexpr int SEARCH_ITERATIONS = 100000;
    volatile int foundCount = 0;
    const auto searchStart = std::chrono::steady_clock::now();
    for (int i = 0; i < SEARCH_ITERATIONS; ++i) {
        if (table.searchPlate(target.c_str()) != nullptr) {
            ++foundCount;
        }
    }
    const auto searchEnd = std::chrono::steady_clock::now();

    const double insertTotalUs =
        std::chrono::duration<double, std::micro>(insertEnd - insertStart).count();
    const double insertAverageNs = insertTotalUs * 1000.0 / count;
    const double searchAverageNs =
        std::chrono::duration<double, std::nano>(searchEnd - searchStart).count()
        / SEARCH_ITERATIONS;

    std::cout << std::left << std::setw(10) << count
              << std::right << std::setw(12) << std::fixed << std::setprecision(6)
              << table.getLoadFactor()
              << std::setw(18) << insertTotalUs
              << std::setw(20) << insertAverageNs
              << std::setw(20) << searchAverageNs
              << std::setw(14) << foundCount << '\n';
}
} // namespace

int main() {
    std::cout << "PROJ-17 - HASH TABLE BENCHMARK (steady_clock)\n";
    std::cout << "Compiler: " << __VERSION__ << " | C++ standard: C++17\n";
    std::cout << "Search iterations per dataset: 100000\n\n";
    std::cout << std::left << std::setw(10) << "Records"
              << std::right << std::setw(12) << "Alpha"
              << std::setw(18) << "Insert total us"
              << std::setw(20) << "Insert avg ns"
              << std::setw(20) << "Search avg ns"
              << std::setw(14) << "Found" << '\n';
    std::cout << std::string(94, '-') << '\n';

    printBenchmarkRow(100);
    printBenchmarkRow(1000);
    printBenchmarkRow(10000);
    return 0;
}
