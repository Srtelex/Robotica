int motor1Pin1 = 2;
int motor1Pin2 = 3;
int motor2Pin1 = 4;
int motor2Pin2 = 5;
int motor3Pin1 = 6;
int motor3Pin2 = 7;
int motor4Pin1 = 8;
int motor4Pin2 = 9;

int xPin = A1;
int yPin = A0;

int echoPin = 10;
int trigPin = 11;

const int redLed = 12;
const int yellowLed = 13;
const int greenLed = 14;

int buttonPin = 15;
bool isJoystickControl = false;
bool lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor3Pin1, OUTPUT);
  pinMode(motor3Pin2, OUTPUT);
  pinMode(motor4Pin1, OUTPUT);
  pinMode(motor4Pin2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);

  stopMotors();
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonState == LOW) {
      isJoystickControl = !isJoystickControl;
    }
  }

  lastButtonState = buttonState;

  if (isJoystickControl) {
    controlWithJoystick();
  } else {
    controlWithUltrasonic();
  }

  controlTrafficLight();

  delay(20);
  Serial.println(xPin);
}

void controlWithJoystick() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);

  if (yValue < 400) {
    moveForward();
  } else if (yValue > 600) {
    moveBackward();
  } else if (xValue < 400) {
    turnLeft();
  } else if (xValue > 600) {
    turnRight();
  } else {
    stopMotors();
  }
}

void controlWithUltrasonic() {
  long distance = measureDistance();

  Serial.print("Distância: ");
  Serial.println(distance);

  if (distance < 30) {
    turnLeft();
  } else {
    if (digitalRead(greenLed) == HIGH) {
      moveForward();
    } else {
      stopMotors();
    }
  }
}

void controlTrafficLight() {
  static unsigned long previousMillis = 0;
  static int state = 0;
  unsigned long currentMillis = millis();

  if (state == 0 && currentMillis - previousMillis >= 5000) {
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, HIGH);
    previousMillis = currentMillis;
    state = 2;
  } else if (state == 2 && currentMillis - previousMillis >= 10000) {
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, HIGH);
    previousMillis = currentMillis;
    state = 1;
  } else if (state == 1 && currentMillis - previousMillis >= 2000) {
    digitalWrite(yellowLed, LOW);
    digitalWrite(redLed, HIGH);
    previousMillis = currentMillis;
    state = 0;
  }
}

void moveForward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(motor3Pin1, HIGH);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor4Pin1, HIGH);
  digitalWrite(motor4Pin2, LOW);
}

void moveBackward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, HIGH);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2, HIGH);
}

void turnLeft() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, HIGH);
  digitalWrite(motor4Pin1, HIGH);
  digitalWrite(motor4Pin2, LOW);
}

void turnRight() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  digitalWrite(motor3Pin1, HIGH);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2, HIGH);
}

void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2, LOW);
}

long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  long distance = (duration / 2) / 29.1;

  return distance;
}
