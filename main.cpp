//  Generated Leanbot Arduino code
//    by Leanbot Blockly Editor 2026-02-14

#include <Leanbot.h>
#include <Arduino_APDS9960.h>
#include <string.h>
// Motor command units: steps/second (LbMotion.runLR).
// These are initial test values, not RPM conversions.
const int TURN_STEPS_PER_SEC = 850;
const int MOVE_STEPS_PER_SEC = 1200;

// ================================================================
// MAP SURVEY / KHAO SAT TOAN BO 24 CANH CUA LUOI 4x4
// Survey only. Single main.cpp.
// ================================================================
const int SURVEY_SPEED_STEPS = 1200;       // steps/second
const int SURVEY_MAX_EDGE_MM = 650;      // safety limit; calibrate on mat that
const int SURVEY_MAX_CURVE_MM = 1300;    // two segments over rounded corner
const unsigned long SURVEY_EDGE_TIMEOUT_MS = 15000UL;
const unsigned long SURVEY_CURVE_TIMEOUT_MS = 26000UL;
const unsigned long SURVEY_LINE_LOST_MS = 650UL;
const unsigned long SURVEY_EXIT_BLACK_MS = 60UL;
const int SURVEY_BLACK_SAMPLES = 3;
const uint16_t MARKER_BEEP_HZ = 1050;   // low pitch: H/V checkpoint
const uint16_t NODE_BEEP_HZ = 1900;     // high pitch: junction reached
const uint16_t MARKER_BEEP_MS = 25;     // very short while line-following
const uint16_t NODE_BEEP_MS = 80;
const unsigned long RGB_REFRESH_MS = 35UL;

// ================================================================
// STORAGE BLOCKS: edit these SIX lines before a match.
// G=Green, B=Blue, P=Purple, none=empty storage location.
// Example placement only (S1/S3/S5); NOT a recommended physical layout.
// ================================================================
#define STORAGE_S1 "G"
#define STORAGE_S2 "none"
#define STORAGE_S3 "B"
#define STORAGE_S4 "none"
#define STORAGE_S5 "P"
#define STORAGE_S6 "none"

enum StorageColor : uint8_t {
  STORAGE_EMPTY = 0,
  STORAGE_GREEN = 1,
  STORAGE_BLUE = 2,
  STORAGE_PURPLE = 3,
  STORAGE_INVALID = 255
};

struct StorageState {
  uint8_t color; // see StorageColor
  bool used;     // once picked, do not pick the same block again
};

StorageState storage[6];
const char* const storageSettings[6] = {
  STORAGE_S1, STORAGE_S2, STORAGE_S3,
  STORAGE_S4, STORAGE_S5, STORAGE_S6
};

uint8_t parseStorageColor(const char* label) {
  if (strcmp(label, "none") == 0) return STORAGE_EMPTY;
  if (strcmp(label, "G") == 0) return STORAGE_GREEN;
  if (strcmp(label, "B") == 0) return STORAGE_BLUE;
  if (strcmp(label, "P") == 0) return STORAGE_PURPLE;
  return STORAGE_INVALID;
}

void initializeStorage() {
  uint8_t countG=0, countB=0, countP=0;
  Serial.println(F("STORAGE_CONFIG_BEGIN"));
  for (uint8_t i=0; i<6; ++i) {
    storage[i].color=parseStorageColor(storageSettings[i]);
    storage[i].used=false;
    Serial.print('S');
    Serial.print(i+1);
    Serial.print('=');
    Serial.println(storageSettings[i]);
    if (storage[i].color==STORAGE_GREEN) ++countG;
    else if (storage[i].color==STORAGE_BLUE) ++countB;
    else if (storage[i].color==STORAGE_PURPLE) ++countP;
    else if (storage[i].color==STORAGE_INVALID)
      Serial.println(F("ERROR STORAGE_INVALID_COLOR"));
  }
  if (countG!=1 || countB!=1 || countP!=1)
    Serial.println(F("WARNING STORAGE_EXPECT_ONE_EACH_G_B_P"));
  Serial.println(F("STORAGE_CONFIG_END"));
}

