#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define dht_pin 7 
#define dht_type  DHT11 
DHT dht = DHT(dht_pin, dht_type);

LiquidCrystal_I2C lcd(0x27,16,2);  
RTC_PCF8563 rtc;

uint8_t ora;
uint8_t  minut;
uint8_t  secunda;
uint8_t zi;
uint8_t luna;
uint16_t  an;
char zs;

int buttonPin1 = 2;
int buttonPin2 = 3;
int buttonPin3 = 4;

int buzzer_pin = 8;

int zile_luna [] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char zi_sapt[] = {'D', 'L', 'M', 'M', 'J', 'V', 'S'};
const String setari[] = {" ", "O", "M", "S", "Z", "L", "A", "12", "24"};
int i = 0, format_ora = 24;
int stare_button1 = 0;
int stare_button2 = 0;
int stare_button3 = 0;


byte grad[8] = { // caracter gradul de la Celsius
  B00110,
  B01001,
  B01001,
  B00110,
  0,0,0,0
};

void setup() {
  Serial.begin(9600);

  rtc.begin();
  dht.begin();
 
  lcd.init();
  lcd.backlight(); 

  if (rtc.isrunning() == 1){
    //lcd.print("RCT DA");
    //rtc.adjust(DateTime(2021,11,29,19,20,30));
  } 
  else lcd.print("RTC NU");

  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);

  lcd.createChar(0, grad);

  //pinMode(buzzer_pin, INPUT);
}

void loop() {
  
   DateTime clk = rtc.now();
   
   
   ora =  clk.hour();
   minut = clk.minute();
   secunda = clk.second();
   zi = clk.day();
   luna = clk.month();
   an = clk.year();
   zs = zi_sapt[clk.dayOfTheWeek()];

   stare_button3 = digitalRead(buttonPin3);
   stare_button2 = digitalRead(buttonPin2);
   stare_button1 = digitalRead(buttonPin1);

   if(ora == 6 && minut == 30){
      tone(buzzer_pin, 600, 500);
    }
   
   display_setari(clk);
  
    rtc.adjust(DateTime(an,luna,zi,ora,minut,secunda));
    
    display_temperatura();

    delay(1000);
}

void display_temperatura(){ //afisare tempertura
    float  temp = dht.readTemperature();
 
    lcd.setCursor(9,0);
    lcd.print(temp);
    lcd.write(byte(0)); 
    lcd.print("C");


  }

void diplay_data_timp(DateTime clk){
   display_ora(clk);
   lcd.print(":");
   display_minut(clk);
   lcd.print(":");
   display_secunda(clk);

   
   display_zi(clk);
   lcd.setCursor(2,1);
   lcd.print("-");
   display_luna();
   lcd.print("-");
   display_an();

   display_zi_sapt();
   
 }

 void display_ora(DateTime clk){
   lcd.setCursor(0,0);
   if(ora < 10) lcd.print("0");
   if(format_ora == 12){
    if(ora == 13){ ora = 0; zi++; display_zi(clk); display_ora(clk);}
    else lcd.print(ora);
   }
   else if(format_ora == 24) {
    if(ora == 24){ ora = 0; zi++; display_zi(clk); display_ora(clk);}
    else lcd.print(ora);
    }
  }

  void display_minut(DateTime clk){
   lcd.setCursor(3,0);
   if(minut < 10) lcd.print("0");
   if(minut == 60){ minut = 0; ora++; display_ora(clk); display_minut(clk);}
   else lcd.print(minut);
  }

  void display_secunda(DateTime clk){
   lcd.setCursor(6,0);
   if(secunda < 10) lcd.print("0");
   if(secunda == 59){ secunda = 0; minut++; display_minut(clk); display_secunda(clk);}
   else lcd.print(secunda);
  }

   void display_zi(DateTime clk){
   int j = 0;
   lcd.setCursor(0,1);
   if(zi < 10) lcd.print("0");
   for(j = 1; j <= 12; j++){
      if(luna == j){
        if(zile_luna[j] == zi){
          zi = 1; luna++; 
          display_luna(); display_zi(clk); }
        else lcd.print(zi);
        
        break;
      }
    }
  }

  void display_luna(){
   lcd.setCursor(3,1);
   if(luna < 10) lcd.print("0");
   if(luna == 13){ luna = 1; display_luna(); an++; display_an();}
   else lcd.print(luna);
  }

  void display_an(){
   lcd.setCursor(6,1);
   lcd.print(an);
  }

  void display_zi_sapt(){
    lcd.setCursor(11,1);
    lcd.print(zs);
  }

 void modifica_data_timp(DateTime clk){

     if(stare_button2 == HIGH){
       if(i == 1){
          ora++;
          display_ora(clk);}
       if(i == 2){
          minut++;
          display_minut(clk);}
        if(i == 3){
          secunda++;
          display_secunda(clk);}
       if(i == 4){
          zi++;
          display_zi(clk);
          }
        if(i == 5){
          luna++;
          display_luna();}
        if(i == 6){
          an++;
          display_an();}
        if(i == 7){
          format_ora = 12;
          ora = clk.twelveHour();
          //if(clk.isPM()){lcd.setCursor(14,1); lcd.print("PM");}
          //else {lcd.setCursor(14,1);lcd.print("AM");}
          display_ora(clk);
          }
        if(i == 8){
            format_ora = 24;
            ora = clk.hour();
            if(!clk.isPM()) { ora = ora + 12;}
            //lcd.setCursor(14,1); lcd.print("   ");
            display_ora(clk);
        }
      }
 }

void delete_space(){
    if(i == 0) {lcd.setCursor(14,1);lcd.print(" ");}
    lcd.setCursor(13,1); //afisare caracterul ce se noreste a fis modificat
    lcd.print(setari[i]);
  }


 void display_setari(DateTime clk){
    lcd.setCursor(13,1); //afisare caracterul ce se noreste a fis modificat
    lcd.print(setari[i]);
    
    
    if(stare_button1 == HIGH || setari[i] == " "){
      //noTone(buzzer_pin);
      i = 0;
      rtc.start();
      if(format_ora == 12){
        ora = clk.twelveHour();
        if(!clk.isPM()){lcd.setCursor(14,1); lcd.print("PM");}
        else {lcd.setCursor(14,1);lcd.print("AM");}
        }
      else if(format_ora == 24){
        ora = clk.hour();
        //if(!clk.isPM()) { ora = ora + 12;}
        lcd.setCursor(14,1); lcd.print("   ");}
      diplay_data_timp(clk);
      }
    else {
      rtc.stop(); 
      if(setari[i] == "12" || setari[i] == "24"){
        lcd.setCursor(15,1); lcd.print(" ");} 
      else {lcd.setCursor(14,1); lcd.print("  ");}
      modifica_data_timp(clk);}
      
    if(stare_button3 == HIGH){ // schimbarea setarilor
      i++;
      if(i == 9) i = 0;
    }
 }

 
