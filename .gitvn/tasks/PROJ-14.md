---
id: PROJ-14
title: "Xây dựng bộ sinh 1.000 dữ liệu mẫu chuẩn UTF-8 & Stress Test File"
status: todo
priority: high
assignee: duquochung
reporter: hnamhocit
sprintId: sprint-3
storyPoints: 3
createdAt: 2026-11-02T14:30:00.000Z
updatedAt: 2026-11-02T14:30:00.000Z
---

### Mục tiêu
Viết hàm sinh ngẫu nhiên 1.000 bản ghi biển số xe chuẩn ab-cd-ghxyz thuộc nhiều tỉnh thành để phục vụ kiểm thử và demo.

### Acceptance Criteria
- Sinh file dữ liệu mẫu ít nhất 1.000 bản ghi không bị trùng lặp biển số.
- Phân bố đều các tỉnh thành lớn (59, 29, 61, 43, 60...).
- Thử nghiệm stress test nạp file 10.000 bản ghi thành công trong thời gian < 100ms.
