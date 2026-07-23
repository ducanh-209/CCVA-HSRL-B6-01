# Xe tự hành-bảng B6-WRO 2026
Dự án xe tự hànhthực hiện nhiệm vụ di chuyển theo luật thi đấu bảng B6-WRO2026 với 2 vòng thi đấu Open Challange và Obstacle Challenge.
# Giới thiệu
Tên nhóm CCVA-HSLR-B6-01

Nhóm gồm 3 thành viên: Vũ Nam Anh, Vũ Đức Anh, Nguyễn Lĩnh Hoàng Sơn

Đơn vị đại diện: Hung Steam Robotics Lab-THPT Chu Văn An

## Cấu trúc thư mục

Repository được tổ chức theo các thư mục chức năng nhằm thuận tiện cho việc cài đặt, phát triển và tham khảo tài liệu của dự án.

```text
CCVA-HSRL-B6-01
│
├── Libraries/     # Thư viện sử dụng cho Arduino IDE    
├── Sources/       # Mã nguồn chương trình điều khiển robot
        ├── Osbtacle_Challenge/ # Mã vòng thử thách vượt chướng ngại vật
        └── Open_Challenge/  # Mã vòng thử thách mở
├── Instruction/           # Hướng dẫn cài đặt và sử dụng
├── OpenMV/                # Chương trình xử lý ảnh cho OpenMV
├── Pictures/              # Hình ảnh robot và các thành phần
└── README.md              # Tài liệu giới thiệu dự án
```

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

## 📋 Quy Trình Hoạt Động (Flow)ử nghiệm và hoạt động của hệ thống.
