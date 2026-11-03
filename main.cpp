//  Generated Leanbot Arduino code
//    by Leanbot Blockly Editor 2026-02-14

#include <Leanbot.h>
// Motor command units: steps/second (LbMotion.runLR).
// These are initial test values, not RPM conversions.
const int TURN_STEPS_PER_SEC = 850;
const int MOVE_STEPS_PER_SEC = 1200;

// ================================================================
// MAP SURVEY / KHAO SAT TOAN BO 24 CANH CUA LUOI 4x4
// Survey only. Single main.cpp.
// ================================================================
const int SURVEY_SPEED_STEPS = 850;       // steps/second
const int SURVEY_MAX_EDGE_MM = 650;      // safety limit; calibrate on mat that
const int SURVEY_MAX_CURVE_MM = 1300;    // two segments over rounded corner
const unsigned long SURVEY_EDGE_TIMEOUT_MS = 15000UL;
const unsigned long SURVEY_CURVE_TIMEOUT_MS = 26000UL;
const unsigned long SURVEY_LINE_LOST_MS = 650UL;
const unsigned long SURVEY_EXIT_BLACK_MS = 60UL;
const int SURVEY_BLACK_SAMPLES = 3;


int line;
// Digital 4-sensor line tracking: uses calibrated black/white bitmask only.
// Corrections are initial tuning values; validate sensor orientation on hardware.
const float LINE_TURN_SMALL = 0.50f;
const float LINE_TURN_MEDIUM = 0.70f;
const float LINE_TURN_LARGE = 1.00f;
const unsigned long LINE_LOSS_HOLD_MS = 40;

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

void setup() {
  Serial.begin(115200);
  Leanbot.begin();
  LbIRLine.doManualCalibration(TB1A);
}


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
// MAP SURVEY: Jrc = r*4+c. Checkpoints Hrc / Vrc.
// J00, J03, J30 are rounded curves; J33 assumed rounded for the
// test route (verify physical corner). NEVER turn in place on curves.
// All 24 physical grid edges are covered by the route below.
// Survey-only firmware: no old mission or gripper actions.
// ================================================================
const uint8_t MAP_ROUTE[] = {
  15,14,13,14,10,6,2,1,2,3,7,6,5,1,0,4,5,9,13,12,8,4,8,9,10,11,7,11,15
};
const int MAP_ROUTE_SIZE = sizeof(MAP_ROUTE) / sizeof(MAP_ROUTE[0]);
bool mapCovered[24] = {false};
int mapCoveredCount = 0;
int surveyHeading = 3; // NORTH=0, EAST=1, SOUTH=2, WEST=3
int surveyCurrentNode = 15;
unsigned long surveyLostAt = 0;
int surveyLastTurn = 0;

bool surveyCurve(int node) {
  return node == 0 || node == 3 || node == 12 || node == 15;
}

int surveyDirection(int a, int b) {
  int ar=a/4, ac=a%4, br=b/4, bc=b%4;
  if (ar==br && bc==ac+1) return 1; // east
  if (ar==br && bc==ac-1) return 3; // west
  if (ac==bc && br==ar+1) return 2; // south
  if (ac==bc && br==ar-1) return 0; // north
  return -1;
}

void surveyNodePrint(int n) {
  Serial.print("J");
  Serial.print(n/4);
  Serial.print(n%4);
  Serial.print(" (row=");
  Serial.print(n/4);
  Serial.print(",col=");
  Serial.print(n%4);
  Serial.print(")");
}

void surveyMarkerPrint(int a, int b) {
  Serial.print(a/4 == b/4 ? 'H' : 'V');
  if (a/4 == b/4) {
    Serial.print(a/4);
    Serial.print(min(a%4,b%4));
  } else {
    Serial.print(min(a/4,b/4));
    Serial.print(a%4);
  }
}

int surveyEdgeIndex(int a, int b) {
  if (a/4 == b/4 && abs(a-b)==1) return (a/4)*3 + min(a%4,b%4);
  if (a%4 == b%4 && abs(a-b)==4) return 12 + min(a/4,b/4)*4 + a%4;
  return -1;
}

void surveyMarkCovered(int a, int b) {
  int e=surveyEdgeIndex(a,b);
  if (e>=0 && !mapCovered[e]) {
    mapCovered[e]=true;
    ++mapCoveredCount;
  }
}

