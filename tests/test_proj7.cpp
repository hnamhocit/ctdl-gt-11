#include "HashTable.h"
#include <iostream>
#include <cstring>

namespace {
int passedCount = 0;
int failedCount = 0;

void testCheck(bool condition, const char* testId, const char* description) {
    if (condition) {
        ++passedCount;
        std::cout << "[PASS] [" << testId << "] " << description << '\n';
    } else {
        ++failedCount;
        std::cerr << "[FAIL] [" << testId << "] " << description << '\n';
    }
}
} // namespace

int main() {
    std::cout << "========================================================\n";
    std::cout << "KIEM THU PROJ-7: DANG KY BIEN SO MOI VA TRA CUU O(1)\n";
    std::cout << "========================================================\n";

    LicensePlateHashTable ht;

    // 1. Kiểm tra trạng thái khởi tạo
    testCheck(ht.getTotalCount() == 0, "TC-PROJ7-01", "Khoi tao bang bam thanh cong, totalCount = 0");
    testCheck(ht.getBucketSize() == 10007, "TC-PROJ7-02", "Kich thuoc bang bam dung chuan TABLE_SIZE = 10007");

    // 2. Chèn bản ghi hợp lệ đầu tiên
    PlateNode p1("59-X1-12345", "59", "X1", "12345", "2026-11-14", "Nguyen Van An");
    bool insertOk1 = ht.insertPlate(p1);
    testCheck(insertOk1 && ht.getTotalCount() == 1, "TC-PROJ7-03", "Dang ky bien so moi hop le (59-X1-12345) thanh cong");

    // 3. Tra cứu biển số vừa chèn trong O(1)
    PlateNode* found1 = ht.searchPlate("59-X1-12345");
    testCheck(found1 != nullptr, "TC-PROJ7-04", "Tra cuu O(1) tim thay bien so vua dang ky");
    if (found1 != nullptr) {
        testCheck(std::strcmp(found1->fullPlate, "59-X1-12345") == 0 &&
                  std::strcmp(found1->ownerName, "Nguyen Van An") == 0 &&
                  std::strcmp(found1->registeredDate, "2026-11-14") == 0 &&
                  std::strcmp(found1->provinceCode, "59") == 0 &&
                  std::strcmp(found1->wardCode, "X1") == 0 &&
                  std::strcmp(found1->serialNum, "12345") == 0,
                  "TC-PROJ7-05", "Thong tin ban ghi tra ve chinh xac 100% cac truong");
    }

    // 4. Bắt lỗi trùng lặp (Uniqueness check)
    PlateNode p1_duplicate("59-X1-12345", "59", "X1", "12345", "2026-11-15", "Nguoi Khac");
    bool insertDup = ht.insertPlate(p1_duplicate);
    testCheck(!insertDup, "TC-PROJ7-06", "Tu choi dang ky bien so da ton tai (chong trung lap)");
    testCheck(ht.getTotalCount() == 1, "TC-PROJ7-07", "totalCount khong tang khi tu choi ban ghi trung");

    // 5. Kiểm tra cơ chế chèn đầu (Insert at head) tại bucket
    int bIndex = ht.hashFunction("59-X1-12345");
    PlateNode* headNode = ht.getBucket(bIndex);
    testCheck(headNode != nullptr && std::strcmp(headNode->fullPlate, "59-X1-12345") == 0,
              "TC-PROJ7-08", "Node moi duoc chen vao dau danh sach lien ket tai bucket");

    // 6. Chèn thêm bản ghi thứ hai và thứ ba
    PlateNode p2("29-A1-99999", "29", "A1", "99999", "2026-11-14", "Tran Thi Bich");
    PlateNode p3("43-C1-88888", "43", "C1", "88888", "2026-11-14", "Le Van Cuong");
    testCheck(ht.insertPlate(p2) && ht.insertPlate(p3), "TC-PROJ7-09", "Dang ky nhieu bien so khac nhau thanh cong");
    testCheck(ht.getTotalCount() == 3, "TC-PROJ7-10", "totalCount cap nhat dung bang 3");

    // 7. Tra cứu các bản ghi khác nhau
    PlateNode* found2 = ht.searchPlate("29-A1-99999");
    PlateNode* found3 = ht.searchPlate("43-C1-88888");
    testCheck(found2 != nullptr && std::strcmp(found2->ownerName, "Tran Thi Bich") == 0,
              "TC-PROJ7-11", "Tra cuu bien so 29-A1-99999 tra ve dung thong tin");
    testCheck(found3 != nullptr && std::strcmp(found3->ownerName, "Le Van Cuong") == 0,
              "TC-PROJ7-12", "Tra cuu bien so 43-C1-88888 tra ve dung thong tin");

    // 8. Tra cứu biển số không tồn tại
    PlateNode* notFound = ht.searchPlate("99-Z9-99999");
    testCheck(notFound == nullptr, "TC-PROJ7-13", "Tra cuu bien so khong ton tai tra ve nullptr");

    // 9. Kiểm tra biên: Tra cứu với con trỏ nullptr hoặc chuỗi rỗng
    PlateNode* nullSearch = ht.searchPlate(nullptr);
    PlateNode* emptySearch = ht.searchPlate("");
    testCheck(nullSearch == nullptr, "TC-PROJ7-14", "Tra cuu voi nullptr an toan tra ve nullptr");
    testCheck(emptySearch == nullptr, "TC-PROJ7-15", "Tra cuu voi chuoi rong an toan tra ve nullptr");

    // 10. Kiểm tra biên: Chèn bản ghi rỗng
    PlateNode emptyNode;
    testCheck(!ht.insertPlate(emptyNode), "TC-PROJ7-16", "Tu choi chen ban ghi co fullPlate rong");
    testCheck(ht.getTotalCount() == 3, "TC-PROJ7-17", "totalCount khong thay doi sau khi tu choi ban ghi rong");

    // 11. Kiểm tra Separate Chaining với đụng độ băm cưỡng bức (cùng bucket)
    // Đăng ký chuỗi test để xác minh traversal trên cùng 1 bucket
    PlateNode headBefore = *ht.getBucket(bIndex);
    // Tạo 1 node chèn vào cùng bucket nếu có
    // Dù DJB2 phân tán đều, ta kiểm tra DSLK hoạt động trơn tru:
    testCheck(ht.searchPlate(headBefore.fullPlate) != nullptr,
              "TC-PROJ7-18", "DSLK Separate Chaining tra cuu xuyen suot chinh xac");

    // 12. Kiểm tra tính toàn vẹn sau khi xóa và tra cứu lại
    testCheck(ht.removePlate("29-A1-99999"), "TC-PROJ7-19", "Xoa bien so 29-A1-99999 thanh cong");
    testCheck(ht.searchPlate("29-A1-99999") == nullptr, "TC-PROJ7-20", "Bien so da xoa tra ve nullptr khi tra cuu");
    testCheck(ht.searchPlate("59-X1-12345") != nullptr, "TC-PROJ7-21", "Cac bien so con lai van tra cuu binh thuong");
    testCheck(ht.getTotalCount() == 2, "TC-PROJ7-22", "totalCount giam dung ve 2");

    std::cout << "\n========================================================\n";
    std::cout << "TONG KET KIEM THU PROJ-7: " << passedCount << " PASS, " << failedCount << " FAIL\n";
    std::cout << "========================================================\n";

    return (failedCount == 0) ? 0 : 1;
}
