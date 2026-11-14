#include <iostream>

#include "FileIO.h"
#include "HashTable.h"

#ifdef _WIN32
#include <windows.h>
#endif

static const char *DATA_FILE = "data/sample_plates.txt";

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // để console Windows hiển thị đúng tiếng Việt
#endif
    std::cout << "CTDL&GT - De tai 11: Quan ly bien so xe\n";

    LicensePlateHashTable table;

    // Tự động nạp dữ liệu khi khởi động
    LoadStats stats;
    if (loadFromFile(table, DATA_FILE, &stats))
    {
        std::cout << "Da nap " << stats.loaded << " bien so (bo qua "
                  << stats.invalid << " dong loi, " << stats.duplicates
                  << " dong trung)\n";
    }
    else
    {
        std::cout << "Chua co file du lieu, bat dau voi bang rong.\n";
    }

    // TODO: menu/UI của nhóm (MenuUI) sẽ được gắn vào đây.

    // Tự động lưu dữ liệu khi thoát
    if (!saveToFile(table, DATA_FILE))
    {
        std::cerr << "Canh bao: khong the luu du lieu ra " << DATA_FILE << "\n";
    }
    return 0;
}