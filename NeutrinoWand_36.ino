// This code has initially been created entirelly by Arpehem and is free to be used, modified or updated by anyone as long as this credit remains.
// This code is compatible with a common cathode bargraph only.

#include <FireTimer.h>
#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>
#include <BfButton.h>
#include <BfButtonManager.h>
#include <HT16K33.h>
#include <FastLED.h>
# define NUM_LEDS 4
# define NUM_STRIPS 1
# define WAND_LEDS 4
#define WAND_LED_PIN 10

HT16K33 matrix = HT16K33();
char Volume=20;
int bargraph=0;
int BGadress[4]={1,4,8,2};
int demo;
char S = 35;
int i;
int overloadtime=5000;
int J=0;
int K=0;
int shooting = 0;
enum currentstate{on,off,boot,cycle,fire,overloading,overload,overheat,music,volumep,ventp}state;
unsigned long myWandTime;
unsigned long previousWandTime = 0;
unsigned long previousLightTime=0;
unsigned long previousAlarmTime=0;
unsigned long previousShootLight=0;
unsigned long ShootingTime = 0;
unsigned long PreviousShootingTime = 0;
bool isFire = false;
bool wasFire =false;
bool booted=false;
bool issmoke=false;
bool up = true;
bool lightcycle =true;
bool alarmlighton =false;
int firepin = 5;
int bootpin = 2;
int ventpin = 6;
int smokeswitch = 4;
int lightswitch =3;
int rot1 = 13;
int firelight = 11;
int powerlight = 12;


CRGB wand_leds[WAND_LEDS];
CLEDController *controllers[NUM_STRIPS];


int btnPin=7;
int DT=8;
int CLK=9;
BfButton btn(BfButton::STANDALONE_DIGITAL,btnPin,true,LOW);

int counter = 0;
int angle = 0;
int aState;
int aLastState;

void pressHandler (BfButton *btn, BfButton::press_pattern_t pattern){  //////////////////////////////////ROT Handler///////////////////////////////////////
  switch (pattern) {
    case BfButton::SINGLE_PRESS:
   // myMP3.playAdvertisement(8);
    if (state==volumep){
      state=cycle;
     Serial.write('c');
      delay(50);
    }
    else{
      Serial.write('V');
     delay(50);
      state=volumep;
          matrix.clear();
    i=Volume;
    angle=Volume;
    counter=Volume;
    cycle_switch();
  matrix.write();
    }
      break;
      
    case BfButton::DOUBLE_PRESS:
   // myMP3.playAdvertisement(8); 
         if (state==music){
      state=cycle;
Serial.write('m');
delay(50);
    }
    else{
      state=music;
Serial.write('m');
delay(50);
    }
      break;
      
    case BfButton::LONG_PRESS:
    Serial.write('n');
delay(50);
    
      break;
  }
}




