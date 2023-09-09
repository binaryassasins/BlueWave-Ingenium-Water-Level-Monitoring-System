/*Water level monitoring system with leak detection and automatic water restoration (BlueWave Ingenium)
https://srituhobby.com
https://github.com/binaryassasins
*/
//Include the library files
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL36NWPZej"
#define BLYNK_TEMPLATE_NAME "Water Level Monitoring System"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);
char auth[] = "xEu-8YMNt42vj_lXohByTE7N0LPM5HIK";//Enter your Auth token
char ssid[] = "SITIHOME@unifi";//Enter your WIFI name
char pass[] = "0392813931def";//Enter your WIFI password
BlynkTimer timer;
// Define the component pins
#define trig 13   // Replace D7 with the correct GPIO number
#define echo 15   // Replace D8 with the correct GPIO number
#define LED1 16  // Replace D0 with the correct GPIO number
#define LED2 0   // Replace D3 with the correct GPIO number
#define LED3 2   // Replace D4 with the correct GPIO number
#define LED4 14  // Replace D5 with the correct GPIO number
#define LED5 12  // Replace D6 with the correct GPIO number
#define relay 0
//Enter your tank max value(CM)
int MaxLevel = 20;
int Level1 = (MaxLevel * 75) / 100;
int Level2 = (MaxLevel * 65) / 100;
int Level3 = (MaxLevel * 55) / 100;
int Level4 = (MaxLevel * 45) / 100;
int Level5 = (MaxLevel * 35) / 100;
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  lcd.setCursor(0, 0);
  lcd.print("Water level");
  lcd.setCursor(4, 1);
  lcd.print("Monitoring");
  delay(4000);
  lcd.clear();
  //Call the functions
  timer.setInterval(100L, ultrasonic);
}
//Get the ultrasonic sensor values
void ultrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  int distance = t / 29 / 2;
  int blynkDistance = (distance - MaxLevel) * -1;
  if (distance <= MaxLevel) {
    Blynk.virtualWrite(V0, blynkDistance);
  } else {
    Blynk.virtualWrite(V0, 0);
  }
  lcd.setCursor(0, 0);
  lcd.print("Level:");
  if (Level1 <= distance) {
    lcd.setCursor(8, 0);
    lcd.print("Very Low");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
  } else if (Level2 <= distance && Level1 > distance) {
    lcd.setCursor(8, 0);
    lcd.print("Low");
    lcd.print(" ");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
  } else if (Level3 <= distance && Level2 > distance) {
    lcd.setCursor(8, 0);
    lcd.print("Medium");
    lcd.print(" ");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
  } else if (Level4 <= distance && Level3 > distance) {
    lcd.setCursor(8, 0);
    lcd.print("High");
    lcd.print(" ");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, LOW);
  } else if (Level5 >= distance) {
    lcd.setCursor(8, 0);
    lcd.print("Full");
    lcd.print(" ");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, HIGH);
  }
}
//Get the button value
BLYNK_WRITE(V1) {
  bool Relay = param.asInt();
  if (Relay == 1) {
    digitalWrite(relay, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Motor is ON ");
  } else {
    digitalWrite(relay, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Motor is OFF");
    }
}

// Rain sensor module calibration -> retrieve the value to force shut down water pump
// void raindrops() {} 

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}