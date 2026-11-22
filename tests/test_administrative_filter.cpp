#include "HashTable.h"
#include "AdministrativeFilter.h"
#include <iostream>
#include <cassert>

static int failures = 0;
#define CHECK(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "FAIL [" << __FILE__ << ":" << __LINE__ << "]: " << msg << "\n"; \
            failures++; \
        } else { \
            std::cout << "PASS: " << msg << "\n"; \
        } \
    } while (0)

int main() {
    std::cout << "========================================\n";
    std::cout << "KIEM THU TOAN DIEN: AdministrativeFilter\n";
    std::cout << "========================================\n";

    LicensePlateHashTable ht;
    AdministrativeFilter filter;

    // ------------------------------------------------------------------
    // 1. Kiem thu tren bang bam rong & tham so bien (Edge Cases)
    // ------------------------------------------------------------------
    CHECK(filter.countPlatesInProvince(ht, "59") == 0, "Bang rong: Dem bien so tinh 59 bang 0");
    CHECK(filter.countPlatesInProvince(ht, nullptr) == 0, "Tham so nullptr: Tra ve 0 an toan");

    std::cout << "\n--- Test displayProvinceStatistics (Bang rong) ---\n";
    filter.displayProvinceStatistics(ht);

    std::cout << "\n--- Test displayPlatesOfWard (Bang rong) ---\n";
    filter.displayPlatesOfWard(ht, "59", "X1");
    filter.displayPlatesOfWard(ht, nullptr, "X1");

    // ------------------------------------------------------------------
    // 2. Nap du lieu mau vao bang bam
    // ------------------------------------------------------------------
    PlateNode p1("59-X1-12345", "59", "X1", "12345", "2026-10-30", "Nguyen Van An");
    PlateNode p2("59-X1-67890", "59", "X1", "67890", "2026-10-30", "Tran Thi B");
    PlateNode p3("59-X2-11111", "59", "X2", "11111", "2026-10-31", "Le Van C");
    PlateNode p4("29-A1-99999", "29", "A1", "99999", "2026-10-31", "Pham Van D");

    CHECK(ht.insertPlate(p1), "Chen thanh cong bien so 59-X1-12345");
    CHECK(ht.insertPlate(p2), "Chen thanh cong bien so 59-X1-67890");
    CHECK(ht.insertPlate(p3), "Chen thanh cong bien so 59-X2-11111");
    CHECK(ht.insertPlate(p4), "Chen thanh cong bien so 29-A1-99999");

    // ------------------------------------------------------------------
    // 3. Kiem thu ham dem so xe theo tinh
    // ------------------------------------------------------------------
    CHECK(filter.countPlatesInProvince(ht, "59") == 3, "Dem so bien so tinh 59 dung 3");
    CHECK(filter.countPlatesInProvince(ht, "29") == 1, "Dem so bien so tinh 29 dung 1");
    CHECK(filter.countPlatesInProvince(ht, "99") == 0, "Dem so bien so tinh khong ton tai dung 0");

    // ------------------------------------------------------------------
    // 4. Kiem thu Chuc nang 3: Loc danh sach ma xa duy nhat (PROJ-9)
    // ------------------------------------------------------------------
    std::cout << "\n--- Test displayWardsOfProvince (Tinh 59 - Co du lieu) ---\n";
    filter.displayWardsOfProvince(ht, "59");

    std::cout << "\n--- Test displayWardsOfProvince (Tinh 79 - Khong co du lieu) ---\n";
    filter.displayWardsOfProvince(ht, "79");

    // ------------------------------------------------------------------
    // 5. Kiem thu Chuc nang 4: Loc danh sach bien so theo Xa dang bang (PROJ-15)
    // ------------------------------------------------------------------
    std::cout << "\n--- Test displayPlatesOfWard (Tinh 59, Xa X1 - Co du lieu) ---\n";
    filter.displayPlatesOfWard(ht, "59", "X1");

    std::cout << "\n--- Test displayPlatesOfWard (Tinh 59, Xa Z9 - Xa rong) ---\n";
    filter.displayPlatesOfWard(ht, "59", "Z9");

    // ------------------------------------------------------------------
    // 6. Kiem thu ham thong ke tong hop cac tinh thanh
    // ------------------------------------------------------------------
    std::cout << "\n--- Test displayProvinceStatistics (Co du lieu) ---\n";
    filter.displayProvinceStatistics(ht);

    if (failures == 0) {
        std::cout << "\nTAT CA TEST CUA AdministrativeFilter DEU PASS!\n";
        return 0;
    }
    return 1;
}