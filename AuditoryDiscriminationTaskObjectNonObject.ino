// Cue and Stimulus combination is controlled by the "stimVal" variable set by stepping through the "sequence" array.
// Possible States:  Go (0) and NoGo (1) Auditory Stimuli outputs
//#define DEBUG 1

#include <Event.h>
#include <Timer.h>
#include <LED.h>
//#include <SoftwareSerial.h>
#include <Timer.h>
#include <Wire.h>
#include <Servo.h>

#define REWARDPIN 42
//#define REWARDPINNoGo   12
//#define REWARDPINcenter  14

#define TransPin_Go         19 //led transistor center 18
#define TransPin_Reward     2 //led transistor center 18
#define TransPin_C          18 //led transistor center 18
#define TransPin_Left       20
#define TransPin_Right      3 //need to define

#define GODELAY 3
#define NOGODELAY 3
#define REWARDDELAY 8
#define PUNISHDELAY 5
#define DELAY 5
#define ADDRESS 1
#define AIRPUFF 10
#define holdMin 50      // ms, defines time before the cue starts playing
#define cueDuration 0 // ms, length of the cue (brown noise)
#define ledDuration 0 //ms,

#define reward        1
#define success       2

#define stimulus      3
#define trigger       4
#define punish        5
#define triggerrepeat 6
#define miss          7

#define Val1PIN 26
#define Val2PIN 28
#define Val3PIN 30
#define Val4PIN 38   


#define lasTTL 5
#define TTLPINBlue2 9
#define TTLPINOutput A4


#define Go      1
#define NoGo    0

#define audio   1
#define visual  0

#define left    1
#define right   0

// Introduce pump definitions
// In order for deliverreward to work as expected, rewardMin/pumpStep & rewardMax/pumpStep need to be integers
#define pumpStep 2
#define rewardMin 6
#define rewardMax 16

//SoftwareSerial mySerial(8,9);

volatile int ison = 0;
volatile int state;
long int timecueon = 999999999;

Timer t;
int mis;

Timer h;
int hold;

Timer e;
int held;

Servo servo1;
Servo servo2;

String assignString = "";

int breakoutVar = 0;
int autoTrial    = 0;
int randNumber   = 0;
int currentStim  = 0;
int stimduration = 1000;
int stimDelay    = 1000;
int precuedelay  = 0;
long number      = 0;
int holdTime     = 0;
int required     = 300;
int openTime     = 500;
int rewardpoke   = 0;
//int jitter       = 150;
int count1       = 0;
int count0       = 0;
int distractor   = 0;
int stimOnTime   = 0;
int stimOffTime  = 0;
int currTime     = 0;
int counter      = 0;
int longDuration = 100;
//int sequence[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//int sequence[]   = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};//
int sequence[] = {0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1};
////laser + cue
//int stimSequence[] = {2,3,0,3,1,0,0,3,1,3,2,0,1,2,3,0,3,2,0,3,1,2,0,1,2,1,0,0,3,1,3,0,3,2,1,0,2,0,3,0,3,1,2,0,1,2,1,0,0,3,1,3,2,0,1,2,3,0,3,2,0,3,1,2,0,1,2,1,0,0,3,1,3,2,3,2,1,0,2,0,3,0,3,1,2,0,1,2,1,0,0,3,1,3,2,0,1,2,3,0,3,2,0,3,1,2,0,1,2,1,0,0,3,1,3,2,3,2,1,0,2,0,3,0,3,1,2,0,1,2,1,0,0,3,1,3,2,0,1,2,3,0,3,2,0,3,1,2,0,1,2,1,0,0,3,1,3,2,3};
//int stimSequence[]= {0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2};
//cue training
//int stimSequence[]={0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2,0,0,2,0,2,0,2,0,0,2,2,0,2,0,2};
//Noise only
//int stimSequence[]= {0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1};
//training
//int stimSequence[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,1,0,0,2,2,0,1,0,2,0,0,1,0,2,0,0,0,0,0,0,0,0,0,0,0};

//more noise +cue
//int stimSequence[] = {2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1, 2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1, 2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1, 2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1, 2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1, 2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1, 2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1, 2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1, 2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1, 2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1, 2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1, 2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1, 2, 0, 1, 0, 0, 2, 1, 0, 1, 1, 2, 0, 2, 2, 1};

