#include "MatrixMiniR4.h"
#include <algorithm> 

unsigned int camData[10];
float last_error = 0;
int cube_color, x, y, last_cube_color, count = 0, step = 0, count_degrees = 0;
int line_blue = 0, line_red = 0;
bool final = false;
int MAUVACH = 0;
int start_approach = -1;

// Hằng số
const int Y_IGNORE = 40; // Vị trí tối thiểu để đọc khối theo trục Y (nếu xa hơn sẽ không đọc)
const int Y_SENSE = 132;
const int GREEN = 200; // Vị trí né khối theo trục x của màu xanh
const int RED = 100; // Vị trí né khối theo trục y của màu đỏ
const int EXIT = 12; // Điều kiện kết thúc vòng chạy
const float WALL_THRESHOLD = 180.0;


// Giới hạn góc xoay
float limit(float value, float min, float max) {
  if (value < min) value = min;
  if (value > max) value = max;
  return value;
}
/*
  Tính toán vị trí xoay
  value: góc muốn xoay (0 = đi thẳng)
  l: giới hạn góc xoay
*/
void servoMotor(float value, float l = 43) {
  float diff = 90 + limit(value, -l, l);
  MiniR4.RC1.setAngle(diff);
}

bool isCornerColor() {
  bool isOrange = (line_blue < 100) && (line_red > 75);
  bool isBlue = (line_red < 70) && ((line_blue - line_red) > 40);
  return isOrange || isBlue;}
bool isOrange() {
  return (line_blue < 100) && (line_red > 75);
}
bool isBlue() {
  return (line_red < 70) && ((line_blue - line_red) > 40);
}
bool isFirstCornerLine() {
  return (MAUVACH == 1) ? isOrange() : isBlue();
}
bool isSecondCornerLine() {
  return (MAUVACH == 1) ? isBlue() : isOrange();
}
void getcolor() {
  line_blue = MiniR4.I2C3.MXColorV3.getB();
  line_red = MiniR4.I2C3.MXColorV3.getR();
}



// Hàm kiểm tra xem sau khi né khối thì sát bên hông xe có tường hay không
bool kiem_tra_tuong(int color) {
  float tuong_trai = MiniR4.I2C1.MXLaserV2.getDistance();
  float tuong_phai = MiniR4.I2C2.MXLaserV2.getDistance();

  if (color == 0) { 
    // Vừa né khối Xanh (né sang Trái) -> Kiểm tra tường Trái
    return (tuong_trai < WALL_THRESHOLD && tuong_trai > 0);
  } 
  else if (color == 1) { 
    // Vừa né khối Đỏ (né sang Phải) -> Kiểm tra tường Phải
    return (tuong_phai < WALL_THRESHOLD && tuong_phai > 0);
  }
  return false;
}



// dò đường trái trong loop
void bam_line_trai(float khoang_cach, float kp, float kd) {
  float error = (khoang_cach / 0.707) - MiniR4.I2C1.MXLaserV2.getDistance();
  float D = error - last_error;
  float correction = error * kp + D * kd;
  servoMotor(correction, 40);
  last_error = error;
}

// dò đường phải trong loop
void bam_line_phai(float khoang_cach, float kp, float kd) {
  float error = MiniR4.I2C2.MXLaserV2.getDistance() - (khoang_cach / 0.707);
  float D = error - last_error;
  float correction = error * kp + D * kd;
  servoMotor(correction);
  last_error = error;
}

void ne_buoc_1(float khoang_cach, float goc_re) {
  servoMotor(goc_re);
  int startDegrees = abs(MiniR4.M1.getDegrees());
  while (abs(MiniR4.M1.getDegrees() - startDegrees) * 21.352 / 360.0 <= khoang_cach) {
    // if (turn()) break; 
  }
}

// Kiểm tra điều kiện kết thúc nếu: đi đủ khoảng cách + chạm đường xanh cam hoặc vòng đầu tiên + chạm đường xanh cam
void line_counter() {
  getcolor();
  
  if ((abs(count_degrees - MiniR4.M1.getDegrees()) * 21.352 / 360 > 100 and isCornerColor()) or (count == 0 and isCornerColor())) {
    count++;
    count_degrees = MiniR4.M1.getDegrees();
  }
}
 
