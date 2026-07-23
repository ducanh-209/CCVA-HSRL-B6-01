# 🏎️ Xe Tự Hành - Bảng B6 (WRO 2026)

> Dự án xe tự hành thực hiện nhiệm vụ di chuyển theo luật thi đấu **Bảng B6 - WRO 2026** với 2 vòng thi đấu: **Open Challenge** và **Obstacle Challenge**.

---

## 👥 Giới Thiệu Nhóm

* **Tên nhóm:** `CCVA-HSRL-B6-01`
* **Đơn vị đại diện:** Hung Steam Robotics Lab - THPT Chu Văn An
* **Thành viên nhóm:**
  1. 👨‍💻 **Vũ Nam Anh**
  2. 👨‍💻 **Vũ Đức Anh**
  3. 👨‍💻 **Nguyễn Lĩnh Hoàng Sơn**

---

## 📁 Cấu Trúc Thư Mục

Repository được tổ chức theo các thư mục chức năng nhằm thuận tiện cho việc cài đặt, phát triển và tham khảo tài liệu của dự án.

```text
CCVA-HSRL-B6-01
│
├── 📦 Libraries/          # Thư viện tùy chỉnh cho Arduino IDE    
├── 📜 Sources/            # Mã nguồn chương trình điều khiển robot
│   ├── 🏁 Open_Challenge/     # Mã nguồn vòng thử thách mở
│   └── 🚧 Obstacle_Challenge/ # Mã nguồn vòng thử thách vượt chướng ngại vật
├── 📑 Instruction/        # Hướng dẫn cài đặt và sử dụng
├── 📷 OpenMV/             # Chương trình xử lý ảnh cho OpenMV
├── 🖼️ Pictures/           # Hình ảnh robot (đủ 6 góc nhìn)
└── 📘 README.md           # Tài liệu giới thiệu dự án
'''
### Mô tả các thư mục

#### Libraries/`

Chứa file sủa đổi thư viện cần thiết để biên dịch và chạy chương trình trên **Arduino IDE**. Người dùng nên cài đặt các thư viện này trước khi nạp chương trình cho robot.

#### `Sources/`

Chứa mã nguồn chính của robot, bao gồm các thuật toán điều khiển, xử lý cảm biến và các hàm phục vụ quá trình thi đấu của 2 vòng thử thách.

#### `Instruction/`

Chứa các tài liệu hướng dẫn cài đặt môi trường phát triển, lắp ráp robot và các hướng dẫn cần thiết để sử dụng dự án.

#### `OpenMV/`

Chứa chương trình chạy trên camera **OpenMV**, phục vụ việc xử lý ảnh và nhận diện đối tượng trong quá trình robot hoạt động.

#### `Pictures/`

Lưu trữ hình ảnh của robot trong 6 hướng.

# Cài đặt môi trường phát triển

Để lập trình và vận hành robot, cần cài đặt các phần mềm và thư viện theo các bước sau.

## 1. Cài đặt Arduino IDE

Truy cập trang web: https://www.matrixrobotics.com/adv-program-resources và làm theo hướng dẫn để cài đặt và set up cho Adurino

---

## 2. Cài đặt OpenMV IDE

Hướng dẫn cài đặt:

> https://wro-learn.org/en_us/wiki/m-vision-camera

---
Sử dụng: https://github.com/ducanh-209/CCVA-HSRL-B6-01/tree/main/OpenMV để cài đặt chương trình cho cảm biến
## 3. Cài đặt thư viện giao tiếp với camera

Để MATRIX Mini R4 có thể giao tiếp với camera OpenMV giống như miêu tả trong báo cáo kỹ thuật:

https://github.com/ducanh-209/CCVA-HSRL-B6-01/tree/main/Libraries

Sau khi tải về, sao chép tệp **MiniR4SmartCamReader.h** vào thư mục sensor trong thư viện MatrixMiniR4 của Adurino và thay thế.

---

## Hoàn tất

Sau khi hoàn thành các bước trên, môi trường phát triển đã được thiết lập đầy đủ và sẵn sàng để sử dụng các mã nguồn chính của Repository này.

## Bộ dụng cụ và hướng dẫn lắp ráp

Hướng dẫn lắp ráp chi tiết được cung cấp tại liên kết dưới đây: 

