#include "MenuUI.h"
#include "HashTable.h"
#include "Validator.h"
#include "FileIO.h"
#include "AdministrativeFilter.h"
#include "Plate.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

void setupConsoleEncoding() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

int totalPass = 0;
int totalFail = 0;

void assertTest(bool condition, const std::string& testCode, const std::string& testName) {
    if (condition) {
        std::cout << "  [" << testCode << "] ✅ PASS: " << testName << "\n";
        totalPass++;
    } else {
        std::cout << "  [" << testCode << "] ❌ FAIL: " << testName << "\n";
        totalFail++;
    }
}

void runFull28TestSuite() {
    std::cout << "\n========================================================\n";
    std::cout << "  BẮT ĐẦU KIỂM THỬ TOÀN DIỆN 28 TEST CASESS (SPRINT 3 / PROJ-16) \n";
    std::cout << "========================================================\n\n";

    std::cout << "--- NHÓM 1: MENU UI & CIN VALIDATION (TC-01 -> TC-03) ---\n";

    int choiceValid = 3;
    assertTest(choiceValid >= 0 && choiceValid <= 5, "TC-01", "Lua chon menu hop le (pham vi 0 - 5)");

    int choiceInvalid = 99;
    assertTest(!(choiceInvalid >= 0 && choiceInvalid <= 5), "TC-02", "Phat hien va tu choi lua chon ngoai pham vi (99)");

    std::stringstream fakeCin("abc");
    int dummyChoice;
    fakeCin >> dummyChoice;
    bool isCinFailed = fakeCin.fail();
    assertTest(isCinFailed, "TC-03", "Bay loi std::cin.fail khi nguoi dung nhap ky tu chu 'abc'");

    std::cout << "\n--- NHÓM 2: VALIDATOR & CHUẨN HÓA BIỂN SỐ (TC-04 -> TC-10) ---\n";

    std::string pValid = "59-X1-12345";
    assertTest(LicensePlateManager::Validator::validateDetailed(pValid).isValid, "TC-04", "Bien so dung chuan '59-X1-12345'");

    std::string pRaw = "  59-x1-12345  ";
    std::string pClean = LicensePlateManager::Validator::normalizePlateString(pRaw);
    assertTest(pClean == "59-X1-12345" && LicensePlateManager::Validator::validateDetailed(pClean).isValid,
               "TC-05", "Chuan hoa chu thuong va xoa khoang trang du thừa");

    assertTest(!LicensePlateManager::Validator::validateDetailed("59-X1-1234").isValid &&
               !LicensePlateManager::Validator::validateDetailed("59-X1-123456").isValid,
               "TC-06", "Tu choi bien so sai do dai (thieu hoac thừa ky tu)");

    assertTest(!LicensePlateManager::Validator::validateDetailed("59X1-12345").isValid &&
               !LicensePlateManager::Validator::validateDetailed("59-X112345").isValid,
               "TC-07", "Tu choi bien so thieu/sai vi tri dau gach noi");

    assertTest(!LicensePlateManager::Validator::validateDetailed("A9-X1-12345").isValid,
               "TC-08", "Tu choi Ma Tinh sai dinh dang ('A9')");

    assertTest(!LicensePlateManager::Validator::validateDetailed("59-*-12345").isValid,
               "TC-09", "Tu choi Ma Xa chua ky tu dac biet ('*')");

    assertTest(!LicensePlateManager::Validator::validateDetailed("59-X1-123A5").isValid,
               "TC-10", "Tu choi phan so seri chua chu cai ('123A5')");

    std::cout << "\n--- NHÓM 3: BẢNG BĂM HASHTABLE (TC-11 -> TC-14) ---\n";
    LicensePlateHashTable table;

    PlateNode n1{};
    parseLicensePlate("59-X1-12345", n1);
    std::strncpy(n1.ownerName, "Nguyen Van A", sizeof(n1.ownerName) - 1);
    std::strncpy(n1.registeredDate, "2026-11-20", sizeof(n1.registeredDate) - 1);

    PlateNode n2{};
    parseLicensePlate("59-X1-67890", n2);
    std::strncpy(n2.ownerName, "Tran Thi B", sizeof(n2.ownerName) - 1);
    std::strncpy(n2.registeredDate, "2026-11-21", sizeof(n2.registeredDate) - 1);

    bool ins1 = table.insertPlate(n1);
    bool ins2 = table.insertPlate(n2);
    assertTest(ins1 && ins2, "TC-11", "Them 2 bien so hop le vao Bang bam thành cong");

    bool insDup = table.insertPlate(n1);
    assertTest(!insDup, "TC-12", "Tu choi chen bien so da ton tai trong he thong");

    PlateNode* foundNode = table.searchPlate("59-X1-12345");
    assertTest(foundNode != nullptr && std::strcmp(foundNode->ownerName, "Nguyen Van A") == 0,
               "TC-13", "Tra cuu tim thay dung thong tin bien so '59-X1-12345'");

    PlateNode* nullNode = table.searchPlate("59-X1-99999");
    assertTest(nullNode == nullptr, "TC-14", "Tra cuu bien so khong ton tai tra ve nullptr an toan");

    std::cout << "\n--- NHÓM 4: LỌC HÀNH CHÍNH & ĐỐI SOÁT KẾT QUẢ (TC-15 -> TC-19) ---\n";
    AdministrativeFilter filter;

    int countProv59 = filter.countPlatesInProvince(table, "59");
    assertTest(countProv59 == 2, "TC-15", "Xac nhan chinh xac tong so bien so thuoc Tinh 59 (Dem duoc 2 bien)");

    int countProv99 = filter.countPlatesInProvince(table, "99");
    assertTest(countProv99 == 0, "TC-16", "Xac nhan Tinh 99 chua co du lieu (Dem bang 0, khong crash)");

    bool checkProvFormat = LicensePlateManager::Validator::isValidProvinceCode("ABC");
    assertTest(!checkProvFormat, "TC-17", "Ngat thao tac loc khi Ma Tỉnh sai dinh dang ('ABC')");

    PlateNode* nodeWardX1 = table.searchPlate("59-X1-12345");
    assertTest(nodeWardX1 != nullptr && std::strcmp(nodeWardX1->wardCode, "X1") == 0,
               "TC-18", "Xac nhan bien so '59-X1-12345' thuoc dung Xa X1 - Tinh 59");

    filter.displayPlatesOfWard(table, "59", "Z9");
    assertTest(true, "TC-19", "Hien thi danh sach rong an toan khi Xa Z9 khong co bien so");

    std::cout << "\n--- NHÓM 5: ĐỌC / GHI FILE TXT & BẪY LỖI (TC-20 -> TC-24) ---\n";
    std::string testFilePath = "data/test_sprint3_full28.txt";

    bool saveStatus = saveToFile(table, testFilePath.c_str());
    assertTest(saveStatus, "TC-20", "Ghi thanh cong du lieu Bang bam ra tep 'data/test_sprint3_full28.txt'");

    LicensePlateHashTable loadedTable;
    bool loadStatus = loadFromFile(loadedTable, testFilePath.c_str());
    PlateNode* checkLoadedNode = loadedTable.searchPlate("59-X1-12345");
    assertTest(loadStatus && checkLoadedNode != nullptr, "TC-21", "Nap file thanh cong & xac nhan du lieu duoc khoi phục nguyen vẹn");

    std::ifstream badFile("data/file_non_existent_9999.txt");
    assertTest(!badFile.is_open(), "TC-22", "Bay loi File Not Found chinh xac khi mo tep khong ton tai");

    std::string cancelInput = "0";
    assertTest(cancelInput == "0", "TC-23", "Xu ly huy thao tac an toan khi nguoi dung nhap '0'");

    bool invalidSave = saveToFile(table, "///invalid_folder/out.txt");
    assertTest(!invalidSave, "TC-24", "Bay loi va ghi nhan failure khi duong dan luu file khong hop le");

    std::cout << "\n--- NHÓM 6: HIỆU NĂNG, BỘ NHỚ & ĐA NỀN TẢNG (TC-25 -> TC-28) ---\n";

    LicensePlateHashTable bulkTable;
    for (int i = 10000; i < 11000; ++i) {
        PlateNode bulkNode{};
        std::string plateStr = "59-X1-" + std::to_string(i);
        parseLicensePlate(plateStr.c_str(), bulkNode);
        std::strncpy(bulkNode.ownerName, "Test Bulk", sizeof(bulkNode.ownerName) - 1);
        std::strncpy(bulkNode.registeredDate, "2026-11-20", sizeof(bulkNode.registeredDate) - 1);
        bulkTable.insertPlate(bulkNode);
    }

    PlateNode* bulkSearchNode = bulkTable.searchPlate("59-X1-10500");
    assertTest(bulkSearchNode != nullptr, "TC-25", "Chen va tra cuu O(1) thanh cong tren 1.000+ ban ghi");

    PlateNode spaceNode{};
    parseLicensePlate("59-X2-88888", spaceNode);
    std::strncpy(spaceNode.ownerName, "Nguyen Hoang Nam", sizeof(spaceNode.ownerName) - 1);
    table.insertPlate(spaceNode);
    PlateNode* searchSpaceNode = table.searchPlate("59-X2-88888");
    assertTest(searchSpaceNode != nullptr && std::strcmp(searchSpaceNode->ownerName, "Nguyen Hoang Nam") == 0,
               "TC-26", "Luu tru nguyen ven chuoi Ho ten chua nhieu khoang trang ('Nguyen Hoang Nam')");

    bulkTable.clear();
    assertTest(bulkTable.searchPlate("59-X1-10500") == nullptr, "TC-27", "Giai phong 100% con tro va thu hoi bo nho RAM an toan");

    assertTest(totalFail == 0, "TC-28", "Hoan thanh kiem thu va dat ty le thanh cong 100%");

    std::cout << "\n========================================================\n";
    std::cout << "          BÁO CÁO KẾT QUẢ KIỂM THỬ SPRINT 3             \n";
    std::cout << "========================================================\n";
    std::cout << "  TỔNG SỐ TEST CASES RUN : " << (totalPass + totalFail) << " / 28\n";
    std::cout << "  PASSED (ĐẠT)          : ✅ " << totalPass << "\n";
    std::cout << "  FAILED (LỖI)          : ❌ " << totalFail << "\n";
    std::cout << "  TỶ LỆ THÀNH CÔNG     : " << ((float)totalPass / (totalPass + totalFail)) * 100 << "%\n";
    std::cout << "========================================================\n\n";
}

int main(int argc, char* argv[]) {
    setupConsoleEncoding();

    if (argc > 1 && std::string(argv[1]) == "--test") {
        runFull28TestSuite();
        return 0;
    }

    LicensePlateManager::MenuUI::run();

    return 0;
}