//int stimSequence[] = {2,0,0,0,0,2,0,0,0,0,2,0,2,2,0,2,0,0,0,0,2,0,0,0,0,2,0,2,2,0,2,0,0,0,0,2,0,0,0,0,2,0,2,2,0,2,0,0,2,0,2,0,0,0,0,2,0,2,2,0,2,0,0,0,0,2,0,2,0,0,2,0,2,2,0,2,0,2,0,0,2,0,0,2,0,2,0,2,2,0,2,0,0,0,0,2,0,0,0,0,2,0,2,2,0,2,0,0,0,0,2,0,0,0,0,2,0,2,2,0,2,0,0,0,0,2,0,0,0,0,2,0,2,2,0,2,0,0,0,0,2,0,2,0,0,2,0,2,2,0,2,2,0,0,0,2,0,0,2,0,2,0,2,2,0,2,0,0,0,0,2,0,2,0,0,2,0,2,2,0,2,0,0,0,0,2,0,0,0,0,2,0,2,2,0};

//no noise
int stimSequence[] = {0,0,1,1,2,2,3,3,0,0,1,1,2,2,3,3};//0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3,0, 1, 2, 3};

//int stimSequence[] = {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
//int stimSequence[] = {2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3};

// trial type, 1=audio, 0=visual
byte trialtype[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};

// side, 1=left, 0=right
byte sides[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};


int trialcount = 0;
int currentTrial = trialtype[0];
int trialtypeLength = sizeof(trialtype) / sizeof(trialtype[0]);

int sidecount = 0;
int currentSide = sides[sidecount];


int count        = 1;
int stimVal = sequence[count];
int noiseVal = stimSequence[count];
int laserPulseInterval = 5;
int laserPulseMax = required / laserPulseInterval;
int laserPulseCount = 0;
int laserOn      = 0;
int lasState     = LOW;
int laserTrial   = 0;
int NoGoHigh    = 0;
int correctCounter = -1; // counts how many the mouse has previously gotten correct (in a row), init value = -1
int CORRECTCOUNTERMAX = (rewardMax - rewardMin) / pumpStep; // the max value that the counter can be
int cuePlay = 0; // is cue playing


LED ledTG         = LED(13);//Not Assigned
LED ledTB         = LED(13);//Not Assigned
LED ledIR_Go      = LED(6);
LED ledIR_Reward  = LED(10);
LED ledIR_C       = LED(8);
LED trialLED      = LED(7);

LED ledIR_left    = LED(4); //define
LED ledIR_right   = LED(12);

LED rightPokeLED  = LED(13); //define this properly later
LED leftPokeLED   = LED(5);


unsigned long logTime;
unsigned long prevMillis = millis();

enum ntype{StimO1, StimO2, StimO3, StimO4, StimN1, StimN2, StimN3, StimN4, silence};
//enum ntype {silence, nogonoise, gonoise, go, nogo, brown, blue};
void removed();
void misss();
void openrewardpoke();
void closegopoke();
void closerewardpoke();

// Go NoGo Noise
void play(ntype n) {
  switch (n) {
    case StimO1:
      digitalWrite(Val1PIN, LOW);
      digitalWrite(Val2PIN, LOW);
      digitalWrite(Val3PIN, LOW);
      digitalWrite(Val4PIN, LOW);
      break;
    case StimO2:
      digitalWrite(Val1PIN, LOW);
      digitalWrite(Val2PIN, LOW);
      digitalWrite(Val3PIN, LOW);
      digitalWrite(Val4PIN, HIGH);
      break;
    case StimO3:
      digitalWrite(Val1PIN, LOW);
      digitalWrite(Val2PIN, LOW);
      digitalWrite(Val3PIN, HIGH);
      digitalWrite(Val4PIN, LOW);
      break;
    case StimO4:
      digitalWrite(Val1PIN, LOW);
      digitalWrite(Val2PIN, LOW);
      digitalWrite(Val3PIN, HIGH);
      digitalWrite(Val4PIN, HIGH);
      break;
    case StimN1:
      digitalWrite(Val1PIN, LOW);
      digitalWrite(Val2PIN, HIGH);
      digitalWrite(Val3PIN, LOW);
      digitalWrite(Val4PIN, LOW);
      break;
    case StimN2:
      digitalWrite(Val1PIN, LOW);
      digitalWrite(Val2PIN, HIGH);
      digitalWrite(Val3PIN, LOW);
      digitalWrite(Val4PIN, HIGH);
      break;
    case StimN3:
      digitalWrite(Val1PIN, LOW);
      digitalWrite(Val2PIN, HIGH);
      digitalWrite(Val3PIN, HIGH);
      digitalWrite(Val4PIN, LOW);
      break;
    case StimN4:
      digitalWrite(Val1PIN, LOW);
      digitalWrite(Val2PIN, HIGH);
      digitalWrite(Val3PIN, HIGH);
      digitalWrite(Val4PIN, HIGH);
      break;
    case silence:
      digitalWrite(Val1PIN, HIGH);
      digitalWrite(Val2PIN, HIGH);
      digitalWrite(Val3PIN, HIGH);
      digitalWrite(Val4PIN, HIGH);
      break;

    // Do we need these?
//    case brown:
//      digitalWrite(TTLPINgo, LOW);
//      digitalWrite(TTLPINnogo, LOW);
//      digitalWrite(TTLPINNoise, LOW);
//      trialLED.on();
//    case blue:
//      digitalWrite(TTLPINgo, LOW);
//      digitalWrite(TTLPINnogo, LOW);
//      digitalWrite(TTLPINNoise, LOW);
//      trialLED.on();
//      break;
  }
}