void surveyTurnToward(int wanted) {
  int diff=(wanted-surveyHeading+4)%4;
  if (diff==1) turnRight(90);
  else if (diff==2) turnRight(180);
  else if (diff==3) turnLeft(90);
  surveyHeading=wanted;
}

// Restricted-speed line follower for survey (no 2000-step spikes).
void surveyFollowMotor() {
  if (line==0) {
    if (!surveyLostAt) surveyLostAt=millis();
    if ((unsigned long)(millis()-surveyLostAt)<LINE_LOSS_HOLD_MS) {
      LbMotion.runLR(SURVEY_SPEED_STEPS,SURVEY_SPEED_STEPS);
    } else if (surveyLastTurn<0) {
      LbMotion.runLR(0,SURVEY_SPEED_STEPS);
    } else if (surveyLastTurn>0) {
      LbMotion.runLR(SURVEY_SPEED_STEPS,0);
    } else {
      LbMotion.runLR(SURVEY_SPEED_STEPS,SURVEY_SPEED_STEPS);
    }
    return;
  }
  surveyLostAt=0;
  float correction=lineTurnCorrection(line);
  if (line!=15) {
    if (correction<0) surveyLastTurn=-1;
    else if (correction>0) surveyLastTurn=1;
  }
  int l=SURVEY_SPEED_STEPS, r=SURVEY_SPEED_STEPS;
  if (correction<0) l=(int)(SURVEY_SPEED_STEPS*(1.0f+correction));
  else if (correction>0) r=(int)(SURVEY_SPEED_STEPS*(1.0f-correction));
  LbMotion.runLR(l,r);
}

// One leg or two legs joined by a physical curve.
// Serial positions are PREDICTED. '1111' detections are OBSERVED.
// Marker sequence: straight = midpoint + destination junction;
// curve = midpoint-in + midpoint-out + destination junction.
// Last edge ending at J33 is distance-terminated (no junction at corner).
bool surveyTraverse(int from, int via, int to, bool isCurve) {
  int firstTo=isCurve?via:to;
  int wanted=surveyDirection(from,firstTo);
  if (wanted<0) {
    Serial.println("ERROR INVALID_EDGE");
    return false;
  }
  if (!surveyCurve(from)) surveyTurnToward(wanted);
  else if (surveyHeading!=wanted) {
    Serial.println("ERROR CORNER_HEADING (cannot spin at curve)");
    return false;
  }

  const bool lastCorner=(to==15 && !isCurve);
  const int expected=isCurve?3:(lastCorner?1:2);
  const int distanceLimit=isCurve?SURVEY_MAX_CURVE_MM:SURVEY_MAX_EDGE_MM;
  const unsigned long timeLimit=isCurve?SURVEY_CURVE_TIMEOUT_MS:SURVEY_EDGE_TIMEOUT_MS;

  Serial.print("EDGE_START from=");
  surveyNodePrint(from);
  if (isCurve) {
    Serial.print(" via_curve=");
    surveyNodePrint(via);
  }
  Serial.print(" to=");
  surveyNodePrint(to);
  Serial.print(" expected_1111=");
  Serial.print(expected);
  Serial.print(" dir=");
  Serial.println(wanted);

  int markerCount=0, blackStreak=0;
  bool locked=true;  // do not count the departure node as a new marker
  unsigned long whiteStart=0, started=millis(), lastLineSeen=started;
  long startDistance=LbMotion.getDistanceMm(), firstMarkAt=-1;
  unsigned long lastRgbUpdate=0;

  while (true) {
    LbIRLine.read();
    line=LbIRLine.value();
    unsigned long now=millis();
    long dist=abs(LbMotion.getDistanceMm()-startDistance);

    if (line!=0) lastLineSeen=now;
    if ((unsigned long)(now-lastLineSeen)>SURVEY_LINE_LOST_MS ||
        (unsigned long)(now-started)>timeLimit || dist>distanceLimit) {
      LbMotion.stopAndWait();
      Serial.print("ERROR SENSOR_LOSS_OR_TIMEOUT from=");
      surveyNodePrint(from);
      Serial.print(" to=");
      surveyNodePrint(to);
      Serial.print(" read_1111=");
      Serial.print(markerCount);
      Serial.print(" odom_mm=");
      Serial.println(dist);
      return false;
    }

    surveyFollowMotor();
    if ((unsigned long)(now-lastRgbUpdate)>=40UL) {
      LbRGB.clear();
      LbIRLine.displayOnRGB(0x00ff00);
      lastRgbUpdate=now;
    }

    if (line==15) {
      whiteStart=0;
      if (blackStreak<SURVEY_BLACK_SAMPLES) ++blackStreak;
      if (!locked && blackStreak>=SURVEY_BLACK_SAMPLES) {
        locked=true;
        ++markerCount;
        if (markerCount==1) firstMarkAt=dist;
        Serial.print("MARKER observed_1111 #");
        Serial.print(markerCount);
        Serial.print(" odom_mm=");
        Serial.print(dist);
        Serial.print(" expected=");
        if (!isCurve || markerCount==1) {
          surveyMarkerPrint(from,firstTo);
        } else if (markerCount==2) {
          surveyMarkerPrint(via,to);
        } else {
          surveyNodePrint(to);
        }
        Serial.println();
        if (markerCount>expected) {
          LbMotion.stopAndWait();
          Serial.println("ERROR TOO_MANY_MARKERS");
          return false;
        }
        if (!lastCorner && markerCount==expected) {
          LbMotion.stopAndWait();
          // The sensor is ahead of the wheel axle: center at final junction.
          forwardMm(10,SURVEY_SPEED_STEPS);
          break;
        }
      }
    } else {
      blackStreak=0;
      if (locked) {
        if (!whiteStart) whiteStart=now;
        if ((unsigned long)(now-whiteStart)>=SURVEY_EXIT_BLACK_MS) {
          locked=false;
        }
      }
    }
    // The last corner J33 has no true junction mark. Travel approximately
    // twice the distance to the midpoint marker, then stop on odometry.
    if (lastCorner && markerCount==1 && firstMarkAt>20 &&
        dist>=2*firstMarkAt) {
      LbMotion.stopAndWait();
      Serial.println("CORNER_END J33 is ODOMETRY_INFERRED");
      break;
    }
  }

  surveyMarkCovered(from,firstTo);
  if (isCurve) surveyMarkCovered(via,to);
  surveyCurrentNode=to; // estimated by map, NOT a measured global position
  surveyHeading=isCurve?surveyDirection(via,to):wanted;
  Serial.print("NODE_PREDICTED ");
  surveyNodePrint(surveyCurrentNode);
  Serial.print(" heading_NESW=");
  Serial.print(surveyHeading);
  Serial.print(" observed_1111=");
  Serial.print(markerCount);
  Serial.print(" covered_edges=");
  Serial.print(mapCoveredCount);
  Serial.println("/24");
  return true;
}

