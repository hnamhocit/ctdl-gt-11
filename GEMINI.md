# Gemini & Antigravity Agent Guidelines

> Dự án: **Đề tài 11 - Quản lý Biển Số Xe (CTDL&GT)**  
> Thư mục tài liệu chi tiết: [docs/](file:///home/hnamhocit/Workspace/cpp/ctdl-gt-11/docs/)  
> Tài liệu quy tắc chuẩn: [AGENTS.md](file:///home/hnamhocit/Workspace/cpp/ctdl-gt-11/AGENTS.md)

## Core Directives For Gemini / Antigravity Agent:

1. **Academic Constraints**:
   - MUST NOT use `<map>`, `<unordered_map>`, `<unordered_set>`, `<set>`, or `<list>` for core plate storage.
   - Always implement manual pointer-based Hash Table (`PlateNode* buckets[10007]`) with Separate Chaining.
   - Hash function: DJB2 algorithm with multiplier 33 and initial value 5381.

2. **Code Quality**:
   - C++17 standard, modular design with `.h` in `include/` and `.cpp` in `src/`.
   - Clean compilation: `g++ -std=c++17 -Wall -Wextra -Wpedantic -O2`. Zero warnings.
   - Zero memory leaks: Implement thorough `clear()` and destructor freeing every heap-allocated node.

3. **Format Standards**:
   - License plate format: `ab-cd-ghxyz` (ab: 2 digits, cd: 2 alphanumeric, ghxyz: 5 digits). Total length 11.
   - Validate and normalize inputs (trim, uppercase).

4. **Oral Defense Support**:
   - Add clear comments explaining Big-O complexity and pointer manipulation logic for all methods so the 6 team members can defend and live-code in front of examiners.
