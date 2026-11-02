---
trigger: always_on
description: Core architectural rules and constraints for Topic 11 CTDL&GT (License Plate Management System in C++)
---

# Quy Tắc Bắt Buộc Dự Án: Quản Lý Biển Số Xe (Đề tài 11)

1. **Ràng buộc CTDL cốt lõi**:
   - Tuyệt đối không dùng `std::unordered_map`, `std::map`, `std::unordered_set`, `std::set`, `std::list`.
   - Bảng băm phải dùng mảng con trỏ thuần `PlateNode* buckets[10007]` và danh sách liên kết đơn (Separate Chaining).
   - Hàm băm: DJB2 hash algorithm.

2. **Tiêu chuẩn C++ & Biên dịch**:
   - Biên dịch với cờ `-std=c++17 -Wall -Wextra -Wpedantic -O2`. Không để sinh warning.
   - Quản lý bộ nhớ: Mọi `new` phải có `delete`. Destructor và `clear()` giải phóng toàn bộ nodes trong mọi bucket.

3. **Định dạng biển số**:
   - Chuẩn `ab-cd-ghxyz` (ab: 2 số, cd: 2 chữ/số viết hoa, ghxyz: 5 số).

4. **Hỗ trợ vấn đáp & phân công**:
   - Thêm chú thích Big-O và cơ chế con trỏ cho mọi hàm để sinh viên giải thích được khi bảo vệ.
   - Tuân thủ phân công công việc của 6 thành viên trong nhóm.
