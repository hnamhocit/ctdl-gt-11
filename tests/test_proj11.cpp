#include "HashTable.h"
#include "Validator.h"

#include <cstring>
#include <iostream>
#include <string>

namespace {
int passed = 0;
int failed = 0;

void check(bool condition, const char* id, const char* description) {
    if (condition) {
        ++passed;
        std::cout << "PASS [" << id << "] " << description << '\n';
    } else {
        ++failed;
        std::cout << "FAIL [" << id << "] " << description << '\n';
    }
}

PlateNode makeNode(const char* plate) {
    PlateNode node;
    if (!parseLicensePlate(plate, node)) {
        return PlateNode();
    }
    std::strcpy(node.registeredDate, "2026-11-11");
    std::strcpy(node.ownerName, "Test Owner");
    return node;
}
} // namespace

int main() {
    using LicensePlateManager::ValidationError;
    using LicensePlateManager::Validator;

    std::cout << "=== PROJ-11: 20 UNIT TESTS (Validator + HashTable) ===\n";

    // Validator: 12 cases
    check(Validator::validateDetailed("59-X1-12345").isValid,
          "TC-01", "Bien so dung dinh dang duoc chap nhan");
    check(Validator::normalizePlateString("59-x1-12345") == "59-X1-12345" &&
          Validator::isValidLicensePlateFormat(
              Validator::normalizePlateString("59-x1-12345")),
          "TC-02", "Chu thuong duoc chuan hoa thanh chu hoa");
    check(Validator::normalizePlateString(" 59-X1-12345 ") == "59-X1-12345",
          "TC-03", "Khoang trang dau cuoi duoc loai bo");
    check(Validator::validateDetailed("59-X1-1234").error == ValidationError::INVALID_LENGTH,
          "TC-04", "Sai do dai bi tu choi");
    check(Validator::validateDetailed("59.X1-12345").error == ValidationError::MISSING_HYPHEN,
          "TC-05", "Thieu dau gach noi bi tu choi");
    check(Validator::validateDetailed("5A-X1-12345").error == ValidationError::INVALID_PROVINCE_CODE,
          "TC-06", "Ma tinh chua chu cai bi tu choi");
    check(Validator::validateDetailed("5.-X1-12345").error == ValidationError::INVALID_PROVINCE_CODE,
          "TC-07", "Ma tinh chua ky tu dac biet bi tu choi");
    check(Validator::validateDetailed("59-X@-12345").error == ValidationError::INVALID_COMMUNE_CODE,
          "TC-08", "Ma xa chua ky tu dac biet bi tu choi");
    check(Validator::validateDetailed("59-X1-12A45").error == ValidationError::INVALID_SERIAL_NUMBER,
          "TC-09", "Serial chua chu cai bi tu choi");
    check(Validator::validateDetailed("59-X1-123456").error == ValidationError::INVALID_LENGTH,
          "TC-10", "Serial qua dai lam sai tong do dai");
    check(Validator::validateDetailed("").error == ValidationError::INVALID_LENGTH,
          "TC-11", "Chuoi rong bi tu choi");
    check(Validator::validateDetailed(Validator::normalizePlateString("           ")).error == ValidationError::INVALID_LENGTH,
          "TC-12", "Chuoi chi co khoang trang bi tu choi");

    // Hash Table: 8 cases
    LicensePlateHashTable table;
    check(table.getTotalCount() == 0 && table.getLoadFactor() == 0.0,
          "TC-13", "Bang bam moi khoi tao rong");
    PlateNode valid = makeNode("59-X1-12345");
    check(table.insertPlate(valid) && table.getTotalCount() == 1,
          "TC-14", "Chen ban ghi hop le thanh cong");
    check(!table.insertPlate(valid) && table.getTotalCount() == 1,
          "TC-15", "Khong cho phep chen trung bien so");
    check(table.searchPlate("59-X1-12345") != nullptr,
          "TC-16", "Tra cuu tim thay ban ghi da chen");
    check(table.searchPlate("99-Z9-99999") == nullptr,
          "TC-17", "Tra cuu bien so khong ton tai tra ve nullptr");
    check(!table.insertPlate(PlateNode()) && table.getTotalCount() == 1,
          "TC-18", "Khong cho phep chen ban ghi co khoa rong");
    check(table.removePlate("59-X1-12345") &&
          table.searchPlate("59-X1-12345") == nullptr &&
          table.getTotalCount() == 0,
          "TC-19", "Xoa ban ghi thanh cong va cap nhat so luong");
    table.insertPlate(makeNode("29-A1-99999"));
    table.clear();
    check(table.getTotalCount() == 0 &&
          table.searchPlate("29-A1-99999") == nullptr,
          "TC-20", "clear giai phong du lieu va dua bang bam ve rong");

    std::cout << "\n=== KET QUA: " << passed << "/20 PASS, "
              << failed << " FAIL ===\n";
    return failed == 0 ? 0 : 1;
}
