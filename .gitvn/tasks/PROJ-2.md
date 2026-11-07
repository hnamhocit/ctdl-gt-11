---
id: PROJ-2
title: "Cài đặt module Validator kiểm tra định dạng biển số ab-cd-ghxyz"
status: in_progress
priority: urgent
assignee: trinhquocthai
reporter: hnamhocit
sprintId: sprint-1
storyPoints: 5
createdAt: 2026-11-02T14:30:00.000Z
updatedAt: 2026-11-07T04:17:17.614Z
---

### Mục tiêu
Xây dựng bộ kiểm tra tính hợp lệ cú pháp nghiêm ngặt của biển số, mã tỉnh, mã xã và các hàm chuẩn hóa chuỗi.

### Acceptance Criteria
- Hàm isValidLicensePlateFormat kiểm tra chuẩn ab-cd-ghxyz (độ dài 11 ký tự).
- Bắt lỗi mã tỉnh không phải số (00-99), mã xã sai ký tự, số serial không đủ 5 số.
- Hàm normalizePlateString loại bỏ khoảng trắng thừa và viết hoa mã xã.
