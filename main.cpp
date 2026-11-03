//  Generated Leanbot Arduino code
//    by Leanbot Blockly Editor 2026-02-14

#include <Leanbot.h>
#include <Arduino_APDS9960.h>
// Motor command units: steps/second (LbMotion.runLR).
// These are initial test values, not RPM conversions.
const int TURN_STEPS_PER_SEC = 850;
const int MOVE_STEPS_PER_SEC = 1200;

int ObjRed, ObjGreen, ObjBlue;
String color;
String mau;  // Color result used by the original mission
// Describe this function...
void rotate45RightTakeCorner() {
  LbMotion.stopAndWait();
  LbMotion.runLR(TURN_STEPS_PER_SEC, -TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLR(MOVE_STEPS_PER_SEC, MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(8 * 10);
  LbMotion.stopAndWait();
  LbGripper.close();
  LbMotion.runLR(-MOVE_STEPS_PER_SEC, -MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(8 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLR(-TURN_STEPS_PER_SEC, TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
void rotate45RightTake() {
  LbMotion.stopAndWait();
  LbMotion.runLR(TURN_STEPS_PER_SEC, -TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLR(MOVE_STEPS_PER_SEC, MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbGripper.close();
  LbMotion.runLR(-MOVE_STEPS_PER_SEC, -MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLR(-TURN_STEPS_PER_SEC, TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
void rotate45RightThrow() {
  LbMotion.stopAndWait();
  LbMotion.runLR(TURN_STEPS_PER_SEC, -TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLR(MOVE_STEPS_PER_SEC, MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbGripper.open();
  LbMotion.runLR(-MOVE_STEPS_PER_SEC, -MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLR(-TURN_STEPS_PER_SEC, TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
// Tha khoi tai goc o vuong: quay phai 45 do, tien 80 mm,
// mo kep, lui ve va quay ve huong ban dau.
// distanceMm co the dieu chinh theo tung goc A/B/C/...
void rotate45RightThrowCorner(int distanceMm = 80) {
  LbMotion.stopAndWait();
  LbMotion.runLR(TURN_STEPS_PER_SEC, -TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLR(MOVE_STEPS_PER_SEC, MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(distanceMm);
  LbMotion.stopAndWait();
  LbGripper.open();
  LbMotion.runLR(-MOVE_STEPS_PER_SEC, -MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(distanceMm);
  LbMotion.stopAndWait();
  LbMotion.runLR(-TURN_STEPS_PER_SEC, TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

void rotate45RightPushCorner() {
  LbGripper.close();
  LbMotion.stopAndWait();
  LbMotion.runLR(TURN_STEPS_PER_SEC, -TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLR(MOVE_STEPS_PER_SEC, MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(12 * 10);
  LbMotion.stopAndWait();
  LbGripper.open();
  LbMotion.runLR(-MOVE_STEPS_PER_SEC, -MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(12 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLR(-TURN_STEPS_PER_SEC, TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
void rotate45LeftTakeCorner() {
  LbMotion.stopAndWait();
  LbMotion.runLR(-TURN_STEPS_PER_SEC, TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLR(MOVE_STEPS_PER_SEC, MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(8 * 10);
  LbMotion.stopAndWait();
  LbGripper.close();
  LbMotion.runLR(-MOVE_STEPS_PER_SEC, -MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(8 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLR(TURN_STEPS_PER_SEC, -TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
void rotate45LeftTake() {
  LbMotion.stopAndWait();
  LbMotion.runLR(-TURN_STEPS_PER_SEC, TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLR(MOVE_STEPS_PER_SEC, MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbGripper.close();
  LbMotion.runLR(-MOVE_STEPS_PER_SEC, -MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLR(TURN_STEPS_PER_SEC, -TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
void rotate45LeftThrow() {
  LbMotion.stopAndWait();
  LbMotion.runLR(-TURN_STEPS_PER_SEC, TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLR(MOVE_STEPS_PER_SEC, MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbGripper.open();
  LbMotion.runLR(-MOVE_STEPS_PER_SEC, -MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(10 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLR(TURN_STEPS_PER_SEC, -TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

// Describe this function...
// Tha khoi tai goc o vuong ben trai, doi xung voi RightThrowCorner.
void rotate45LeftThrowCorner(int distanceMm = 80) {
  LbMotion.stopAndWait();
  LbMotion.runLR(-TURN_STEPS_PER_SEC, TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLR(MOVE_STEPS_PER_SEC, MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(distanceMm);
  LbMotion.stopAndWait();
  LbGripper.open();
  LbMotion.runLR(-MOVE_STEPS_PER_SEC, -MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(distanceMm);
  LbMotion.stopAndWait();
  LbMotion.runLR(TURN_STEPS_PER_SEC, -TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

void rotate45LeftPushCorner() {
  LbGripper.close();
  LbMotion.stopAndWait();
  LbMotion.runLR(-TURN_STEPS_PER_SEC, TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
  LbMotion.runLR(MOVE_STEPS_PER_SEC, MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(12 * 10);
  LbMotion.stopAndWait();
  LbGripper.open();
  LbMotion.runLR(-MOVE_STEPS_PER_SEC, -MOVE_STEPS_PER_SEC);
  LbMotion.waitDistanceMm(12 * 10);
  LbMotion.stopAndWait();
  LbMotion.runLR(TURN_STEPS_PER_SEC, -TURN_STEPS_PER_SEC);
  LbMotion.waitRotationDeg(45);
  LbMotion.stopAndWait();
}

int line;
int speed;
// Digital 4-sensor line tracking: uses calibrated black/white bitmask only.
// Corrections are initial tuning values; validate sensor orientation on hardware.
const float LINE_TURN_SMALL = 0.50f;
const float LINE_TURN_MEDIUM = 0.70f;
const float LINE_TURN_LARGE = 1.00f;
const unsigned long LINE_LOSS_HOLD_MS = 40;
int lastLineTurn = 0; // -1: slow left wheel; +1: slow right wheel
unsigned long lineLossStartedAt = 0;

float lineTurnCorrection(int state) {
  switch (state) {
    // Preserve turning directions of the original sketch.
    case 4:  return -LINE_TURN_SMALL;
    case 12: return -LINE_TURN_MEDIUM;
    case 8:  return -LINE_TURN_LARGE;
    case 14: return -LINE_TURN_MEDIUM;
    case 2:  return  LINE_TURN_SMALL;
    case 3:  return  LINE_TURN_MEDIUM;
    case 1:  return  LINE_TURN_LARGE;
    case 7:  return  LINE_TURN_MEDIUM;
    case 6:  // centered
    case 15: // junction / all black
    default: return 0.0f;
  }
}

void followLineMotorOnly();  // runFollowLine() calls this below

void runFollowLine() {
  LbIRLine.read();
  line = LbIRLine.value();
  followLineMotorOnly();
  LbRGB.clear();
  LbIRLine.displayOnRGB(0x00ff00);
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
  // On complete line loss, travel straight briefly (for small gaps),
  // then search in the direction of the last observed deviation.
  if (line == 0) {
    if (lineLossStartedAt == 0) lineLossStartedAt = millis();
    if ((unsigned long)(millis() - lineLossStartedAt) < LINE_LOSS_HOLD_MS) {
      LbMotion.runLR(speed, speed);
      return;
    }
    int slow = (int)(speed * (1.0f - LINE_TURN_LARGE));
    if (lastLineTurn < 0) LbMotion.runLR(slow, speed);
    else if (lastLineTurn > 0) LbMotion.runLR(speed, slow);
    else LbMotion.runLR(speed, speed);
    return;
  }
  lineLossStartedAt = 0;
  float correction = lineTurnCorrection(line);
  // Do not overwrite last deviation on full black / junction.
  if (line != 15) {
    if (correction < 0) lastLineTurn = -1;
    else if (correction > 0) lastLineTurn = 1;
  }
  // Extreme states 1000 and 0001: outside wheel at max 2000 steps/s,
  // inside wheel stopped. Keep the original steering direction.
  if (line == 8) {
    LbMotion.runLR(0, 2000);
    return;
  }
  if (line == 1) {
    LbMotion.runLR(2000, 0);
    return;
  }

  int left = speed;
  int right = speed;
  if (correction < 0) left = (int)(speed * (1.0f + correction));
  else if (correction > 0) right = (int)(speed * (1.0f - correction));
  LbMotion.runLR(left, right);
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

    // Hien thi 4 cam bien IR tren LED RGB trong luc do line.
    // Cac mat phat hien vach den se hien thi mau xanh la.
    LbRGB.clear();
    LbIRLine.displayOnRGB(0x00ff00);

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

        // Den junction thu n: cam bien o phia truoc truc banh.
        // Tien them 10 mm de dua truc banh ve gan tam junction,
        // sau do moi cho phep lenh xoay tiep theo.
        if (junctionCount >= n) {
          LbMotion.runLR(MOVE_STEPS_PER_SEC, MOVE_STEPS_PER_SEC);
          LbMotion.waitDistanceMm(10);
          LbMotion.stopAndWait();
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
// APDS9960 five-color recognition under switched RGB illumination.
// Gains and thresholds are initial estimates; calibrate using real blocks.
// ==================================================
const float COLOR_GAIN_R = 1.0f;
const float COLOR_GAIN_G = 1.0f;
const float COLOR_GAIN_B = 1.0f;
const float COLOR_MIN_SIGNAL = 10.0f;
const unsigned long COLOR_TIMEOUT_MS = 350;
bool colorReadOK = false;

bool _coreRead(int& r, int& g, int& b, CRGB ledColor) {
  LbRGB.fillColor(ledColor);
  LbRGB.show();
  LbDelay(20);  // Allow illumination to settle.
  // Discard the previously available sample to avoid using the last LED color.
  if (APDS.colorAvailable()) {
    int oldR, oldG, oldB;
    APDS.readColor(oldR, oldG, oldB);
  }
  unsigned long t0 = millis();
  while (!APDS.colorAvailable()) {
    if ((unsigned long)(millis() - t0) >= COLOR_TIMEOUT_MS) return false;
  }
  APDS.readColor(r, g, b);
  return true;
}

void Obj_readRGB() {
  int rRed = 0, gGreen = 0, bBlue = 0;
  int rawR = 0, rawG = 0, rawB = 0;
  int darkR = 0, darkG = 0, darkB = 0;

  colorReadOK = false;
  ObjRed = ObjGreen = ObjBlue = 0;
  int previousBrightness = LbRGB.getBrightness();
  LbRGB.setBrightness(255);

  bool ok = _coreRead(rRed, rawG, rawB, CRGB::Red);
  if (ok) ok = _coreRead(rawR, gGreen, rawB, CRGB::Lime);
  if (ok) ok = _coreRead(rawR, rawG, bBlue, CRGB::Blue);
  if (ok) ok = _coreRead(darkR, darkG, darkB, CRGB::Black);

  LbRGB.setBrightness(previousBrightness);
  LbRGB.clear();
  LbRGB.show();
  if (!ok) return;

  ObjRed = (rRed > darkR) ? (rRed - darkR) : 0;
  ObjGreen = (gGreen > darkG) ? (gGreen - darkG) : 0;
  ObjBlue = (bBlue > darkB) ? (bBlue - darkB) : 0;
  colorReadOK = true;
}

// Returns: "Red", "Yellow", "Green", "Blue", "Purple" or "Unknown".
String Obj_detectColor() {
  Obj_readRGB();
  if (!colorReadOK) return "Unknown";

  float r = ObjRed * COLOR_GAIN_R;
  float g = ObjGreen * COLOR_GAIN_G;
  float b = ObjBlue * COLOR_GAIN_B;
  if (r < COLOR_MIN_SIGNAL && g < COLOR_MIN_SIGNAL && b < COLOR_MIN_SIGNAL) return "Unknown";

  String detected = "Unknown";
  if (r >= COLOR_MIN_SIGNAL && g >= COLOR_MIN_SIGNAL &&
      r > 1.45f * b && g > 1.45f * b &&
      r < 2.0f * g && g < 2.0f * r) {
    detected = "Yellow";
  } else if (r >= COLOR_MIN_SIGNAL && b >= COLOR_MIN_SIGNAL &&
             r > 1.45f * g && b > 1.45f * g &&
             r < 2.2f * b && b < 2.2f * r) {
    detected = "Purple";
  } else if (g >= COLOR_MIN_SIGNAL && g > 1.30f * r && g > 1.30f * b) {
    detected = "Green";
  } else if (b >= COLOR_MIN_SIGNAL && b > 1.30f * r && b > 1.30f * g) {
    detected = "Blue";
  } else if (r >= COLOR_MIN_SIGNAL && r > 1.30f * g && r > 1.30f * b) {
    detected = "Red";
  }

  if (detected == "Red") LbRGB.fillColor(CRGB::Red);
  else if (detected == "Yellow") LbRGB.fillColor(CRGB::Yellow);
  else if (detected == "Green") LbRGB.fillColor(CRGB::Green);
  else if (detected == "Blue") LbRGB.fillColor(CRGB::Blue);
  else if (detected == "Purple") LbRGB.fillColor(CRGB::Purple);
  else LbRGB.fillColor(CRGB(50, 50, 50));
  LbRGB.show();
  return detected;
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
// Small reusable wrappers around existing Leanbot motion API.
// No coordinates, pose estimation, or automatic path planning.
void turnRight(int angleDeg, int stepsPerSec = TURN_STEPS_PER_SEC) {
  LbMotion.runLR(stepsPerSec, -stepsPerSec);
  LbMotion.waitRotationDeg(angleDeg);
  LbMotion.stopAndWait();
}

void turnLeft(int angleDeg, int stepsPerSec = TURN_STEPS_PER_SEC) {
  LbMotion.runLR(-stepsPerSec, stepsPerSec);
  LbMotion.waitRotationDeg(angleDeg);
  LbMotion.stopAndWait();
}

void forwardMm(int distanceMm, int stepsPerSec = MOVE_STEPS_PER_SEC) {
  LbMotion.runLR(stepsPerSec, stepsPerSec);
  LbMotion.waitDistanceMm(distanceMm);
  LbMotion.stopAndWait();
}

void backwardMm(int distanceMm, int stepsPerSec = MOVE_STEPS_PER_SEC) {
  LbMotion.runLR(-stepsPerSec, -stepsPerSec);
  LbMotion.waitDistanceMm(distanceMm);
  LbMotion.stopAndWait();
}

// ================================================================
// SIMPLE A-B-D MISSION (NO POSITION / HEADING ESTIMATION)
// Fixed-color plan: A2 Yellow, B4 Green, D3 Blue.
// Reserve blocks: S1 Green, S3 Blue.
//
// IMPORTANT - PROVISIONAL ROUTE:
// START at CTRL with the robot ALREADY FACING NORTHWEST.
// Drive forward, turn LEFT 45 deg to face WEST, then follow the
// UPPER approach line toward the A/B/D side of the course.
// Do NOT use the lower reserve-block road as the exit from CTRL.
// Subsequent junction counts and B/D handling remain test assumptions;
// the S1/S3 branch is visited only during reserve pickup missions.
// Run with only ONE robot during first tests.
// ================================================================

void loop() {
  static bool finished = false;
  if (finished) return;
  finished = true; // Prevent accidentally repeating the mission.

  LbMission.begin(TB1A + TB1B);
  LbGripper.open();

  // 1. EXIT CTRL VIA THE UPPER LINE (NOT THE LOWER S1/S3/S5 ROAD).
  // Dat Leanbot o CTRL, DA HUONG TAY BAC ngay tu dau.
  forwardMm(130);       // Tien thang ve phia Tay Bac.
  turnLeft(45);        // Tay Bac -> Tay, vao nhanh line phia tren.
  followLineUntilNthJunction(2, 1500);

  // 2. PUSH B4 GREEN INTO B DIAMOND.
  // Approach from the southeast side; 45 deg left from north.
  rotate45RightPushCorner();

  // 3. PUSH D3 BLUE INTO D DIAMOND.
  // Tiep can tu phia TAY BAC: robot quay sang DONG NAM de day.
  // Sau khi lui ve junction, quay trai 45 do de bam line huong DONG.
  LbGripper.close();
  turnLeft(45);
  forwardMm(120);
  LbGripper.open();
  backwardMm(120);
  turnLeft(135);

  // 4. GET Greeen AT S3.
  // Moi chang di qua 2 junction (da cong them 1 junction).
  followLineUntilNthJunction(2, 1500);
  turnRight(90);
  followLineUntilNthJunction(2, 1500);

  LbGripper.open();
  forwardMm(65);
  LbGripper.close();
  backwardMm(65);

  // 5. RETURN TO D AND PLACE BLUE.
  // Ve D: 2 junction moi chang.
  turnRight(180);
  followLineUntilNthJunction(2, 1500);
  turnLeft(90);
  followLineUntilNthJunction(2, 1500);

  // Tiep can tu TAY BAC, day theo huong DONG NAM; khong qua S1/S3/S5.
  turnRight(45);
  forwardMm(125);
  LbGripper.open();
  backwardMm(125);
  turnLeft(45);   // Khoi phuc huong truoc khi quay phai 45 do de tha Blue.

  // 6. RETURN TO S1 TO TAKE blue (2 junction moi chang).
  followLineUntilNthJunction(2, 1500);
  turnRight(90);
  followLineUntilNthJunction(2, 1500);

// take the Yellow cube 
  rotate45RightTake();
  turnRight(180);
  //return crl wwith yellow cube 
  followLineUntilNthJunction (2,1500);
  turnLeft(90);
  followLineUntilNthJunction(4,1500);
  turnRight(45);
  forwardMm(300);
  LbGripper.open(); // through the yellow cube away 
  backwardMm(300);
  
  // take the blue cube 

  turnLeft(45);
  followLineUntilNthJunction (2,1500);
  turnRight(45);
  forwardMm(85);
  LbGripper.close();
  backwardMm(85);

  turnRight(45);
  followLineUntilNthJunction(2,1500);
  rotate45RightThrowCorner(); // Goc o vuong: 80 mm, thay cho throw thuong 100 mm.


// return base CRL 

  turnRight(180);
  followLineUntilNthJunction(2,1500);
  turnLeft(45);
  forwardMm(200);

  LbMotion.stopAndWait();
  Leanbot.tone(1500, 200);
  LbMission.end();
}