// Khi đi tới vị trí góc sa bàn (ở giữa 2 đường xanh cam) robot sẽ xoay 1 góc rộng để quét các khối tiếp theo (nếu có)
bool turn() {
  getcolor();
  bool atcorner = false; 
  if (isFirstCornerLine() and count != EXIT) {
    
    line_counter(); 
    MiniR4.Vision.Data(camData);
    cube_color = camData[0];
    y = camData[2];
    if (y < Y_IGNORE) cube_color = 255;

    if (cube_color != 255 || step == 1) {
      return false; 
    }
    atcorner = true;
    last_error = 0;
    MiniR4.LED.setColor(1, 255, 255, 0);

    if (cube_color == 255) {
      MiniR4.LED.setColor(1, 255, 0, 255);
      unsigned int timeStart = millis();

      float goc_quay = ((last_cube_color == 1) ? 35.0 : 35.0) * MAUVACH;
      while (!isSecondCornerLine() and cube_color == 255 and millis() - timeStart < 4000) {
        line_counter();
        MiniR4.Vision.Data(camData);
        servoMotor(goc_quay);
        cube_color = camData[0];
        y = camData[2];
        if (y < Y_IGNORE) cube_color = 255;
        if (cube_color != 255) {
          MiniR4.LED.setColor(1, 255, 0, 255);
          break;
        } 

        MiniR4.M1.setSpeed(20);
        getcolor();
      }
      if (cube_color == 255) {
        MiniR4.LED.setColor(1, 255, 100, 0);
        servoMotor(-35.0 * MAUVACH); 
        MiniR4.M1.setSpeed(-20);    
        int startDegrees = abs(MiniR4.M1.getDegrees());
    
        while (abs(MiniR4.M1.getDegrees() - startDegrees) * 21.352 / 360.0 <= 17.0) {
          MiniR4.Vision.Data(camData);
          cube_color = camData[0];
          y = camData[2];
          if (y < Y_IGNORE) cube_color = 255;
          if (cube_color != 255 && y < Y_SENSE) {
            MiniR4.LED.setColor(1, 255, 0, 255);
            break;
          }
        }
        servoMotor(0);
      }
    }
    getcolor();
  }
  return atcorner;
}



// dò đường không trong loop 2 bên:
bool bam_line_trai_cm(float quang_duong, float khoang_cach = 200, float kp = 0.15, float kd = 0.1) {
  last_error = 0;
  int startDegrees = abs(MiniR4.M1.getDegrees());
  while (abs(MiniR4.M1.getDegrees() - startDegrees) * 21.352 / 360.0 <= quang_duong) {
    bam_line_trai(khoang_cach, kp, kd); 
    if (turn()) return false;         
  }
  return true;
}

bool bam_line_phai_cm(float quang_duong, float khoang_cach = 200, float kp = 0.15, float kd = 0.1) {
  last_error = 0;
  int startDegrees = abs(MiniR4.M1.getDegrees());
  while (abs(MiniR4.M1.getDegrees() - startDegrees) * 21.352 / 360.0 <= quang_duong) {
    bam_line_phai(khoang_cach, kp, kd); 
    if (turn()) return false;     
  }
  return true;
}

bool dichuyen_check_all(float quang_duong) {
  last_error = 0;
  int startDegrees = abs(MiniR4.M1.getDegrees());
  while (abs(MiniR4.M1.getDegrees() - startDegrees) * 21.352 / 360.0 <= quang_duong) {
    if (turn()) return false; 

    MiniR4.Vision.Data(camData);
    cube_color = camData[0];
    y = camData[2];

    if (cube_color != 255 && y >= Y_IGNORE) {
      return false;
    }
  }
  return true;
}

void last_step(bool adjust = true) {
  if (final) {
    servoMotor(0);
    bool check_2a = dichuyen_check_all(7.0);
    bool check_2b = false;
    if (!check_2a) {
      final = false; 
      return;}

    float goc_ngoat_lai = (last_cube_color == 0) ? 45.0 : -45.0;
    servoMotor(goc_ngoat_lai);
    check_2b = dichuyen_check_all(16.0);
    if (!check_2b) {
      final = false;
      return;       
    }

    if (adjust) {
      bool co_tuong = kiem_tra_tuong(last_cube_color);
      if (co_tuong) {
        if (last_cube_color == 0) {
          MiniR4.LED.setColor(1, 0, 255, 255);
          bam_line_trai_cm(8.0, 250, 0.2, 0.1);
        } 
        else if (last_cube_color == 1) {
          MiniR4.LED.setColor(1, 255, 255, 0);
          bam_line_phai_cm(8.0, 250, 0.2, 0.1);
        }
      } 
      else {
        float goc_tra_lan = (last_cube_color == 0) ? -13.0 : 13.0;
        servoMotor(goc_tra_lan);
        bool buoc3_ok = dichuyen_check_all(6.0);
        if (!buoc3_ok) {
        final = false;
          return;    // THOÁT HÀM NGAY nếu phát hiện khối ở bước 3
        }
        
        servoMotor(0);
        }
      }

    final = false;
    last_error = 0;
    last_cube_color = 255;
  } 
}

