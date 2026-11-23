#include "FileIO.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <random>
namespace
{

    const char *const HEADER_LINE =
        "fullPlate|provinceCode|wardCode|serialNum|registeredDate|ownerName";

    // Bộ mẫu dùng 8 mã tỉnh và 8 mã xã mỗi tỉnh, tạo 8 × 8 = 64 cặp.
    // Mỗi cặp có 100.000 số thứ tự (00000–99999), tổng cộng 6.400.000 biển số.
    const int PROVINCE_COUNT = 8;
    const int WARD_COUNT = 8;
    const int MAX_SAMPLE_COUNT = PROVINCE_COUNT * WARD_COUNT * 100000;

    bool isDigit(char c) { return c >= '0' && c <= '9'; }
    bool isUpperAlnum(char c) { return isDigit(c) || (c >= 'A' && c <= 'Z'); }

    /** Kiểm tra dạng ab-cd-ghxyz (11 ký tự). O(1). */
    bool isWellFormedPlate(const std::string &p)
    {
        if (p.size() != 11)
            return false;
        if (!isDigit(p[0]) || !isDigit(p[1]) || p[2] != '-')
            return false;
        if (!isUpperAlnum(p[3]) || !isUpperAlnum(p[4]) || p[5] != '-')
            return false;
        for (int i = 6; i < 11; ++i)
        {
            if (!isDigit(p[i]))
                return false;
        }
        return true;
    }

    /** Cắt khoảng trắng/tab ở hai đầu chuỗi. O(L). */
    std::string trim(const std::string &s)
    {
        const char *ws = " \t";
        std::size_t b = s.find_first_not_of(ws);
        if (b == std::string::npos)
            return "";
        std::size_t e = s.find_last_not_of(ws);
        return s.substr(b, e - b + 1);
    }

    /**
     * Tách dòng theo '|' vào fields[0..maxFields-1].
     * @return tổng số trường thực tế của dòng (có thể lớn hơn maxFields).
     */
    int splitFields(const std::string &line, std::string fields[], int maxFields)
    {
        int n = 0;
        std::string cur;
        for (char c : line)
        {
            if (c == FILE_DELIMITER)
            {
                if (n < maxFields)
                    fields[n] = cur;
                ++n;
                cur.clear();
            }
            else
            {
                cur += c;
            }
        }
        if (n < maxFields)
            fields[n] = cur;
        return n + 1;
    }

    /** Chép src vào mảng char[destSize]; false nếu không vừa (tránh tràn bộ đệm). */
    bool copyField(char *dest, std::size_t destSize, const std::string &src)
    {
        if (src.size() >= destSize)
            return false;
        std::memcpy(dest, src.c_str(), src.size() + 1);
        return true;
    }

    /** true nếu chuỗi chứa ký tự làm hỏng cấu trúc file. */
    bool hasForbiddenChar(const char *s)
    {
        return std::strchr(s, FILE_DELIMITER) != nullptr ||
               std::strchr(s, '\n') != nullptr ||
               std::strchr(s, '\r') != nullptr;
    }

    /** Biến số nguyên thành chuỗi có đệm số 0 phía trước, ví dụ (7,5) -> "00007". */
    std::string zeroPad(long value, std::size_t width)
    {
        std::string s = std::to_string(value);
        while (s.size() < width)
            s.insert(s.begin(), '0');
        return s;
    }