void setup() {       ///////////////////////////SETUP///////////////////////////////////
Serial.begin(9600);
//mySerial.begin(9600);
//myMP3.begin(mySerial, true);
pinMode(CLK, INPUT_PULLUP);
pinMode(DT, INPUT_PULLUP);
aLastState = digitalRead(CLK);
//myMP3.volume(Volume);
  //Button settings
  btn.onPress(pressHandler)
  .onDoublePress(pressHandler) // default timeout
  .onPressFor(pressHandler, 1000); // custom timeout for 1 second

  matrix.init(0x70);   // start the matrix on I2C address 0x70
  delay(1500);
state= off;
pinMode(firepin, INPUT);
pinMode(bootpin, INPUT);
pinMode(ventpin, INPUT);
pinMode(smokeswitch, INPUT);
pinMode (lightswitch, INPUT);
pinMode (rot1, INPUT);
pinMode (11, OUTPUT);
pinMode (12, OUTPUT);
//analog i/o
pinMode (A0, OUTPUT);
pinMode (A1, OUTPUT);
pinMode (A2, OUTPUT);
pinMode (A3, OUTPUT);
pinMode (A6, INPUT);
pinMode (A7, INPUT);

controllers[0] = &FastLED.addLeds<WS2812, WAND_LED_PIN, RGB>(wand_leds, WAND_LEDS); // Assign wand LED to controller 0
  FastLED.setMaxPowerInVoltsAndMilliamps(5,500);

analogWrite(A0, 175); // light slo-blo as pack status light
FastLED.setBrightness(255);
FastLED.clear();
FastLED.show(); 
}
void loop() {                  //////////////////////////////////////LOOP//////////////////////////////////////

myWandTime=millis();  // Records current time

if (myWandTime>previousShootLight+25&&isFire==true){
  shooting++;
  if (shooting>3){
    shooting=0;
  }
     FastLED.clear();
//wand_leds[min(shooting+1,3)] = CRGB(150,30,255);
wand_leds[random(0,3)] = CRGB(75,30,255);
wand_leds[random(0,3)] = CRGB(200,30,30);
wand_leds[random(0,3)] = CRGB(150,30,0);
wand_leds[random(0,3)] = CRGB(150,30,0);
wand_leds[random(0,3)] = CRGB(150,30,0);
wand_leds[max(shooting-1,0)] = CRGB(150,30,0);
wand_leds[shooting] = CRGB(254,100,0);
  FastLED.show(); 
  previousShootLight=millis();
}
if (digitalRead(smokeswitch)==true&&issmoke==false){
Serial.write('y');
issmoke=true;
}
if (digitalRead(smokeswitch)==false&&issmoke==true){
Serial.write('x');
issmoke=false;
}
if (digitalRead(lightswitch)==true&&state==off&&digitalRead(bootpin)==false){
state=on;
analogWrite(A7,130);
}
if (digitalRead(lightswitch)==false&&state!=off){
state=off;

}
if(state==off||state==overload){
analogWrite(A1,0);
analogWrite(A2,0);
analogWrite(A3,0);
analogWrite(A6,0);
analogWrite(A7,0);
}


if(myWandTime>previousLightTime+600&&digitalRead(lightswitch)==true&&lightcycle==false&&booted==true){
  analogWrite(A1,130);
  analogWrite(A3,0);
  lightcycle=true;
  previousLightTime=millis();
}
if(myWandTime>previousLightTime+600&&digitalRead(lightswitch)==true&&lightcycle==true&&booted==true){
  analogWrite(A3,130);
  analogWrite(A1,0);
  lightcycle=false;
  previousLightTime=millis();
}

  btn.read();
  
  aState = digitalRead(CLK);

                                  //////////////////////////Encoder rotation tracking////////////////////////////////
  if (aState != aLastState){     
     if (digitalRead(DT) != aState) { 
       counter ++;
       angle ++;
      //        counter=min(counter,29);
     //  angle=min(angle,29);
       
     }
     else {
       counter--;
       angle --;
      // counter=max(counter,0);
      // angle=max(angle,0);
     }
     if (counter >=28 ) {
       counter =28;
     }
     if (counter <=-0 ) {
       counter =-0;
     }
    // Serial.println(counter); 
  }   
  aLastState = aState;
if (state==volumep&&Volume!=counter){
    matrix.clear();
    Volume=counter;
    Volume=min(Volume,28);
    Volume=max(Volume,0);
    i=Volume;
    Serial.write(Volume);
    cycle_switch();
  matrix.write();
  
 // myMP3.volume(Volume); 
 // delay(50);
 //   Serial.begin(9600);
}
  
if(digitalRead(ventpin)==true&&state==fire||digitalRead(ventpin)==true&&state==overheat||digitalRead(ventpin)==true&&state==overloading){ ///////////////////////////// if pack is shooting and the vent pin is pressed before the forced venting, calls venting without shuting down/////////////////////////////
  vent();
  state=cycle;
  Serial.write('c');
  delay(50);
  loopsound(2);
  i=0;
  J=0;
  S=35;
  Serial.write(S);
}

if(state!=off&&state!=on&&state!=cycle&&state!=overload&&state!=overloading&&state!=overheat&&state!=music&&state!=volumep&&state!=boot&&isFire==false){
  state=cycle;
    Serial.write('c');
  delay(50);
  S=35;
  Serial.write(S);
 }
if(state==music && myWandTime > previousWandTime+35){
  cyclesequence();
}
if(state==overloading&&isFire==false||state==overheat&&isFire==false){
  vent();
  state=cycle;
    Serial.write('c');
  delay(50);
  S=35;
  Serial.write(S);
}

//if(digitalRead(bootpin)==false){ //if switch is off, state is off
//  state=off;

//}
isFire=digitalRead(firepin); // Reads fire button input and sets isFire accordignly
//if(state==off){
//  isFire=false;
//}

if(state!=boot&&state!=on&&digitalRead(bootpin)==false&&booted==true){ ///////////////////////////////////// if state is not boot state, pack is booted and switch is off, calls shutdown////////////////////////////////////
  state=off;
Serial.write("f");
delay(50);
shutdownsequence();
  
  booted=false;
  state=off;
  matrix.clear();
  matrix.write();
  i=0;
}
if(state==on&&digitalRead(bootpin)==true&&booted==false){ //////////////////////////////////// if pack is not booted and switch is on, calls boot//////////////////////////////////////
  state=boot;
  Serial.write("b");
delay(50);
}
if(state==off&&digitalRead(bootpin)==true&&booted==true){ /////////////////////////////////////// if pack is off but still booted and switch is on, calls shutdown/////////////////////////////////
  state=off;
 // shutdownoverloadsequence();
 shutdownsequence();
Serial.write("f");
delay(50);
}

  if(state==boot){  /////////////////////////////// Check for boot state and calls bootsequence/////////////////////////////////////////////////

  playsound(1);
  bootsequence();
  
  }
if (myWandTime > previousAlarmTime+200&&state==overloading||myWandTime > previousAlarmTime+200&&state==overheat){ // Check for cycle , calls cyclesequence
  alarmlight();
}
if (myWandTime > previousWandTime+28 && isFire == false && booted == true&&state==cycle){ // Check for cycle , calls cyclesequence
  cyclesequence();
}
if (myWandTime > previousWandTime+S && isFire == true && booted == true&&S>1){ // Check for fire , calls firesequence
firesequence();
}
if (myWandTime > previousWandTime+35 && isFire == true && booted == true&&S==1&&J<300 && state != off && state != overload&&state!=ventp){ // Check for fire , calls overheat sequence


  cycleoverheatsequence();
}
if (myWandTime > previousWandTime+35 && isFire == true && booted == true&&state!=ventp&&S==1&&J==300&&K<125&& state != off){ // Check for fire , calls overload/vent sequence


  overloadsequence();
}
}
void demo_cycle (){            //////////////////////////////// Demo of bargraph animations without timing or trigger//////////////////////////////////////////////
    demo++;
  for (int i=0; i<2;i++){
    cyclesequence();
  }   
    for (int i=0; i<10;i++){
    firesequence();
  } 
    for (int i=0; i<20;i++){
    cycleoverheatsequence();
  }  
  overloadsequence();

}
void alarmlight(){
  switch (alarmlighton) {
  case true:
        analogWrite(A2, 0);
    alarmlighton=false;
    previousAlarmTime=millis();  
    break;
  case false:
    analogWrite(A2, 130);
    alarmlighton=true;
    previousAlarmTime=millis();  
    break;
  default:
    // statements
    break;

}
}
void playsound(int sound){
//    mySerial.begin(9600);
 // delay(50);
 // myMP3.play(sound); 
 // delay(50);
 //   Serial.begin(9600);
 
}
void loopsound(int sound){
 //   mySerial.begin(9600);
//  delay(50);
//  myMP3.loop(sound); 
//  delay(50);
//    Serial.begin(9600);
  
}
void bootsequence(){  // Boot Sequence
  for(int i=0&&Serial.write("b");i<28;i++){
    matrix.clear();
    cycle_boot(i);
    matrix.write();
    delay(125);
    }
  for(int i=28;i<57;i++){
    matrix.clear();
    cycle_boot(i);
    matrix.write();
    delay(25);
  }
  booted=true;
  loopsound(2);
  state=cycle;
      Serial.write('c');
    delay(50);
  }
