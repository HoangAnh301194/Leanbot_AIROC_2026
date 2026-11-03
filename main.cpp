//  Generated Leanbot Arduino code
//    by Leanbot Blockly Editor 2026-02-14

#include <Leanbot.h>
#include <Arduino_APDS9960.h>
int ObjRed, ObjGreen, ObjBlue;
String color;
String mau;  // Color result used by the original mission
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
// ================================================================
// EXPERIMENTAL FIXED MISSION: A2=Yellow, B4=Green, D3=Blue
// Reserves: S1=Green, S3=Blue. Two-robot collision handling is
// NOT implemented. This is a bench-testing route, not a field-
// verified course map. Place robot ONLY at the specified start.
// Start junction: (col=0,row=1), heading SOUTH. NOT at CTRL.
// Map/grid approach positions and distances must be calibrated.
// ================================================================
const int ABD_NAV_SPEED = 900;              // steps/s
const int ABD_MOVE_RPM = 25;
const int ABD_TURN_RPM = 20;
const int ABD_GRID_PITCH_MM = 406;          // nominal: 1219.2 / 3
const int ABD_JUNCTION_MIN_MM = 260;        // initial estimates
const int ABD_JUNCTION_MAX_MM = 550;
const unsigned long ABD_NAV_TIMEOUT_MS = 7000;
const unsigned long ABD_LOST_TIMEOUT_MS = 250;
const int ABD_APPROACH_A_MM = 145;
const int ABD_PUSH_B_MM = 145;
const int ABD_PUSH_D_MM = 145;
const int ABD_PLACE_B_MM = 125;
const int ABD_PLACE_D_MM = 125;
const int ABD_PLACE_CTRL_MM = 175;
const int ABD_S1_LEFT_OFFSET_MM = 45;
const int ABD_RESERVE_APPROACH_MM = 65;

int abdCol = 0;
int abdRow = 1;
int abdHeading = 4;   // 0=N, 2=E, 4=S, 6=W; odd values = diagonals
bool abdMissionFinished = false;

void abdFail() {
  LbMotion.stopAndWait();
  while (true) {
    Leanbot.tone(800, 200);
    LbDelay(800);
  }
}

void abdMoveMm(int signedMm) {
  if (signedMm == 0) return;
  int rpm = (signedMm > 0) ? ABD_MOVE_RPM : -ABD_MOVE_RPM;
  LbMotion.runLRrpm(rpm, rpm);
  LbMotion.waitDistanceMm(abs(signedMm));
  LbMotion.stopAndWait();
}

void abdFace(int target) {
  target = (target + 8) % 8;
  int diff = (target - abdHeading + 8) % 8;
  if (diff == 0) return;
  if (diff <= 4) {
    LbMotion.runLRrpm(ABD_TURN_RPM, -ABD_TURN_RPM);
    LbMotion.waitRotationDeg(diff * 45);
  } else {
    LbMotion.runLRrpm(-ABD_TURN_RPM, ABD_TURN_RPM);
    LbMotion.waitRotationDeg((8 - diff) * 45);
  }
  LbMotion.stopAndWait();
  abdHeading = target;
}

// Detect next full-black junction (1111, three consecutive reads).
// Bounded by encoder distance and elapsed time to avoid an infinite loop.
bool abdOneJunction() {
  unsigned long startMs = millis();
  unsigned long lostAt = 0;
  int startDistance = LbMotion.getDistanceMm();
  int streak = 0;
  speed = ABD_NAV_SPEED;
  lineLossStartedAt = 0;
  lastLineTurn = 0;

  while (true) {
    LbIRLine.read();
    line = LbIRLine.value();
    followLineMotorOnly();

    int travelled = abs(LbMotion.getDistanceMm() - startDistance);
    if (travelled >= ABD_JUNCTION_MIN_MM && line == 15) {
      streak++;
    } else {
      streak = 0;
    }
    if (streak >= 3) {
      LbMotion.stopAndWait();
      return true;
    }

    if (line == 0) {
      if (lostAt == 0) lostAt = millis();
      if ((unsigned long)(millis() - lostAt) >= ABD_LOST_TIMEOUT_MS) {
        LbMotion.stopAndWait();
        return false;
      }
    } else {
      lostAt = 0;
    }

    if (travelled >= ABD_JUNCTION_MAX_MM ||
        (unsigned long)(millis() - startMs) >= ABD_NAV_TIMEOUT_MS) {
      LbMotion.stopAndWait();
      return false;
    }
  }
}