struct Sequence {
  byte* arr;
  int size;
  int count;

  Sequence(byte* arr);
  int next();
};

int Sequence::next() {
  int temp = count;
  ++count;
  return arr[temp % size];
}

Sequence::Sequence(byte* arr) {
  this->arr = arr;
  this->size = sizeof(arr) / sizeof(arr[0]);
  this->count = 0;
}

Sequence trialSeq = Sequence(trialtype);
Sequence sideSeq = Sequence(sides);

void serLog(String str)

{

  Serial.print("LOG");

  Serial.print(str);

  Serial.print("\n");

}



void serLogTime()
{

  logTime = millis();
  Serial.print("TIME");
  Serial.print(logTime);
  Serial.print("\n");
}

void serDebug(String str) {
#ifdef DEBUG
  Serial.print(millis());
  Serial.print(" ");
  Serial.println(str);
#endif
}

void serFunc(String func)

{
  Serial.print("CMD");
  Serial.print(func);
  Serial.print("\n");
}

void pokeGo()
{
  rewardpoke = 0;
  if (state == stimulus)
  {
    if (currentStim == Go) {
      state = success;
    }
    else if (currentStim != Go) {
      state = punish;
    }
  }
}

void pokeReward()
{
  serLog("reward collected");
  serLogTime();
}

void holdThrough()
{
  rewardpoke = 0;
  if (state == stimulus)
  {
    if (currentStim == NoGo) {
      state = success;
    }
    else if (currentStim != NoGo) {
      state = punish;
    }
  }
}

void stimon()
{
  ison = 1;

  if (currentStim == Go)
  {
    serLog("stim Go, NonObject on");
    serLogTime();

    switch (noiseVal) {
      case 0:
        play(StimN1);
        serLog("NonObject 1");
        serLogTime();
        break;
      case 1:
        play(StimN2);
        serLog("NonObject 2");
        serLogTime();
        break;
      case 2:
        play(StimN3);
        serLog("NonObject 3");
        serLogTime();
        break;
      case 3:
        play(StimN4);
        serLog("NonObject 4");
        serLogTime();
        break;
    }

    delay(1000);
    play(silence);
  }
  else if (currentStim == NoGo)
  {
    NoGoHigh = 0;//random(2);
    if (NoGoHigh == 0)
    {
      serLog("stim NoGo, Object on");
      serLogTime();
      //digitalWrite(TTLPINnogo,HIGH);
      switch (noiseVal) {
      case 0:
        play(StimO1);
        serLog("Object 1");
        serLogTime();
        break;
      case 1:
        play(StimO2);
        serLog("Object 2");
        serLogTime();
        break;
      case 2:
        play(StimO3);
        serLog("Object 3");
        serLogTime();
        break;
      case 3:
        play(StimO4);
        serLog("Object 4");
        serLogTime();
        break;
    }
      delay(1000);
      play(silence);
    }
    play(silence);
    digitalWrite(TTLPINBlue2, LOW);
  }


  // visual stimulus
  if (currentSide == left) {
    leftPokeLED.on();
  }
  else {
    rightPokeLED.on();
  }

  stimOnTime = millis();
  stimOffTime = millis() + stimduration;
}


