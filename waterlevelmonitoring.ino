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
#define raindropDO 14
#define relay 12
//Enter your tank max value(CM)
int MaxLevel = 20;
int Level1 = (MaxLevel * 85) / 100; //(MaxLevel * 75) / 100;
int Level2 = (MaxLevel * 70) / 100; //(MaxLevel * 65) / 100;
int Level3 = (MaxLevel * 50) / 100; //(MaxLevel * 55) / 100;
int Level4 = (MaxLevel * 35) / 100; //(MaxLevel * 45) / 100;
int Level5 = (MaxLevel * 25) / 100; //(MaxLevel * 35) / 100;
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(relay, OUTPUT);
  // raindrop calibration
  pinMode(raindropDO, INPUT);
  digitalWrite(relay, HIGH); //digitalWrite(relay, HIGH)
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  lcd.setCursor(0, 0);
  lcd.print("Water level");
  lcd.setCursor(4, 1);
  lcd.print("Monitoring");
  delay(4000);
  lcd.clear();
  lcd.print("BlueWave");
  lcd.setCursor(4, 1);
  lcd.print("Ingenium");
  delay(4000);
  lcd.clear();
  //Call the functions
  timer.setInterval(100L, ultrasonic);
  raindrops();
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
  lcd.print("                ");
  if (Level1 <= distance) {
    lcd.setCursor(8, 0);
    lcd.print("Very Low");
    lcd.print(" ");
    if (raindrops() == "No Leak Detected.") {
      digitalWrite(relay, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Pump is ON ");
      Blynk.virtualWrite(V3, HIGH);
      lcd.print("               ");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print(raindrops());
      lcd.print("               ");
      delay(1500);
    } else if (raindrops() == "Leak Detected!") {
      digitalWrite(relay, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Pump is OFF ");
      Blynk.virtualWrite(V3, LOW);
      lcd.print("               ");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print(raindrops());
      lcd.print("               ");
      delay(1500);
    } 
  } else if (Level2 <= distance && Level1 > distance) {
    lcd.setCursor(8, 0);
    lcd.print("Low");
    lcd.print(" ");
    if (raindrops() == "No Leak Detected.") {
      digitalWrite(relay, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Pump is ON ");
      Blynk.virtualWrite(V3, HIGH);
      lcd.print("               ");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print(raindrops());
      lcd.print("               ");
      delay(1500);
    } else if (raindrops() == "Leak Detected!") {
      digitalWrite(relay, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Pump is OFF ");
      Blynk.virtualWrite(V3, LOW);
      lcd.print("               ");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print(raindrops());
      lcd.print("               ");
      delay(1500);
    }
  } else if (Level3 <= distance && Level2 > distance) {
    lcd.setCursor(8, 0);
    lcd.print("Medium");
    lcd.print(" ");
    if (raindrops() == "No Leak Detected.") {
      digitalWrite(relay, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Pump is OFF ");
      Blynk.virtualWrite(V3, LOW);
      lcd.print("               ");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print(raindrops());
      lcd.print("               ");
      delay(1500);
    } else if (raindrops() == "Leak Detected!") {
      digitalWrite(relay, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Pump is OFF ");
      Blynk.virtualWrite(V3, LOW);
      lcd.print("               ");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print(raindrops());
      lcd.print("               ");
      delay(1500);
    }
  } else if (Level4 <= distance && Level3 > distance) {
    lcd.setCursor(8, 0);
    lcd.print("High");
    lcd.print(" ");
    if (raindrops() == "No Leak Detected.") {
      digitalWrite(relay, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Pump is OFF ");
      Blynk.virtualWrite(V3, LOW);
      lcd.print("               ");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print(raindrops());
      lcd.print("               ");
      delay(1500);
    } else if (raindrops() == "Leak Detected!") {
      digitalWrite(relay, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Pump is OFF ");
      Blynk.virtualWrite(V3, LOW);
      lcd.print("               ");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print(raindrops());
      lcd.print("               ");
      delay(1500);
    }
  } else if (Level5 >= distance) {
    lcd.setCursor(8, 0);
    lcd.print("Full");
    lcd.print(" ");
    if (raindrops() == "No Leak Detected.") {
      digitalWrite(relay, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Pump is OFF ");
      Blynk.virtualWrite(V3, LOW);
      lcd.print("               ");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print(raindrops());
      lcd.print("               ");
      delay(1500);
    } else if (raindrops() == "Leak Detected!") {
      digitalWrite(relay, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Pump is OFF ");
      Blynk.virtualWrite(V3, LOW);
      lcd.print("               ");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print(raindrops());
      lcd.print("               ");
      delay(1500);
    }
  }
}

// Rain sensor module calibration -> retrieve the value to force shut down water pump
const char* raindrops() {
  int raindropState = digitalRead(raindropDO);
  const char* stateText;
  if (raindropState == LOW) {
    Blynk.virtualWrite(V2, "Leak Detected!");
    stateText = "Leak Detected!";
    // suspend auto pump
    //digitalWrite(relay, HIGH);
  } else {
    Blynk.virtualWrite(V2, "No Leak Detected.");
    stateText = "No Leak Detected.";
    //digitalWrite(relay, LOW);
  }
  return stateText;
} 

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}