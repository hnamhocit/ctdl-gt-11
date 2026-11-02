# 05. Chính Sách Sử Dụng AI & Mẫu Báo Cáo Mục 6 (AI Guidelines & Section 6)

> **Căn cứ**: Mục 3 & Mục 5.4 trong Đề cương Bài tập Dự án học phần CTDL&GT.

---

## 1. Chính Sách Sử Dụng AI Của Học Phần

Học phần **không cấm** sinh viên sử dụng các công cụ Trí tuệ Nhân tạo (Generative AI), mà ngược lại khuyến khích khai thác AI như một trợ lý lập trình thông minh để nâng cao năng suất và chất lượng sản phẩm. Tuy nhiên, nhóm phải tuân thủ nghiêm ngặt 3 nguyên tắc sau:

1. **Phạm vi được phép hỗ trợ**:
   - Phân tích yêu cầu và thiết kế kiến trúc tổng thể.
   - Gợi ý công thức hàm băm phân bố đều (DJB2, Polynomial Hash).
   - Đề xuất khung mã nguồn (boilerplate / skeleton) cho Struct và Class.
   - Rà soát lỗi cú pháp, rò rỉ bộ nhớ (Valgrind / AddressSanitizer) và debug.
   - Tự động sinh tập dữ liệu ngẫu nhiên (1.000 biển số xe hợp lệ).
   - Đề xuất các ca kiểm thử biên (edge test cases) và hỗ trợ hành văn kỹ thuật trong báo cáo.

2. **Nguyên tắc Chịu Trách Nhiệm 100%**:
   - Sinh viên phải đọc, hiểu và làm chủ từng dòng code được sinh ra.
   - Trong buổi vấn đáp, nếu sinh viên không giải thích được lý do chọn thuật toán, ý nghĩa các con trỏ hoặc không thể sửa code theo yêu cầu của giảng viên, sinh viên sẽ bị chấm ở **Mức 1 (0 - 3.9 điểm)**.

3. **Mục bắt buộc trong Báo cáo Dự án (Mục 6)**:
   - Báo cáo chính thức phải có riêng **Mục 6: "Khai thác và sử dụng AI"**.
   - Báo cáo phải công khai minh bạch: Công cụ đã dùng, Prompt đã gửi, nội dung AI sinh ra, và quy trình nhóm đã kiểm tra xác minh.

---

## 2. Khung Nội Dung Mẫu Cho Mục 6 Trong Báo Cáo Dự Án

Thành viên phụ trách báo cáo (**Huỳnh Thị Khánh Ly**) và các thành viên khác sử dụng mẫu dưới đây để đưa vào báo cáo chính thức:

```markdown
# MỤC 6: KHAI THÁC VÀ SỬ DỤNG AI TRONG DỰ ÁN

## 6.1. Danh mục các công cụ AI được sử dụng
Nhóm 11 đã sử dụng phối hợp các công cụ hỗ trợ phát triển:
- Google Gemini / Antigravity CLI: Phân tích kiến trúc hệ thống, sinh tài liệu AI-first và thiết lập quy tắc dự án.
- Cursor / Claude: Hỗ trợ rà soát con trỏ thuần và tối ưu hóa giải phóng bộ nhớ trong hàm clear().
- ChatGPT: Hỗ trợ sinh dữ liệu mẫu 1.000 bản ghi biển số xe theo chuẩn ab-cd-ghxyz.

## 6.2. Nhật ký Prompt và nội dung sinh ra

### Ca 1: Gợi ý giải thuật băm chuỗi biển số xe
- Prompt: "Tôi đang làm đồ án CTDL&GT quản lý biển số xe dạng ab-cd-ghxyz bằng ngôn ngữ C++ với cấu trúc Bảng băm con trỏ thuần kích thước 10007. Hãy gợi ý hàm băm chuỗi tối ưu để hạn chế va chạm."
- Nội dung AI phản hồi: AI đề xuất giải thuật băm DJB2 với trọng số nhân 33 và phép dịch bit.
- Quy trình kiểm chứng của nhóm:
  + Thành viên Nguyễn Hoàng Nam đã kiểm tra công thức toán học và đo đạc tỷ lệ va chạm trên tập dữ liệu 1.000 mẫu (tỷ lệ va chạm < 5%).
  + Sinh viên tự viết lại bằng C++ thuần, không dùng thư viện ngoài.

### Ca 2: Sinh bộ kiểm thử biên (Edge Test Cases)
- Prompt: "Hãy liệt kê 20 trường hợp kiểm thử biên cho việc kiểm tra chuỗi biển số xe ab-cd-ghxyz."
- Nội dung AI phản hồi: Cung cấp danh sách các chuỗi sai dấu gạch nối, sai độ dài, chứa ký tự đặc biệt, và chuỗi rỗng.
- Quy trình kiểm chứng của nhóm:
  + Thành viên Huỳnh Quốc Việt đưa danh sách vào module Validator để chạy tự động và kiểm tra tính chính xác của regex/hàm kiểm tra thủ công.

## 6.3. Đánh giá tính hữu ích và giới hạn của AI
- Ưu điểm: Tiết kiệm 40% thời gian viết tài liệu và sinh bộ dữ liệu thử nghiệm; hỗ trợ tìm lỗi tràn buffer bộ nhớ nhanh chóng.
- Giới hạn: AI thường có xu hướng dùng các thư viện tiện lợi như std::unordered_map hoặc std::string nếu không được chỉ thị chặt chẽ; nhóm đã chủ động loại bỏ toàn bộ để tự tay hiện thực con trỏ thuần đúng theo quy định học phần.
```

---

## 3. Hướng Dẫn Sinh Viên Chuẩn Bị Vấn Đáp Về Phần AI

Khi giảng viên hỏi: *"Đoạn code này có phải do em tự viết không hay AI viết?"*, sinh viên cần tự tin trả lời theo nguyên tắc:
1. Xác nhận trung thực: *"Dạ thưa Thầy/Cô, nhóm em có sử dụng AI (Gemini/Claude) để gợi ý cấu trúc khung và tham khảo giải thuật băm DJB2, điều này nhóm đã trình bày rất chi tiết và minh bạch tại Mục 6 của Báo cáo."*
2. Khẳng định năng lực làm chủ code: *"Sau khi nhận gợi ý từ AI, em đã tự mình viết lại toàn bộ bằng con trỏ thuần, kiểm tra từng bước giải phóng bộ nhớ để tránh memory leak và chạy thử nghiệm 20 test case. Em xin phép được giải thích cặn kẽ từng dòng lệnh và sẵn sàng sửa code trực tiếp theo yêu cầu của Thầy/Cô ạ."*
