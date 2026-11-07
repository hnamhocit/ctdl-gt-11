#include "Validator.h"
#include <cctype>

namespace LicensePlateManager {

/**
 * @brief Chuẩn hóa biển số: bỏ khoảng trắng và chuyển chữ thường thành chữ hoa.
 * Độ phức tạp thời gian: O(L) với L là độ dài chuỗi (L <= 20 ký tự).
 * Độ phức tạp không gian: O(L) cấp phát chuỗi kết quả.
 */
std::string Validator::normalizePlateString(const std::string& rawInput) {
    std::string result;
    result.reserve(rawInput.size());

    for (char ch : rawInput) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            result += static_cast<char>(
                std::toupper(static_cast<unsigned char>(ch))
            );
        }
    }

    return result;
}

/**
 * @brief Kiểm tra mã tỉnh gồm đúng 2 chữ số (00 - 99).
 * Độ phức tạp: O(1) thời gian, O(1) không gian.
 */
bool Validator::isValidProvinceCode(const std::string& provinceCode) {
    if (provinceCode.length() != 2) return false;

    return std::isdigit(static_cast<unsigned char>(provinceCode[0])) &&
           std::isdigit(static_cast<unsigned char>(provinceCode[1]));
}

/**
 * @brief Kiểm tra mã xã gồm đúng 2 ký tự chữ hoặc số (A-Z, 0-9).
 * Độ phức tạp: O(1) thời gian, O(1) không gian.
 */
bool Validator::isValidCommuneCode(const std::string& communeCode) {
    if (communeCode.length() != 2) return false;

    for (char ch : communeCode) {
        if (!std::isalnum(static_cast<unsigned char>(ch))) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Kiểm tra số serial gồm đúng 5 chữ số (00000 - 99999).
 * Độ phức tạp: O(1) thời gian, O(1) không gian.
 */
bool Validator::isValidSerialNumber(const std::string& serialNumber) {
    if (serialNumber.length() != 5) return false;

    for (char ch : serialNumber) {
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Kiểm tra nhanh biển số có hợp lệ hay không.
 * Độ phức tạp: O(1) thời gian, O(1) không gian.
 */
bool Validator::isValidLicensePlateFormat(const std::string& plate) {
    return validateDetailed(plate).isValid;
}

/**
 * @brief Kiểm tra chi tiết từng thành phần của biển số theo cấu trúc ab-cd-ghxyz.
 * Độ phức tạp: O(1) thời gian (chuỗi dài 11), O(1) không gian.
 */
ValidationResult Validator::validateDetailed(const std::string& plate) {
    // Bước 1: Kiểm tra độ dài biển số (phải đúng 11 ký tự)
    if (plate.length() != 11) {
        return {
            false,
            ValidationError::INVALID_LENGTH,
            "Lỗi: Độ dài biển số không đúng 11 ký tự (Chuẩn: ab-cd-ghxyz, VD: 59-X1-12345)."
        };
    }

    // Bước 2: Kiểm tra vị trí dấu gạch nối '-' tại chỉ số 2 và 5
    if (plate[2] != '-' || plate[5] != '-') {
        return {
            false,
            ValidationError::MISSING_HYPHEN,
            "Lỗi: Thiếu hoặc sai vị trí dấu gạch nối '-'."
        };
    }

    // Bước 3: Kiểm tra mã tỉnh gồm 2 chữ số (ký tự 0..1)
    std::string province = plate.substr(0, 2);
    if (!isValidProvinceCode(province)) {
        return {
            false,
            ValidationError::INVALID_PROVINCE_CODE,
            "Lỗi: Mã tỉnh '" + province + "' không hợp lệ. Phải gồm đúng 2 ký tự số (00-99)."
        };
    }

    // Bước 4: Kiểm tra mã xã gồm 2 ký tự chữ hoặc số (ký tự 3..4)
    std::string commune = plate.substr(3, 2);
    if (!isValidCommuneCode(commune)) {
        return {
            false,
            ValidationError::INVALID_COMMUNE_CODE,
            "Lỗi: Mã xã '" + commune + "' chứa ký tự sai. Phải là 2 ký tự chữ/số (A-Z, 0-9)."
        };
    }

    // Bước 5: Kiểm tra số serial gồm 5 chữ số (ký tự 6..10)
    std::string serial = plate.substr(6, 5);
    if (!isValidSerialNumber(serial)) {
        return {
            false,
            ValidationError::INVALID_SERIAL_NUMBER,
            "Lỗi: Số serial '" + serial + "' không hợp lệ. Phải đủ 5 ký tự số (00000-99999)."
        };
    }

    // Nếu vượt qua tất cả bước kiểm tra: biển số hợp lệ hoàn toàn
    return {
        true,
        ValidationError::NONE,
        "Biển số xe hợp lệ."
    };
}

} // namespace LicensePlateManager