## Hệ thống cảm biến
- Hai laser sensor V2
- Color senser V3
- M-vision camera
- Động cơ DC
- Servo
## Mã nguồn chính
# 🤖 Obstacle Challenge

Chương trình được viết cho board điều khiển **Matrix Mini R4** (Arduino framework) phục vụ cho robot dò đường, quét và né tránh khối màu (xanh/đỏ) dựa trên Vision Sensor và Laser Distance Sensor.

---

## 🛠 Thư Viện & Phần Cứng Yêu Cầu

### 1. Phần cứng
* **Bo điều khiển:** Matrix Mini R4.
* **Động cơ:** Động cơ chính cắm tại cổng `M1`.
* **Servo:** Servo bẻ lái cắm tại cổng `RC1`.
* **Cảm biến Laser (MXLaserV2):** 
  * `I2C1`: Cảm biến bên Trái.
  * `I2C2`: Cảm biến bên Phải.
* **Cảm biến Màu sắc (MXColorV3):** Cắm tại cổng `I2C3` (đọc đường vạch góc sa bàn).
* **Cảm biến Nhận diện Hình ảnh (Matrix Vision):** Nhận diện màu sắc và tọa độ khối (`cube_color`, `x`, `y`).

### 2. Thư viện
* `MatrixMiniR4.h`
* `<algorithm>`

---

## ⚙️ Các Hằng Số Cấu Hình Chính

| Tên Hằng Số | Giá Trị | Ý Nghĩa |
| :--- | :--- | :--- |
| `Y_IGNORE` | `40` | Tọa độ Y tối thiểu. Khối nằm xa hơn mức này sẽ bị bỏ qua (`cube_color = 255`). |
| `Y_SENSE` | `132` | Khoảng cách Y bắt đầu kích hoạt chế độ tiếp cận khối. |
| `GREEN` | `200` | Tọa độ X mục tiêu để điều hướng né khối màu Xanh. |
| `RED` | `100` | Tọa độ X mục tiêu để điều hướng né khối màu Đỏ. |
| `EXIT` | `12` | Số vạch góc tối đa cần vượt qua trước khi dừng chương trình. |
| `WALL_THRESHOLD` | `180.0` | Ngưỡng khoảng cách nhận biết tường bên hông (mm). |

---
---

## 🚀 Hướng Dẫn Sử Dụng & Thao Tác

1. **Nạp Code:** Mở file trong Arduino IDE (đã cài đặt thư viện `MatrixMiniR4`) và tiến hành nạp vào board.
2. **Khởi động:** Sau khi bật nguồn, robot ở trạng thái chờ lệnh bấm nút.
3. **Thao tác Nút bấm:**
   * **Nút DOWN (`BTN_DOWN`):** Chế độ Test/Debug Vision Sensor (vòng lặp đọc dữ liệu camera liên tục).
   * **Nút UP (`BTN_UP`):** Bắt đầu cho robot chạy chương trình chính.

---

## 📌 Giải Thích Các Hàm Chính Trong Code

* `servoMotor(float value, float l = 43)`: Điều chỉnh góc Servo bẻ lái, có giới hạn góc trong khoảng `[-l, l]`.
* `bam_line_trai()` / `bam_line_phai()`: Sử dụng thuật toán PD (Proportional-Derivative) để duy trì khoảng cách bám tường theo Laser.
* `turn()`: Tự động phát hiện khi chạm vạch góc sa bàn, thực hiện hành vi xoay đầu quét camera tìm khối màu tiếp theo.
* `last_step()`: Chuỗi hành động trả lái và đi thẳng/nhập làn an toàn sau khi đã né xong một khối.
* `line_counter()`: Đếm số vạch góc sa bàn robot đã đi qua để kiểm soát điều kiện dừng (`EXIT`).

# 🤖 Open Challenge

Mã nguồn triển khai bộ điều khiển **PID / PD** giúp robot bám tường (trái/phải), di chuyển giữa 2 tường và tự động nhận diện vạch màu (cam/xanh) để thực hiện chuỗi nhiệm vụ di chuyển tuần hoàn.

---

## 🛠 Phần Cứng & Kết Nối Cổng

* **Bo điều khiển:** Matrix Mini R4
* **Động cơ:**
  * `M1`: Động cơ di chuyển chính (có Encoder).
  * `M2`: Động cơ phụ (dùng trong hàm di chuyển giữa 2 tường).
