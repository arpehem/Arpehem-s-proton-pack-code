// This code has initially been created entirelly by Arpehem and is free to be used, modified or updated by anyone as long as this credit remains.
// This code is compatible a smoker for the N-filter but is optional.

#include <DFPlayerMini_Fast.h>
#include <FastLED.h>
#include <SoftwareSerial.h>
#define POWERCELL_LED_PIN 3
#define CYCLOTRON_LED_PIN 2
# define CELL_LEDS 16
# define Cyclotron_LEDS 4
# define NUM_LEDS 20
# define NUM_STRIPS 2

int i =0;
int V = 90;
int y = 16;
int powercell_min = 0;
int powecell_cycle = -1;
int powercell = 0;
int cyclotron = 0;
int cyclotron_reset = 0;
int Volume=20;
int smokepin=5;
int purgepin=4;
bool dim;
bool alarmbool=false;
bool smoking = false;
bool smoke = false;
int dim_count;
float gBrightness = 5;
unsigned long myTime;
unsigned long previousMyTime = 0;
unsigned long previousdimTime = 0;
CRGB cell_leds[CELL_LEDS]; // Powercell LED array
CRGB cyclo_leds[Cyclotron_LEDS];
CLEDController *controllers[NUM_STRIPS]; // Controller array
enum currentstate{off,boot,cycle,fire,overloading,overload,overheat,music,volumep,ventp}state;
char message;
bool booted=false;

SoftwareSerial mySerial(11, 10); // RX, TX
DFPlayerMini_Fast myMP3;

void setup() {
  Serial.begin(9600);
mySerial.begin(9600);
myMP3.begin(mySerial, true);
pinMode(smokepin, OUTPUT);
pinMode(purgepin, OUTPUT);
digitalWrite(smokepin, LOW);
digitalWrite(purgepin, LOW);
  controllers[0] = &FastLED.addLeds<WS2812, POWERCELL_LED_PIN, GRB>(cell_leds, CELL_LEDS); // Assign Powercell LED to controller 0
  controllers[1] = &FastLED.addLeds<WS2812, CYCLOTRON_LED_PIN, RGB>(cyclo_leds, Cyclotron_LEDS); // Assign Cyclotron LED to controller 1
  FastLED.setMaxPowerInVoltsAndMilliamps(5,500);
  FastLED.clear();
  FastLED.show();
FastLED.setBrightness(75);
myMP3.volume(Volume);

}
void loop() {
    myTime = millis();

  if(Serial.available()){
    message=Serial.read();

  if(isAlpha(message)){
    messageswitch(message);

  }
else{
    if(state==volumep){
          Volume=message;
    myMP3.volume(Volume);
    }
    else{
      int translator=message;
      
      V=max(map(translator,1,35,0,90),15);
    }

    

}
  }
  if (myTime > previousMyTime+90+20&&booted==true&&state==overheat){
    alarm();
        if (smoke==true){
digitalWrite(smokepin, HIGH);
  }
    
    smoking=true;

  }
  if (myTime > previousMyTime+90+20&&booted==true&&state==overload){
    alarm();
    
  }
  if (myTime > previousMyTime+V&&booted==true&&state!=overload&&state!=overheat){
    cyclesequence();
     
  }

//debug();

}
void messageswitch(char var){
  switch (var) {
      case 'x':
    smoke=false;
    break;
      case 'y':
    smoke=true;
    break;
  case 'b':
    bootsequence();
    break;
  case 'f':
  if (state!=off){
        shutdownsequence();        
        }
    
    break;
      case 'm':
      if (state!=music){
        myMP3.repeatFolder(2);          
        state=music;
      }
      else{
        state=cycle;
        loopsound(2);
      }

    break;
          case 'n':
         myMP3.playNext();

    break;
          case 's':
      if(state!=fire){
        state=fire;
        loopsound(11);
      }
    break;
              case 'c':
      if(state!=cycle&&state!=music&&state!=volumep){
        state=cycle;
        loopsound(2);
         }
    break;
    case 'h':
      if(state!=overheat){
        state=overheat;
        loopsound(16);
       
         }
    break;
        case 'o':
      if(state!=overload){
        state=overload;
        
         }
    break;
            case 'v':
      if(state!=ventp){
        state=ventp;
        vent();
        loopsound(2);
        
         }
    break;
    case 'V':
      if(state!=volumep){
        state=volumep;
        advertise(8);
      }
  default:
    // statements
    break;
}
}
void vent(){
  if (smoking==false&&smoke==true){
    digitalWrite(smokepin, HIGH);
    delay(500);
  }
  playsound(9);
    if (smoke==true){
digitalWrite(purgepin, HIGH);
  }

  gBrightness=200;
  FastLED.clear();
  FastLED.show();
  for(y=0;y<16;y++){
    cell_leds[y] = CRGB(0,0,255);
    controllers[0]->showLeds(gBrightness);
  }
    for(y=0;y<4;y++){
    cyclo_leds[y] = CRGB(255,0,0);
    controllers[1]->showLeds(gBrightness);
  }

//  for (y=0;y<34;y++){
//    gBrightness=gBrightness-8;
//    cell_leds[max(15-y/2,0)] = CRGB(0,0,0);
//    controllers[0]->showLeds(75);
//    controllers[1]->showLeds(max(gBrightness,0));
//    delay(100);
//  }
delay(750);
for(cyclotron=0;cyclotron<4;cyclotron++){
 // for (int k=0; k<7;k++){
cyclo_leds[cyclotron] = CRGB(0,0,0);
controllers[1]->showLeds(gBrightness);
//}
delay(650);
}
 i =0;
 V = 90;
 y = 16;
 powercell_min = 0;
 powecell_cycle = -1;
 powercell = 0;
 cyclotron = 0;
 cyclotron_reset = 0;
    FastLED.clear();
  FastLED.show();
  purge_powercell();
  previousMyTime = myTime;
  digitalWrite(smokepin, LOW);
  smoking = false;
  digitalWrite(purgepin, LOW);
  state=cycle;
  loopsound(2);
  
}
void alarm(){

    gBrightness=175;
  if(alarmbool==false){
    FastLED.clear();
      for (y=0;y<9;y++){
    cell_leds[y*2] = CRGB(0,0,255);
    alarmbool=true;
    controllers[0]->showLeds(gBrightness);
    
    
  }
      for (int z=0;z<4;z++){
      cyclo_leds[z] = CRGB(255,0,0);
      controllers[1]->showLeds(gBrightness);
    }
  }
else{
  FastLED.clear();
    for (y=1;y<9;y++){
    cell_leds[y*2-1] = CRGB(0,0,255);
    cyclo_leds[Cyclotron_LEDS] = CRGB(0,0,0);
    alarmbool=false;
    controllers[0]->showLeds(gBrightness);
    controllers[1]->showLeds(0);
    }
  for (int z=0;z<4;z++){
      cyclo_leds[z] = CRGB(0,0,0);
      controllers[1]->showLeds(0);
    }  
  }
  FastLED.show();
  previousMyTime = myTime;

}
void playsound(int sound){
    mySerial.begin(9600);
 // delay(50);
  myMP3.play(sound); 
  delay(50);
    Serial.begin(9600);
 
}
void advertise(int sound){
    mySerial.begin(9600);
 // delay(50);
  myMP3.playAdvertisement(sound); 
  delay(50);
    Serial.begin(9600);
 
}
void bootsequence(){
 // Serial.print("boot");
 // Serial.println();
 playsound(1);
  for(int b=0; b<15;b++){
    for (int L=0; L<4;L++){
      cyclo_leds[L] = CRGB(255,0,0);
    }
 
for (int y=15; y>=powercell_min;y--){
  cell_leds[y] = CRGB(0,0,255);
 gBrightness=pow(gBrightness,1.008);
 gBrightness=min(gBrightness,150);
  controllers[0]->showLeds(max(gBrightness,75));
  controllers[1]->showLeds(min(gBrightness,225));
  cell_leds[min(y+1,15)] = CRGB(0,0,0);
  delay(25);
  }
  powercell_min++;
  }
  purge_powercell();
  for(int Z=0; Z<28;Z++){
cyclo_leds[Z] = CRGB(0,0,0);
}
    booted=true;
     state=cycle;
        loopsound(2);

  }
  void shutdownsequence(){
playsound(10);
gBrightness=200;
for(int y=15; y>0 ; y--){
  cell_leds[y] = CRGB(0,0,255);
}
for(int y=4; y>0 ; y--){
  cyclo_leds[y-1] = CRGB(255,0,0);
}

controllers[0]->showLeds(gBrightness);
controllers[1]->showLeds(gBrightness);

for(int y=15; y>0 ;y--){
  cell_leds[y] = CRGB(0,0,0);
  gBrightness=(max(gBrightness-13,0));
  controllers[0]->showLeds(min(gBrightness,100));
  controllers[1]->showLeds(gBrightness);
  delay(150);
}
FastLED.clear();
FastLED.show();   
 
    booted=false;
    state=off;
    gBrightness=5;

i =0;
V = 90;
y = 16;
powercell_min = 0;
powecell_cycle = -1;
powercell = 0;
cyclotron = 0;
cyclotron_reset = 0;
 //purge_powercell();
  }
