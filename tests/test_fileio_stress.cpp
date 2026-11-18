#include "FileIO.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <iostream>

static int failures = 0;
#define CHECK(cond)                                                   \
    do                                                                \
    {                                                                 \
        if (!(cond))                                                  \
        {                                                             \
            ++failures;                                               \
            std::cout << "FAIL dòng " << __LINE__ << ": " #cond "\n"; \
        }                                                             \
    } while (0)

/** Đếm số biển số thuộc một tỉnh bằng cách duyệt toàn bộ bucket. */
static int countProvince(const LicensePlateHashTable &table, const char *code)
{
    int count = 0;
    for (int i = 0; i < table.getBucketSize(); ++i)
    {
        for (const PlateNode *n = table.getBucket(i); n != nullptr; n = n->next)
        {
            if (std::strcmp(n->provinceCode, code) == 0)
                ++count;
        }
    }
    return count;
}

int main()
{
    // A. 1000 bản ghi: không trùng, các tỉnh lớn phân bố đều
    CHECK(generateSampleDataset("stress_1000.txt", 1000));
    LicensePlateHashTable small;
    LoadStats ss;
    CHECK(loadFromFile(small, "stress_1000.txt", &ss));
    CHECK(ss.loaded == 1000 && ss.duplicates == 0 && ss.invalid == 0);

    const char *required[] = {"59", "29", "61", "43", "60"};
    for (const char *code : required)
    {
        int c = countProvince(small, code);
        std::cout << "Tinh " << code << ": " << c << " ban ghi\n";
        CHECK(c >= 112 && c <= 138); // chênh lệch không quá 10% so với 125
    }

    // B. Nhiều seed khác nhau vẫn không bao giờ trùng biển số
    const unsigned int seeds[] = {1, 2, 99999, 2026};
    for (unsigned int seed : seeds)
    {
        CHECK(generateSampleDataset("stress_seed.txt", 2000, seed));
        LicensePlateHashTable t;
        LoadStats st;
        CHECK(loadFromFile(t, "stress_seed.txt", &st));
        CHECK(st.loaded == 2000 && st.duplicates == 0 && st.invalid == 0);
    }

    // C. Stress test: nạp 10.000 bản ghi, trung vị 5 lần chạy phải dưới 100 ms
    const int N = 10000;
    CHECK(generateSampleDataset("stress_10000.txt", N));
    double times[5];
    for (int run = 0; run < 5; ++run)
    {
        LicensePlateHashTable table;
        LoadStats st;
        auto t0 = std::chrono::steady_clock::now();
        bool ok = loadFromFile(table, "stress_10000.txt", &st);
        auto t1 = std::chrono::steady_clock::now();
        times[run] = std::chrono::duration<double, std::milli>(t1 - t0).count();
        CHECK(ok && st.loaded == N && st.duplicates == 0 && st.invalid == 0);
        CHECK(table.getTotalCount() == N);
    }
    std::sort(times, times + 5);
    std::cout << "Nap 10000 ban ghi (ms): min " << times[0] << ", trung vi " << times[2]
              << ", max " << times[4] << "\n";
    CHECK(times[2] < 100.0);

    const char *temps[] = {"stress_1000.txt", "stress_seed.txt", "stress_10000.txt"};
    for (const char *name : temps)
        std::remove(name);

    std::cout << (failures == 0 ? "All FileIO stress tests passed\n" : "Co test that bai\n");
    return failures == 0 ? 0 : 1;
}