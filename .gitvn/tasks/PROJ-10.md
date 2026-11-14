---
id: PROJ-10
title: "Xây dựng Console Menu tương tác & Xử lý bẫy lỗi nhập liệu"
status: review
priority: high
assignee: trinhquocthai
reporter: hnamhocit
sprintId: sprint-2
storyPoints: 3
createdAt: 2026-11-02T14:30:00.000Z
updatedAt: 2026-11-14T04:42:30.710Z
---

### Mục tiêu
Hiện thực giao diện Console điều hướng bằng phím số, kết hợp với module Validator để kiểm soát dữ liệu đầu vào người dùng.

### Acceptance Criteria
- Menu hiển thị 6 chức năng rõ ràng, phân luồng điều hướng thân thiện.
- Bẫy lỗi nhập ký tự chữ khi chọn menu số, ngăn ngừa vòng lặp vô tận cin.fail().
- Xuất thông báo lỗi chi tiết khi người dùng nhập sai định dạng biển số.