void dichuyen_cm(float cm) {
  int startDegrees = abs(MiniR4.M1.getDegrees());
  while (abs(MiniR4.M1.getDegrees() - startDegrees) * 21.352 / 360.0 <= cm) {
  }
}

void setup() {
  // put your setup code here, to run once:
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2);
  Serial.begin(9600);
  MiniR4.I2C1.MXLaserV2.begin();
  MiniR4.I2C2.MXLaserV2.begin();
  MiniR4.I2C3.MXColorV3.begin();
  MiniR4.M1.setReverse(false);
  MiniR4.RC1.setHWDir(false);
  MiniR4.RC1.setAngle(90);
  MiniR4.Vision.Begin();
  MiniR4.Vision.reset();

   
}

void loop() { 
  
  while (true) {
    if (MiniR4.BTN_DOWN.getState()) {
      while(1) {
        MiniR4.Vision.Data(camData);
      }
    } else if (MiniR4.BTN_UP.getState()) {
      break;
    }
  }
  MiniR4.Vision.Data(camData);

  int p = 0; 
  int s = 0;
  while (p < 50) {
    int r = MiniR4.I2C2.MXLaserV2.getDistance(); 
    int l = MiniR4.I2C1.MXLaserV2.getDistance(); 
    if (r > l) s++; else s--;
    p += 1;
  }
MAUVACH = (s > 0) ? 1 : -1;
  MiniR4.M1.resetCounter();

  servoMotor(45 * MAUVACH);
  MiniR4.M1.setSpeed(30);
  dichuyen_cm(7.0);

  servoMotor(-45 * MAUVACH);
  MiniR4.M1.setSpeed(-30);
  dichuyen_cm(5.0);

  MiniR4.M1.setSpeed(30);
  servoMotor(45 * MAUVACH);
  dichuyen_cm(15.0);

  servoMotor(-45 * MAUVACH);
  dichuyen_cm(5.0);
  servoMotor(0);

while (true) {
  MiniR4.Vision.Data(camData);
  cube_color = camData[0];
  x = camData[1];
  y = camData[2];
  
  if (y < Y_IGNORE) cube_color = 255;
  MiniR4.M1.setSpeed(30);
  
  getcolor();
  turn();
  if (cube_color != 255 and y < Y_SENSE) {
    last_step(false);
    MiniR4.LED.setColor(1, 0, 0, 255);
    MiniR4.M1.setSpeed(40);

    if (start_approach == -1) {
      start_approach = abs(MiniR4.M1.getDegrees());
    }
float cm_da_di = (abs(MiniR4.M1.getDegrees()) - start_approach) * 21.352 / 360.0;
      if (cm_da_di >= 5.0) {
        step = 1;
      }
    final = false;
    if (cube_color == 0) {
      servoMotor(int(x - GREEN) * 0.5, 30);
    } 
    else if (cube_color == 1) {
      servoMotor(int(x - RED) * 0.5, 30);
    } 
  }
  else if (cube_color == 0 && step == 1) {
      MiniR4.LED.setColor(1, 0, 255, 0);
      MiniR4.M1.setSpeed(20);
      ne_buoc_1(15.0, - 45.0);
      start_approach = -1;
      step = 0;
      final = true;
      last_cube_color = 0;
      
  } else if (cube_color == 1 && step == 1 && y < 200) {
      MiniR4.LED.setColor(1, 255, 0, 0);
      MiniR4.M1.setSpeed(20);
      ne_buoc_1(15.0, 45.0);
      start_approach = -1;
      step = 0;
      final = true;
      last_cube_color = 1;
      
  } else if (cube_color == 255 and !isCornerColor()) {
    last_step();
    MiniR4.LED.setColor(1, 0, 0, 0);
    MiniR4.M1.setSpeed(45);
    if (MAUVACH == 1) {
      bam_line_trai(450, 0.2, 0.1);
    } else {
      bam_line_phai(450, 0.2, 0.1);
    }
    step = 0;
  }
    line_counter();
    if (count == EXIT) {
      MiniR4.M1.setBrake(true);
      break;
  } 
}
}