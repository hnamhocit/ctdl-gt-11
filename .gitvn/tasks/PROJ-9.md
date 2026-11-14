---
id: PROJ-9
title: "Hiện thực Chức năng 3: Lọc danh sách mã Xã của Tỉnh (Khử trùng lặp)"
status: review
priority: urgent
assignee: nguyenngochung
reporter: hnamhocit
sprintId: sprint-2
storyPoints: 5
createdAt: 2026-11-02T14:30:00.000Z
updatedAt: 2026-11-10T14:32:05.922Z
---

### Mục tiêu
Hiện thực giải thuật duyệt bảng băm để trích xuất các mã xã cd thuộc tỉnh ab chỉ định và loại bỏ hoàn toàn trùng lặp.

### Acceptance Criteria
- Duyệt qua toàn bộ buckets và danh sách liên kết để lọc node có provinceCode == ab.
- Khử trùng lặp bằng thuật toán mảng danh sách distinct, chỉ in mỗi mã xã một lần.
- Thông báo rõ ràng nếu tỉnh chưa có phương tiện nào đăng ký.