// Returns zero-based slot index, or -1 if no unused storage of color.
// This is inventory lookup only; no location-to-node mapping yet.
int8_t findAvailableStorage(uint8_t color) {
  for(uint8_t i=0; i<6; ++i)
    if(storage[i].color==color && !storage[i].used) return (int8_t)i;
  return -1;
}

void markStorageUsed(uint8_t index) {
  if(index<6 && storage[index].color!=STORAGE_EMPTY)
    storage[index].used=true;
}

// ================================================================
// USER CONFIG: seven pollution positions, colors learned ON THE RUN.
// Edit only these seven locations and the six STORAGE_Sn definitions.
// The seven example coordinates below are NOT competition positions.
// ================================================================
#define POLLUTION_1 "A2"
#define POLLUTION_2 "B4"
#define POLLUTION_3 "D3"
#define POLLUTION_4 "C1"
#define POLLUTION_5 "E2"
#define POLLUTION_6 "F3"
#define POLLUTION_7 "H1"

enum DetectedColor : uint8_t {
  COL_UNKNOWN=0, COL_RED=1, COL_YELLOW=2, COL_GREEN=3,
  COL_BLUE=4, COL_PURPLE=5
};
enum PollutionStatus : uint8_t {
  POL_PENDING=0, POL_CONTAINED=1, POL_NEUTRALIZED=2,
  POL_RETURNED=3, POL_UNCERTAIN=4
};
enum AnchorType : uint8_t { ANCHOR_NODE=0, ANCHOR_MARKER=1 };
enum ApproachSide : int8_t { LEFT_45=-1, STRAIGHT=0, RIGHT_45=1 };

struct ActionPoint {
  uint8_t type;
  uint8_t node;       // reference junction
  uint8_t next;       // next junction across marker (only for marker)
  uint8_t heading;    // cardinal facing before object approach
  int8_t side;        // -1 = left45, +1 = right45, 0 = straight
};
struct PollutionItem {
  uint8_t location;   // zone*4 + (slot-1), 0..31
  uint8_t color;
  uint8_t status;
};
PollutionItem pollution[7];
const char* const pollutionSettings[7] = {
  POLLUTION_1, POLLUTION_2, POLLUTION_3, POLLUTION_4,
  POLLUTION_5, POLLUTION_6, POLLUTION_7
};
bool missionConfigValid=true;

// A,B,C,D,E,F,G,H in the 3x3 board:
// A=(0,0), B=(1,0), C=(0,1), D=(2,0),
// E=(1,1), F=(0,2), G=(2,1), H=(1,2).
uint8_t zoneRow(uint8_t z) {
  switch(z) {
    case 0: case 2: case 5: return 0; // A,C,F
    case 1: case 4: case 7: return 1; // B,E,H
    default: return 2;               // D,G
  }
}
uint8_t zoneCol(uint8_t z) {
  switch(z) {
    case 0: case 1: case 3: return 0;
    case 2: case 4: case 6: return 1;
    default: return 2;
  }
}

