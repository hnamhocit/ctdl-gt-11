#include <filesystem>
#include <iostream>
#include <string>

#include "FileIO.h"
#include "HashTable.h"
#include "MenuUI.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

/**
 * @brief Xác định đường dẫn file dữ liệu tự động thích ứng với thư mục thực thi.
 * 
 * Giải quyết triệt để lỗi "File not found" khi người dùng thực thi ứng dụng
 * từ các thư mục làm việc khác nhau:
 * - Chạy từ thư mục gốc dự án: "./bin/main" -> tìm thấy "data/sample_plates.txt"
 * - Chạy từ thư mục nhị phân: "cd bin && ./main" -> tìm thấy "../data/sample_plates.txt"
 * - Chạy từ thư mục build của IDE: tìm thấy "../../data/sample_plates.txt"
 * 
 * @param filename Tên file dữ liệu cần định vị (mặc định: "sample_plates.txt").
 * @return std::string Đường dẫn file hợp lệ đã kiểm tra tồn tại, hoặc đường dẫn khả dĩ nhất.
 * 
 * Độ phức tạp:
 * - Thời gian: O(1) do số lượng đường dẫn ứng viên là hằng số.
 * - Không gian: O(1).
 */
static std::string resolveDataFilePath(const std::string &filename = "sample_plates.txt")
{
    const std::string candidates[] = {
        "data/" + filename,
        "../data/" + filename,
        "../../data/" + filename
    };

    // 1. Ưu tiên đường dẫn mà file thực sự tồn tại
    for (const auto &path : candidates)
    {
        if (fs::exists(path))
        {
            return path;
        }
    }

    // 2. Nếu file chưa có, tìm thư mục data/ hợp lệ để chuẩn bị cho việc lưu mới
    const std::string dirCandidates[] = {"data", "../data", "../../data"};
    for (const auto &dir : dirCandidates)
    {
        if (fs::is_directory(dir))
        {
            return dir + "/" + filename;
        }
    }

    // 3. Fallback mặc định theo chuẩn cấu trúc thư mục dự án
    return "data/" + filename;
}

/**
 * @brief Nạp dữ liệu, chạy menu và lưu các bản ghi được thêm trong phiên.
 * @return 0 khi hoàn tất, 1 nếu tự động lưu thất bại.
 * @note Thời gian phụ thuộc vào các thao tác trong phiên. Không gian O(M + N)
 * với M là số bucket và N là số bản ghi, chưa tính bộ đệm nhập liệu.
 */
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::cout << "CTDL&GT - De tai 11: Quan ly bien so xe\n";

    LicensePlateHashTable table;
    const std::string dataFilePath = resolveDataFilePath();

    // Tự động nạp dữ liệu khi khởi động
    LoadStats stats;
    bool isLoaded = loadFromFile(table, dataFilePath.c_str(), &stats);
    if (isLoaded)
    {
        std::cout << "Da nap " << stats.loaded << " bien so tu [" << dataFilePath << "] (bo qua "
                  << stats.invalid << " dong loi, " << stats.duplicates
                  << " dong trung)\n";
    }
    else
    {
        std::cout << "Chua co file du lieu tai [" << dataFilePath << "], bat dau voi bang rong.\n";
    }

    const auto initialCount = table.getTotalCount();
    LicensePlateManager::MenuUI::run(table);

    // Menu chỉ thêm bản ghi, nên số lượng thay đổi khi có dữ liệu mới cần lưu.
    if (table.getTotalCount() != initialCount)
    {
        if (!saveToFile(table, dataFilePath.c_str()))
        {
            std::cerr << "Canh bao: khong the luu du lieu ra " << dataFilePath << "\n";
            return 1;
        }
    }
    return 0;
}
