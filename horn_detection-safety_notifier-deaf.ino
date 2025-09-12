
#define TRIG_PIN_L 5
#define ECHO_PIN_L 18

#define TRIG_PIN_C 23
#define ECHO_PIN_C 25

#define TRIG_PIN_R 26
#define ECHO_PIN_R 27


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(TRIG_PIN_L, OUTPUT);
  pinMode(TRIG_PIN_C, OUTPUT);
  pinMode(TRIG_PIN_R, OUTPUT);

  pinMode(ECHO_PIN_L, INPUT);
  pinMode(ECHO_PIN_C, INPUT);
  pinMode(ECHO_PIN_R, INPUT);
}

float distance(int trig_pin,int echo_pin){
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig_pin, LOW);

  // timeout of 30000 microsecond  which is 5 meter range otherwise function may hang over since no echo comeback if distance is large in real traffics
  long duration = pulseIn(echo_pin, HIGH, 30000);
 
  if (duration == 0) return 999; // no echo ~ treat as far
  // Converting into cm and dividing it by two since distance covered was 2 times by wave
  return (duration * 0.0343/2);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  float distance_L = distance(TRIG_PIN_L, ECHO_PIN_L);
  float distance_C = distance(TRIG_PIN_C, ECHO_PIN_C);
  float distance_R = distance(TRIG_PIN_R, ECHO_PIN_R);

  if(distance_L < 50 || distance_R < 50 || distance_C < 50){
    Serial.print("Watch out All Sides");
  }
  delay(200);
}
