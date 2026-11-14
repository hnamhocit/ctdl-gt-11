---
id: PROJ-17
title: "Đo đạc Benchmark hiệu năng (100 - 10.000 mẫu) & Kiểm tra Valgrind"
status: review
priority: high
assignee: huynhquocviet
reporter: hnamhocit
sprintId: sprint-3
storyPoints: 3
createdAt: 2026-11-02T14:30:00.000Z
updatedAt: 2026-11-14T04:37:06.308Z
---

### Mục tiêu
Thực thi đo lường thời gian chèn/tra cứu với tập dữ liệu 100, 1.000, 10.000 bản ghi bằng std::chrono và chạy Valgrind.

### Acceptance Criteria
- Thu thập số liệu đo lường microsecond/nanosecond cụ thể cho Báo cáo Mục 5 & Mục 7.
- Chạy kiểm tra Valgrind trên Linux với kết quả 0 errors from 0 contexts.
- Lập bảng so sánh hệ số tải alpha và thời gian thực thi trung bình.
