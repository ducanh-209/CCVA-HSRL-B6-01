import sensor, image, time  # Import image sensor and time modules
from matrix_mini import send_data  # Import the send_data function from the matrix_mini module

# Initialize the sensor
sensor.reset()  # Reset the sensor
sensor.set_pixformat(sensor.RGB565)  # Set the pixel format to RGB565
sensor.set_framesize(sensor.QVGA)  # Set the image resolution to QVGA (320x240)
sensor.skip_frames(time=2000)  # Wait for 2 seconds to stabilize the sensor

# Set image flipping
sensor.set_vflip(True)  # Vertically flip the image
sensor.set_hmirror(True)  # Horizontally mirror the image

# Define color threshold range (HSV), suitable for detecting the target color
# Hue, Saturation, and Value range
threshold = [(33, 48, -128, -16, 58, 9),  (87, 30, 127, 29, 127, -61)]  #green  #red
clock = time.clock()  # Create a clock to calculate frames per second (FPS) of image processing
hotspot1 = (-1, 0, 0, 0)  # Thong tin cua bien bao so 1
hotspot2 = (-1, 0, 0, 0)  # Thong tin cua bien bao so 2

def tracking(index):
    global hotspot1, hotspot2
    blobs = img.find_blobs([threshold[index]], pixels_threshold=110, area_threshold=110)

    # if index == 2:
    #     index = 0

    # if index == 3:
    #     index = 1

    if blobs:  # If blobs are found

        # Find the largest blob by area
        max_blob = max(blobs, key=lambda b: b[2] * b[3])

        img.draw_rectangle(max_blob.rect())  # Draw a rectangle around the largest blob

        # Calculate the center coordinates of the blob
        x_center = max_blob.cx()
        y_center = max_blob.cy()

        # Calculate the blob's area and round it to an integer
        blob_area = round(max_blob.area() / 2)

        # Mark the center point and coordinates text on the image
        img.draw_cross(x_center, y_center)  # Draw a cross at the center
        img.draw_string(0, 0, str(x_center) + ", " + str(y_center))  # Display the coordinates

        if hotspot1 == (-1, 0, 0, 0):
            hotspot1 = (index, x_center, y_center, blob_area)
        elif hotspot2 == (-1, 0, 0, 0) and hotspot1[0] != index:
            hotspot2 = (index, x_center, y_center, blob_area)
        # print(hotspot1, hotspot2)
        # send_data([hotspot1[0], hotspot1[1], hotspot1[2], hotspot1[3], hotspot2[0], hotspot2[1], hotspot2[2], hotspot2[3]])


while True:
    hotspot1 = (-1, 0, 0, 0)  # Thong tin cua bien bao so 1
    hotspot2 = (-1, 0, 0, 0)  # Thong tin cua bien bao so 2
    clock.tick()  # Start timing
    img = sensor.snapshot()  # Capture an image

    # print(clock.fps())  # Output the current FPS

    # Find blobs (color regions) in the image that match the threshold1
    for i in (0, 1):
        tracking(i)
    TH = 0
    if hotspot1[0] == 0 and hotspot2[0] == -1:  # th xanh
        TH = 1
    elif hotspot1[0] == 1 and hotspot2[0] == -1:  # th do
        TH = 2
    elif hotspot2[0] != -1 and hotspot1[2] <= hotspot2[2]:  # do gan
        if hotspot2[1] > 160 and hotspot1[1] > 160:
            TH = 3
        elif hotspot2[1] > 160 and hotspot1[1] < 160:
            TH = 4
        elif hotspot2[1] < 160 and hotspot1[1] > 160:
            TH = 5
        elif hotspot2[1] < 160 and hotspot1[1] < 160:
            TH = 6
    elif hotspot2[0] != -1 and hotspot1[2] > hotspot2[2]:  # xanh gan
        if hotspot2[1] > 160 and hotspot1[1] > 160:
            TH = 7
        elif hotspot1[1] > 160 and hotspot2[1] < 160:
            TH = 8
        elif hotspot1[1] < 160 and hotspot2[1] > 160:
            TH = 9
        elif hotspot1[1] < 160 and hotspot2[1] < 160:
            TH = 10
    if TH != 0:
        print (TH)
