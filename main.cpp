//  Generated Leanbot Arduino code
//    by Leanbot Blockly Editor 2026-02-14

#include <Leanbot.h>
#include <Arduino_APDS9960.h>
int ObjRed, ObjGreen, ObjBlue;
String color;
// Describe this function...
void rotate45RightTakeCorner() {
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(20, -20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(30, 30);
  LbMotion.waitDistanceMm(8 * 10);
  LbMotion.stopAndWait();
  LbGripper.close();
  LbMotion.runLRrpm(-30, -30);
  LbMotion.waitDistanceMm(8 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(-20, 20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
void rotate45RightTake() {
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(20, -20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(30, 30);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbGripper.close();
  LbMotion.runLRrpm(-30, -30);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(-20, 20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
void rotate45RightThrow() {
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(20, -20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(30, 30);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbGripper.open();
  LbMotion.runLRrpm(-30, -30);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(-20, 20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
void rotate45RightPushCorner() {
  LbGripper.close();
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(20, -20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(30, 30);
  LbMotion.waitDistanceMm(12 * 10);
  LbMotion.stopAndWait();
  LbGripper.open();
  LbMotion.runLRrpm(-30, -30);
  LbMotion.waitDistanceMm(12 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(-20, 20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
void rotate45LeftTakeCorner() {
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(-20, 20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(30, 30);
  LbMotion.waitDistanceMm(8 * 10);
  LbMotion.stopAndWait();
  LbGripper.close();
  LbMotion.runLRrpm(-30, -30);
  LbMotion.waitDistanceMm(8 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(20, -20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
void rotate45LeftTake() {
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(-20, 20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(30, 30);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbGripper.close();
  LbMotion.runLRrpm(-30, -30);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(20, -20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
void rotate45LeftThrow() {
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(-20, 20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(30, 30);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbGripper.open();
  LbMotion.runLRrpm(-30, -30);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(20, -20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
void rotate45LeftPushCorner() {
  LbGripper.close();
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(-20, 20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(30, 30);
  LbMotion.waitDistanceMm(12 * 10);
  LbMotion.stopAndWait();
  LbGripper.open();
  LbMotion.runLRrpm(-30, -30);
  LbMotion.waitDistanceMm(12 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLRrpm(20, -20);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

int line;
int speed;
// Describe this function...
void runFollowLine() {
  LbIRLine.read();
  line = LbIRLine.value();
  LbRGB.clear();
  LbIRLine.displayOnRGB(0x00ff00);
  switch (line) {
    case 4:
    case 14:
    case 12:
    case 8:
      LbMotion.runLR(speed / 8, speed);
      break;

    case 2:
    case 7:
    case 3:
    case 1:
      LbMotion.runLR(speed, speed / 8);
      break;

    case 9:
    case 13:
    case 11:
    case 10:
    case 5:
    default:
      LbMotion.runLR(speed, speed);
   }

}

bool juntion;
int distance;
// Describe this function...
void folowinglineUntilJunction() {

  { // Dò line cho tới khi gặp Juntion thì dừng
    juntion = false;
    while ((juntion == false)) {
      speed = 1000;
      distance = 1000;
      followJuntion();
    }

    juntion = false;
  }

}

int target;
// Describe this function...
void followLine() {

  { // Dò line theo vận tốc và quãng đường , trước khi dùng thì set speed và set  distance
    target = LbMotion.getDistanceMm() + distance;
    followLineCarefully();
    while (LbIRLine.isBlackDetected()) {
      followLineCarefully();
      if ((LbMotion.getDistanceMm() > target)) {
        if ((line == 6)) {
          break;
        }
      }
      LbMotion.waitDistanceMm(1);
    }

  }

}

// Describe this function...
void followJuntion() {
  target = LbMotion.getDistanceMm() + distance;
  followLineCarefully();
  while (LbIRLine.isBlackDetected() && (juntion == false)) {
    followLineCarefully();
    if ((LbMotion.getDistanceMm() > target)) {
      if ((line == 6)) {
        break;
      }
    }
    LbMotion.waitDistanceMm(1);
  }

}

// Describe this function...
void followLineCarefully() {
  for (int count = 0; count < 3; count++) {
    runFollowLine();
    if ((line == 15)) {
      juntion = true;
      Leanbot.tone(1500, 100);
      LbDelay(100);
      LbRGB.clear();
      LbRGB.fillColor(0xffff00, BITMAP(ledO, ledA, ledB, ledC, ledD, ledE, ledF));
      LbRGB.show();
      break;
    } else if (LbIRLine.isBlackDetected()) {
      break;
    }
    LbMotion.waitDistanceMm(1);
  }

}


/*
 * Dieu khien dong co do line
 * Khong cap nhat LED RGB de giu mau bao nga tu.
 */
void followLineMotorOnly() {
  LbRGB.clear();
  LbIRLine.displayOnRGB(0x00ff00);
  switch (line) {
    case 4:
    case 14:
    case 12:
    case 8:
      LbMotion.runLR(speed / 8, speed);
      break;

    case 2:
    case 7:
    case 3:
    case 1:
      LbMotion.runLR(speed, speed / 8);
      break;

    default:
      LbMotion.runLR(speed, speed);
      break;
  }
}

/*
 * Phat buzzer va hien thi mau LED
 * theo thu tu nga tu da gap.
 */
void notifyJunction(int junctionNumber) {
  unsigned long color;

  switch ((junctionNumber - 1) % 6) {
    case 0:
      color = 0xFF0000; // Do
      break;
    case 1:
      color = 0x0000FF; // Xanh duong
      break;
    case 2:
      color = 0xFFFF00; // Vang
      break;
    case 3:
      color = 0xFF00FF; // Tim
      break;
    case 4:
      color = 0x00FFFF; // Xanh cyan
      break;
    default:
      color = 0xFF8000; // Cam
      break;
  }

  // Hien thi mau moi
  LbRGB.clear();
  LbRGB.fillColor(
    color,
    BITMAP(ledO, ledA, ledB, ledC, ledD, ledE, ledF)
  );
  LbRGB.show();

  // Keu buzzer 100 ms
  Leanbot.tone(1500, 100);
}

/*
 * Do line lien tuc den nga tu thu n.
 *
 * n: So nga tu can dem.
 *
 * Robot:
 * - Do line
 * - Moi nga tu keu buzzer va doi mau LED
 * - Chi dem mot lan moi nga tu
 * - Dung khi gap nga tu thu n
 */
void followLineUntilNthJunction(int n, int v) {

  if (n <= 0) {
    LbMotion.stopAndWait();
    return;
  }

  speed = v;

  int junctionCount = 0;

  bool junctionLocked = false;

  int blackStreak = 0;

  unsigned long exitTime = 0;

  const unsigned long EXIT_DEBOUNCE_MS = 60;
  const int BLACK_CONFIRM_SAMPLES = 3;

  while (junctionCount < n) {

    // Doc cam bien line
    LbIRLine.read();
    line = LbIRLine.value();

    // Dieu khien robot bam line
    followLineMotorOnly();

    if (line == 15) {

      // Dang o vung den cua nga tu
      exitTime = 0;

      if (blackStreak < BLACK_CONFIRM_SAMPLES) {
        blackStreak++;
      }

      // Chi dem neu chua khoa nga tu hien tai
      if (!junctionLocked &&
          blackStreak >= BLACK_CONFIRM_SAMPLES) {

        junctionLocked = true;

        junctionCount++;

        // Neu la nga tu cuoi, dung robot
        if (junctionCount >= n) {
          LbMotion.stopAndWait();
        }

        // Phat buzzer va hien thi LED
        notifyJunction(junctionCount);

        // Ket thuc khi gap nga tu thu n
        if (junctionCount >= n) {
          return;
        }
      }

    } else {

      // Khong con nhan du 4 mat den
      blackStreak = 0;

      if (junctionLocked) {

        if (exitTime == 0) {
          exitTime = millis();
        }

        // Chi mo khoa sau khi roi nga tu 60 ms
        if (millis() - exitTime >= EXIT_DEBOUNCE_MS) {
          junctionLocked = false;
          exitTime = 0;
        }
      }
    }
  }

  LbMotion.stopAndWait();
}


// ==================================================
// ĐỌC RGB — GIỮ CÁCH ĐO CỦA BẠN
// ==================================================
void _coreRead(int& r, int& g, int& b, CRGB ledColor)
{
  LbRGB.fillColor(ledColor);
  LbRGB.show();

  while (!APDS.colorAvailable());

  APDS.readColor(r, g, b);
}

void Obj_readRGB()
{
  int rRed, gGreen, bBlue;
  int rr, gg, bb;

  int originalBrightness = LbRGB.getBrightness();
  LbRGB.setBrightness(255);

  _coreRead(rRed, gg, bb, CRGB::Red);
  _coreRead(rr, gGreen, bb, CRGB::Lime);
  _coreRead(rr, gg, bBlue, CRGB::Blue);
  _coreRead(rr, gg, bb, CRGB::Black);

  LbRGB.setBrightness(originalBrightness);

  ObjRed   = rRed   - rr;
  ObjGreen = gGreen - gg;
  ObjBlue  = bBlue  - bb;
}

// ==================================================
// NHẬN DIỆN MÀU
// ==================================================
boolean BasicObj_isRed()
{
  if (ObjRed <= ObjGreen) return false;
  if (ObjRed <= ObjBlue)  return false;
  if (ObjRed <= 3)        return false;

  return true;
}

boolean BasicObj_isGreen()
{
  if (ObjGreen <= ObjRed)  return false;
  if (ObjGreen <= ObjBlue) return false;
  if (ObjGreen <= 3)       return false;

  return true;
}

boolean BasicObj_isPurple()
{
  if (ObjBlue <= ObjRed)   return false;
  if (ObjBlue <= ObjGreen) return false;
  if (ObjBlue <= 3)        return false;

  if (ObjRed <= 0)         return false;
  if (ObjGreen <= 0)       return false;

  // Purple: G < 2 × R.
  if ((long)ObjGreen >= 2L * ObjRed) return false;

  return true;
}

boolean BasicObj_isBlue()
{
  if (ObjBlue <= ObjRed)   return false;
  if (ObjBlue <= ObjGreen) return false;
  if (ObjBlue <= 3)        return false;

  if (ObjRed <= 0)         return false;
  if (ObjGreen <= 0)       return false;

  // Blue: G >= 2 × R.
  if ((long)ObjGreen < 2L * ObjRed) return false;

  return true;
}

// ==================================================
// ĐỌC MÀU VÀ HIỂN THỊ LÊN LED
// ==================================================
String Obj_detectColor()
{
  Obj_readRGB();

  
  if (BasicObj_isGreen())
  {
    LbRGB.fillColor(CRGB::Green);
    LbRGB.show();
    return "Green";
  }
  else if (BasicObj_isPurple())
  {
    LbRGB.fillColor(CRGB::Purple);
    LbRGB.show();
    return "Purple";
  }
  else if (BasicObj_isBlue())
  {
    LbRGB.fillColor(CRGB::Blue);
    LbRGB.show();
    return "Blue";
  }
  else 
  {
    LbRGB.fillColor(CRGB::Red);
    LbRGB.show();
    return "Red";
  }
  LbRGB.fillColor(CRGB(50, 50, 50));
  LbRGB.show();
  return "Unknown";
}


void setup() {
  Leanbot.begin();
  APDS.begin();
  LbIRLine.doManualCalibration(TB1A);
}


/*
followLineUntilNJunction(3); dò line cho tới khi đi qua 3 juntion , lưu ý vị trí bắt đầu phải nằm trên line , ko trùng tại juntion 
*/

void followingLine(int x, int y ){
  speed = x;
  distance = y;
  followLine();
}
// folllowingLine(2000,300);
void loop() {

  // String colorTest = Obj_detectColor();
  // Serial.println (colorTest);
  // delay(300);
  // if (colorTest == "Blue"){

  // }
  // else if (colorTest == "Red"){

  // }
  // else {

  // }
   LbMission.begin(TB1A + TB1B);
  di_thang(10);
  re_phai(85);
  speed = 1500;
  distance = 400;
   followLine();
   LbMotion.stopAndWait();
  re_phai(135);
  di_thang(8);
  mau = Obj_detectColor();			// Read and display color LED RGB
  LbDelay(500);
  if(mau == "Blue")
  {
      di_lui(8);
re_phai(45);
      
  speed = 1500;
  distance = 300;
   followLine();
   LbMotion.stopAndWait();
re_trai(85);
  speed = 1500;
  distance = 300;
   followLine();
   LbMotion.stopAndWait();
LbGripper.moveTo(90);
re_trai(85);
  speed = 1500;
  distance = 300;
   followLine();
   LbMotion.stopAndWait();
re_trai(85);
  speed = 1500;
  distance = 300;
   followLine();
   LbMotion.stopAndWait();
re_trai(135);
 di_thang(15);
LbGripper.moveTo(0);
 di_lui(10);
re_trai(45);
  }
  else if(mau == "Green")
  {
            di_lui(8);
re_phai(45);
      
  speed = 1500;
  distance = 320;
   followLine();
   LbMotion.stopAndWait();
re_trai(85);
  speed = 1500;
  distance = 320;
   followLine();
   LbMotion.stopAndWait();
re_phai(90);
di_thang(10);
re_trai(85);
LbGripper.moveTo(90);
re_trai(85);
  speed = 1500;
  distance = 400;
   followLine();
   LbMotion.stopAndWait();
re_trai(85);
  speed = 1500;
  distance = 320;
   followLine();
   LbMotion.stopAndWait();
re_trai(135);
 di_thang(15);
LbGripper.moveTo(0);
 di_lui(10);
re_trai(45);
  }
else if(mau == "Purple")
  {
 di_lui(8);
re_phai(45);
      
  speed = 1500;
  distance = 620;
   followLine();
   LbMotion.stopAndWait();
LbGripper.moveTo(90);
re_phai(175);
  speed = 1500;
  distance = 620;
   followLine();
   LbMotion.stopAndWait();
re_phai(135);
 di_thang(15);
LbGripper.moveTo(0);
 di_lui(10);
re_trai(45);
  }
else 
  {
	LbGripper.moveTo(90);
di_lui(8);
	re_phai(45);
 speed = 1500;
  distance = 320;
   followLine();
   LbMotion.stopAndWait();
re_trai(42);
di_thang(10);
LbGripper.moveTo(0);
di_lui(10);
re_trai(130);
 speed = 1500;
  distance = 320;
   followLine();
   LbMotion.stopAndWait();
re_phai(85);
  }
  // Dò Line
  // { 
  //   speed = 1500;
  //   distance = 90;
  //   followLine();
  // }
LbMission.end();
//    LbMission.begin(TB1A + TB1B);
// followLineUntilNthJunction(3, 1500);


// LbMission.end();
}





//https://lesysang1322002.github.io/Leanbot-Advanced/advance

