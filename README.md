# Ứng dụng Chuyển đổi Thời lượng Video
## 📋 Mục lục
* [Giới thiệu](#-giới-thiệu)
* [Tính năng](#-tính-năng)
* [Yêu cầu hệ thống](#-yêu-cầu-hệ-thống)
* [Cài đặt và Chạy](#-cài-đặt-và-chạy)
    * [Cách 1: Sử dụng phiên bản đã đóng gói (Dành cho người dùng cuối Windows)](#cách-1-sử-dụng-phiên-bản-đã-đóng-gói-dành-cho-người-dùng-cuối-windows)
    * [Cách 2: Biên dịch từ mã nguồn (Dành cho nhà phát triển)](#cách-2-biên-dịch-từ-mã-nguồn-dành-cho-nhà-phát-triển)
* [Cách sử dụng](#-cách-sử-dụng)
* [Cấu trúc dự án](#-cấu-trúc-dự-án)
* [Khắc phục sự cố thường gặp](#-khắc-phục-sự-cố-thường-gặp)
* [Đóng góp](#-đóng-góp)
* [Giấy phép](#-giấy-phép)
* [Liên hệ](#-liên-hệ)
---
##  Giới thiệu
Đây là một ứng dụng desktop đơn giản, trực quan được phát triển bằng ngôn ngữ C kết hợp với thư viện đồ họa GTK+ 3. Ứng dụng này cung cấp một công cụ tiện lợi để tính toán thời lượng cuối cùng của một video sau khi được tua nhanh ở các hệ số khác nhau (x1.2, x1.5, x1.8, và x2.0).
Mục tiêu của dự án là giúp người dùng dễ dàng ước tính thời gian cần thiết để xem xong các video đã được tăng tốc, đặc biệt hữu ích cho các buổi học, bài giảng hoặc podcast.
##  Tính năng
* **Tính toán nhanh chóng**: Nhập thời lượng video gốc (giờ, phút, giây) và nhận kết quả ngay lập tức.
* **Hỗ trợ đa tốc độ**: Tính toán và hiển thị thời lượng tua nhanh cho các hệ số $x1.2$, $x1.5$, $x1.8$, và $x2.0$.
* **Giao diện người dùng đồ họa (GUI)**:
    * Thiết kế thân thiện, dễ sử dụng.
    * Kết quả được hiển thị trong các **tab riêng biệt** cho từng tốc độ tua nhanh, giúp dễ dàng so sánh và theo dõi.
* **Ứng dụng độc lập (Portable) cho Windows**: Phiên bản đã đóng gói cho phép người dùng cuối chạy ứng dụng trực tiếp mà không cần cài đặt thêm thư viện hay trình biên dịch.
* **Không có cửa sổ console thừa thãi**: Phiên bản Windows đã biên dịch sẽ chạy ứng dụng GUI mà không có cửa sổ dòng lệnh đen bật kèm.
##  Yêu cầu hệ thống
### Đối với người dùng cuối (phiên bản đã đóng gói)
* Hệ điều hành: Windows (phiên bản 64-bit hoặc 32-bit, tùy thuộc vào bản dựng bạn cung cấp).
* Không yêu cầu cài đặt thêm phần mềm nào khác.
### Đối với nhà phát triển (biên dịch từ mã nguồn)
* Hệ điều hành: Windows, macOS, hoặc Linux.
* Trình biên dịch C: GCC (phiên bản 8 trở lên được khuyến nghị) hoặc Clang.
* Thư viện phát triển GTK+ 3 (phiên bản 3.24 trở lên được khuyến nghị).
* Công cụ `pkg-config`.
* MSYS2 MinGW 64-bit (đối với Windows) hoặc Xcode Command Line Tools (đối với macOS) hoặc `build-essential` (đối với Linux).
## Cài đặt và Chạy
### Cách 1: Sử dụng phiên bản đã đóng gói (Dành cho người dùng cuối Windows)
Đây là cách dễ nhất để chạy ứng dụng mà không cần cài đặt bất kỳ công cụ phát triển nào.
1.  **Tải xuống**:
    * Truy cập trang [https://github.com/thd180306/VideoConverterC-GTK].
    * Tải xuống tệp `.zip` mới nhất có tên dạng `video_converter_gtk_windows_vX.Y.Z.zip` (hoặc tương tự).
2.  **Giải nén**:
    * Giải nén toàn bộ nội dung của tệp `.zip` vào một thư mục bất kỳ trên máy tính của bạn (ví dụ: `C:\Apps\VideoConverter`).
3.  **Chạy ứng dụng**:
    * Mở thư mục đã giải nén và nhấp đúp vào tệp `video_converter_gtk.exe`.
##  Cách sử dụng
1.  Mở ứng dụng `video_converter_gtk.exe`.
2.  Trong phần "Nhập thời lượng video gốc:", nhập số giờ, phút và giây tương ứng của video của bạn.
3.  Nhấp vào nút **"Tính toán Thời lượng Tua nhanh"**.
4.  Ứng dụng sẽ tự động chuyển sang tab "Thời lượng Gốc" và tạo ra các tab mới, mỗi tab hiển thị thời lượng video sau khi được tua nhanh ở tốc độ cụ thể ($x1.2$, $x1.5$, $x1.8$, và $x2.0$).
##  Cấu trúc dự án
 video_converter_gtk.c  # Mã nguồn chính của ứng dụng
 README.md              # Tệp tài liệu này
.gitignore             # Các quy tắc bỏ qua tệp của Git (ví dụ: tệp thực thi, đối tượng biên dịch)
 LICENSE                # (Nếu có) Thông tin giấy phép của dự án
## Khắc phục sự cố thường gặp
* **Lỗi `gcc: command not found` hoặc `pkg-config: command not found`:**
    * Đảm bảo bạn đang sử dụng **"MSYS2 MinGW 64-bit Terminal"** trên Windows.
    * Xác minh rằng bạn đã cài đặt đầy đủ các gói `mingw-w64-x86_64-toolchain` và `mingw-w64-x86_64-gtk3` bằng `pacman -S`.
    * Kiểm tra lại chính tả của lệnh `pkg-config` (không có 's' ở cuối).
* **Lỗi `fatal error: gtk/gtk.h: No such file or directory`:**
    * Bạn đã bỏ lỡ hoặc gõ sai phần `pkg-config --cflags --libs gtk+-3.0` trong lệnh biên dịch. Đảm bảo sử dụng lệnh đầy đủ và dấu huyền (backtick) đúng cách.
* **Cửa sổ console đen bật lên cùng ứng dụng GUI:**
    * Đảm bảo bạn đã thêm cờ `-mwindows` vào cuối lệnh biên dịch `gcc`.
## Đóng góp
Mọi đóng góp cho dự án đều được chào đón! Nếu bạn có ý tưởng cải tiến, tìm thấy lỗi, hoặc muốn bổ sung tính năng, vui lòng:
1.  Fork (tạo bản sao) kho lưu trữ này.
2.  Tạo một nhánh mới cho tính năng/sửa lỗi của bạn (`git checkout -b feature/tinh-nang-moi`).
3.  Thực hiện các thay đổi và cam kết chúng (`git commit -m 'Thêm tính năng X'`).
4.  Đẩy nhánh của bạn lên bản sao của bạn (`git push origin feature/tinh-nang-moi`).
5.  Mở một Pull Request (Yêu cầu kéo) từ kho lưu trữ của bạn sang kho lưu trữ chính.
## 📄 Giấy phép
Dự án này được phân phối dưới Giấy phép MIT. Xem tệp `LICENSE`để biết thêm chi tiết.
## Liên hệ
[Trịnh Hoài Đức] - [DucTH.B24AT065@stu.ptit.edu.vn]
Liên kết dự án: [https://github.com/thd180306/VideoConverterC-GTK]
