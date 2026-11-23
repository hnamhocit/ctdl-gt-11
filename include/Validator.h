#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

namespace LicensePlateManager {

/** Các lỗi định dạng biển số. */
enum class ValidationError {
    NONE = 0,                  // Biển số hợp lệ
    INVALID_LENGTH,            // Sai độ dài biển số (khác 11 ký tự)
    MISSING_HYPHEN,            // Thiếu hoặc sai vị trí dấu '-' (tại index 2 và 5)
    INVALID_PROVINCE_CODE,     // Mã tỉnh không hợp lệ (không phải 2 chữ số 00-99)
    INVALID_COMMUNE_CODE,      // Mã xã không hợp lệ (không phải 2 ký tự chữ/số)
    INVALID_SERIAL_NUMBER      // Số serial không hợp lệ (không phải 5 chữ số)
};

/** Kết quả kiểm tra biển số. */
struct ValidationResult {
    bool isValid;               // Trạng thái hợp lệ (true/false)
    ValidationError error;      // Loại lỗi gặp phải
    std::string errorMessage;   // Nội dung thông báo lỗi bằng tiếng Việt
};

/** Kiểm tra biển dạng ab-cd-ghxyz, ví dụ "59-X1-12345". */
class Validator {
public:
    /** Bỏ khoảng trắng và viết hoa rawInput, trả chuỗi đã chuẩn hóa. Thời gian và bộ nhớ O(L). */
    static std::string normalizePlateString(const std::string& rawInput);

    /** Trả true nếu plate đúng định dạng. O(1) thời gian và bộ nhớ. */
    static bool isValidLicensePlateFormat(const std::string& plate);

    /** Trả trạng thái và lỗi định dạng của plate. O(1) thời gian và bộ nhớ. */
    static ValidationResult validateDetailed(const std::string& plate);

    /** Trả true nếu provinceCode có hai chữ số, ví dụ "59". O(1) thời gian và bộ nhớ. */
    static bool isValidProvinceCode(const std::string& provinceCode);

    /** Trả true nếu communeCode có hai chữ hoa hoặc chữ số, ví dụ "X1". O(1) thời gian và bộ nhớ. */
    static bool isValidCommuneCode(const std::string& communeCode);

    /** Kiểm tra wardCode theo cùng quy tắc với isValidCommuneCode. O(1). */
    static bool isValidWardCode(const std::string& wardCode) {
        return isValidCommuneCode(wardCode);
    }

    /** Trả true nếu serialNumber có năm chữ số, ví dụ "00007". O(1) thời gian và bộ nhớ. */
    static bool isValidSerialNumber(const std::string& serialNumber);
};

} // namespace LicensePlateManager

#endif // VALIDATOR_H