# Flappy Nyan

Đây là dự án Bài tập lớn môn Lập trình nâng cao (INT2215_3) tại Trường Đại học Công nghệ, Đại học Quốc gia Hà Nội.

**Sinh viên thực hiện:** Cao Đức Phát - Lớp 2425II_INT2215_3

## Mô tả Game

**Flappy Nyan** là một trò chơi được lấy cảm hứng từ tựa game Flappy Bird nổi tiếng của tác giả Dong Nguyen, kết hợp với hình ảnh và hiệu ứng của nhân vật Nyan Cat.

* **Lối chơi:** Người chơi điều khiển chú mèo cầu vồng (Nyan Cat) bay qua các chướng ngại vật là những chiếc ống xanh di chuyển liên tục từ phải sang trái. Mỗi lần vượt qua một cặp ống, người chơi được cộng 1 điểm. Mục tiêu là đạt được điểm số cao nhất có thể.
* **Logic:** Game xử lý tương tác vật lý của chú mèo, các hiệu ứng hoạt ảnh, việc lưu và đọc điểm cao từ file, hiển thị điểm số bằng hình ảnh, hiệu ứng cuộn của nền và mặt đất, cũng như quản lý các trạng thái khác nhau của game (Menu, Chơi, Game Over,...).

## Tính năng

Trò chơi bao gồm các chức năng chính sau:

### Lối chơi cốt lõi
* Điều khiển nhân vật bay lên bằng phím `Space` hoặc `Chuột trái`.
* Áp dụng hiệu ứng trọng lực lên nhân vật.
* Tạo ống ngẫu nhiên với khoảng trống cố định giữa ống trên và ống dưới.
* Ống di chuyển liên tục từ phải sang trái màn hình.
* Phát hiện va chạm giữa nhân vật với ống, mặt đất và trần.
* Tính điểm khi nhân vật vượt qua một cặp ống.
* Chuyển sang trạng thái "Game Over" khi có va chạm.

### Hình ảnh và Hoạt ảnh
* Nhân vật mèo có hoạt ảnh bay (sử dụng nhiều frame ảnh).
* Nhân vật xoay góc theo vận tốc bay lên/rơi xuống.
* Hiệu ứng nhân vật lơ lửng tại màn hình Menu và Info.
* Nền và mặt đất di chuyển cuộn với tốc độ khác nhau tạo hiệu ứng chiều sâu (parallax scrolling).
* Hiển thị điểm số bằng hình ảnh các chữ số (từ `0.png` đến `9.png`).
* Màn hình Menu chính với thông điệp/nút bấm có hoạt ảnh.
* Màn hình Game Over hiển thị điểm đạt được, điểm cao nhất và nút chơi lại.
* Màn hình Pause (Tạm dừng) với các tùy chọn Tiếp tục (Resume), Thoát (Quit), bật/tắt âm thanh/nhạc nền.
* Màn hình Info (Thông tin/Hướng dẫn) với nút quay lại (Back), bật/tắt âm thanh/nhạc nền.

### Âm thanh
* Nhạc nền vui nhộn (có thể bật/tắt).
* Hiệu ứng âm thanh cho các hành động: nhảy, va chạm, qua ống (ghi điểm), thua cuộc (có thể bật/tắt riêng biệt với nhạc nền).

### Trạng thái game và Giao diện người dùng
* Quản lý các trạng thái game: `Menu`, `Starting` (Chuẩn bị bắt đầu), `Playing` (Đang chơi), `Game Over`, `Pause`, `Info`, `Resuming` (Tiếp tục sau khi Pause).
* Xử lý đầu vào từ bàn phím và chuột.
* Các nút bấm và icon có tính tương tác trên các màn hình.
* Có độ trễ ngắn khi bắt đầu màn chơi mới hoặc tiếp tục sau khi tạm dừng.

### Lưu trữ
* Lưu điểm số cao nhất vào file `highScore.txt`.
* Tự động đọc điểm cao nhất từ file khi khởi động game và cập nhật nếu người chơi phá kỷ lục.

### Công cụ
* **Ngôn ngữ:** C++
* **Thư viện đồ họa:** SDL2 (Simple DirectMedia Layer 2)
## Tài nguyên và Tham khảo

* **Đồ họa & Âm thanh cơ bản:** Lấy từ [samuelcust/flappy-bird-assets](https://github.com/samuelcust/flappy-bird-assets) trên GitHub.
* **Model nhân vật Nyan Cat:** Tham khảo từ [Tenor GIF Search](https://tenor.com/search/nyan-cat-gifs).
* **Nhạc nền:** Tham khảo từ [Nyan Cat! Official](https://www.youtube.com/watch?v=2yJgwwDcgV8).
* Một số icon và hình ảnh khác được tìm kiếm và chỉnh sửa bằng Adobe Photoshop.

## Demo

Bạn có thể xem demo gameplay tại đây: [Link](https://youtu.be/O32fEhu6FwU)
