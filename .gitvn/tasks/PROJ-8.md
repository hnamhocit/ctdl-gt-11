---
id: PROJ-8
title: "Hiện thực chức năng Đọc/Ghi dữ liệu file TXT (loadFromFile, saveToFile)"
status: review
priority: urgent
assignee: duquochung
reporter: hnamhocit
sprintId: sprint-2
storyPoints: 5
createdAt: 2026-11-02T14:30:00.000Z
updatedAt: 2026-11-12T04:02:55.515Z
---

### Mục tiêu
Hiện thực module đọc và ghi tệp TXT bền vững, tự động nạp khi khởi chạy và tự động lưu khi thoát ứng dụng.

### Acceptance Criteria
- Nạp chính xác từng dòng dữ liệu vào Bảng Băm thông qua insertPlate.
- Tự động bỏ qua các dòng dữ liệu bẩn/lỗi mà không crash chương trình.
- Lưu toàn bộ dữ liệu hiện hành trong bảng băm ra file TXT khi thoát.