    /**
     * Chuyển một dòng text thành PlateNode.
     * @param reason  (ra) lý do nếu dòng không hợp lệ.
     * @return true nếu dòng hợp lệ.
     */
    bool parseLine(const std::string &line, PlateNode &node, std::string &reason)
    {
        std::string f[FILE_FIELD_COUNT];
        int count = splitFields(line, f, FILE_FIELD_COUNT);
        if (count != FILE_FIELD_COUNT)
        {
            reason = "cần " + std::to_string(FILE_FIELD_COUNT) + " trường, có " +
                     std::to_string(count);
            return false;
        }
        for (int i = 0; i < FILE_FIELD_COUNT; ++i)
            f[i] = trim(f[i]);

        if (!isWellFormedPlate(f[0]))
        {
            reason = "biển số sai định dạng ab-cd-ghxyz";
            return false;
        }
        // fullPlate phải khớp với 3 trường tách ra, tránh dữ liệu mâu thuẫn
        if (f[0].compare(0, 2, f[1]) != 0 || f[0].compare(3, 2, f[2]) != 0 ||
            f[0].compare(6, 5, f[3]) != 0)
        {
            reason = "fullPlate không khớp provinceCode/wardCode/serialNum";
            return false;
        }
        if (!isValidRegistrationDate(f[4]))
        {
            reason = "ngày sai định dạng yyyy-mm-dd";
            return false;
        }
        if (f[5].empty())
        {
            reason = "tên chủ xe rỗng";
            return false;
        }

        PlateNode tmp; // constructor đã đặt next = nullptr
        if (!copyField(tmp.fullPlate, sizeof(tmp.fullPlate), f[0]) ||
            !copyField(tmp.provinceCode, sizeof(tmp.provinceCode), f[1]) ||
            !copyField(tmp.wardCode, sizeof(tmp.wardCode), f[2]) ||
            !copyField(tmp.serialNum, sizeof(tmp.serialNum), f[3]) ||
            !copyField(tmp.registeredDate, sizeof(tmp.registeredDate), f[4]) ||
            !copyField(tmp.ownerName, sizeof(tmp.ownerName), f[5]))
        {
            reason = "có trường quá dài so với bộ đệm của PlateNode";
            return false;
        }
        node = tmp;
        return true;
    }

} // namespace

