#include <iostream>

#include "FileIO.h"
#include "HashTable.h"
#include "MenuUI.h"

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * Nạp dữ liệu, chạy menu và lưu khi có bản ghi mới. Trả 1 nếu lưu thất bại, ngược lại trả 0.
 * Thời gian theo các thao tác trong phiên. Bộ nhớ O(M + N) cho M bucket và N bản ghi, cộng bộ đệm nhập liệu.
 */
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::cout << "CTDL&GT - De tai 11: Quan ly bien so xe\n";

    LicensePlateHashTable table;
    constexpr char dataFilePath[] = LICENSE_PLATE_DATA_DIR "/sample_plates.txt";

    // Tự động nạp dữ liệu khi khởi động
    LoadStats stats;
    bool isLoaded = loadFromFile(table, dataFilePath, &stats);
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
        if (!saveToFile(table, dataFilePath))
        {
            std::cerr << "Canh bao: khong the luu du lieu ra " << dataFilePath << "\n";
            return 1;
        }
    }
    return 0;
}
