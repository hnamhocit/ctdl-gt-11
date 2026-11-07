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
    std::cout << "KIEM THU: AdministrativeFilter\n";
    std::cout << "========================================\n";

    LicensePlateHashTable ht;
    AdministrativeFilter filter;

    PlateNode p1("59-X1-12345", "59", "X1", "12345", "2026-10-30", "Nguyen Van An");
    PlateNode p2("59-X1-67890", "59", "X1", "67890", "2026-10-30", "Tran Thi B");
    PlateNode p3("59-X2-11111", "59", "X2", "11111", "2026-10-31", "Le Van C");
    PlateNode p4("29-A1-99999", "29", "A1", "99999", "2026-10-31", "Pham Van D");

    ht.insertPlate(p1);
    ht.insertPlate(p2);
    ht.insertPlate(p3);
    ht.insertPlate(p4);

    CHECK(filter.countPlatesInProvince(ht, "59") == 3, "Dem so bien so tinh 59 dung 3");
    CHECK(filter.countPlatesInProvince(ht, "29") == 1, "Dem so bien so tinh 29 dung 1");
    CHECK(filter.countPlatesInProvince(ht, "99") == 0, "Dem so bien so tinh khong ton tai dung 0");

    std::cout << "\n--- Test displayWardsOfProvince ---\n";
    filter.displayWardsOfProvince(ht, "59");

    std::cout << "\n--- Test displayPlatesOfWard ---\n";
    filter.displayPlatesOfWard(ht, "59", "X1");

    if (failures == 0) {
        std::cout << "\nTAT CA TEST CUA AdministrativeFilter DEU PASS!\n";
        return 0;
    }
    return 1;
}