// Location slots 1(NW),2(SW),3(NE),4(SE) of each zone.
// Most are approached diagonally from the adjacent grid junction.
// At the 3 rounded OUTER corners, use the nearest line MARKER
// instead of attempting to stop at the curve itself.
ActionPoint pollutionPoint(uint8_t location) {
  uint8_t z=location/4,slot=location%4+1;
  uint8_t r=zoneRow(z),c=zoneCol(z);
  ActionPoint p={ANCHOR_NODE, (uint8_t)(4*r+c), 255, 1, RIGHT_45};
  if(slot==1) { // NW pollution position; diagonally SE from top-left
    p.node=(uint8_t)(4*r+c);p.heading=1;p.side=RIGHT_45;
  } else if(slot==2) { // SW pollution; NE from bottom-left
    p.node=(uint8_t)(4*(r+1)+c);p.heading=0;p.side=RIGHT_45;
  } else if(slot==3) { // NE pollution; SW from top-right
    p.node=(uint8_t)(4*r+c+1);p.heading=3;p.side=LEFT_45;
  } else { // SE pollution; NW from bottom-right
    p.node=(uint8_t)(4*(r+1)+c+1);p.heading=0;p.side=LEFT_45;
  }
  // A1 cannot use rounded J00. H00 approached from J01, heading WEST.
  if(z==0 && slot==1) {
    p={ANCHOR_MARKER,1,0,3,LEFT_45};
  }
  // F3 cannot use rounded J03. H02 approached from J02, heading EAST.
  if(z==5 && slot==3) {
    p={ANCHOR_MARKER,2,3,1,RIGHT_45};
  }
  // D2 cannot use rounded J30. H30 approached from J31, heading WEST.
  if(z==3 && slot==2) {
    p={ANCHOR_MARKER,13,12,3,RIGHT_45};
  }
  return p;
}

// Slot mapping from the field diagram:
// S1(bottom H31), S3(bottom J32), S5(bottom H32).
// S2(right V13), S4(right J23), S6(right V23).
// Distance/approach angle need hardware validation for real block offsets.
ActionPoint storagePoint(uint8_t zeroBased) {
  switch(zeroBased) {
    case 0: return {ANCHOR_MARKER,14,13,3,LEFT_45};  // S1
    case 1: return {ANCHOR_MARKER,7,11,2,LEFT_45};   // S2
    case 2: return {ANCHOR_NODE,14,255,2,STRAIGHT};  // S3
    case 3: return {ANCHOR_NODE,11,255,1,STRAIGHT};  // S4
    case 4: return {ANCHOR_MARKER,14,15,1,RIGHT_45}; // S5
    default:return {ANCHOR_MARKER,11,15,2,LEFT_45};  // S6
  }
}

bool initializePollution() {
  Serial.println(F("POLLUTION_CONFIG_BEGIN"));
  bool valid=true;
  uint32_t used=0;
  for(uint8_t i=0;i<7;i++) {
    const char* p=pollutionSettings[i];
    bool itemOK=(p[0]>='A' && p[0]<='H' &&
                 p[1]>='1' && p[1]<='4' && p[2]=='\0');
    if(!itemOK) {
      valid=false;
      pollution[i].location=255;
      Serial.println(F("ERROR POLLUTION_LOCATION_FORMAT"));
    } else {
      uint8_t value=(uint8_t)((p[0]-'A')*4+(p[1]-'1'));
      pollution[i].location=value;
      if(used & (1UL<<value)) {
        valid=false;Serial.println(F("ERROR DUPLICATE_POLLUTION_LOCATION"));
      }
      used|=(1UL<<value);
    }
    pollution[i].color=COL_UNKNOWN;
    pollution[i].status=POL_PENDING;
    Serial.print('P');Serial.print(i+1);Serial.print('=');
    Serial.println(p);
  }
  Serial.println(F("POLLUTION_CONFIG_END"));
  return valid;
}





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


// ================================================================
// APDS9960: 5 colors with Leanbot RGB illumination (no String).
// Sensor must see the object's surface at scan distance.
// UNKNOWN/MYSTERY cannot be reliably distinguished by this classifier.
// ================================================================
bool apdsReady=false;
const float COLOR_GAIN_R=1.0f,COLOR_GAIN_G=1.0f,COLOR_GAIN_B=1.0f;
const float COLOR_MIN_SIGNAL=10.0f;
const unsigned long COLOR_TIMEOUT_MS=350UL;
int16_t colorR=0,colorG=0,colorB=0;

