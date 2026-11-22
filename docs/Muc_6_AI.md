#### 6. KHAI THÁC VÀ SỬ DỤNG AI

#### 6.1. Danh sách công cụ AI khai thác

Trong quá trình thực hiện đề tài, các thành viên đã kết hợp một số công cụ AI để hỗ trợ tìm hiểu giải thuật, sinh dữ liệu kiểm thử và rà soát lỗi mã nguồn: 

* **ChatGPT:** Dùng để tham khảo cách tối ưu hàm băm DJB2, tạo nhanh 1.000 dòng dữ liệu biển số xe mẫu theo định dạng ab-cd-ghxyz và gợi ý các trường hợp nhập sai dữ liệu.
* **Cursor AI & Claude:** Tích hợp trực tiếp trong VS Code để hỗ trợ nhắc cú pháp C++17, kiểm tra giúp các lỗi liên quan đến con trỏ thuần và xem lại logic giải phóng bộ nhớ trong hàm clear().
* **Google Gemini & Antigravity CLI:** Dùng để hỗ trợ tổng hợp tài liệu kỹ thuật từ các tệp ghi chú trong thư mục docs/, tạo các tệp quy tắc ràng buộc (.cursorrules, GEMINI.md) và rà soát lỗi diễn đạt cho báo cáo.

#### 6.2. Mục đích phối hợp cùng AI

Thay vì để AI viết hộ toàn bộ chương trình, chúng em chỉ sử dụng AI như một công cụ hỗ trợ ở các khâu tốn nhiều thời gian thủ công: 

* **Tham khảo công thức hàm băm:** Tìm hiểu cách tính toán hàm băm chuỗi sao cho ít bị đụng độ nhất với kích thước bảng băm là số nguyên tố 10007.
* **Kiểm tra lỗi con trỏ và bộ nhớ:** Nhờ AI đọc lướt qua các hàm thêm, xóa node để phát hiện sớm những chỗ dễ bị rò rỉ bộ nhớ (Memory Leak) hoặc lỗi treo con trỏ (Dangling Pointer).
* **Tạo dữ liệu lớn để chạy thử:** Sinh tự động 1.000 bản ghi biển số xe kèm thông tin địa phương tiếng Việt có dấu chuẩn UTF-8 để tiết kiệm thời gian nhập tay.
* **Liệt kê các ca kiểm thử (Test Cases):** Gợi ý các tình huống người dùng nhập sai định dạng (thiếu dấu gạch nối, nhập chữ thay vì số, để trống...) để xây dựng bộ kiểm thử ngoại lệ.

#### 6.3. Quy trình kiểm tra, biên dịch sạch và xác minh mã nguồn từ AI trước khi tích hợp

Do quy định của học phần không cho phép dùng các thư viện tiện lợi có sẵn như std::unordered_map, std::map hay std::list mà bắt buộc phải tự cài đặt bằng con trỏ thuần, mọi gợi ý từ AI đều được kiểm tra lại kỹ lưỡng theo 4 bước trước khi tích hợp vào nhánh chính: 

1. **Thiết lập quy tắc cấm thư viện:** Tạo các tệp .cursorrules, AGENTS.md và GEMINI.md ngay tại thư mục gốc của dự án để nhắc AI chỉ được sử dụng chuẩn C++17 và con trỏ thuần ngay từ đầu.
2. **Tự viết lại mã nguồn theo đúng thiết kế:** Nếu AI gợi ý các thư viện cấm hoặc viết code quá rườm rà, các thành viên chủ động xóa bỏ và tự viết lại toàn bộ thao tác cấp phát động (new) và giải phóng bộ nhớ (delete / delete[]).
3. **Biên dịch kiểm tra cảnh báo (Zero-Warning Build):** Tất cả các tệp mã nguồn trước khi đẩy lên GitHub đều phải chạy biên dịch với bộ cờ -std=c++17 -Wall -Wextra -Wpedantic để đảm bảo không còn lỗi cú pháp hay cảnh báo dư thừa.
4. **Chạy thử nghiệm thực tế:** Đưa mã nguồn vào chạy thực tế với 1.000 biển số mẫu để đo tỷ lệ va chạm (kết quả đạt mức < 5%), đồng thời chạy qua bộ 20 Test Cases tự động để kiểm tra độ ổn định.

#### 📊 AI PROMPT LOG

##### Thành viên 1: Nguyễn Hoàng Nam

