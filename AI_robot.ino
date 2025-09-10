
#include <WiFi.h>

// Setting the Neccessary Library
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>

// l298n Driver Pins
#define in1 25
#define in2 26
#define in3 27
#define in4 14

// Encoder pins
#define encoderPinA1 = 34; // Encoder for in1 and in2 motor channel
#define encoderPinA2 = 35; // Encoder for in3 and in4 motor channel
volatile long stepCount = 0;

// Wheel Setup
const int step_per_rev = 20;       // Encoder pulse count is 20
const float wheel_diameter = 0.066;  // Diameter of BO motor wheel is 0.066 meter or 6.6 centi-meter
const float wheel_circumference = 3.1416 * wheel_diameter; 

// Timing to find velocity
unsigned long lastTime = 0;
long lastTickCount = 0;

const char* ssid = "Yekjheti";
const char* pass = "H3KWGU42";

const char* host = "192.168.100.54";
const uint16_t port = 12345;

// Initializing the Library Object
WiFiClient client;
// Adafruit_MPU6050 mpu;

void IRAM_ATTR encoderISR(){
  stepCount++;
} 

void forward(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void left(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void right(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stopMotors(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void setup(){
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // Enabling the WiFi
  Serial.print("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  if(!client.connect(host, port)){
    Serial.println("Connection to server failed");
    return;
  }
  Serial.println("Connected to Server");

  // Enabling the MPU sensor
  if(mpu.begin()){
    Serial.println("MPU6050 Found!");
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  // mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(ENCODER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA1), encoderISR, RISING);
  lastTime = millis();
}

void loop(){

  // Calculate the distance covered by wheel
  long currentStep = stepCount;
  float revolutions = (float)currentStep/ step_per_rev;
  float distance = revolutions * wheel_circumference;  // Using step count of encoder and multiplying it with wheel's cirmcumference gives the distance covered by wheel

  // Calculate velocity every 500 ms
  unsigned long now = millis();
  if (now-lastTime >= 500){
    long stepInInterval = currentStep - lastStepCount;
    float revs = (float)stepInInterval / step_per_rev;
    float dist = revs * wheel_circumference;
    float velocity_meter_per_sec = (dist/(now-lastTime))*1000.0;

    lastStepCount = currentStep;
    lastTime = now;
  }

  if(client.available()){
    String command = client.readStringUntil('\n');
    command.trim();

    int spaceIdx = command.indexOf(' ');
    String name;
    int time = 0;

    if (spaceIdx > 0) {
      name = command.substring(0, spaceIdx);
      time = command.substring(spaceIdx + 1).toInt();
    } else {
      name = command; // only direction, no number
    }

    Serial.print("Command: ");
    Serial.println(name);
    Serial.print("Time: ");
    Serial.println(time);

    if(name == "forward"){
      forward();
      delay(time);
      stopMotors();
    }
    else if(name == "backward"){
      backward();
      delay(time);
      stopMotors();
    }
    else if(name == "left"){
      left();
      delay(time);
      stopMotors();
    }
    else if(name == "right"){
      right();
      delay(time);
      stopMotors();
    }
  }
  // sensors_event_t a, g, temp;
  // mpu.getEvent(&a, &g, &temp);

  // String accelData = String(a.acceleration.x)+ "," + String(a.acceleration.y) + "\n";
  // client.print(accelData);
}
