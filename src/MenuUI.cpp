#include "MenuUI.h"

#include "AdministrativeFilter.h"
#include "FileIO.h"
#include "HashTable.h"
#include "Plate.h"
#include "Validator.h"

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

namespace LicensePlateManager {
namespace {

/**
 * @brief In lời nhắc và đọc một dòng, giữ khoảng trắng bên trong.
 * @param prompt Lời nhắc nhập liệu.
 * @param value Chuỗi nhận kết quả đã bỏ khoảng trắng ở hai đầu.
 * @return true khi đọc được một dòng, false khi luồng kết thúc hoặc lỗi.
 * @note Thời gian và không gian O(L) với L là chiều dài dòng.
 */
bool readInput(const char* prompt, std::string& value) {
    std::cout << prompt;
    if (!std::getline(std::cin, value)) {
        return false;
    }

    const auto first = value.find_first_not_of(" \t\r");
    if (first == std::string::npos) {
        value.clear();
    } else {
        value = value.substr(first, value.find_last_not_of(" \t\r") - first + 1);
    }
    return true;
}

/**
 * @brief Nhập và lưu một biển số mới vào bảng băm.
 * @param table Bảng băm nhận bản ghi hợp lệ.
 * @note Không trả về giá trị. Thời gian O(L + C), không gian O(L),
 * với L là tổng độ dài đầu vào và C là độ dài chuỗi bucket cần tra cứu.
 */
void registerPlate(LicensePlateHashTable& table) {
    const std::string plate = MenuUI::inputAndValidateLicensePlate();
    if (plate.empty()) {
        return;
    }
    if (table.searchPlate(plate.c_str())) {
        std::cout << "Biển số " << plate << " đã tồn tại.\n";
        return;
    }

    PlateNode node{};
    parseLicensePlate(plate.c_str(), node);

    std::string owner;
    if (!readInput("Nhập họ tên chủ xe: ", owner)) {
        return;
    }
    if (owner.empty() || owner.size() >= sizeof(node.ownerName) ||
        owner.find_first_of("|\r") != std::string::npos ||
        owner.find('\0') != std::string::npos) {
        std::cout << "Tên chủ xe phải có từ 1 đến 49 byte và không chứa ký tự phân cách.\n";
        return;
    }

    std::string date;
    if (!readInput("Nhập ngày đăng ký (YYYY-MM-DD): ", date)) {
        return;
    }
    if (!isValidRegistrationDate(date)) {
        std::cout << "Ngày đăng ký không đúng định dạng YYYY-MM-DD.\n";
        return;
    }

    std::memcpy(node.ownerName, owner.c_str(), owner.size() + 1);
    std::memcpy(node.registeredDate, date.c_str(), date.size() + 1);
    if (table.insertPlate(node)) {
        std::cout << "Đã đăng ký biển số " << plate << ".\n";
    }
}

/**
 * @brief Hiển thị thông tin đăng ký của một biển số.
 * @param table Bảng băm được tra cứu.
 * @note Không trả về giá trị. Thời gian O(L + C), không gian O(L),
 * với L là tổng độ dài đầu vào và C là độ dài chuỗi bucket cần tra cứu.
 */
void searchPlate(LicensePlateHashTable& table) {
    const std::string plate = MenuUI::inputAndValidateLicensePlate();
    if (plate.empty()) {
        return;
    }
    const PlateNode* node = table.searchPlate(plate.c_str());
    if (node) {
        std::cout << "Biển số: " << node->fullPlate
                  << "\nChủ xe: " << node->ownerName
                  << "\nNgày đăng ký: " << node->registeredDate << '\n';
    } else {
        std::cout << "Không tìm thấy biển số " << plate << ".\n";
    }
}

/**
 * @brief Hiển thị danh sách mã xã của tỉnh hoặc biển số của một xã.
 * @param table Bảng băm được lọc.
 * @param byWard true để lọc biển số theo tỉnh và xã.
 * @note Không trả về giá trị. Thời gian O(L + M + N * K) khi liệt kê xã,
 * O(L + M + N) khi liệt kê biển số. Không gian O(L + W), với L là độ dài
 * đầu vào, M là số bucket, N là số bản ghi, K là số xã tìm được và W là
 * dung lượng bộ đệm cố định của bộ lọc.
 */
void showAdministrativeList(const LicensePlateHashTable& table, bool byWard) {
    std::string province;
    if (!readInput("Nhập mã tỉnh (VD: 59): ", province)) {
        return;
    }
    if (!Validator::isValidProvinceCode(province)) {
        std::cout << "Mã tỉnh phải gồm đúng hai chữ số.\n";
        return;
    }

    AdministrativeFilter filter;
    if (!byWard) {
        filter.displayWardsOfProvince(table, province.c_str());
        return;
    }

    std::string ward;
    if (!readInput("Nhập mã xã (VD: X1): ", ward)) {
        return;
    }
    ward = Validator::normalizePlateString(ward);
    if (!Validator::isValidWardCode(ward)) {
        std::cout << "Mã xã phải gồm đúng hai ký tự chữ hoặc số.\n";
        return;
    }
    filter.displayPlatesOfWard(table, province.c_str(), ward.c_str());
}

/**
 * @brief Nạp bổ sung hoặc lưu dữ liệu theo đường dẫn người dùng chọn.
 * @param table Bảng băm dùng để nạp và lưu.
 * @note Không trả về giá trị. Khi nạp R dòng dài tối đa L, thời gian O(R * (L + C))
 * với C là độ dài chuỗi bucket lớn nhất. Khi lưu, thời gian O(M + N)
 * với M bucket và N bản ghi. Không gian phụ O(P + L) khi nạp và O(P)
 * khi lưu, với P là độ dài đường dẫn.
 */
void showFileMenu(LicensePlateHashTable& table) {
    std::cout << "\n1. Nạp bổ sung dữ liệu từ tệp TXT\n"
              << "2. Lưu dữ liệu ra tệp TXT\n"
              << "0. Quay lại menu chính\n";
    const int choice = MenuUI::getValidChoice(0, 2);
    if (choice <= 0) {
        return;
    }

    std::string path;
    if (!readInput("Nhập đường dẫn tệp TXT (0 để hủy): ", path) || path == "0") {
        return;
    }
    if (path.empty()) {
        std::cout << "Đường dẫn tệp không được để trống.\n";
        return;
    }

    if (choice == 1) {
        LoadStats stats;
        if (loadFromFile(table, path.c_str(), &stats)) {
            std::cout << "Đã nạp " << stats.loaded << " biển số, bỏ qua "
                      << stats.invalid << " dòng lỗi và " << stats.duplicates << " dòng trùng.\n";
        } else {
            std::cout << "Không thể mở tệp [" << path << "].\n";
        }
    } else if (saveToFile(table, path.c_str())) {
        std::cout << "Đã lưu dữ liệu vào [" << path << "].\n";
    } else {
        std::cout << "Không thể lưu dữ liệu vào [" << path << "].\n";
    }
}

} // namespace

/** Hiển thị menu chính. Không có tham số hay giá trị trả về. Thời gian và không gian O(1). */
void MenuUI::displayMenu() {
    std::cout << "\n========================================================\n"
              << " HỆ THỐNG QUẢN LÝ BIỂN SỐ XE - NHÓM 11\n"
              << "========================================================\n"
              << " 1. Đăng ký biển số xe mới\n"
              << " 2. Tra cứu thông tin biển số xe\n"
              << " 3. Hiển thị danh sách mã xã theo mã tỉnh\n"
              << " 4. Hiển thị danh sách biển số xe theo xã\n"
              << " 5. Nạp hoặc lưu dữ liệu từ tệp TXT\n"
              << " 0. Thoát chương trình\n";
}

/**
 * @brief Đọc lựa chọn nguyên trong khoảng [minVal, maxVal].
 * @param minVal Cận dưới của lựa chọn.
 * @param maxVal Cận trên của lựa chọn.
 * @return Lựa chọn hợp lệ hoặc -1 nếu luồng nhập kết thúc hoặc lỗi.
 * @note Thời gian O(L) với L là tổng độ dài các dòng đã đọc.
 * Không gian O(K) với K là chiều dài dòng dài nhất.
 */
int MenuUI::getValidChoice(int minVal, int maxVal) {
    std::string line;
    const std::string prompt = "Nhập lựa chọn (" + std::to_string(minVal) +
                               " - " + std::to_string(maxVal) + "): ";
    while (readInput(prompt.c_str(), line)) {
        std::istringstream input(line);
        int choice;
        if (input >> choice && input.eof() && choice >= minVal && choice <= maxVal) {
            return choice;
        }
        std::cout << "Lựa chọn không hợp lệ. Vui lòng nhập lại.\n";
    }
    return -1;
}

/**
 * @brief Đọc và chuẩn hóa biển số đến khi hợp lệ.
 * @return Biển số đã chuẩn hóa hoặc chuỗi rỗng nếu luồng nhập kết thúc hoặc lỗi.
 * @note Không có tham số. Thời gian O(L) với L là tổng độ dài các dòng đã đọc.
 * Không gian O(K) với K là chiều dài dòng dài nhất.
 */
std::string MenuUI::inputAndValidateLicensePlate() {
    std::string rawInput;
    while (readInput("Nhập biển số xe (VD: 59-X1-12345): ", rawInput)) {
        const std::string plate = Validator::normalizePlateString(rawInput);
        const ValidationResult result = Validator::validateDetailed(plate);
        if (result.isValid) {
            return plate;
        }
        std::cout << result.errorMessage << " Vui lòng nhập lại.\n";
    }
    return {};
}

/**
 * @brief Chạy menu độc lập với bảng băm rỗng.
 * @note Không có tham số hay giá trị trả về. Thời gian O(M) để tạo và
 * hủy bảng rỗng cộng với thao tác trong phiên. Không gian O(M + N)
 * cho M bucket và N bản ghi, cộng bộ đệm nhập và FileIO.
 */
void MenuUI::run() {
    LicensePlateHashTable table;
    run(table);
}

/**
 * @brief Chạy menu trên bảng băm đã được ứng dụng khởi tạo.
 * @param table Bảng băm dùng chung trong suốt phiên làm việc.
 * @note Không trả về giá trị. Thời gian O(T + S), với T là số lượt chọn và
 * S là tổng thời gian thực hiện chúng. Không gian bằng mức sử dụng lớn nhất
 * của một thao tác, cộng các bản ghi mới được giữ trong bảng.
 */
void MenuUI::run(LicensePlateHashTable& table) {
    while (std::cin) {
        displayMenu();
        switch (getValidChoice(0, 5)) {
        case 1:
            registerPlate(table);
            break;
        case 2:
            searchPlate(table);
            break;
        case 3:
            showAdministrativeList(table, false);
            break;
        case 4:
            showAdministrativeList(table, true);
            break;
        case 5:
            showFileMenu(table);
            break;
        case 0:
            std::cout << "\nCảm ơn bạn đã sử dụng hệ thống. Tạm biệt!\n";
            return;
        default:
            return;
        }
    }
}

} // namespace LicensePlateManager
