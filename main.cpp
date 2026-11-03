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
const int SURVEY_SPEED_STEPS = 1200;       // steps/second
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