void trigr()
{
  attachInterrupt(digitalPinToInterrupt(TransPin_C), removed, FALLING);
  unsigned long currMillisLaser = millis();
  if (state == trigger || state == triggerrepeat)
  {
    trialLED.off();
    delay(holdMin);

    serFunc("initiation");
    serLog("initiation");
    serLogTime();
    if (state == trigger) rewardpoke = 1;

    hold = t.after(required + cueDuration, [] () {
      autoTrial = 0;
      randNumber = random(2);
      digitalWrite(TTLPINOutput, HIGH);
      delay(5);
      digitalWrite(TTLPINOutput, LOW);
      laserTrial = random(2);
      if (currentStim == Go)
      {
        mis = h.after(GODELAY * 1000, misss);
        attachInterrupt(digitalPinToInterrupt(TransPin_C), trigr, RISING);
      }
      if (currentStim == NoGo)
      {
        held = h.after(NOGODELAY * 1000, holdThrough);
      }
      serFunc("trial start");
      serLog("autostart");
      serLogTime();
      ledTG.off();
      digitalWrite(lasTTL, LOW);
      trialLED.off();
      state = stimulus;

    });
  }
}

void autoStart()
{
  autoTrial = 0;
  randNumber = random(2);
  digitalWrite(TTLPINOutput, HIGH);
  delay(5);
  digitalWrite(TTLPINOutput, LOW);
  laserTrial = random(2);
  if (currentStim == Go)
  {
    mis = t.after(GODELAY * 1000, misss);
    attachInterrupt(digitalPinToInterrupt(TransPin_C), trigr, RISING);
  }
  if (currentStim == NoGo)
  {
    held = t.after(NOGODELAY * 1000, holdThrough);
  }
  serFunc("trial start");
  serLog("autostart");
  serLogTime();
  ledTG.off();
  digitalWrite(lasTTL, LOW);
  state = stimulus;
}

void misss()
{
  state = miss;
  serLog("missed your shot!");
  serLogTime();
  play(silence);
}

void deliverreward()
{
  serLog("reward delivered");
  serLogTime();

  int numPulses = rewardMin / pumpStep + correctCounter;

  for (int i = 0; i < numPulses; i++) {
    digitalWrite(REWARDPIN, HIGH);
    delay(200);
    digitalWrite(REWARDPIN, LOW);
    delay(100);
  }

  serLog("delivered " + String(numPulses * pumpStep) + "mL");
  serLogTime();

  openrewardpoke();
  play(silence);
  delay(500);
}

void quickstimoff()
{
  serLog("stim off");
  serLogTime();
  serFunc("session stop R");
  serFunc("session stop L");
  closegopoke();
  ison = 0;
  play(silence);
}

void stimoff()
{
  serLog("stim off");
  serLogTime();
  serFunc("session stop R");
  serFunc("session stop L");
  delay(REWARDDELAY * 1000);
  closerewardpoke();
  ison = 0;
  play(silence);
}

void opengopoke()
{
  serDebug("open go poke");
//  servo1.write(145);
//  delay(100);
//  servo1.write(130);
//  delay(100);
//  servo1.write(120);
////  delay(100);
//  servo1.write(90);
//  delay(100);
//  servo1.write(75);
//  delay(100);
//  servo1.write(30);
//  delay(100);
  servo1.write(0);
//  delay(100);
}

void closegopoke()
{
  serDebug("close go poke");
  servo1.write(15);
  delay(250);
  servo1.write(20);
  delay(250);
  servo1.write(30);
  delay(500);
  servo1.write(40);
  delay(500);
  servo1.write(50);
  delay(500);
  servo1.write(60);
  delay(250);
  servo1.write(75);
  delay(250);
  servo1.write(90);
  delay(250);
}

void openrewardpoke()
{
  serDebug("open reward poke");
  servo1.write(5);
  delay(250);
  servo1.write(10);
  delay(250);
  servo1.write(15);
  delay(250);
  servo1.write(20);
  delay(250);
  servo1.write(50);
  delay(50);
  servo1.write(75);
  delay(50);
  servo1.write(175);
  delay(50);
}

void closerewardpoke()
{
  serDebug("close reward poke");
  servo1.write(170);
  delay(250);
  servo1.write(160);
  delay(250);
  servo1.write(160);
  delay(500);
  servo1.write(140);
  delay(500);
  servo1.write(130);
  delay(500);
  servo1.write(120);
  delay(500);
  servo1.write(110);
  delay(500);
  servo1.write(100);
  delay(250);
  servo1.write(90);
  delay(50);
}
////400 ms
//void opengopoke()
//{
//  servo1.write(90);
//  delay(50);
//  servo1.write(80);
//  delay(50);
//  servo1.write(70);
//  delay(50);
//  servo1.write(5);
//  delay(250);
//
//}