/** Kiểm tra d theo dạng yyyy-mm-dd, tháng 01–12 và ngày 01–31. O(1). */
bool isValidRegistrationDate(const std::string &d)
{
    if (d.size() != 10 || d[4] != '-' || d[7] != '-')
        return false;
    for (int i = 0; i < 10; ++i)
    {
        if (i != 4 && i != 7 && !isDigit(d[i]))
            return false;
    }
    int month = (d[5] - '0') * 10 + (d[6] - '0');
    int day = (d[8] - '0') * 10 + (d[9] - '0');
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

/**
 * Nạp filepath vào table, ghi số dòng đã nạp, lỗi và trùng vào stats nếu có.
 * Trả false nếu không mở được tệp. Bộ nhớ phụ O(L) cho một dòng dài L.
 * Thời gian trung bình O(N × L) với N dòng và chuỗi bucket ngắn.
 */
bool loadFromFile(LicensePlateHashTable &table, const char *filepath, LoadStats *stats)
{
    LoadStats local;
    LoadStats &st = stats ? *stats : local;
    st = LoadStats();
    if (!filepath)
        return false;

    // Mở dạng binary: không để Windows tự đổi \r\n, và đọc đúng từng byte UTF-8.
    std::ifstream in(filepath, std::ios::binary);
    if (!in)
        return false;

    std::string line;
    int lineNo = 0;
    while (std::getline(in, line))
    {
        ++lineNo;
        if (lineNo == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0)
            line.erase(0, 3); // bỏ BOM nếu có
        if (!line.empty() && line.back() == '\r')
            line.pop_back(); // bỏ '\r' của file CRLF

        std::string t = trim(line);
        if (t.empty() || t[0] == '#')
            continue; // dòng trống / chú thích
        if (line.compare(0, 10, "fullPlate|") == 0)
            continue; // dòng tiêu đề

        PlateNode node;
        std::string reason;
        if (!parseLine(line, node, reason))
        {
            ++st.invalid;
            std::cerr << "[FileIO] Dòng " << lineNo << " bị bỏ qua: " << reason << "\n";
            continue;
        }
        if (table.insertPlate(node))
        {
            ++st.loaded;
        }
        else
        {
            ++st.duplicates;
            std::cerr << "[FileIO] Dòng " << lineNo << " trùng biển số: " << node.fullPlate << "\n";
        }
    }
    return true;
}

/**
 * Ghi table vào filepath. Trả false nếu dữ liệu không ghi được theo định dạng TXT hoặc ghi tệp thất bại.
 * Thời gian O(M + N) với M bucket và N node. Bộ nhớ phụ O(1) theo kích thước bảng.
 */
bool saveToFile(const LicensePlateHashTable &table, const char *filepath)
{
    if (!filepath)
        return false;
    const int bucketCount = table.getBucketSize();

    // Lượt 1: kiểm tra trước để không ghi dở dang ra file.
    for (int i = 0; i < bucketCount; ++i)
    {
        for (const PlateNode *n = table.getBucket(i); n != nullptr; n = n->next)
        {
            if (hasForbiddenChar(n->fullPlate) || hasForbiddenChar(n->provinceCode) ||
                hasForbiddenChar(n->wardCode) || hasForbiddenChar(n->serialNum) ||
                hasForbiddenChar(n->registeredDate) || hasForbiddenChar(n->ownerName))
            {
                std::cerr << "[FileIO] Không thể lưu, trường chứa ký tự cấm: "
                          << n->fullPlate << "\n";
                return false;
            }
        }
    }

    // Ghi ra file tạm trước; chỉ khi ghi trọn vẹn mới thay file thật.
    const std::string tmpPath = std::string(filepath) + ".tmp";
    {
        std::ofstream out(tmpPath.c_str(), std::ios::binary | std::ios::trunc);
        if (!out)
            return false;

        out << HEADER_LINE << '\n';
        for (int i = 0; i < bucketCount; ++i)
        {
            for (const PlateNode *n = table.getBucket(i); n != nullptr; n = n->next)
            {
                out << n->fullPlate << FILE_DELIMITER << n->provinceCode << FILE_DELIMITER
                    << n->wardCode << FILE_DELIMITER << n->serialNum << FILE_DELIMITER
                    << n->registeredDate << FILE_DELIMITER << n->ownerName << '\n';
            }
        }
        out.flush();
        if (!out)
        {
            out.close();
            std::remove(tmpPath.c_str());
            return false;
        }
    } // file tạm được đóng ở đây

    // Windows không cho rename đè lên file đã có, nên xóa file cũ trước.
    std::remove(filepath);
    return std::rename(tmpPath.c_str(), filepath) == 0;
}

/**
 * Ghi count biển số mẫu vào filepath. seed quyết định phần ngẫu nhiên.
 * Trả false nếu đầu vào không hợp lệ hoặc ghi thất bại. Thời gian O(count), bộ nhớ phụ O(1).
 */
bool generateSampleDataset(const char *filepath, int count, unsigned int seed)
{
    if (!filepath || count <= 0 || count > MAX_SAMPLE_COUNT)
        return false;

    static const char *const provinces[PROVINCE_COUNT] =
        {"59", "29", "61", "43", "60", "51", "30", "79"};
    static const char *const wards[WARD_COUNT] =
        {"X1", "A1", "B2", "C1", "T2", "AB", "C3", "D4"};
    static const char *const lastNames[8] =
        {"Nguyễn", "Trần", "Lê", "Phạm", "Hoàng", "Huỳnh", "Võ", "Đặng"};
    static const char *const middleNames[8] =
        {"Văn", "Thị", "Quốc", "Minh", "Hữu", "Ngọc", "Khánh", "Thanh"};
    static const char *const firstNames[10] =
        {"An", "Bình", "Cường", "Dũng", "Hà", "Hưng", "Lan", "Phúc", "Thái", "Việt"};

    std::mt19937 rng(seed);
    const long serialOffset = static_cast<long>(rng() % 100000);

    std::ofstream out(filepath, std::ios::binary | std::ios::trunc);
    if (!out)
        return false;

    out << HEADER_LINE << '\n';
    for (int i = 0; i < count; ++i)
    {
        // Đổi tỉnh mỗi dòng và đổi xã sau mỗi lượt qua 8 tỉnh.
        const char *prov = provinces[i % PROVINCE_COUNT];
        const char *ward = wards[(i / PROVINCE_COUNT) % WARD_COUNT];
        // Sau mỗi 64 dòng, số thứ tự tiến thêm 7919 theo modulo 100000.
        // Hai số nguyên tố cùng nhau nên số thứ tự không lặp trong một cặp tỉnh–xã.
        long serial = ((static_cast<long>(i / (PROVINCE_COUNT * WARD_COUNT)) + 1) * 7919 +
                       serialOffset) %
                      100000;
        std::string serialStr = zeroPad(serial, 5);

        std::string date = zeroPad(2020 + static_cast<long>(rng() % 7), 4) + "-" +
                           zeroPad(1 + static_cast<long>(rng() % 12), 2) + "-" +
                           zeroPad(1 + static_cast<long>(rng() % 28), 2);
        std::string owner = std::string(lastNames[rng() % 8]) + " " +
                            middleNames[rng() % 8] + " " + firstNames[rng() % 10];

        out << prov << "-" << ward << "-" << serialStr << FILE_DELIMITER
            << prov << FILE_DELIMITER << ward << FILE_DELIMITER << serialStr
            << FILE_DELIMITER << date << FILE_DELIMITER << owner << '\n';
    }
    return static_cast<bool>(out);
}