void purge_powercell(){
for(int j=0; j<16;j++){
cell_leds[j] = CRGB(0,0,0);
i=-1;
controllers[0]->showLeds(75);
}
}
void loopsound(int sound){
    mySerial.begin(9600);
  delay(50);
  myMP3.loop(sound); 
  delay(50);
    Serial.begin(9600);
  
}
void cyclesequence(){
    powecell_cycle++;
    powercell = powecell_cycle;
    previousMyTime = myTime;
   
if (powecell_cycle > 15){
  powercell=powercell-16;
} 
cyclotron = map(powecell_cycle,-1,32,0,4);
cyclotron_reset = cyclotron-1;
    if (cyclotron_reset ==-1){
      cyclotron_reset = 3;
    }
cell_leds[powercell] = CRGB(0,0,255);

//for (int k=0; k<7;k++){
  cyclo_leds[cyclotron] = CRGB(255,0,0);
cyclo_leds[cyclotron_reset] = CRGB(0,0,0);
//}

if (powecell_cycle == 31){
    powecell_cycle=-1;
    }
if (powercell == 15){
  purge_powercell();
  }
 controllers[1]->showLeds(50);
 controllers[0]->showLeds(50);

  
}
void debug(){
Serial.print("powecell_cycle:");
Serial.print(powecell_cycle);
Serial.println();
Serial.print("myTime:");
Serial.print(myTime);
Serial.println();
Serial.print("previousMyTime:");
Serial.print(previousMyTime);
Serial.println();
Serial.print("powercell:");
Serial.print(powercell);
Serial.println();
Serial.print("cyclotron:");
Serial.print(cyclotron);
Serial.println();
}
