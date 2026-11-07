#include "HashTable.h"
#include <iostream>
#include <cassert>
#include <cstring>

static int testFailures = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "FAIL [" << __FILE__ << ":" << __LINE__ << "]: " << msg << "\n"; \
            testFailures++; \
        } else { \
            std::cout << "PASS: " << msg << "\n"; \
        } \
    } while (0)

int main() {
    std::cout << "========================================\n";
    std::cout << "KIEM THU UNIT TEST: PROJ-1 HASHTABLE & PLATENODE\n";
    std::cout << "========================================\n";

    // 1. Kiểm tra PlateNode và các hàm tiện ích
    {
        PlateNode node1;
        TEST_ASSERT(node1.next == nullptr, "PlateNode mac dinh co next == nullptr");
        TEST_ASSERT(node1.fullPlate[0] == '\0', "fullPlate mac dinh rong");

        PlateNode node2("59-X1-12345", "59", "X1", "12345", "2026-10-30", "Nguyen Hoang Nam");
        TEST_ASSERT(std::strcmp(node2.fullPlate, "59-X1-12345") == 0, "fullPlate khoi tao dung");
        TEST_ASSERT(std::strcmp(node2.ownerName, "Nguyen Hoang Nam") == 0, "ownerName khoi tao dung");

        PlateNode parsed;
        bool ok = parseLicensePlate("29-A1-99988", parsed);
        TEST_ASSERT(ok, "parseLicensePlate thanh cong");
        TEST_ASSERT(std::strcmp(parsed.fullPlate, "29-A1-99988") == 0, "parsed fullPlate dung");
        TEST_ASSERT(std::strcmp(parsed.provinceCode, "29") == 0, "parsed provinceCode dung");
        TEST_ASSERT(std::strcmp(parsed.wardCode, "A1") == 0, "parsed wardCode dung");
        TEST_ASSERT(std::strcmp(parsed.serialNum, "99988") == 0, "parsed serialNum dung");

        TEST_ASSERT(!parseLicensePlate("invalid", parsed), "parseLicensePlate tu choi chuoi sai");
    }

    // 2. Khởi tạo Bảng băm
    LicensePlateHashTable ht;
    TEST_ASSERT(ht.getTotalCount() == 0, "Bang bam moi khoi tao co totalCount == 0");
    TEST_ASSERT(ht.getBucketSize() == 10007, "getBucketSize dung 10007");
    TEST_ASSERT(ht.getLoadFactor() == 0.0, "getLoadFactor ban dau bang 0.0");

    for (int i = 0; i < ht.getBucketSize(); ++i) {
        if (ht.getBucket(i) != nullptr) {
            TEST_ASSERT(false, "Toan bo buckets phai la nullptr");
            break;
        }
    }
    TEST_ASSERT(true, "Toan bo 10007 buckets ban dau deu la nullptr");

    // 3. Kiểm tra hàm băm DJB2
    {
        int h1 = ht.hashFunction("59-X1-12345");
        int h2 = ht.hashFunction("59-X1-12345");
        int h3 = ht.hashFunction("59-X1-12346");
        TEST_ASSERT(h1 == h2, "Ham bam DJB2 nhat quan");
        TEST_ASSERT(h1 >= 0 && h1 < 10007, "Chi so bam trong pham vi [0, 10006]");
        TEST_ASSERT(h1 != h3, "DJB2 phan tach bien so sat nhau");
    }

    // 4. Chèn biển số (insertPlate) & chống trùng lặp
    {
        PlateNode p1("59-X1-12345", "59", "X1", "12345", "2026-10-30", "Nguyen Van An");
        PlateNode p2("29-A1-99988", "29", "A1", "99988", "2026-10-31", "Tran Thi Bich");
        PlateNode p3("43-C1-55555", "43", "C1", "55555", "2026-11-01", "Le Van Cuong");

        TEST_ASSERT(ht.insertPlate(p1), "Chen thanh cong p1");
        TEST_ASSERT(ht.insertPlate(p2), "Chen thanh cong p2");
        TEST_ASSERT(ht.insertPlate(p3), "Chen thanh cong p3");
        TEST_ASSERT(ht.getTotalCount() == 3, "totalCount == 3");

        // Chèn trùng biển số p1
        PlateNode p1_dup("59-X1-12345", "59", "X1", "12345", "2026-10-30", "Nguoi Khac");
        TEST_ASSERT(!ht.insertPlate(p1_dup), "Tu choi chen trung bien so 59-X1-12345");
        TEST_ASSERT(ht.getTotalCount() == 3, "totalCount van la 3 sau khi tu choi");
    }

    // 5. Tra cứu (searchPlate)
    {
        PlateNode* found1 = ht.searchPlate("59-X1-12345");
        TEST_ASSERT(found1 != nullptr, "Tim thay 59-X1-12345");
        TEST_ASSERT(std::strcmp(found1->ownerName, "Nguyen Van An") == 0, "Chu xe dung");

        PlateNode* foundNone = ht.searchPlate("99-Z9-99999");
        TEST_ASSERT(foundNone == nullptr, "Khong tim thay bien so khong ton tai");
    }

    // 6. Xóa biển số (removePlate)
    {
        TEST_ASSERT(ht.removePlate("29-A1-99988"), "Xoa thanh cong 29-A1-99988");
        TEST_ASSERT(ht.getTotalCount() == 2, "totalCount giam con 2");
        TEST_ASSERT(ht.searchPlate("29-A1-99988") == nullptr, "Tra cuu lai node da xoa tra ve nullptr");

        // Xóa lại node vừa xóa
        TEST_ASSERT(!ht.removePlate("29-A1-99988"), "Xoa lai node khong ton tai tra ve false");
        TEST_ASSERT(ht.getTotalCount() == 2, "totalCount khong doi");
    }

    // 7. Kiểm tra clear và nạp lại
    {
        ht.clear();
        TEST_ASSERT(ht.getTotalCount() == 0, "Sau khi clear(), totalCount == 0");
        TEST_ASSERT(ht.searchPlate("59-X1-12345") == nullptr, "Khong con tim thay node sau clear");
        
        PlateNode pNew("61-B2-04567", "61", "B2", "04567", "2026-11-02", "Binh Duong");
        TEST_ASSERT(ht.insertPlate(pNew), "Chen lai vao bang sau khi clear thanh cong");
        TEST_ASSERT(ht.getTotalCount() == 1, "totalCount == 1");
    }

    std::cout << "========================================\n";
    if (testFailures == 0) {
        std::cout << "TAT CA CAC TEST CUA PROJ-1 DEU THANH CONG (100% PASS)\n";
        return 0;
    } else {
        std::cerr << "CO " << testFailures << " TEST THAT BAI!\n";
        return 1;
    }
}
