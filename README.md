# 📚 Đồ án: Quản Lý Thư Viện (C++)

## 🌟 Introduction
Nhắc đến học IT ở PTIT mà hông nhắc tới môn này thì thật là thiếu sót. Đây là đồ án **Quản lý thư viện** môn Cấu trúc dữ liệu và giải thuật do thầy Lưu Nguyễn Kì Thư dạy lớp mình.

Đây thực sự là môn mà mình thấy khó nhất trường PTIT này. Qua được môn phải nói là siêu ít, lớp mình có 45 người thì tầm khoảng 15 người qua là hết cỡ 😐 (NÓI CHUNG LÀ TOANG). Chả thế mà nhiều thế hệ gắn ổng cái nick name "Lưu Mãi Kì Thi" cũng đúng 😂😂

Mình chia sẻ phần mã nguồn đồ án này hi vọng giúp các tân binh khóa sau có thể vượt qua môn này dễ thở nhất. Đồ án này gồm nhiều file mã nguồn, thư viện tự viết và các cấu trúc dữ liệu phức tạp đan xen được tích hợp thành 1 bài hoàn chỉnh. Theo mình thấy, cái khó ở đây nằm ở chỗ trước đó chúng ta chỉ làm các bài tập đơn giản, tuyến tính. Do vậy, khi tự dưng phải làm một đồ án lớn yêu cầu kết hợp cả Cây nhị phân tìm kiếm, Danh sách liên kết đơn và Mảng con trỏ, các bạn rất dễ bị nản và ngợp. Chính vì thế mà nhóm mình cũng phải học lại trong học kì hè và may mắn là nhóm mình được thầy phân lại đề tài cũ nên trong lần này nhóm mình đã có kinh nghiệm hơn.

## 🏗️ Architecture
Thay vì nhồi nhét tất cả, mã nguồn của đồ án này được chia thành các module (`.h` và `.cpp`) rất rõ ràng theo từng nghiệp vụ để các bạn gỡ lỗi (debug) dễ dàng hơn:

* **`cautruc.h`**: Nơi chứa "linh hồn" của đồ án. File này định nghĩa toàn bộ các biến toàn cục và cấu trúc dữ liệu cốt lõi: Mảng con trỏ cho *Đầu sách*, Danh sách liên kết đơn cho *Danh mục sách*, Cây nhị phân tìm kiếm cho *Thẻ độc giả* và Danh sách liên kết đơn cho *Mượn trả*. File này cũng kiêm luôn việc xử lý ngày giờ (tính ngày trễ hạn) và các hàm kiểm tra ràng buộc nhập liệu (không cho nhập số vào tên, tự động viết hoa...).
* **`giaodien.h`**: Nơi đảm nhận phần "màu mè" của đồ án. Thay vì dùng thư viện ngoài phức tạp, file này tự xây dựng các hàm vẽ bảng, vẽ khung trên console và bắt sự kiện phím mũi tên (lên, xuống, trái, phải, Enter, Esc). Nó giúp bạn tạo ra các menu thao tác mượt mà và các form điền thông tin rất chuyên nghiệp.
* **`docgia.h`**: Chuyên xử lý các thao tác trên Cây nhị phân Độc giả. Bao gồm việc thêm, xóa, sửa thông tin độc giả và đặc biệt là thuật toán tự động random ra một mã thẻ (số nguyên) đảm bảo không bao giờ trùng lặp với thẻ cũ.
* **`dausach.h`**: Xử lý logic nghiệp vụ cho việc tạo thêm - xóa - sửa Đầu sách (trên mảng) và quản lý các Bản sao sách (trên DSLK). Trong này cũng bao gồm cả việc tự động sinh mã sách (ISBN) và thuật toán chèn có thứ tự để in sách theo thể loại.
* **`muontra.h`**: File chứa logic hóc búa nhất và dễ sai nhất đồ án: Mượn và Trả sách. Chỗ này phải xử lý rất nhiều điều kiện ngặt nghèo của đề bài như: mỗi người chỉ mượn tối đa 3 cuốn, và tuyệt đối khóa mượn nếu độc giả đó đang giữ sách quá hạn 7 ngày.
* **`thongke.h`**: Giải quyết các yêu cầu lọc dữ liệu khó nhằn từ thầy cô. File này chứa các thuật toán để duyệt cây và mảng nhằm in ra "Top 10 sách có số lượt mượn nhiều nhất" và "Danh sách độc giả mượn quá hạn sắp xếp giảm dần theo thời gian".
* **`luutru.h`**: Phần không thể thiếu để đồ án có tính thực tiễn. Chứa các hàm đọc/ghi cấu trúc dữ liệu ra các file text (`.txt`) lưu trong thư mục `data`. Nhờ nó mà tắt app mở lại dữ liệu vẫn còn nguyên.
* **`main.cpp`**: Tệp chạy chính (chùm cuối), có nhiệm vụ kết nối mọi thứ lại với nhau. Quy trình rất gọn gàng: Mở app -> Tải dữ liệu từ file -> Chạy giao diện Menu chính -> Tự động lưu lại toàn bộ dữ liệu trước khi bạn bấm thoát.

## 🚀 Hướng Dẫn Chạy Chương Trình

Mã nguồn được cấu trúc rất gọn gàng với duy nhất một file `main.cpp` chứa hàm `main()`, các logic xử lý khác đều được nhúng trực tiếp qua các file `.h`. Điểm đặc biệt là **toàn bộ mã nguồn xử lý đều được viết dưới dạng hàm `inline` trong các file header**, giúp việc biên dịch (build) trở nên cực kỳ đơn giản.

Bạn có thể chạy đồ án này theo cách sau đây:

### Chạy bằng Visual Studio (Khuyên dùng)
Trong bộ mã nguồn đã có sẵn các file cấu hình project cho Visual Studio như `QLTV.slnx` và `QLTV.vcxproj` (hỗ trợ cả môi trường x86 và x64, sử dụng chuẩn C++20). Nếu máy bạn có sẵn Visual Studio (2019/2022...):

1. Chỉ cần nháy đúp chuột vào file **`QLTV.slnx`** để mở project lên.
2. Nhấn nút **Local Windows Debugger** màu xanh lá cây trên thanh công cụ.
3. Visual Studio sẽ tự động build và bật cửa sổ Console lên cho bạn chiến luôn, không cần cấu hình gì thêm!

---
**⚠️ Lưu ý nhỏ:** 

1. Nếu bạn chạy các bản Visual Studio mới hơn thì phần mềm sẽ yêu cầu bạn cập nhật một số cấu hình để chạy. Các bạn chỉ cần bấm đồng ý rồi chương trình sẽ chạy bình thường.
2. Chương trình sẽ tự động tạo một thư mục `data` để lưu trữ file text. Nếu bạn chạy lần đầu, chương trình báo *"Tai du lieu that bai"* thì đừng lo lắng nhé, hệ thống sẽ tự động khởi tạo cơ sở dữ liệu rỗng và tạo file lưu trữ cho những lần chạy sau. Chúc các bạn pass môn thành công! 🎉
