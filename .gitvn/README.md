# 🤖 GitVN Workspace — Quy chuẩn Cấu trúc Dữ liệu & AI Prompting Guide

> **Dành cho AI Agents (Cursor, Claude, ChatGPT, Gemini, Copilot, Antigravity) và Developers**:
> Thư mục `.gitvn/` là Single Source of Truth (SSOT) cho toàn bộ quy trình Agile Sprint, Kanban Task Board và Tài liệu Kỹ thuật (Docs) được lưu trữ theo chuẩn Git-Native.

---

## 1. Cấu trúc thư mục `.gitvn/`

```bash
.gitvn/
├── README.md           # Tài liệu quy chuẩn này (dành cho AI & Developers)
├── config.yml          # Cấu hình v2.0 (hoặc config.json tương thích cũ)
├── tasks/              # Kiến trúc File rời chống xung đột Git Merge Conflicts (SSOT)
│   ├── PROJ-1.md       # Task chi tiết dạng Markdown + Frontmatter (hoặc PROJ-1.json)
│   ├── PROJ-2.md
│   └── ...
├── tasks.json          # File cache tổng hợp (tương thích ngược)
└── docs/               # Thư mục chứa tài liệu dự án (.md / .mdx)
    ├── index.mdx       # Tổng quan dự án
    └── architecture.mdx# Kiến trúc kỹ thuật
```

---

## 2. Giải pháp chống Xung đột Git (Merge Conflicts) với Kiến trúc File Rời

* **Vấn đề**: Khi team 4–5 người hoặc nhiều AI Agents cùng làm việc trên các branch khác nhau, cùng kéo thả task hoặc cập nhật mô tả task, việc merge branch sẽ gây ra conflict JSON liên tục nếu dùng 1 file `tasks.json` tập trung.
* **Cách khắc phục**: Lưu trữ dạng file rời (tương tự Astro / Docusaurus content collections). Mỗi task là 1 file riêng biệt `.gitvn/tasks/{TASK_ID}.md` hoặc `.gitvn/tasks/{TASK_ID}.json`. Khi đó mỗi người sửa một task sẽ **không bao giờ chạm vào file của nhau**, merge branch hoàn toàn an toàn và trơn tru.

### Quy chuẩn Task File dạng Markdown (`.gitvn/tasks/PROJ-1.md`):

```markdown
---
id: PROJ-1
title: "Khởi tạo dự án & cấu hình môi trường"
status: in_progress
priority: high
assignee: octocat
reporter: admin
sprintId: sprint-1
storyPoints: 3
createdAt: 2026-11-03T00:00:00.000Z
updatedAt: 2026-11-03T00:00:00.000Z
---

### Mục tiêu
- Thiết lập thư mục `.gitvn/` theo chuẩn Git-Native.
- Phân chia công việc và kết nối CI/CD.

### Acceptance Criteria
- [x] Cấu hình file rời trong `.gitvn/tasks/`
- [ ] Build dự án thành công không lỗi
```

---

## 3. Quy chuẩn Schema File `.gitvn/tasks.json` (Tổng hợp)

```json
{
  "owner": "tên_owner_hoặc_org",
  "repo": "tên_repository",
  "taskSeq": 10,
  "tasks": [
    {
      "id": "PROJ-1",
      "title": "Tiêu đề công việc ngắn gọn",
      "description": "Mô tả chi tiết bằng Markdown...",
      "status": "todo",
      "priority": "high",
      "assignee": { "login": "octocat", "name": "The Octocat" },
      "reporter": { "login": "admin", "name": "Admin" },
      "sprintId": "sprint-1",
      "storyPoints": 3,
      "createdAt": "2026-11-02T12:00:00.000Z",
      "updatedAt": "2026-11-02T12:00:00.000Z"
    }
  ],
  "sprints": [
    {
      "id": "sprint-1",
      "name": "Sprint 1 - Nền tảng",
      "goal": "Hoàn thiện kiến trúc cơ bản",
      "startDate": "2026-11-01",
      "endDate": "2026-11-14",
      "status": "active",
      "createdAt": "2026-11-01T00:00:00.000Z"
    }
  ]
}
```

### Enum Constraints:
* **status**: `"backlog" | "todo" | "in_progress" | "review" | "done"`
* **priority**: `"low" | "medium" | "high" | "urgent"`
* **sprintId**: chuỗi sprint ID (vd: `"sprint-1"`) hoặc `null` nếu là Backlog.
* **sprints[].status**: `"planned" | "active" | "completed"`

### Git-Agile Automation (Đồng bộ PR với Kanban):
* Khi có PR đang mở (`state: "open"`) liên kết với Task ID trong tiêu đề hoặc nhánh: Task `"in_progress"` tự động chuyển sang `"review"` kèm badge PR.
* Khi PR được merge (`merged_at`): Task `"review"` tự động chuyển sang `"done"`.
