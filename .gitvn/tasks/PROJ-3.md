---
id: PROJ-3
title: "Thiết kế giao thức tệp TXT và module FileIO.h"
status: todo
priority: high
assignee: duquochung
reporter: hnamhocit
sprintId: sprint-1
storyPoints: 3
createdAt: 2026-11-02T14:30:00.000Z
updatedAt: 2026-11-04T15:33:19.422Z
---

### Mục tiêu
Thiết lập kiến trúc lưu trữ tệp dữ liệu phân cách bằng ký tự gạch đứng `|` và khai báo API nạp/lưu dữ liệu.

### Acceptance Criteria
- Định dạng bản ghi: fullPlate|provinceCode|wardCode|serialNum|date|ownerName.
- Hỗ trợ mã hóa tiếng Việt UTF-8 không lỗi font chữ.
- Khai báo giao diện các hàm: loadFromFile, saveToFile, generateSampleDataset.
