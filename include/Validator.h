#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

namespace LicensePlateManager {

/**
 * @brief Phân loại các trường hợp lỗi khi xác thực định dạng biển số xe.
 */
enum class ValidationError {
    NONE = 0,                  // Biển số hợp lệ
    INVALID_LENGTH,            // Sai độ dài biển số (khác 11 ký tự)
    MISSING_HYPHEN,            // Thiếu hoặc sai vị trí dấu '-' (tại index 2 và 5)
    INVALID_PROVINCE_CODE,     // Mã tỉnh không hợp lệ (không phải 2 chữ số 00-99)
    INVALID_COMMUNE_CODE,      // Mã xã không hợp lệ (không phải 2 ký tự chữ/số)
    INVALID_SERIAL_NUMBER      // Số serial không hợp lệ (không phải 5 chữ số)
};

/**
 * @brief Cấu trúc lưu trữ kết quả kiểm tra chi tiết biển số xe.
 */
struct ValidationResult {
    bool isValid;               // Trạng thái hợp lệ (true/false)
    ValidationError error;      // Loại lỗi gặp phải
    std::string errorMessage;   // Nội dung thông báo lỗi bằng tiếng Việt
};

/**
 * @brief Lớp tiện ích kiểm tra và chuẩn hóa dữ liệu biển số xe (chuẩn ab-cd-ghxyz).
 */
class Validator {
public:
    /**
     * @brief Chuẩn hóa chuỗi biển số: loại bỏ khoảng trắng và chuyển chữ thường thành chữ hoa.
     * @param rawInput Chuỗi đầu vào từ người dùng
     * @return std::string Chuỗi đã chuẩn hóa
     * Độ phức tạp thời gian: O(L) với L là độ dài chuỗi
     * Độ phức tạp không gian: O(L)
     */
    static std::string normalizePlateString(const std::string& rawInput);

    /**
     * @brief Kiểm tra nhanh biển số có đúng định dạng chuẩn ab-cd-ghxyz hay không.
     * @param plate Chuỗi biển số xe
     * @return true nếu hợp lệ, false nếu không hợp lệ
     * Độ phức tạp thời gian: O(1) (độ dài cố định 11)
     * Độ phức tạp không gian: O(1)
     */
    static bool isValidLicensePlateFormat(const std::string& plate);

    /**
     * @brief Kiểm tra chi tiết từng thành phần và trả về mã lỗi kèm thông báo cụ thể.
     * @param plate Chuỗi biển số xe
     * @return ValidationResult Kết quả kiểm tra chi tiết
     * Độ phức tạp thời gian: O(1)
     * Độ phức tạp không gian: O(1)
     */
    static ValidationResult validateDetailed(const std::string& plate);

    /**
     * @brief Kiểm tra mã tỉnh ab gồm đúng 2 ký tự số (00 - 99).
     * @param provinceCode Chuỗi mã tỉnh
     * @return true nếu hợp lệ
     * Độ phức tạp thời gian: O(1)
     * Độ phức tạp không gian: O(1)
     */
    static bool isValidProvinceCode(const std::string& provinceCode);

    /**
     * @brief Kiểm tra mã xã cd gồm đúng 2 ký tự chữ hoặc số (A-Z, 0-9).
     * @param communeCode Chuỗi mã xã
     * @return true nếu hợp lệ
     * Độ phức tạp thời gian: O(1)
     * Độ phức tạp không gian: O(1)
     */
    static bool isValidCommuneCode(const std::string& communeCode);

    /**
     * @brief Bí danh tương thích cho isValidCommuneCode theo tài liệu đặc tả (Ward Code).
     */
    static bool isValidWardCode(const std::string& wardCode) {
        return isValidCommuneCode(wardCode);
    }

    /**
     * @brief Kiểm tra số serial ghxyz gồm đúng 5 ký tự số (00000 - 99999).
     * @param serialNumber Chuỗi số serial
     * @return true nếu hợp lệ
     * Độ phức tạp thời gian: O(1)
     * Độ phức tạp không gian: O(1)
     */
    static bool isValidSerialNumber(const std::string& serialNumber);
};

} // namespace LicensePlateManager

#endif // VALIDATOR_H