bool readIlluminated(int &r,int &g,int &b,CRGB led) {
  LbRGB.fillColor(led);LbRGB.show();LbDelay(20);
  if(APDS.colorAvailable()) {
    int orr,ogg,obb;APDS.readColor(orr,ogg,obb);
  }
  unsigned long started=millis();
  while(!APDS.colorAvailable()) {
    if((unsigned long)(millis()-started)>COLOR_TIMEOUT_MS) return false;
  }
  APDS.readColor(r,g,b);
  return true;
}
uint8_t detectColorOnce() {
  if(!apdsReady) return COL_UNKNOWN;
  int rr=0,gg=0,bb=0,tmpR=0,tmpG=0,tmpB=0;
  int darkR=0,darkG=0,darkB=0;
  int oldBrightness=LbRGB.getBrightness();
  LbRGB.setBrightness(255);
  bool ok=readIlluminated(rr,tmpG,tmpB,CRGB::Red);
  if(ok) ok=readIlluminated(tmpR,gg,tmpB,CRGB::Lime);
  if(ok) ok=readIlluminated(tmpR,tmpG,bb,CRGB::Blue);
  if(ok) ok=readIlluminated(darkR,darkG,darkB,CRGB::Black);
  LbRGB.setBrightness(oldBrightness);
  LbRGB.clear();LbRGB.show();
  if(!ok) return COL_UNKNOWN;
  float red=(rr>darkR ? rr-darkR : 0)*COLOR_GAIN_R;
  float green=(gg>darkG ? gg-darkG : 0)*COLOR_GAIN_G;
  float blue=(bb>darkB ? bb-darkB : 0)*COLOR_GAIN_B;
  colorR=(int16_t)red;colorG=(int16_t)green;colorB=(int16_t)blue;
  if(red<COLOR_MIN_SIGNAL && green<COLOR_MIN_SIGNAL &&
     blue<COLOR_MIN_SIGNAL) return COL_UNKNOWN;
  if(red>=COLOR_MIN_SIGNAL && green>=COLOR_MIN_SIGNAL &&
     red>1.45f*blue && green>1.45f*blue &&
     red<2.0f*green && green<2.0f*red) return COL_YELLOW;
  if(red>=COLOR_MIN_SIGNAL && blue>=COLOR_MIN_SIGNAL &&
     red>1.45f*green && blue>1.45f*green &&
     red<2.2f*blue && blue<2.2f*red) return COL_PURPLE;
  if(green>=COLOR_MIN_SIGNAL && green>1.30f*red &&
     green>1.30f*blue) return COL_GREEN;
  if(blue>=COLOR_MIN_SIGNAL && blue>1.30f*red &&
     blue>1.30f*green) return COL_BLUE;
  if(red>=COLOR_MIN_SIGNAL && red>1.30f*green &&
     red>1.30f*blue) return COL_RED;
  return COL_UNKNOWN;
}

const __FlashStringHelper* colorName(uint8_t c) {
  switch(c) {
    case COL_RED:return F("RED");
    case COL_YELLOW:return F("YELLOW");
    case COL_GREEN:return F("GREEN");
    case COL_BLUE:return F("BLUE");
    case COL_PURPLE:return F("PURPLE");
    default:return F("UNKNOWN_OR_MYSTERY");
  }
}

uint8_t detectColorStable() {
  uint8_t a=detectColorOnce();LbDelay(40);
  uint8_t b=detectColorOnce();
  if(a!=COL_UNKNOWN && a==b) return a;
  LbDelay(40);
  uint8_t c=detectColorOnce();
  if(a!=COL_UNKNOWN && a==c) return a;
  if(b!=COL_UNKNOWN && b==c) return b;
  return COL_UNKNOWN;
}

bool validateStorageConfiguration() {
  uint8_t green=0,blue=0,purple=0;
  bool valid=true;
  for(uint8_t i=0;i<6;i++) {
    if(storage[i].color==STORAGE_GREEN) ++green;
    else if(storage[i].color==STORAGE_BLUE) ++blue;
    else if(storage[i].color==STORAGE_PURPLE) ++purple;
    else if(storage[i].color!=STORAGE_EMPTY) valid=false;
  }
  if(green!=1 || blue!=1 || purple!=1) valid=false;
  if(!valid) Serial.println(F("ERROR STORAGE_MUST_CONTAIN_ONE_G_ONE_B_ONE_P"));
  return valid;
}