void surveyAllLines() {
  LbMotion.stopAndWait();
  Serial.println("MAP_SURVEY_START: place at J33 facing WEST along bottom line");
  Serial.println("REMOVE all blocks; check path and clearance before running");
  Serial.println("Coordinates are graph indices, NOT absolute x/y measurements");
  Serial.println("Calibrate expected FULL_BLACK events and distances on real field");
  LbMission.begin(TB1A + TB1B);
  bool ok=true;
  for (int i=0;i<MAP_ROUTE_SIZE-1;) {
    int from=MAP_ROUTE[i];
    int to=MAP_ROUTE[i+1];
    bool curved=surveyCurve(to) && i+2<MAP_ROUTE_SIZE;
    if (curved) {
      int next=MAP_ROUTE[i+2];
      // At curves, go around in one continuous line-following movement.
      if (surveyDirection(to,next)<0 || next==from) {
        Serial.println("ERROR INVALID_CURVE_TRANSITION");
        ok=false;
        break;
      }
      ok=surveyTraverse(from,to,next,true);
      i+=2;
    } else {
      ok=surveyTraverse(from,255,to,false);
      ++i;
    }
    if (!ok) break;
  }
  LbMotion.stopAndWait();
  Serial.print(ok && mapCoveredCount==24 ?
               "MAP_SURVEY_DONE covered=" : "MAP_SURVEY_STOPPED covered=");
  Serial.print(mapCoveredCount);
  Serial.println("/24");
  Leanbot.tone(ok && mapCoveredCount==24 ? 1500 : 500, 400);
  LbMission.end();
}
void loop() {
  static bool surveyDone = false;
  if (surveyDone) return;
  surveyDone = true;
  surveyAllLines();
}
