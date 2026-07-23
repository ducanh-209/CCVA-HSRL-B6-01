import sensor, image, time
from matrix_mini import send_data, uart, green_led
import fill_light

# Khởi tạo cảm biến
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)

sensor.set_vflip(True)
sensor.set_hmirror(True)

fill_light.on()

threshold_cube = [(26, 18, -128, -15, 127, -128),
(26, 0, 39, 127, -128, 38),
(33, 0, 33, 127, -128, 127),
(30, 8, -128, -14, 127, -128)]
clock = time.clock()
cube_data = (-1, 0, 0, 0)


def sense(cube_color):
    global cube_data
    if cube_color == 3:
        blobs = img.find_blobs([threshold_cube[cube_color]], pixels_threshold=300, area_threshold=200)
    elif cube_color == 2:
        blobs = img.find_blobs([threshold_cube[cube_color]], pixels_threshold=600, area_threshold=200)
    else:
        blobs = img.find_blobs([threshold_cube[cube_color]], pixels_threshold=400, area_threshold=200)
    if blobs:
        max_blob = max(blobs, key=lambda b: b.cy())
        img.draw_rectangle(max_blob.rect())
        x_center = max_blob.cx()
        y_center = max_blob.cy()
        blob_area = round(max_blob.area() / 2)
        img.draw_cross(x_center, y_center)
        img.draw_string(max_blob.x(), max_blob.y() - 15, f"C:{cube_color}, X:{x_center}, Y:{y_center}", color=(255, 255, 255))
        if cube_color == 2:
            cube_color = 1
        if cube_color == 3:
            cube_color = 0
        if cube_data[2] <= y_center:
            cube_data = (cube_color, x_center, y_center, blob_area)

# while True:
#     fill_light.brightness(100)
#     # Kiểm tra xem robot có gửi lệnh yêu cầu lấy dữ liệu không
#             # CHỤP ẢNH NGAY TẠI THỜI ĐIỂM ROBOT YÊU CẦU
#     img = sensor.snapshot()

#     nearest_data = (-1, 0, 0, 0)
#     finding(0)
#     finding(1)
#     finding(2)
#     finding(3)
#     # Gửi dữ liệu mới nhất về ngay lập tức
#     if nearest_data != (-1, 0, 0, 0):
#         send_data([nearest_data[0], nearest_data[1], nearest_data[2], nearest_data[3]])
#     else:
#         # Nếu không thấy vật thể, gửi gói dữ liệu trống để robot không bị đợi timeout
#         send_data([255, 0, 0, 0])


while True:
    # Kiểm tra xem robot có gửi lệnh yêu cầu lấy dữ liệu không
    if uart.any():
        cmd = uart.read(1)  # Đọc 1 byte lệnh từ robot
        if cmd == b'G':  # Nếu robot gửi đúng ký tự 'G'
            clock.tick()

            # CHỤP ẢNH NGAY TẠI THỜI ĐIỂM ROBOT YÊU CẦU
            img = sensor.snapshot()

            cube_data = (-1, 0, 0, 0)
            sense(0)
            sense(1)
            sense(2)
            sense(3)
            print(cube_data)
            # Gửi dữ liệu mới nhất về ngay lập tức
            if cube_data != (-1, 0, 0, 0):
                send_data([cube_data[0], cube_data[1], cube_data[2], cube_data[3]])
            else:
                # Nếu không thấy vật thể, gửi gói dữ liệu trống để robot không bị đợi timeout
                send_data([255, 0, 0, 0])
        if cmd == b'L':
            green_led.on()
