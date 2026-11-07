#include "FileIO.h"

#include <cstring>
#include <fstream>
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

int main(int argc, char **argv)
{
    // Dùng để sinh data/sample_plates.txt: test_fileio.exe --gen-sample
    if (argc > 1 && std::strcmp(argv[1], "--gen-sample") == 0)
        return generateSampleDataset("data/sample_plates.txt", 1000) ? 0 : 1;

    // 1. Sinh 1000 bản ghi rồi nạp lại: không được trùng, không được lỗi
    CHECK(generateSampleDataset("t1.txt", 1000));
    LicensePlateHashTable t1;
    LoadStats s1;
    CHECK(loadFromFile(t1, "t1.txt", &s1));
    CHECK(s1.loaded == 1000 && s1.duplicates == 0 && s1.invalid == 0);
    CHECK(t1.getTotalCount() == 1000);

    // 2. Lưu rồi nạp lại phải ra cùng số bản ghi
    CHECK(saveToFile(t1, "t2.txt"));
    LicensePlateHashTable t2;
    CHECK(loadFromFile(t2, "t2.txt"));
    CHECK(t2.getTotalCount() == 1000);

    // 3. BOM + CRLF + tiêu đề + chú thích + dòng lỗi + dòng trùng
    {
        std::ofstream f("t3.txt", std::ios::binary);
        f << "\xEF\xBB\xBF"
             "fullPlate|provinceCode|wardCode|serialNum|registeredDate|ownerName\r\n"
          << "# chu thich\r\n"
          << "59-X1-12345|59|X1|12345|2026-10-30|Nguyễn Văn An\r\n"
          << "\r\n"
          << "thieu|truong\r\n"
          << "29-A1-99988|29|A1|99988|2026-10-31|Trần Thị Bích\r\n"
          << "59-X1-12345|59|X1|12345|2026-10-30|Nguyễn Văn An\r\n"
          << "59-X1-12346|59|X2|12346|2026-10-30|Lệch xã\r\n";
    }
    LicensePlateHashTable t3;
    LoadStats s3;
    CHECK(loadFromFile(t3, "t3.txt", &s3));
    CHECK(s3.loaded == 2 && s3.duplicates == 1 && s3.invalid == 2);

    // 4. Tiếng Việt UTF-8 giữ nguyên
    PlateNode *p = t3.searchPlate("59-X1-12345");
    CHECK(p != nullptr && std::strcmp(p->ownerName, "Nguyễn Văn An") == 0);

    // 5. File không tồn tại / đường dẫn không ghi được
    LicensePlateHashTable t5;
    CHECK(!loadFromFile(t5, "khong_ton_tai.txt"));
    CHECK(!saveToFile(t1, "thu_muc_khong_co/x.txt"));

    // 6. Tên chủ xe chứa '|' phải bị từ chối khi lưu
    LicensePlateHashTable t6;
    PlateNode bad;
    std::strcpy(bad.fullPlate, "59-X1-00001");
    std::strcpy(bad.provinceCode, "59");
    std::strcpy(bad.wardCode, "X1");
    std::strcpy(bad.serialNum, "00001");
    std::strcpy(bad.registeredDate, "2026-10-30");
    std::strcpy(bad.ownerName, "A|B");
    CHECK(t6.insertPlate(bad));
    CHECK(!saveToFile(t6, "t6.txt"));

    std::cout << (failures == 0 ? "All FileIO tests passed\n" : "Có test thất bại\n");
    return failures == 0 ? 0 : 1;
}