| Công cụ | Mục đích & Prompt (Câu lệnh cụ thể) | Code sinh ra từ AI | Quy trình kiểm chứng thực tế |
| :--- | :--- | :--- | :--- |
ChatGPT & Claude | "Gợi ý hàm băm chuỗi ít đụng độ cho bảng băm con trỏ thuần kích thước 10007 quản lý biển số ab-cd-ghxyz và cách viết hàm clear() tránh leak bộ nhớ." | AI gợi ý công thức DJB2 (nhân 33, dịch bit) cùng vòng lặp duyệt mảng xóa node. | Đổi kiểu dữ liệu sang size_t để tránh tràn số âm, tự viết lại toàn bộ thao tác chèn và xóa node bằng con trỏ thuần, chạy thử 1.000 mẫu đạt tỷ lệ va chạm < 5% và biên dịch không có cảnh báo.
##### Thành viên 2: Dư Quốc Hưng

| Công cụ | Mục đích & Prompt (Câu lệnh cụ thể) | Code sinh ra từ AI | Quy trình kiểm chứng thực tế |
| :--- | :--- | :--- | :--- |
ChatGPT | "Tạo 1.000 dòng dữ liệu biển số xe ngẫu nhiên đúng định dạng ab-cd-ghxyz kèm tên tỉnh/xã tiếng Việt có dấu UTF-8 lưu vào file TXT." | AI xuất ra file dữ liệu mẫu và gợi ý dùng std::getline để đọc từng dòng. | Kiểm tra ngẫu nhiên định dạng dữ liệu, đồng thời phát hiện lỗi dính ký tự xuống dòng \r khi đọc file trên Windows nên đã tự viết thêm đoạn cắt bỏ \r và khoảng trắng thừa trước khi lưu.

##### Thành viên 3: Nguyễn Ngọc Hưng

| Công cụ | Mục đích & Prompt (Câu lệnh cụ thể) | Code sinh ra từ AI | Quy trình kiểm chứng thực tế |
| :--- | :--- | :--- | :--- |
Cursor AI | "Viết hàm lọc danh sách biển số xe theo tên Tỉnh hoặc Xã và loại bỏ kết quả trùng lặp (Distinct) trong C++17." | AI viết hàm có dùng thư viện std::unordered_set để đánh dấu phần tử đã duyệt. | Học phần cấm dùng container có sẵn nên thành viên đã bỏ phần code này, tự tạo danh sách liên kết phụ bằng con trỏ để kiểm tra trùng lặp và chạy thử lại qua module AdministrativeFilter.

##### Thành viên 4: Trịnh Quốc Thái

| Công cụ | Mục đích & Prompt (Câu lệnh cụ thể) | Code sinh ra từ AI | Quy trình kiểm chứng thực tế |
| :--- | :--- | :--- | :--- |
Cursor AI & ChatGPT | "Liệt kê 20 trường hợp nhập sai định dạng biển số ab-cd-ghxyz và cách xử lý lỗi trôi lệnh khi nhập menu trên Console." | AI liệt kê các lỗi như thiếu dấu gạch nối, dư ký tự, nhập chữ vào ô số và gợi ý dùng cin.ignore(). | Dựa vào danh sách này để tự viết các hàm kiểm tra từng ký tự trong module Validator, sau đó nhập thử nhiều kiểu dữ liệu sai trên Console để đảm bảo chương trình báo lỗi gọn gàng và không bị treo.

##### Thành viên 5: Huỳnh Quốc Việt

| Công cụ | Mục đích & Prompt (Câu lệnh cụ thể) | Code sinh ra từ AI | Quy trình kiểm chứng thực tế |
| :--- | :--- | :--- | :--- |
ChatGPT & Gemini | "Hướng dẫn dùng thư viện  đo thời gian chạy các hàm thêm, tìm, xóa trên bảng băm 10007 ô và cách tổ chức 20 Test Cases." | AI gợi ý khung đo thời gian theo đơn vị giây (seconds) và mẫu kiểm tra đúng/sai. | Bảng băm tìm quá nhanh khiến đơn vị giây toàn hiện số 0, thành viên đã tự đổi sang micro giây (microseconds) và nano giây (nanoseconds) trong Benchmark.h, đồng thời viết hoàn chỉnh 20 Test Cases để kiểm tra toàn bộ dự án.

##### Thành viên 6: Huỳnh Thị Khánh Ly

| Công cụ | Mục đích & Prompt (Câu lệnh cụ thể) | Code sinh ra từ AI | Quy trình kiểm chứng thực tế |
| :--- | :--- | :--- | :--- |
Google Gemini | "Tổng hợp nội dung từ các file ghi chú trong thư mục docs thành dàn ý báo cáo 8 mục và rà soát thuật ngữ kỹ thuật." | AI sắp xếp lại bố cục từ Mục 1 đến Mục 8 và gợi ý cách trình bày bảng đánh giá. | Tự đối chiếu từng thông số (kích thước bảng 10007, định dạng ab-cd-ghxyz, tỷ lệ va chạm < 5%) với kết quả chạy thử thực tế và chỉnh sửa câu chữ cho tự nhiên, đúng với phần việc của cả 6 thành viên.