//900 ms
//void closegopoke()
//{
//   servo1.write(5);
//   delay(100);
//   servo1.write(10);
//   delay(100);
//   servo1.write(20);
//   delay(100);
//   servo1.write(30);
//   delay(100);
//   servo1.write(40);
//   delay(100);
//   servo1.write(50);
//   delay(100);
//   servo1.write(60);
//   delay(100);
//   servo1.write(70);
//   delay(100);
//   servo1.write(90);
//   delay(100);
//}
//
////1700 ms
//void openrewardpoke()
//{
//   servo1.write(95);
//   delay(250);
//   servo1.write(100);
//   delay(250);
//   servo1.write(110);
//   delay(250);
//   servo1.write(120);
//   delay(250);
//   servo1.write(130);
//   delay(250);
//   servo1.write(140);
//   delay(250);
//   servo1.write(150);
//   delay(50);
//   servo1.write(160);
//   delay(50);
//   servo1.write(170);
//   delay(50);
//   servo1.write(180);
//   delay(50);
//}
//
////2100 ms
//void closerewardpoke()
//{
//   servo1.write(175);
//   delay(250);
//   servo1.write(170);
//   delay(250);
//    servo1.write(160);
//   delay(250);
//   servo1.write(150);
//   delay(250);
//    servo1.write(140);
//   delay(250);
//   servo1.write(130);
//   delay(250);
//   servo1.write(120);
//   delay(250);
//   servo1.write(110);
//   delay(250);
//   servo1.write(100);
//   delay(50);
//                                                                                                                                                                                                1.write(90);
//   delay(50);
//}

void removed()
{
  attachInterrupt(digitalPinToInterrupt(TransPin_C), trigr, RISING);
  t.stop(hold);
  serFunc("removed");
  serLog("removed");
  serLogTime();
  laserOn = 0;
  digitalWrite(lasTTL, LOW);
  if (state == trigger || state == triggerrepeat)  trialLED.off();
}

void pokeLeft() {
  if (state != stimulus)
    return;

  serLog("poke left");
  serLogTime();
  rewardpoke = 0;
  if (currentTrial == visual && currentSide == left) {
    state = success;
  }
  else {
    state = punish;
  }
}

void pokeRight() {
  if (state != stimulus)
    return;

  rewardpoke = 0;

  serLog("poke right");
  serLogTime();

  if (currentTrial == visual && currentSide == right) {
    state = success;
  }
  else {
    state = punish;
  }
}

void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));

  //digitalWrite(0,LOW);
  digitalWrite(TransPin_Go, HIGH);
  digitalWrite(TransPin_Reward, HIGH);
  digitalWrite(TransPin_C, HIGH);
  digitalWrite(TransPin_Left, HIGH);
  digitalWrite(TransPin_Right, HIGH);

  servo1.attach(49);
  servo2.attach(48);
  servo2.write(90);
  servo1.write(90);

  attachInterrupt(digitalPinToInterrupt(TransPin_Go), pokeGo, RISING); //pin0
  attachInterrupt(digitalPinToInterrupt(TransPin_Reward), pokeReward, RISING);  //pin1
  attachInterrupt(digitalPinToInterrupt(TransPin_C), trigr, RISING);   //pin2
  attachInterrupt(digitalPinToInterrupt(TransPin_Left), pokeLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(TransPin_Right), pokeRight, RISING);

  pinMode(REWARDPIN,      OUTPUT);
  pinMode(AIRPUFF,        OUTPUT);
  pinMode(Val1PIN,       OUTPUT);
  pinMode(Val2PIN,     OUTPUT);
  pinMode(Val3PIN,     OUTPUT);
  pinMode(Val4PIN,     OUTPUT);
  pinMode(TTLPINOutput,   OUTPUT);
  //pinMode(TTLPINNoise,   OUTPUT);
  //pinMode(TTLPINBlue2,         OUTPUT);
  pinMode(lasTTL,         OUTPUT);


  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  ledIR_Reward.on(); //turn on IR LEDs in pokes
  ledIR_Go.on(); //Outer Right
  ledIR_C.on(); //Center
  ledIR_right.on();
  ledIR_left.on();


  digitalWrite(lasTTL, LOW);
  digitalWrite(TTLPINOutput, LOW);
  digitalWrite(Val1PIN, HIGH);
  digitalWrite(Val2PIN, HIGH);
  digitalWrite(Val3PIN, HIGH);
  digitalWrite(Val4PIN, HIGH);
  
  
  //digitalWrite(TTLPINnogo, LOW);
  //digitalWrite(TTLPINgo, LOW);
  //digitalWrite(TTLPINNoise, LOW);
  //digitalWrite(TTLPINBlue2, LOW);

  ledTG.off();
  ledTB.off();
  rewardpoke = 0;
  state = trigger;
  trialLED.off();

  delay(4 * 1000);


}