void abdGoTo(int col, int row) {
  if (col < 0 || col > 3 || row < 0 || row > 3) abdFail();
  while (abdCol != col) {
    if (abdCol < col) {
      abdFace(2);                   // east
      if (!abdOneJunction()) abdFail();
      abdCol++;
    } else {
      abdFace(6);                   // west
      if (!abdOneJunction()) abdFail();
      abdCol--;
    }
  }
  while (abdRow != row) {
    if (abdRow < row) {
      abdFace(4);                   // south
      if (!abdOneJunction()) abdFail();
      abdRow++;
    } else {
      abdFace(0);                   // north
      if (!abdOneJunction()) abdFail();
      abdRow--;
    }
  }
}

void abdPush(int heading, int mm) {
  int previous = abdHeading;
  abdFace(heading);
  LbGripper.close();
  abdMoveMm(mm);
  LbGripper.open();
  abdMoveMm(-mm);
  abdFace(previous);
}

void abdTake(int heading, int mm) {
  int previous = abdHeading;
  abdFace(heading);
  LbGripper.open();
  abdMoveMm(mm);
  LbGripper.close();
  abdMoveMm(-mm);
  abdFace(previous);
}

void abdPlace(int heading, int mm) {
  int previous = abdHeading;
  abdFace(heading);
  abdMoveMm(mm);
  LbGripper.open();
  abdMoveMm(-mm);
  abdFace(previous);
}

// PROVISIONAL: locations S1/S3 are represented relative to (2,3).
// Actual reserve-side access and clearance must be checked on the board.
void abdTakeS3() {
  abdGoTo(2, 3);
  abdFace(4);
  LbGripper.open();
  abdMoveMm(ABD_RESERVE_APPROACH_MM);
  LbGripper.close();
  abdMoveMm(-ABD_RESERVE_APPROACH_MM);
}

void abdTakeS1() {
  abdGoTo(2, 3);
  abdFace(6);
  abdMoveMm(ABD_S1_LEFT_OFFSET_MM);
  abdFace(4);
  LbGripper.open();
  abdMoveMm(ABD_RESERVE_APPROACH_MM);
  LbGripper.close();
  abdMoveMm(-ABD_RESERVE_APPROACH_MM);
  abdFace(2);
  abdMoveMm(ABD_S1_LEFT_OFFSET_MM);
  // Pose is back at (2,3) only if this offset was accurately executed.
}

void abdExecuteMission() {
  // B4 Green: push original block into B diamond
  abdGoTo(1, 2);
  abdPush(7, ABD_PUSH_B_MM);

  // D3 Blue: push original block into D diamond
  abdPush(5, ABD_PUSH_D_MM);

  // S3 Blue -> D diamond
  abdTakeS3();
  abdGoTo(1, 2);
  abdPlace(5, ABD_PLACE_D_MM);

  // S1 Green -> B diamond
  abdTakeS1();
  abdGoTo(1, 2);
  abdPlace(7, ABD_PLACE_B_MM);

  // A2 Yellow -> CTRL
  abdGoTo(0, 1);
  abdTake(1, ABD_APPROACH_A_MM);
  abdGoTo(2, 2);
  abdPlace(3, ABD_PLACE_CTRL_MM);
  LbMotion.stopAndWait();
}

void loop() {
  if (abdMissionFinished) return;

  LbMission.begin(TB1A + TB1B);
  // Must physically start at junction (0,1) pointing south.
  // If competition requires CTRL start, this route MUST be remapped.
  abdCol = 0;
  abdRow = 1;
  abdHeading = 4;

  abdExecuteMission();
  abdMissionFinished = true;
  Leanbot.tone(1500, 200);
  LbMission.end();
}

