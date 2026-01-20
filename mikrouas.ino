#define WATER_PIN   A0

#define TRIG_FRONT  5
#define ECHO_FRONT  4

#define LED_RED     6
#define LED_GREEN   8
#define BUZZER_PIN  7
#define MOTOR_PIN   9

long duration;
int distanceFront;
int waterValue;

int readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return 999;

  return duration * 0.034 / 2;
}

void setSafeMode() {
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(MOTOR_PIN, LOW);
}

void setDangerMode(bool blink, int onTime, int offTime) {
  digitalWrite(LED_GREEN, LOW);

  digitalWrite(LED_RED, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(MOTOR_PIN, HIGH);

  if (blink) {
    delay(onTime);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(MOTOR_PIN, LOW);
    delay(offTime);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  setSafeMode(); // kondisi awal aman
}

void loop() {
  waterValue    = analogRead(WATER_PIN);
  distanceFront = readUltrasonic(TRIG_FRONT, ECHO_FRONT);

  Serial.print("Water: ");
  Serial.print(waterValue);
  Serial.print(" | Front: ");
  Serial.println(distanceFront);

  // ===== 1. AIR (PRIORITAS TERTINGGI) =====
  if (waterValue > 400) {
    setDangerMode(true, 300, 200);
    return;
  }

  // ===== 2. OBJEK DEPAN =====
  if (distanceFront < 20) {
    setDangerMode(false, 0, 0); // nyala terus
    return;
  }
  else if (distanceFront < 50) {
    setDangerMode(true, 150, 150);
    return;
  }
  else if (distanceFront < 100) {
    setDangerMode(true, 300, 300);
    return;
  }

  // ===== 3. AMAN =====
  setSafeMode();
  delay(100);
}
