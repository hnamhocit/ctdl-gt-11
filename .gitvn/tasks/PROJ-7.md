---
id: PROJ-7
title: "Hiện thực chức năng Đăng ký biển số mới và Tra cứu O(1)"
status: todo
priority: high
assignee: hnamhocit
reporter: hnamhocit
sprintId: sprint-2
storyPoints: 3
createdAt: 2026-11-02T14:30:00.000Z
updatedAt: 2026-11-02T14:30:00.000Z
---

### Mục tiêu
Cài đặt phương thức insertPlate và searchPlate trên Bảng Băm sử dụng con trỏ thuần và cơ chế Separate Chaining.

### Acceptance Criteria
- Chèn biển số mới vào đầu danh sách liên kết tại bucket tương ứng trong O(1) trung bình.
- Tra cứu biển số trả về PlateNode* hoặc nullptr trong O(1).
- Bắt lỗi trùng lặp: không cho phép chèn nếu biển số đã tồn tại.
