# PROJ-17 — Kết quả kiểm tra bộ nhớ Valgrind (AC-03)

Người thực hiện: Huỳnh Quốc Việt. Ngày chạy: 20/11/2026.
Môi trường: WSL2 Ubuntu, g++ 15.2.0, Valgrind 3.26.0.

## Lệnh thực thi
g++ -std=c++17 -g -O0 -Iinclude src/Plate.cpp src/HashTable.cpp src/Validator.cpp tests/test_proj11.cpp -o bin/test_proj11
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind_report.txt ./bin/test_proj11

## Log thực tế
=== KET QUA: 20/20 PASS, 0 FAIL ===
==2578== HEAP SUMMARY:
==2578==     in use at exit: 0 bytes in 0 blocks
==2578==   total heap usage: 19 allocs, 19 frees, 75,936 bytes allocated
==2578== All heap blocks were freed -- no leaks are possible
==2578== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

## Kết luận
AC-03 đạt: không rò rỉ bộ nhớ, không có lỗi truy cập bộ nhớ.