void shutdownsequence(){ 
  Serial.write("f");
  
  playsound(10);
  analogWrite(A1,0);
  analogWrite(A3,0);
  for(int i=28;i<57;i++){
    matrix.clear();
    cycle_boot(i);
    matrix.write();
    delay(75);
  }
  state=off;
  digitalWrite(firelight,LOW);
  
  delay(50);
  booted=false;
  matrix.clear();
  matrix.write();
  i=0;
  previousWandTime=myWandTime;
  
}
void shutdownoverloadsequence(){  
Serial.write("f");
playsound(10);  
    for(int i=28;i<57;i++){
    matrix.clear();
    cycle_boot(i);
    matrix.write();
    delay(25);
  }
  state=overload;
  
 
  delay(50);
  booted=false;
  i=0;
  matrix.clear();
  matrix.write();
  
  
  previousWandTime=myWandTime;
}
void cyclesequence(){  // Normal cycle sequence
    if(wasFire==true){
    wasFire=false;
    FastLED.clear();
    FastLED.show(); 
    i=0;
    state=cycle;
    S=35;
    Serial.write(S);
    Serial.write('c');
    delay(50);
    digitalWrite(firelight,LOW);
    analogWrite(A2,0);
  }
  if(state!=cycle&&state!=music&&state!=volumep){
    loopsound(2);
    state=cycle;
        Serial.write('c');
    delay(50);
    
    
  }
  if (i>28){
    i=28;
  }
  if (i==28){
    up=false;
  }
  if (i==0){
    up=true;
  }
  if(up == true){
    i++;
  }
    if(up==false){
    i--;
  }
matrix.clear();
cycle_switch();
matrix.write();
previousWandTime=myWandTime;
}
void firesequence(){  // Fire suquence
  if (wasFire==false){
  wasFire=true;
  Serial.write('s');
  delay(50);
  loopsound(11);
digitalWrite(firelight,HIGH);
 }

 if (state!=fire){
    i=0;
  state=fire;
  J=0;
 }
 ShootingTime=millis();
if(ShootingTime>PreviousShootingTime+500){
  S--;
  PreviousShootingTime=millis();
Serial.write(S);

}
 if (i>19){
   i=0;
  }
  i++;
matrix.clear();
fire_switch();
matrix.write();
previousWandTime=myWandTime;
}
void vent(){
  state=ventp;
  FastLED.clear(); 
  FastLED.show(); 
  analogWrite(A2,0);
       Serial.write('v');
       digitalWrite(firelight,LOW);
    delay(50);
  playsound(9);
  wasFire=false;
  for (int i=24; i<34;i++){
  matrix.clear();
cycle_overload(i);
matrix.write();
delay(35);
}


matrix.clear();
cycle_overload(35);
matrix.write();
delay(3000);
state=off;
i=0;
previousWandTime=myWandTime;
}

