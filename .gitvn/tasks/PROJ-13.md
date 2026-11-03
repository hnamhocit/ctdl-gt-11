---
id: PROJ-13
title: "Quản lý thu hồi bộ nhớ Heap (clear, Destructor) & Code Review lõi"
status: todo
priority: high
assignee: hnamhocit
reporter: hnamhocit
sprintId: sprint-3
storyPoints: 3
createdAt: 2026-11-02T14:30:00.000Z
updatedAt: 2026-11-02T14:30:00.000Z
---

### Mục tiêu
Hoàn thiện cơ chế thu hồi 100% vùng nhớ Heap trong hàm clear() và destructor, tối ưu hiệu năng con trỏ toàn hệ thống.

### Acceptance Criteria
- Duyệt qua 10.007 buckets và delete mọi PlateNode mà không sót byte nào.
- Kiểm tra bằng Valgrind: 0 bytes leaked in 0 blocks.
- Review chuẩn hóa mã nguồn toàn bộ các file .h và .cpp của nhóm.
