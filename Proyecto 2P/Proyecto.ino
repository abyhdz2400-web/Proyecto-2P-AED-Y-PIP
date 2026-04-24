#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// -------- CONFIGURACIÓN --------
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int LED = 13;
const int BUZZ = 8;
const int ALERTA_TEMP = 25; // °C

// -------- SETUP --------
void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);

  Serial.begin(9600);
  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.print("Iniciando...");
  delay(1500);
}

// -------- LOOP --------
void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  lcd.clear();

  // Validar lectura
  if (isnan(temp) || isnan(hum)) {
    lcd.setCursor(0, 0);
    lcd.print("Error sensor");
    
    digitalWrite(LED, LOW);
    digitalWrite(BUZZ, LOW);

    Serial.println("Error sensor");
    delay(1000);
    return;
  }

  // Mostrar en LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(hum);
  lcd.print(" %");

  // Monitor serial
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" C  Hum: ");
  Serial.print(hum);
  Serial.println(" %");

  // Alerta
  bool alerta = (temp > ALERTA_TEMP);

  digitalWrite(LED, alerta);

  if (alerta) {
    tone(BUZZ, 1000); // sonido buzzer
  } else {
    noTone(BUZZ);
  }

  delay(1000);
}