void loop()
{
  h.update();
  t.update();
  switch (state)
  {
    case stimulus:

      t.stop(hold);
      if ((ledTG.getState() || ledTB.getState()) && (rewardpoke == 1))
      {
        ledTG.off();
        ledTB.off();
      }
      if (!ison)
      {
        serDebug("stim played");
        stimon();
        delay(stimDelay);
        opengopoke();
      }
      currTime = millis();

      if (currTime >= stimOnTime + ledDuration) {
        leftPokeLED.off();
        rightPokeLED.off();
      }
      break;
    case punish:
      h.stop(mis); h.stop(held);
      correctCounter = -1; //reset counter
      quickstimoff();
      t.stop(mis);
      delay(PUNISHDELAY * 1000);
      delay(PUNISHDELAY * 1000);
      breakoutVar = random(5);
      if (breakoutVar < 5)
      {
        state = triggerrepeat;
      }
      else
      {
        state = trigger;
      }
      break;
    case success:
          h.stop(mis); h.stop(held);
      if (correctCounter < CORRECTCOUNTERMAX) correctCounter += 1;
      t.stop(mis);
      deliverreward();
      stimoff();
      play(silence);
      delay(DELAY * 1000);
      state = trigger;
      break;
    case miss:
          h.stop(mis); h.stop(held);
      correctCounter = -1; //reset counter
      rewardpoke = 0;
      quickstimoff();
      play(silence);
      delay(PUNISHDELAY * 1000);
      breakoutVar = random(5);
      if (breakoutVar < 5)
      {
        state = triggerrepeat;
      }
      else
      {
        state = trigger;
      }

      break;
    case trigger:
      if ((!ledTG.getState() && !ledTB.getState()) && (rewardpoke == 0))
      {

        count = count + 1;
        trialcount = trialcount + 1;
        if (count > 80)
        {
          count = 1;
        }

        if (trialcount >= trialtypeLength)
        {
          trialcount = 0;
        }

        stimVal = sequence[count];
        noiseVal = stimSequence[count];
        currentStim = stimVal;
        currentTrial = trialSeq.next();
        currentSide = sideSeq.next();


        play(silence);

        trialLED.off();
        serLog("trial available");
        serLogTime();
        if (currentTrial == visual) {
          //serLog("trial available visual");
          if (currentSide == left) {
            // serLog("trial available left");
          }
          else {
            // serLog("trial available right");
          }
        }
        else if (currentTrial == audio) {
          //  serLog("trial available audio");
          if (stimVal == Go)
          {
            ledTG.on();
            serLog("trial available go");
          }
          else if (stimVal == NoGo)
          {
            ledTG.on();
            serLog("trial available no-go");
          }
        }

        ledTG.on();

        serLogTime();
        serLog("LEDTB On");
        serLogTime();


        //trigr();
      }

      if (cuePlay && millis() - prevMillis >= cueDuration) {
        play(silence);
        serLog("end cue playing");
        serLogTime();
        cuePlay = 0;
      }
      break;
    case triggerrepeat:
      if ((!ledTG.getState() && !ledTB.getState()) && (rewardpoke == 0))
      {
        if (count > 80)
        {
          count = 1;
        }

        trialLED.off();
        serLog("trial available");
        serLogTime();
        play(silence);

        if (currentTrial == visual) {
          //serLog("trial available visual");
          if (currentSide == left) {
            // serLog("trial available left");
          }
          else {
            //  serLog("trial available right");
          }
        }
        else if (currentTrial == audio) {
          //serLog("trial available audio");
          if (stimVal == Go)
          {
            ledTG.on();
            serLog("trial available go");
          }
          else if (stimVal == NoGo)
          {
            ledTG.on();
            serLog("trial available no-go");
          }
        }

        ledTG.on();


        serLogTime();
        serLog("LEDTB On");
        serLogTime();
        //trigr();
      }

      if (cuePlay && millis() - prevMillis >= cueDuration) {
        play(silence);
        serLog("end cue playing");
        serLogTime();
        cuePlay = 0;
      }
      break;
  }
}