* **Servo Lái:** `RC1` (Điều chỉnh góc lái từ $47^\circ$ đến $133^\circ$, mặc định $90^\circ$ đi thẳng).
* **Cảm biến Laser (MXLaserV2):**
  * `I2C1`: Đo khoảng cách tường Trái.
  * `I2C2`: Đo khoảng cách tường Phải.
* **Cảm biến Màu sắc (MXColorV3):** `I2C3` (Đọc giá trị R, B để phát hiện đường vạch Cam / Xanh).

---

## 📐 Nguyên Lý Tính Toán Chồng Góc Laser

Đoạn code sử dụng hệ số chia `0.707` ($\approx \sin(45^\circ)$ hoặc $\cos(45^\circ)$) để quy đổi khoảng cách cảm biến laser đặt nghiêng $45^\circ$ so với tường về khoảng cách vuông góc thực tế:

$$d_{\text{thực tế}} = \frac{mm}{0.707}$$

---

## 📚 Danh Sách Các Hàm Chính

### 1. Hàm Bám Tường Theo Quãng Đường (PD Control)
Điều khiển robot bám tường và tự dừng khi đi đủ quãng đường `cm` / `number`.

* `tuongtraiquangduong_n_tocdo_n_Kp_n_Kd_n_kc_n(...)`: Bám tường **Trái**.
* `tuongphaiquangduong_n_tocdo_n_Kp_n_Kd_n_kc_n(...)`: Bám tường **Phải**.

### 2. Hàm Bám Tường Đến Khi Gặp Vạch Màu (PID Control)
Bám tường cho đến khi cảm biến màu sắc nhận diện được góc sa bàn (Cam hoặc Xanh).

* `dibamtuongtraivoitocdo_n_khoangcach_n_n_n_n(...)`: Bám tường **Trái**.
* `dibamtuongphaivoitocdo_n_khoangcach_n_n_n_n(...)`: Bám tường **Phải**.
* `dithangmaucam_n_Kp_n_Ki_n_Kd_n_khoangcach_n(...)`: Bám tường Trái cho tới khi gặp **màu Cam**.
* `dithangmauxanh_n_Kp_n_Ki_n_Kd_n_khoangcach_n(...)`: Bám tường Phải cho tới khi gặp **màu Xanh**.

### 3. Hàm Rẽ Cố Định (Chạy Mù - Dead Reckoning)
* `remu_n_quangduong_n_goc_n(speed, cm, goc)`: Khóa cố định góc lái `goc` và cho robot chạy đúng quãng đường `cm`.

### 4. Hàm Bám Giữa 2 Tường
* `digiua2tuongvoitocdo_n_Kp_n_Ki_n_Kd_n(...)`: So sánh khoảng cách 2 bên tường (`laser_trai - laser_phai`) và điều chỉnh công suất riêng biệt cho 2 động cơ `M1`, `M2`.

---

## 🎯 Hàm Nhiệm Vụ Tổng (NV1)

Hàm `NV1voitocbandau_n_tocdithang_n_tocdore_n_Kp_n_Ki_n_Kd_n_Khoangcachtuong_n(...)` tự động thực hiện chuỗi hành vi:

1. **Tự xác định hướng xuất phát:** So sánh khoảng cách 2 bên laser để chọn bám tường Trái hay Phải.
2. **Nhận diện vạch màu đầu tiên:**
   * **Nếu gặp màu Cam:** Rẽ ngoặt góc $130^\circ$ và lặp lại 11 lần chuỗi *(Bám tường gặp màu cam $\rightarrow$ Rẽ)*.
   * **Nếu gặp màu Xanh:** Rẽ ngoặt góc $50^\circ$ và lặp lại 11 lần chuỗi *(Bám tường gặp màu xanh $\rightarrow$ Rẽ)*.
3. **Thoát chuỗi:** Chạy thêm một đoạn cố định $60\text{ cm}$ và phanh dừng động cơ (`setBrake(true)`).

---

## 🚀 Hướng Dẫn Sử Dụng

1. Bật nguồn robot Matrix Mini R4.
2. Nhấn nút **UP** (`BTN_UP`) trên bo điều khiển để bắt đầu thực hiện Nhiệm Vụ 1 (NV1).
