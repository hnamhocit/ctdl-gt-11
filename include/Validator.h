#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

namespace LicensePlateManager {

// Các loại lỗi khi kiểm tra biển số xe
enum class ValidationError {
    NONE = 0,                  // Biển số hợp lệ
    INVALID_LENGTH,            // Sai độ dài biển số
    MISSING_HYPHEN,             // Thiếu hoặc sai vị trí dấu '-'
    INVALID_PROVINCE_CODE,      // Mã tỉnh không hợp lệ
    INVALID_COMMUNE_CODE,       // Mã xã không hợp lệ
    INVALID_SERIAL_NUMBER      // Số serial không hợp lệ
};

// Lưu kết quả kiểm tra biển số
struct ValidationResult {
    bool isValid;               // Trạng thái hợp lệ
    ValidationError error;      // Loại lỗi gặp phải
    std::string errorMessage;   // Nội dung thông báo lỗi
};

// Lớp xử lý và kiểm tra biển số xe
class Validator {
public:
    // Chuẩn hóa biển số: bỏ khoảng trắng, chuyển mã xã thành chữ hoa
    static std::string normalizePlateString(const std::string& rawInput);

    // Kiểm tra biển số có đúng định dạng hay không
    static bool isValidLicensePlateFormat(const std::string& plate);

    // Kiểm tra chi tiết và trả về lỗi cụ thể
    static ValidationResult validateDetailed(const std::string& plate);

    // Kiểm tra mã tỉnh gồm 2 chữ số
    static bool isValidProvinceCode(const std::string& provinceCode);

    // Kiểm tra mã xã gồm 2 ký tự chữ hoặc số
    static bool isValidCommuneCode(const std::string& communeCode);

    // Kiểm tra số serial gồm 5 chữ số
    static bool isValidSerialNumber(const std::string& serialNumber);
};

}

#endif // VALIDATOR_H