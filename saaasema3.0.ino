#define BLYNK_TEMPLATE_ID           "XXXX"
#define BLYNK_DEVICE_NAME           "Toppilan sääasema"
#define BLYNK_AUTH_TOKEN            "XXXX"
#define BLYNK_PRINT Serial
#define LEDpin1 9           
#define LEDpin2 16
#define LEDpin3 0
#define LEDpin4 2
#define oneWireBus1 14      
#define oneWireBus2 10

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <SHT31.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "XXXX";
char pass[] = "XXXX";

uint32_t start;
uint32_t stop;

int latchPin = 12;
int dataPin = 13;
int clockPin = 15;
const int valosensoripinni = A0;

OneWire oneWire1(oneWireBus1);
OneWire oneWire2(oneWireBus2);
DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);
SHT31 sht;
BlynkTimer timer;

Adafruit_BMP280 bmp;

void sendSensor()
{
  sht.read();
  float kosteussht = sht.getHumidity();
  float lamposht = sht.getTemperature();

  float korkeusbmp = bmp.readAltitude(1020.00);
  float painebmpPa = bmp.readPressure();
  float lampobmp = bmp.readTemperature();
  float painebmp = painebmpPa * 0.01;
  
  sensors1.requestTemperatures();
  float temperatureULKO = sensors1.getTempCByIndex(0);
  sensors2.requestTemperatures();
  float temperatureSISA = sensors2.getTempCByIndex(0);
  int valoarvo = analogRead(A0);                        
  
  Blynk.virtualWrite(V4, painebmp);
  Blynk.virtualWrite(V5, kosteussht);                  
  Blynk.virtualWrite(V6, lamposht);
  Blynk.virtualWrite(V7, temperatureSISA);
  Blynk.virtualWrite(V8, temperatureULKO);
  Blynk.virtualWrite(V9, valoarvo);
                                                       
  int tempd1;
  int tempd2;
  int tempd3;
  int tempd4;
  int tempd5;
  int numero2;
  int numero3;
  int numero4;

  float seg = temperatureULKO;

  if(seg <= -126){
    tempd1 = 255;
    tempd2 = 69;
    tempd3 = 69;
    tempd4 = 69;
  }

  else  {
    if(seg >= 0.0){
      tempd1 = 0;
      tempd2 = (int)((int)seg / 10) % 10;
      tempd3 = (int)seg % 10;
      tempd4 = (seg - ((tempd2 * 10) + tempd3)) * 10;
    }
    else if(seg <= -10.0){
      tempd1 = 64;
      tempd2 = ((int)((int)seg / 10) % 10) * -1;
      tempd3 = ((int)seg % 10) * -1;
      tempd4 = (seg - ((-tempd2 * 10) -tempd3)) * -10;
    } 
    else if(seg < 0.0){
      tempd1 = 0;
      tempd2 = 10;
      tempd3 = ((int)seg % 10) * -1;
      tempd4 = (seg + tempd3) * -10;
    }
  }

  switch (tempd2) {
    case 0:
      numero2 = 0;
      break;
    case 1:  // print 1
      numero2 = 6;
      break;
    case 2:  // print 2
      numero2 = 91;
      break;
    case 3:  // print 3
      numero2 = 79;
      break;
    case 4:  // print 4
      numero2 = 102;
      break;
    case 5:  // print 5
      numero2 = 109;
      break;
    case 6:  // print 6
      numero2 = 125;
      break;
    case 7:  // print 7
      numero2 = 7;
      break;
    case 8:  // print 8
      numero2 = 127;
      break;
    case 9:  // print 9
      numero2 = 111;
      break;
    case 10:
      numero2 = 64;
      break;
    case 69:
      numero2 = 255;
      break;
  }
 
  switch (tempd3) {
    case 0:  // print 0
      numero3 = 191;
      break;
    case 1:  // print 1
      numero3 = 134;
      break;
    case 2:  // print 2
      numero3 = 219;
      break;
    case 3:  // print 3
      numero3 = 207;
      break;
    case 4:  // print 4
      numero3 = 230;
      break;
    case 5:  // print 5
      numero3 = 237;
      break;
    case 6:  // print 6
      numero3 = 253;
      break;
    case 7:  // print 7
      numero3 = 135;
      break;
    case 8:  // print 8
      numero3 = 255;
      break;
    case 9:  // print 9
      numero3 = 239;
      break;
    case 69:
      numero3 = 255;
      break; 
  }
  
  switch (tempd4) {
    case 0:  // print 0
      numero4 = 63;
      break;
    case 1:  // print 1
      numero4 = 6;
      break;
    case 2:  // print 2
      numero4 = 91;
      break;
    case 3:  // print 3
      numero4 = 79;
      break;
    case 4:  // print 4
      numero4 = 102;
      break;
    case 5:  // print 5
      numero4 = 109;
      break;
    case 6:  // print 6
      numero4 = 125;
      break;
    case 7:  // print 7
      numero4 = 7;
      break;
    case 8:  // print 8
      numero4 = 127;
      break;
    case 9:  // print 9
      numero4 = 111;
      break;
    case 69:
      numero4 = 255;
      break;
  }
  
  for(int i = 0; i<2000; i++){
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, tempd1);
    digitalWrite(latchPin, HIGH);
    digitalWrite(LEDpin1, LOW);
    delay(1);
    digitalWrite(LEDpin1, HIGH);
 
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, numero2);
    digitalWrite(latchPin, HIGH);
    digitalWrite(LEDpin2, LOW);
    delay(1);
    digitalWrite(LEDpin2, HIGH);

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, numero3);
    digitalWrite(latchPin, HIGH);
    digitalWrite(LEDpin3, LOW);
    delay(1);
    digitalWrite(LEDpin3, HIGH);

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, numero4);
    digitalWrite(latchPin, HIGH);
    digitalWrite(LEDpin4, LOW);
    delay(1);
    digitalWrite(LEDpin4, HIGH);
  }

  Serial.println(tempd1);
  Serial.println(tempd2);
  Serial.println(tempd3);
  Serial.println(tempd4);
  Serial.println(" "); 
  Serial.println(temperatureULKO);
  Serial.println(temperatureSISA);
  Serial.println(valoarvo);
  Serial.println(" ");
  Serial.println(kosteussht);
  Serial.println(lamposht);
  Serial.println(" ");
  Serial.println(korkeusbmp);
  Serial.println(painebmp);
  Serial.println(lampobmp);
  Serial.println(" ");
  Serial.println(" ");
  
}



void setup()
{
  Serial.begin(115200);
  
  sensors1.begin();
  sensors2.begin();
  Blynk.begin(auth, ssid, pass);
  
  Wire.begin();
  sht.begin(0x45);
  Wire.setClock(100000);
  uint16_t stat = sht.readStatus();

  unsigned status;
  status = bmp.begin(0x76);

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  
  timer.setInterval(500L, sendSensor);
  
  pinMode(LEDpin1, OUTPUT);
  pinMode(LEDpin2, OUTPUT);
  pinMode(LEDpin3, OUTPUT);
  pinMode(LEDpin4, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  
}

void loop()
{
  Blynk.run();
  timer.run();
 
}