void overloadsequence(){ // Overheat/vent sequence
 if(state==overheat){
  state=overloading;
 i=0;
 K=0;
 up=true;
     Serial.write('o');
    delay(50);
 }
K++;

   switch (K) {
  case 1:
    up=true;
    break;
  case 35:
    up=false;
    break;
  case 45:
    up=true;
    break;
  case 55:
    up=false;
    break;
  case 65:
    up=true;
    break;
  case 75:
    up=false;
    break;
  case 85:
    up=true;
    break;
  case 95:
    up=false;
    break;
  case 105:
    up=true;
    break;
  case 115:
    up=false;
    break;
  case 124:
  vent();
  analogWrite(A1,0);
analogWrite(A2,0);
analogWrite(A3,0);
analogWrite(A6,0);
analogWrite(A7,0);
shutdownsequence();
state=overload;
isFire=false;
wasFire=false;

      break;
}
   
if(up==true){
  i++;
}
if(up==false){
  i--;
}
if(K<124){
    matrix.clear();
cycle_overload(i);
matrix.write();
previousWandTime=myWandTime;

}
if(K==124){
  i=0;
  J=0;
  K=0;
  S=35;
  Serial.write(S);
}
}
void cycleoverheatsequence(){ // Overload (warning) sequence
   if(state==fire){
  state=overheat;
  i=0;
//J=0;
loopsound(16);
     Serial.write('h');
    delay(50);
 }
i++;
J++;
  matrix.clear();
  cycle_overheat();
  matrix.write();

if (i>4){
  i=0;
}
previousWandTime=myWandTime;
}
void cycle_boot(int boot){
  switch (boot) {
  case 1:
    matrix.setRow(6, 2);
    break;
  case 2:
    matrix.setRow(6, 10);
    break;
  case 3:
    matrix.setRow(6, 14);
    break;
  case 4:
    matrix.setRow(6, 15);
    break;
  case 5:
    matrix.setRow(5, 2);
    matrix.setRow(6, 15);
    break;
  case 6:
    matrix.setRow(5, 10);
    matrix.setRow(6, 15);
    break;
  case 7:
    matrix.setRow(5, 14);
    matrix.setRow(6, 15);
    break;
  case 8:
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 9:
    matrix.setRow(4, 2);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 10:
    matrix.setRow(4, 10);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 11:
    matrix.setRow(4, 14);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 12:
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 13:
    matrix.setRow(3, 2);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 14:
    matrix.setRow(3, 10);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 15:
    matrix.setRow(3, 14);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 16:
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 17:
        matrix.setRow(2, 2);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 18:
        matrix.setRow(2, 10);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 19:
        matrix.setRow(2, 14);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 20:
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 21:
    matrix.setRow(1, 2);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 22:
    matrix.setRow(1, 10);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 23:
    matrix.setRow(1, 14);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 24:
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 25:
    matrix.setRow(0, 2);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 26:
    matrix.setRow(0, 10);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 27:
    matrix.setRow(0, 14);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 28:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 15);
    break;
  case 29:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 13);
    break;
  case 30:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 5);
    break;
  case 31:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    matrix.setRow(6, 1);
    break;
    case 32:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 15);
    break;
    case 33:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 13);
    break;
    case 34:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 5);
    break;
    case 35:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    matrix.setRow(5, 1);
    break;
    case 36:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 15);
    break;
    case 37:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 13);
    break;
    case 38:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 5);
    break;
    case 39:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    matrix.setRow(4, 1);
    break;
    case 40:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 15);
    break;
    case 41:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 13);
    break;
    case 42:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 5);
    break;
    case 43:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    matrix.setRow(3, 1);
    break;
    case 44:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 15);
    break;
    case 45:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 13);
    break;
    case 46:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 5);
    break;
    case 47:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    matrix.setRow(2, 1);
    break;
    case 48:
    matrix.setRow(0, 15);
    matrix.setRow(1, 15);
    break;
    case 49:
    matrix.setRow(0, 15);
    matrix.setRow(1, 13);
    break;
    case 50:
    matrix.setRow(0, 15);
    matrix.setRow(1, 5);
    break;
    case 51:
    matrix.setRow(0, 15);
    matrix.setRow(1, 1);
    break;
    case 52:
    matrix.setRow(0, 15);
    break;
    case 53:
    matrix.setRow(0, 13);
    break;
    case 54:
    matrix.setRow(0, 5);
    break;
    case 55:
    matrix.setRow(0, 1);
    break;
    case 56:
    matrix.setRow(0, 0);
    state= cycle;
    break;
  default:
    // statements
    break;
}
}
void cycle_switch(){
   switch (i) {
  case 1:
    matrix.setRow(0, 1);
       break;
  case 2:
    matrix.setRow(0, 5);
    break;
  case 3:
    matrix.setRow(0, 13);
    break;
  case 4:
    matrix.setRow(0, 15);
    break;
  case 5:
    matrix.setRow(1, 1);
    matrix.setRow(0, 15);
    break;
  case 6:
    matrix.setRow(1, 5);
    matrix.setRow(0, 15);
    break;
  case 7:
    matrix.setRow(1, 13);
    matrix.setRow(0, 15);
    break;
  case 8:
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
  case 9:
    matrix.setRow(2, 1);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
  case 10:
    matrix.setRow(2, 5);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
  case 11:
    matrix.setRow(2, 13);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
  case 12:
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 13:
    matrix.setRow(3, 1);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 14:
    matrix.setRow(3, 5);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 15:
    matrix.setRow(3, 13);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 16:
    matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 17:
    matrix.setRow(4, 1);
    matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 18:
    matrix.setRow(4, 5);
    matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 19:
    matrix.setRow(4, 13);
    matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 20:
   matrix.setRow(4, 15);
   matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 21:
    matrix.setRow(5, 1);
    matrix.setRow(4, 15);
   matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 22:
   matrix.setRow(5, 5);
   matrix.setRow(4, 15);
   matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 23:
    matrix.setRow(5, 13);
    matrix.setRow(4, 15);
   matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 24:
   matrix.setRow(5, 15);
   matrix.setRow(4, 15);
   matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 25:
   matrix.setRow(6, 1);
   matrix.setRow(5, 15);
   matrix.setRow(4, 15);
   matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      case 26:
   matrix.setRow(6, 5);
   matrix.setRow(5, 15);
   matrix.setRow(4, 15);
   matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
    case 27:
   matrix.setRow(6, 13);
   matrix.setRow(5, 15);
   matrix.setRow(4, 15);
   matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
   case 28:
   matrix.setRow(6, 15);
   matrix.setRow(5, 15);
   matrix.setRow(4, 15);
   matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(1, 15);
    matrix.setRow(0, 15);
    break;
      default:
    // statements
    break;
}
}
void fire_switch(){
  switch (i) {
  case 1:
    matrix.setRow(3, 12);
       break;
  case 2:
    matrix.setRow(3, 15);
    break;
  case 3:
    matrix.setRow(3, 15);
    matrix.setRow(2, 2);
    matrix.setRow(4, 1);
    break;
  case 4:
    matrix.setRow(3, 15);
    matrix.setRow(2, 10);
    matrix.setRow(4, 5);
    break;
  case 5:
    matrix.setRow(3, 15);
    matrix.setRow(2, 14);
    matrix.setRow(4, 13);
    break;
  case 6:
    matrix.setRow(3, 15);
    matrix.setRow(2, 15);
    matrix.setRow(4, 15);
    break;
  case 7:
    matrix.setRow(3, 3);
    matrix.setRow(2, 15);
    matrix.setRow(4, 15);
    matrix.setRow(1, 2);
    matrix.setRow(5, 1);
    break;
  case 8:
    matrix.setRow(2, 15);
    matrix.setRow(4, 15);
    matrix.setRow(1, 10);
    matrix.setRow(5, 5);
    break;
  case 9:
    matrix.setRow(2, 13);
    matrix.setRow(4, 14);
    matrix.setRow(1, 14);
    matrix.setRow(5, 13);
    break;
  case 10:
    matrix.setRow(2, 5);
    matrix.setRow(4, 10);
    matrix.setRow(1, 15);
    matrix.setRow(5, 15);
    break;
  case 11:
    matrix.setRow(2, 1);
    matrix.setRow(4, 2);
    matrix.setRow(1, 15);
    matrix.setRow(5, 15);
    matrix.setRow(0, 2);
    matrix.setRow(6, 1);
    break;
  case 12:
    matrix.setRow(1, 15);
    matrix.setRow(5, 15);
    matrix.setRow(0, 10);
    matrix.setRow(6, 5);
    break;
      case 13:
    matrix.setRow(1, 13);
    matrix.setRow(5, 14);
    matrix.setRow(0, 14);
    matrix.setRow(6, 13);
    break;
      case 14:
    matrix.setRow(1, 5);
    matrix.setRow(5, 10);
    matrix.setRow(0, 15);
    matrix.setRow(6, 15);
    break;
      case 15:
    matrix.setRow(1, 1);
    matrix.setRow(5, 2);
    matrix.setRow(0, 15);
    matrix.setRow(6, 15);
    break;
      case 16:
    matrix.setRow(0, 15);
    matrix.setRow(6, 15);
    break;
      case 17:
    matrix.setRow(0, 13);
    matrix.setRow(6, 14);
    break;
      case 18:
    matrix.setRow(0, 5);
    matrix.setRow(6, 10);
    break;
      case 19:
    matrix.setRow(0, 1);
    matrix.setRow(6, 2);
    break;
      default:
    // statements
    break;
}
}
void cycle_overheat(){
  switch (i) {
  case 1:
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 14);  // center
    matrix.setRow(4, 13);  // center
    matrix.setRow(6, 15);  // top
    matrix.setRow(0, 15);  // bottom
    matrix.setRow(5, 2);
    matrix.setRow(1, 1);
    break;
  case 2:
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 7);  // center
    matrix.setRow(4, 11);  // center
    matrix.setRow(6, 14);  // top
    matrix.setRow(0, 13);  // bottom
    matrix.setRow(5, 10);
    matrix.setRow(1, 5);
    break;
      case 3:
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 3);  // center
    matrix.setRow(4, 3);  // center
    matrix.setRow(6, 14);  // top
    matrix.setRow(0, 13);  // bottom
    matrix.setRow(5, 13);
    matrix.setRow(1, 14);
    break;
  case 4:
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 7);  // center
    matrix.setRow(4, 11);  // center
    matrix.setRow(6, 14);  // top
    matrix.setRow(0, 13);  // bottom
    matrix.setRow(5, 10);
    matrix.setRow(1, 5);
    break;
  case 5:
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 14);  // center
    matrix.setRow(4, 13);  // center
    matrix.setRow(6, 15);  // top
    matrix.setRow(0, 15);  // bottom
    matrix.setRow(5, 2);
    matrix.setRow(1, 1);
    break;
  default:
    // statements
    break;
}
}
void cycle_overload(int load){
  switch (load) {
  case 1:
    matrix.setRow(6, 15);  // top
    matrix.setRow(4, 5);  // center
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 10);  // center
    matrix.setRow(0, 15);  // bottom
    break;
  case 2:
    matrix.setRow(6, 11);  // top
    matrix.setRow(5, 2);  // center
    matrix.setRow(4, 12);  // center
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 12);  // center
    matrix.setRow(0, 7);  // bottom
    break;
  case 3:
    matrix.setRow(6, 10);  // top
    matrix.setRow(5, 10);  // center
    matrix.setRow(4, 10);  // center
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 5);  // center
    matrix.setRow(1, 5);  // center
    matrix.setRow(0, 5);  // bottom
    break;
  case 4:
    matrix.setRow(6, 10);  // top
    matrix.setRow(5, 13);  // center
    matrix.setRow(4, 2);  // center
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 1);  // center
    matrix.setRow(1, 14);  // center
    matrix.setRow(0, 5);  // bottom
    break;
  case 5:
    matrix.setRow(6, 10);  // top
    matrix.setRow(5, 10);  // center
    matrix.setRow(4, 10);  // center
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 5);  // center
    matrix.setRow(1, 5);  // center
    matrix.setRow(0, 5);  // bottom
    break;
  case 6:
    matrix.setRow(6, 11);  // top
    matrix.setRow(5, 2);  // center
    matrix.setRow(4, 12);  // center
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 12);  // center
    matrix.setRow(1, 1);  // center
    matrix.setRow(0, 7);  // bottom
    break;
  case 7:
    matrix.setRow(6, 15);  // top
    matrix.setRow(4, 5);  // center
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 10);  // center
    matrix.setRow(0, 15);  // bottom
    break;
  case 8:
    matrix.setRow(6, 14);  // top
    matrix.setRow(4, 1);  // center
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 2);  // center
    matrix.setRow(0, 13);  // bottom
    break;
  case 9:
    matrix.setRow(6, 10);  // top
    matrix.setRow(3, 15); // center
    matrix.setRow(0, 5);  // bottom
    break;
  case 10:
    matrix.setRow(6, 14);  // top
    matrix.setRow(4, 1);  // center
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 2);  // center
    matrix.setRow(0, 13);  // bottom
    break;
  case 11:
    matrix.setRow(6, 7);  // top
    matrix.setRow(5, 0);  // center
    matrix.setRow(4, 5);  // center
    matrix.setRow(3, 12); // center
    matrix.setRow(2, 10);  // center
    matrix.setRow(1, 0);  // center
    matrix.setRow(0, 11);  // bottom
    break;
  case 12:
    matrix.setRow(6, 3);  // top
    matrix.setRow(5, 2);  // center
    matrix.setRow(4, 12);  // center
    matrix.setRow(3, 12); // center
    matrix.setRow(2, 12);  // center
    matrix.setRow(1, 1);  // center
    matrix.setRow(0, 3);  // bottom
    break;
  case 13:
    matrix.setRow(6, 2);  // top
    matrix.setRow(5, 10);  // center
    matrix.setRow(4, 10);  // center
    matrix.setRow(3, 12); // center
    matrix.setRow(2, 5);  // center
    matrix.setRow(1, 5);  // center
    matrix.setRow(0, 1);  // bottom
    break;
  case 14:
    matrix.setRow(6, 2);  // top
    matrix.setRow(5, 13);  // center
    matrix.setRow(4, 2);  // center
    matrix.setRow(3, 12); // center
    matrix.setRow(2, 1);  // center
    matrix.setRow(1, 14);  // center
    matrix.setRow(0, 1);  // bottom
    break;
  case 15:
    matrix.setRow(6, 2);  // top
    matrix.setRow(5, 10);  // center
    matrix.setRow(4, 10);  // center
    matrix.setRow(3, 12); // center
    matrix.setRow(2, 5);  // center
    matrix.setRow(1, 5);  // center
    matrix.setRow(0, 1);  // bottom
    break;
  case 16:
    matrix.setRow(6, 3);  // top
    matrix.setRow(5, 2);  // center
    matrix.setRow(4, 12);  // center
    matrix.setRow(3, 12); // center
    matrix.setRow(2, 12);  // center
    matrix.setRow(1, 1);  // center
    matrix.setRow(0, 1);  // bottom
    break;
   case 17:
    matrix.setRow(6, 7);  // top
    matrix.setRow(4, 5);  // center
    matrix.setRow(3, 12); // center
    matrix.setRow(2, 10);  // center
    matrix.setRow(0, 11);  // bottom
    break;
  case 18:
    matrix.setRow(6, 14);  // top
    matrix.setRow(4, 1);  // center
    matrix.setRow(3, 15); // center
    matrix.setRow(2, 2);  // center
    matrix.setRow(0, 13);  // bottom
    break;
  case 19:
    matrix.setRow(6, 10);  // top
    matrix.setRow(3, 15); // center
    matrix.setRow(0, 5);  // bottom
    break;
  case 20:
    matrix.setRow(6, 12);  // top
    matrix.setRow(4, 1);  // center
    matrix.setRow(3, 3); // center
    matrix.setRow(2, 2);  // center
    matrix.setRow(0, 12);  // bottom
    break;
  case 21:
    matrix.setRow(6, 5);  // top
    matrix.setRow(4, 5);  // center
    matrix.setRow(2, 10);  // center
    matrix.setRow(0, 10);  // bottom
    break;
  case 22:
    matrix.setRow(6, 1);  // top
    matrix.setRow(5, 2);  // center
    matrix.setRow(4, 12);  // center
    matrix.setRow(2, 12);  // center
    matrix.setRow(1, 1);  // center
    matrix.setRow(0, 2);  // bottom
    break;
  case 23:
    matrix.setRow(5, 10);  // center
    matrix.setRow(4, 10);  // center
    matrix.setRow(2, 5);  // center
    matrix.setRow(1, 5);  // center
    break;
  case 24:
    matrix.setRow(5, 13);  // center
    matrix.setRow(4, 2);  // center
    matrix.setRow(2, 1);  // center
    matrix.setRow(1, 14);  // center
    break;
  case 25:
    matrix.setRow(5, 10);  // center
    matrix.setRow(4, 10);  // center
    matrix.setRow(2, 5);  // center
    matrix.setRow(1, 5);  // center
    break;
  case 26:
    matrix.setRow(6, 1);  // top
    matrix.setRow(5, 2);  // center
    matrix.setRow(4, 12);  // center
    matrix.setRow(2, 12);  // center
    matrix.setRow(1, 1);  // center
    matrix.setRow(0, 2);  // bottom
    break;
  case 27:
    matrix.setRow(6, 5);  // top
    matrix.setRow(4, 5);  // center
    matrix.setRow(2, 10);  // center
    matrix.setRow(0, 10);  // bottom
    break;
  case 28:
    matrix.setRow(6, 12);  // top
    matrix.setRow(4, 1);  // center
    matrix.setRow(3, 3); // center
    matrix.setRow(2, 2);  // center
    matrix.setRow(0, 12);  // bottom
    break;
  case 29:
    matrix.setRow(6, 10);  // top
    matrix.setRow(3, 15); // center
    matrix.setRow(0, 5);  // bottom
    break;
  case 30:
    matrix.setRow(6, 12);  // top
    matrix.setRow(4, 1);  // center
    matrix.setRow(3, 3); // center
    matrix.setRow(2, 2);  // center
    matrix.setRow(0, 12);  // bottom
    break;
  case 31:
    matrix.setRow(6, 5);  // top
    matrix.setRow(4, 5);  // center
    matrix.setRow(2, 10);  // center
    matrix.setRow(0, 10);  // bottom
    break;
  case 32:
    matrix.setRow(6, 1);  // top
    matrix.setRow(5, 2);  // center
    matrix.setRow(4, 12);  // center
    matrix.setRow(2, 12);  // center
    matrix.setRow(1, 1);  // center
    matrix.setRow(0, 2);  // bottom
    break;
  case 33:
    matrix.setRow(5, 10);  // center
    matrix.setRow(4, 10);  // center
    matrix.setRow(2, 5);  // center
    matrix.setRow(1, 5);  // center
    break;
   case 34:
    matrix.setRow(5, 13);  // center
    matrix.setRow(4, 2);  // center
    matrix.setRow(2, 1);  // center
    matrix.setRow(1, 14);  // center
    break; 
    case 35:
    matrix.setRow(5, 5);  // center
    matrix.setRow(1, 10);  // center
    break;
  default:
    // statements
    break;
}
}
