---
id: PROJ-1
title: "Thiết kế PlateNode và LicensePlateHashTable (Separate Chaining)"
status: todo
priority: urgent
assignee: hnamhocit
reporter: hnamhocit
sprintId: sprint-1
storyPoints: 5
createdAt: 2026-11-02T14:30:00.000Z
updatedAt: 2026-11-07T07:48:06.745Z
---

### Mục tiêu
Hiện thực cấu trúc PlateNode với mảng ký tự tĩnh char[] thuần và bảng băm LicensePlateHashTable (TABLE_SIZE = 10007).

### Acceptance Criteria
- Struct PlateNode đầy đủ các trường: fullPlate[16], provinceCode[4], wardCode[4], serialNum[8], registeredDate[12], ownerName[50], con trỏ next.
- Khởi tạo mảng buckets[10007] bằng nullptr.
- Giải thuật băm DJB2 ((hash << 5) + hash) + c phân bố đều và hạn chế va chạm.
