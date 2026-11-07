#ifndef FILEIO_H
#define FILEIO_H

#include "Plate.h"
#include "HashTable.h"

/** Ký tự phân cách các trường trong file TXT. */
constexpr char FILE_DELIMITER = '|';

/** Số trường của một bản ghi:
 *  fullPlate|provinceCode|wardCode|serialNum|registeredDate|ownerName */
constexpr int FILE_FIELD_COUNT = 6;

/** Thống kê kết quả nạp file (tùy chọn). */
struct LoadStats
{
    int loaded = 0;     // số dòng nạp thành công
    int duplicates = 0; // số biển số trùng (bị bỏ qua)
    int invalid = 0;    // số dòng sai định dạng (bị bỏ qua)
};

/**
 * Nạp dữ liệu từ file TXT (UTF-8) vào bảng băm. Không xóa dữ liệu đang có.
 * Dòng lỗi bị bỏ qua và ghi cảnh báo ra std::cerr, không làm dừng chương trình.
 * @return false nếu không mở được file; true nếu đọc xong (kể cả có dòng lỗi).
 */
bool loadFromFile(LicensePlateHashTable &table, const char *filepath,
                  LoadStats *stats = nullptr);

/**
 * Ghi toàn bộ bảng băm ra file TXT (UTF-8, không BOM, xuống dòng '\n').
 * @return false nếu không ghi được hoặc có trường chứa '|' / xuống dòng.
 */
bool saveToFile(const LicensePlateHashTable &table, const char *filepath);

/**
 * Sinh file dữ liệu mẫu hợp lệ, không trùng biển số, có tên tiếng Việt.
 * @return false nếu count <= 0, quá lớn, hoặc không ghi được file.
 */
bool generateSampleDataset(const char *filepath, int count);

#endif // FILEIO_H