void setup() {
  Serial.begin(115200);
  Leanbot.begin();
  apdsReady=APDS.begin();
  if(!apdsReady) Serial.println(F("ERROR APDS9960_INIT_FAILED"));
  LbIRLine.doManualCalibration(TB1A);
  initializeStorage();
  missionConfigValid = initializePollution() && validateStorageConfiguration();
  if(!apdsReady) missionConfigValid=false;
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
// HEADING-AWARE DIJKSTRA: grid 4x4, 16 geometric points, 64 states.
// Goals are genuine navigable junctions only, not curved corners.
// Rounded corners J00/J03/J30 are mandatory uninterrupted turns.
// J33 is the initial staging point, facing WEST. Other tests do not
// deliberately navigate back to J33 (corner not reliably localized).
//
// Static arrays: safe for 2 KB RAM microcontrollers. No malloc/String.
// ================================================================
#define STATE_COUNT 64
#define RANDOM_TEST_COUNT 5
#define INVALID_STATE 255
#define EDGE_COST 300U
#define TURN_90_COST 120U
#define TURN_180_COST 270U

// Physical node id: J(row)(col) -> row*4+col.
const uint8_t START_NODE = 15; // J33
const uint8_t FIRST_GOAL = 4; // J10, NOT integer node index 10
const uint8_t START_HEADING = 3; // WEST

uint8_t currentNode = START_NODE;
uint8_t currentHeading = START_HEADING;
unsigned long surveyLostAt = 0;
int8_t surveyLastTurn = 0;

// Fixed-memory Dijkstra working set (~450 bytes).
uint16_t pathCost[STATE_COUNT];
uint8_t pathParent[STATE_COUNT];
uint8_t pathViaCurve[STATE_COUNT];
uint8_t settled[STATE_COUNT];
uint8_t routeStates[STATE_COUNT];
uint8_t routeVias[STATE_COUNT];
uint8_t routeLength = 0;

bool isCurveNode(uint8_t n) {
  return n == 0 || n == 3 || n == 12;
}

bool isGoalNode(uint8_t n) {
  return n < 16 && !isCurveNode(n) && n != 15;
}

int8_t gridNeighbor(uint8_t n, uint8_t dir) {
  const uint8_t row=n/4, col=n%4;
  if (dir==0 && row>0) return n-4; // NORTH
  if (dir==1 && col<3) return n+1; // EAST
  if (dir==2 && row<3) return n+4; // SOUTH
  if (dir==3 && col>0) return n-1; // WEST
  return -1;
}

// Return a traversable route segment: one straight edge, or a
// two-edge continuous curve. Curves cannot be endpoints or spin points.
bool planHop(uint8_t from, uint8_t dir,
             uint8_t &dest, uint8_t &endHeading, uint8_t &via) {
  if (from==15 && dir!=3) return false; // initial J33 exit WEST only
  int8_t adjacent=gridNeighbor(from,dir);
  if (adjacent<0 || adjacent==15) return false;
  via=INVALID_STATE;
  dest=(uint8_t)adjacent;
  endHeading=dir;
  if (isCurveNode(dest)) {
    via=dest;
    int8_t exitNode=-1;
    for (uint8_t d=0;d<4;d++) {
      int8_t k=gridNeighbor(via,d);
      if (k>=0 && k!=from) {
        if (exitNode>=0) return false; // not a true two-leg curve
        exitNode=k;
        endHeading=d;
      }
    }
    if (exitNode<0 || exitNode==15) return false;
    dest=(uint8_t)exitNode;
  }
  return isGoalNode(dest);
}

void printNode(uint8_t n) {
  Serial.print('J');
  Serial.print(n/4);
  Serial.print(n%4);
  Serial.print(F("(r="));
  Serial.print(n/4);
  Serial.print(F(",c="));
  Serial.print(n%4);
  Serial.print(')');
}

void printMarker(uint8_t a, uint8_t b) {
  Serial.print(a/4==b/4 ? 'H' : 'V');
  if (a/4==b/4) {
    Serial.print(a/4);
    Serial.print(min(a%4,b%4));
  } else {
    Serial.print(min(a/4,b/4));
    Serial.print(a%4);
  }
}

int8_t gridDirection(uint8_t from,uint8_t to) {
  for(uint8_t d=0;d<4;d++) if(gridNeighbor(from,d)==to) return (int8_t)d;
  return -1;
}

uint16_t turnPenalty(uint8_t from,uint8_t to) {
  uint8_t d=(to+4-from)%4;
  if(d==0) return 0;
  if(d==2) return TURN_180_COST;
  return TURN_90_COST;
}

bool findPath(uint8_t start, uint8_t heading, uint8_t goal) {
  routeLength=0;
  if(start>=16 || heading>=4 || !isGoalNode(goal)) return false;
  for(uint8_t i=0;i<STATE_COUNT;i++) {
    pathCost[i]=65535U;
    pathParent[i]=INVALID_STATE;
    pathViaCurve[i]=INVALID_STATE;
    settled[i]=0;
  }
  uint8_t startState=(uint8_t)(start*4+heading);
  pathCost[startState]=0;
  for(uint8_t iter=0;iter<STATE_COUNT;iter++) {
    uint8_t best=INVALID_STATE;
    uint16_t bestCost=65535U;
    for(uint8_t i=0;i<STATE_COUNT;i++) {
      if(!settled[i] && pathCost[i]<bestCost) {
        best=i;
        bestCost=pathCost[i];
      }
    }
    if(best==INVALID_STATE) break;
    settled[best]=1;
    uint8_t node=best/4, facing=best%4;
    if(node==goal) break;
    for(uint8_t dir=0;dir<4;dir++) {
      uint8_t dest,ending,via;
      if(!planHop(node,dir,dest,ending,via)) continue;
      uint8_t next=(uint8_t)(dest*4+ending);
      uint16_t cost=(uint16_t)(EDGE_COST*(via==INVALID_STATE?1U:2U));
      cost=(uint16_t)(cost+turnPenalty(facing,dir));
      if(cost>65535U-bestCost) continue;
      uint16_t candidate=(uint16_t)(bestCost+cost);
      if(candidate<pathCost[next]) {
        pathCost[next]=candidate;
        pathParent[next]=best;
        pathViaCurve[next]=via;
      }
    }
  }

  uint8_t goalState=INVALID_STATE;
  uint16_t minCost=65535U;
  for(uint8_t h=0;h<4;h++) {
    uint8_t st=(uint8_t)(goal*4+h);
    if(pathCost[st]<minCost) {
      minCost=pathCost[st];goalState=st;
    }
  }
  if(goalState==INVALID_STATE) return false;

  // Backtrack into fixed arrays, then reverse to start -> goal.
  uint8_t p=goalState;
  while(true) {
    if(routeLength>=STATE_COUNT) return false;
    routeStates[routeLength]=p;
    routeVias[routeLength]=pathViaCurve[p];
    routeLength++;
    if(p==startState) break;
    p=pathParent[p];
    if(p==INVALID_STATE) return false;
  }
  for(uint8_t i=0;i<routeLength/2;i++) {
    uint8_t j=(uint8_t)(routeLength-1-i);
    uint8_t tmp=routeStates[i];routeStates[i]=routeStates[j];routeStates[j]=tmp;
    tmp=routeVias[i];routeVias[i]=routeVias[j];routeVias[j]=tmp;
  }
  Serial.print(F("PLAN start="));printNode(start);
  Serial.print(F(" heading="));Serial.print(heading);
  Serial.print(F(" goal="));printNode(goal);
  Serial.print(F(" cost="));Serial.println(minCost);
  for(uint8_t i=0;i<routeLength;i++) {
    Serial.print(F(" ROUTE_STEP "));Serial.print(i);
    Serial.print(' ');printNode(routeStates[i]/4);
    if(routeVias[i]!=INVALID_STATE) {
      Serial.print(F(" via_curve="));printNode(routeVias[i]);
    }
    Serial.println();
  }
  return true;
}

void turnToward(uint8_t wanted) {
  uint8_t d=(wanted+4-currentHeading)%4;
  if(d==1) turnRight(90);
  else if(d==2) turnRight(180);
  else if(d==3) turnLeft(90);
  currentHeading=wanted;
}

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

// Traverse one planned hop, optionally continuing through a rounded
// corner without stop/turn. Full-black observations are validated against
// the edge pattern, not confused with the graph node identifiers.
bool traverseHop(uint8_t dest,uint8_t via) {
  uint8_t from=currentNode;
  bool curved=(via!=INVALID_STATE);
  int8_t firstDir=gridDirection(from,curved?via:dest);
  int8_t finalDir=curved?gridDirection(via,dest):firstDir;
  if(firstDir<0 || finalDir<0) {
    Serial.println(F("ERROR INVALID_HOP"));
    return false;
  }
  if(from==15) {
    if(currentHeading!=(uint8_t)firstDir) {
      Serial.println(F("ERROR J33_START_HEADING"));
      return false;
    }
  } else {
    turnToward((uint8_t)firstDir);
  }

  // Each straight edge has a middle bar + destination full-black.
  // Curved two-edge hop has: bar 1, bar 2, destination junction.
  uint8_t expected=curved?3:2;
  const uint16_t maxDist=curved?SURVEY_MAX_CURVE_MM:SURVEY_MAX_EDGE_MM;
  const unsigned long maxMs=curved?SURVEY_CURVE_TIMEOUT_MS:SURVEY_EDGE_TIMEOUT_MS;
  Serial.print(F("EDGE_START from="));printNode(from);
  if(curved){Serial.print(F(" via_curve="));printNode(via);}
  Serial.print(F(" to="));printNode(dest);
  Serial.print(F(" expected_1111="));Serial.println(expected);

  uint8_t observed=0,blackStreak=0;
  bool lockBlack=true; // do not recount current junction
  unsigned long whiteAt=0,startAt=millis(),lastSeen=startAt;
  long startMm=LbMotion.getDistanceMm(),previousMarkerMm=-1;
  surveyLostAt=0;
  surveyLastTurn=0;

  while(true) {
    LbIRLine.read();
    line=LbIRLine.value();
    unsigned long now=millis();
    long travel=LbMotion.getDistanceMm()-startMm;
    if(travel<0) travel=-travel;
    if(line!=0) lastSeen=now;
    if((unsigned long)(now-lastSeen)>SURVEY_LINE_LOST_MS ||
       (unsigned long)(now-startAt)>maxMs || travel>maxDist) {
      LbMotion.stopAndWait();
      Serial.print(F("ERROR LOST_LINE_TIMEOUT_OR_DISTANCE mm="));
      Serial.print(travel);
      Serial.print(F(" marker_count="));Serial.println(observed);
      return false;
    }
    surveyFollowMotor();

    // Show the 4 IR line-sensor states on onboard RGB LEDs.
    // Refresh periodically rather than every control iteration.
    static unsigned long lastRgbAt = 0;
    if ((unsigned long)(now-lastRgbAt) >= RGB_REFRESH_MS) {
      lastRgbAt = now;
      LbRGB.clear();
      LbIRLine.displayOnRGB(0x00ff00);
      LbRGB.show();
    }
    if(line==15) {
      whiteAt=0;
      if(blackStreak<SURVEY_BLACK_SAMPLES) blackStreak++;
      if(!lockBlack && blackStreak>=SURVEY_BLACK_SAMPLES) {
        lockBlack=true;
        observed++;
        Serial.print(F("MARKER #"));Serial.print(observed);
        Serial.print(F(" odom_mm="));Serial.print(travel);
        Serial.print(F(" expected="));
        if(observed==expected) printNode(dest);
        else if(curved && observed==2) printMarker(via,dest);
        else printMarker(from,curved?via:dest);
        if(previousMarkerMm>=0) {
          long gap=travel-previousMarkerMm;
          Serial.print(F(" gap_mm="));Serial.print(gap);
          // Earlier survey found ~170 mm between bar and following
          // junction. Between two curved-edge bars the gap is longer.
          if((!curved || observed==3) && (gap<105 || gap>235)) {
            LbMotion.stopAndWait();
            Serial.println(F(" ERROR MARKER_SPACING"));
            return false;
          }
        }
        Serial.println();
        previousMarkerMm=travel;
        if(observed>expected) {
          LbMotion.stopAndWait();
          Serial.println(F("ERROR EXTRA_1111"));
          return false;
        }
        // Final full-black event of a hop is a navigation NODE.
        // Other events are intermediate MARKER bars.
        if(observed==expected) {
          LbMotion.stopAndWait();
          Leanbot.tone(NODE_BEEP_HZ, NODE_BEEP_MS);
        } else {
          Leanbot.tone(MARKER_BEEP_HZ, MARKER_BEEP_MS);
        }
        if(observed==expected) {
          LbMotion.stopAndWait();
          forwardMm(10,SURVEY_SPEED_STEPS); // sensor -> axle offset
          break;
        }
      }
    } else {
      blackStreak=0;
      if(lockBlack) {
        if(!whiteAt) whiteAt=now;
        if((unsigned long)(now-whiteAt)>=SURVEY_EXIT_BLACK_MS) lockBlack=false;
      }
    }
  }

  currentNode=dest;
  currentHeading=(uint8_t)finalDir;
  Serial.print(F("NODE_PREDICTED "));printNode(currentNode);
  Serial.print(F(" heading_NESW="));Serial.println(currentHeading);
  return true;
}

bool navigateTo(uint8_t goal) {
  if(!findPath(currentNode,currentHeading,goal)) {
    Serial.println(F("ERROR NO_PATH"));
    return false;
  }
  // Planner output is not recomputed during a single trip. If a
  // checkpoint verification fails, stop instead of extrapolating.
  for(uint8_t i=1;i<routeLength;i++) {
    uint8_t dest=routeStates[i]/4;
    uint8_t via=routeVias[i];
    if(!traverseHop(dest,via)) {
      Serial.print(F("NAV_ABORT last_confirmed="));printNode(currentNode);
      Serial.println();
      return false;
    }
  }
  LbMotion.stopAndWait();
  Serial.print(F("ARRIVED "));printNode(currentNode);
  Serial.println();
  return true;
}

void runNavigationTest() {
  LbMotion.stopAndWait();
  Serial.println(F("DIJKSTRA_TEST_START from J33 facing WEST"));
  Serial.println(F("FIRST_GOAL=J10 (row 1, col 0); then 5 random junctions"));
  Serial.println(F("REMOVE ALL BLOCKS. No gripper operations."));
  LbMission.begin(TB1A + TB1B);

  bool ok=navigateTo(FIRST_GOAL);
  if(ok) {
    // Random among navigable junctions only; no curve or J33 target.
    randomSeed(micros());
    for(uint8_t test=0;test<RANDOM_TEST_COUNT;test++) {
      uint8_t goal;
      do {
        goal=(uint8_t)random(0,16);
      } while(!isGoalNode(goal) || goal==currentNode);
      Serial.print(F("RANDOM_TEST #"));Serial.print(test+1);
      Serial.print(F(" TARGET="));printNode(goal);Serial.println();
      if(!navigateTo(goal)){ok=false;break;}
    }
  }
  LbMotion.stopAndWait();
  Serial.println(ok?F("NAV_TEST_DONE"):F("NAV_TEST_ABORTED"));
  Leanbot.tone(ok?1500:500,400);
  LbMission.end();
}

void loop() {
  static bool alreadyRun=false;
  if(alreadyRun) return;
  alreadyRun=true;
  runNavigationTest();
}
