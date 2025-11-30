#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Keypad.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;
#define IN1 50
#define IN2 51
#define IN3 52
#define IN4 53
#define ENA 3
#define ENB 4
#define SERVO_PIN 2
#define TRIG_PIN 43
#define ECHO_PIN 44
#define POT_PIN A0
#define SEG_A 22
#define SEG_B 23
#define SEG_C 24
#define SEG_D 25
#define SEG_E 26
#define SEG_F 27
#define SEG_G 28
#define GREEN_LED 8
#define RED_LED 7
#define LEFT_LED 6
#define RIGHT_LED 5
#define BLUE_LED 13
#define WHITE_LED_LEFT 42
#define WHITE_LED_RIGHT 47
#define RED_LED_1 12
#define RED_LED_2 11
#define WHITE_LED 10
const int NOTE_REST = 0;
const int NOTE_C0 = 16;
const int NOTE_CS0 = 17;
const int NOTE_D0 = 18;
const int NOTE_DS0 = 19;
const int NOTE_E0 = 21;
const int NOTE_F0 = 22;
const int NOTE_FS0 = 23;
const int NOTE_G0 = 25;
const int NOTE_GS0 = 26;
const int NOTE_A0 = 28;
const int NOTE_AS0 = 29;
const int NOTE_B0 = 31;
const int NOTE_C1 = 33;
const int NOTE_CS1 = 35;
const int NOTE_D1 = 37;
const int NOTE_DS1 = 39;
const int NOTE_E1 = 41;
const int NOTE_F1 = 44;
const int NOTE_FS1 = 46;
const int NOTE_G1 = 49;
const int NOTE_GS1 = 52;
const int NOTE_A1 = 55;
const int NOTE_AS1 = 58;
const int NOTE_B1 = 62;
const int NOTE_C2 = 65;
const int NOTE_CS2 = 69;
const int NOTE_D2 = 73;
const int NOTE_DS2 = 78;
const int NOTE_E2 = 82;
const int NOTE_F2 = 87;
const int NOTE_FS2 = 93;
const int NOTE_G2 = 98;
const int NOTE_GS2 = 104;
const int NOTE_A2 = 110;
const int NOTE_AS2 = 117;
const int NOTE_B2 = 123;
const int NOTE_C3 = 131;
const int NOTE_CS3 = 139;
const int NOTE_D3 = 147;
const int NOTE_DS3 = 156;
const int NOTE_E3 = 165;
const int NOTE_F3 = 175;
const int NOTE_FS3 = 185;
const int NOTE_G3 = 196;
const int NOTE_GS3 = 208;
const int NOTE_A3 = 220;
const int NOTE_AS3 = 233;
const int NOTE_B3 = 247;
const int NOTE_C4 = 262;
const int NOTE_CS4 = 277;
const int NOTE_D4 = 294;
const int NOTE_DS4 = 311;
const int NOTE_E4 = 330;
const int NOTE_F4 = 349;
const int NOTE_FS4 = 370;
const int NOTE_G4 = 392;
const int NOTE_GS4 = 415;
const int NOTE_A4 = 440;
const int NOTE_AS4 = 466;
const int NOTE_B4 = 494;
const int NOTE_C5 = 523;
const int NOTE_CS5 = 554;
const int NOTE_D5 = 587;
const int NOTE_DS5 = 622;
const int NOTE_E5 = 659;
const int NOTE_F5 = 698;
const int NOTE_FS5 = 740;
const int NOTE_G5 = 784;
const int NOTE_GS5 = 831;
const int NOTE_A5 = 880;
const int NOTE_AS5 = 932;
const int NOTE_B5 = 988;
const int NOTE_C6 = 1047;
const int NOTE_CS6 = 1109;
const int NOTE_D6 = 1175;
const int NOTE_DS6 = 1245;
const int NOTE_E6 = 1319;
const int NOTE_F6 = 1397;
const int NOTE_FS6 = 1480;
const int NOTE_G6 = 1568;
const int NOTE_GS6 = 1661;
const int NOTE_A6 = 1760;
const int NOTE_AS6 = 1865;
const int NOTE_B6 = 1976;
const int NOTE_C7 = 2093;
const int NOTE_CS7 = 2217;
const int NOTE_D7 = 2349;
const int NOTE_DS7 = 2489;
const int NOTE_E7 = 2637;
const int NOTE_F7 = 2794;
const int NOTE_FS7 = 2960;
const int NOTE_G7 = 3136;
const int NOTE_GS7 = 3322;
const int NOTE_A7 = 3520;
const int NOTE_AS7 = 3733;
const int NOTE_B7 = 3951;
const int NOTE_C8 = 4186;
const int NOTE_CS8 = 4435;
const int NOTE_D8 = 4699;
const int NOTE_DS8 = 4978;
const int NOTE_E8 = 5274;
const int NOTE_F8 = 5588;
const int NOTE_FS8 = 5920;
const int NOTE_G8 = 6272;
const int NOTE_GS8 = 6645;
const int NOTE_A8 = 7040;
const int NOTE_AS8 = 7466;
const int NOTE_B8 = 7902;
const int UNIT_TIME = 100;
const int DOT_DURATION = UNIT_TIME;
const int DASH_DURATION = UNIT_TIME * 3;
const int ELEMENT_SPACE = UNIT_TIME;
const int CHAR_SPACE = UNIT_TIME * 3;
const int DISTANCE_UPDATE_INTERVAL = 200;
const int CLICK = 41;
const int VRX = A12;
const int VRY = A8;
const int blueLedPin = 9;
const int orangeLedPin = 10;
const int SWEEP_SPEED_DELAY = 20;
const int TURN_CLEARANCE_DISTANCE = 18;
const int MAZE_TURN_LEFT_SPEED = 255;
const int MAZE_TURN_RIGHT_SPEED = 255;
const int FOLLOW_DISTANCE_CM = 20;
const int FOLLOW_TOLERANCE_CM = 5;
const int MAZE_TURN_LEFT_DURATION = 310;
const int MAZE_TURN_RIGHT_DURATION = 330;
const int TEST_ACTION_DURATION = 1000;
const int LCD_ANIMATION_DELAY = 300;
const unsigned long MAZE_PAUSE_BLINK_INTERVAL = 300;
const long OBSTACLE_DISTANCE = 15;
const byte ROWS = 4;
const byte COLS = 4;
int servoPos = 90;
int segmentPins[7] = {22, 23, 24, 25, 26, 27, 28};
int ledBrightness = 0;
int fadeAmount = 2;
int lcdAnimationPhase = 0;
int variablechanger = 1;
int currentCreditMessage = 0;
int morseRepeater = 1;
int servoPosition = 0;
int servoDirection = 1;
unsigned long lastDistanceUpdateTime = 0;
unsigned long lastSweepTime = 0;
unsigned long lastPauseBlinkTime = 0;
unsigned long lastMessageTime = 0;
unsigned long stateStartTime = 0;
unsigned long lastLcdAnimationUpdate = 0;
unsigned long testStateStartTime = 0;
long distLeft = 0;
long distRight = 0;
bool isCreditsActive = false;
bool isServoMovementEnabled = false;
bool manTstJustEntered = false;
bool isMazePaused = false;
bool sevenSegIsOn = false;
bool whiteLedsOn = false;
bool buzzerIsOn = false;
bool isPauseBlinkOn = true;
bool buzzerEnabled = false;
bool btModeJustEntered = false;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
enum Mode
{
  NONE,
  BLUETOOTH,
  MAZE,
  TESTING,
  CREDITS,
  FOLLOW_ME,
  MAN_TST,
  MORSE_COM,
  JOYSTICK_TEST,
  DICE
};
enum FollowState
{
  FOLLOW_STATE_WAITING,
  FOLLOW_STATE_FOLLOWING
};
enum MazeState
{
  MAZE_STATE_MOVING_FORWARD,
  MAZE_STATE_OBSTACLE_DETECTED,
  MAZE_STATE_SCANNING,
  MAZE_STATE_TURNING_LEFT,
  MAZE_STATE_TURNING_RIGHT,
  MAZE_STATE_TURNING_AROUND,
};
enum TestState
{
  TEST_STATE_INIT,
  TEST_STATE_FORWARD,
  TEST_STATE_BACKWARD,
  TEST_STATE_TURN_LEFT,
  TEST_STATE_TURN_RIGHT,
  TEST_STATE_SERVO_LEFT,
  TEST_STATE_SERVO_CENTER,
  TEST_STATE_SERVO_RIGHT,
  TEST_STATE_CYCLE_COMPLETE
};
enum NoneModeState
{
  NONE_STATE_WAITING,
  NONE_STATE_BT_PRESSED_FIRST
};
void handleManTstMode();
void handleEmergency();
void controlBluetooth();
void handleDice();
void forward();
void backward();
void turnLeft();
void turnRight();
void stopMotors();
void servoSweep();
void clearSegments();
void animateSegmentMenu();
void showWelcome();
void handleJoyTestMode();
void showModeSelection();
void handleMazeMode();
void handleFollowMode();
void animateLcdMenu();
void handleMorseCom();
void displayP();
void displayF();
void handleTestingMode();
void handleCreditsMode();
void morseElement(char element);
void displayDigit(int digit);
void handleManTstMode(char customKey);
void displayLetter(char letter);
void executeMazeTurn(String direction);
void handleRandomizerMode(char customKey);
void displayCentered(String msg, int row);
void updateMovementState(String state, bool forceUpdate = false);
void Beep(int duration);
void Beep(int duration = 100);
void alertBeep();
void welcomeBeep();
void mazeMessageBeep();
void menuBeep();
void morseChar(char character);
long measureDistance();
byte rowPins[ROWS] = {35, 36, 37, 38};
byte colPins[COLS] = {34, 32, 31, 30};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Mode currentMode = NONE;
String mazeMovementState = "Moving";
FollowState currentFollowState = FOLLOW_STATE_WAITING;
MazeState currentMazeState = MAZE_STATE_MOVING_FORWARD;
TestState currentTestState = TEST_STATE_INIT;
NoneModeState currentNoneModeState = NONE_STATE_WAITING;
void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  Serial1.begin(9600);
  myservo.attach(SERVO_PIN);
  pinMode(CLICK, INPUT_PULLUP);
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(orangeLedPin, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  pinMode(RED_LED_1, OUTPUT);
  pinMode(RED_LED_2, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  for (int i = 0; i < 7; i++)
  {
    pinMode(segmentPins[i], OUTPUT);
  }
  showWelcome();
  showModeSelection();
}
void loop()
{
  char customKey = keypad.getKey();
  if (customKey == '#')
  {
    if (currentMode == MAZE || currentMode == BLUETOOTH || currentMode == FOLLOW_ME)
    {
      Beep(50);
      isMazePaused = !isMazePaused;
      if (isMazePaused)
      {
        stopMotors();
        lcd.clear();
        lcd.setCursor(2, 0);
        displayCentered("ROBOT PAUSED", 0);
        lcd.setCursor(0, 1);
        displayCentered("~S15 TO CONTINUE", 1);
        displayP();
        lastPauseBlinkTime = millis();
        isPauseBlinkOn = true;
        digitalWrite(GREEN_LED, LOW);
      }
      else
      {
        lcd.clear();
        updateMovementState(mazeMovementState, true);
        digitalWrite(RED_LED, LOW);
      }
    }
    else
    {
      whiteLedsOn = !whiteLedsOn;
      if (whiteLedsOn)
      {
        digitalWrite(WHITE_LED_LEFT, HIGH);
        digitalWrite(WHITE_LED_RIGHT, HIGH);
      }
      else
      {
        digitalWrite(WHITE_LED_LEFT, LOW);
        digitalWrite(WHITE_LED_RIGHT, LOW);
      }
    }
  }
  if ((currentMode == MAZE || currentMode == BLUETOOTH || currentMode == FOLLOW_ME) && isMazePaused)
  {
    unsigned long currentTime = millis();
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    if (currentTime - lastPauseBlinkTime >= MAZE_PAUSE_BLINK_INTERVAL)
    {
      lastPauseBlinkTime = currentTime;
      isPauseBlinkOn = !isPauseBlinkOn;
      if (isPauseBlinkOn)
      {
        displayP();
        digitalWrite(RED_LED, HIGH);
      }
      else
      {
        clearSegments();
        digitalWrite(RED_LED, LOW);
      }
    }
  }
  if ((currentMode != NONE && customKey == 'D') || (currentMode == NONE && customKey == 'D'))
  {
    handleEmergency();
    showModeSelection();
    return;
  }
  if (currentMode == NONE)
  {
    if (customKey == '1' || (Serial1.available() && Serial1.peek() == '1'))
    {
      if (Serial1.available() && Serial1.peek() == '1')
      {
        Serial1.read();
      }
      Serial.println(" -- BLUETOOTH MODE TRIGGERED !");
      Serial1.println(" -- BLUETOOTH MODE TRIGGERED !");
      Beep();
      displayDigit(1);
      lcd.clear();
      currentMode = BLUETOOTH;
      btModeJustEntered = true;
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      currentNoneModeState = NONE_STATE_WAITING;
    }
    else if (customKey == '2' || (Serial1.available() && Serial1.peek() == '2'))
    {
      if (Serial1.available() && Serial1.peek() == '2')
      {
        Serial1.read();
      }
      clearSegments();
      digitalWrite(segmentPins[4], HIGH);
      digitalWrite(segmentPins[5], HIGH);
      digitalWrite(segmentPins[6], HIGH);
      lcd.clear();
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
      Serial.println(" -- DAMI 3000 uses a Right-Hand Algorithm !");
      Serial1.println(" -- DAMI 3000 uses a Right-Hand Algorithm !");
      displayCentered("DAMI 3000 uses a", 0);
      displayCentered("Right-Hand Alg !", 1);
      stopMotors();
      myservo.write(45);
      mazeMessageBeep();
      delay(1500);
      myservo.write(90);
      digitalWrite(BLUE_LED, LOW);
      lcd.clear();
      displayCentered("Starting in...", 0);
      Serial.println(" -- Starting in 3, 2, 1...");
      Serial1.println(" -- Starting in 3, 2, 1...");
      for (int count = 3; count >= 1; count--)
      {
        displayCentered(String(count), 1);
        digitalWrite(RED_LED, HIGH);
        displayDigit(count);
        if (count == 3)
        {
          tone(BUZZER, NOTE_C3);
          delay(800);
        }
        if (count == 2)
        {
          tone(BUZZER, NOTE_C4);
          delay(800);
        }
        if (count == 1)
        {
          tone(BUZZER, NOTE_C5);
          delay(800);
        }
        clearSegments();
        digitalWrite(RED_LED, LOW);
      }
      clearSegments();
      lcd.clear();
      displayDigit(0);
      Serial.println(" -- GO !");
      Serial1.println(" -- GO !");
      displayCentered("GO !", 0);
      digitalWrite(GREEN_LED, HIGH);
      tone(BUZZER, NOTE_C6);
      delay(1000);
      noTone(BUZZER);
      currentMode = MAZE;
      currentMazeState = MAZE_STATE_MOVING_FORWARD;
      stateStartTime = millis();
      updateMovementState("");
      currentNoneModeState = NONE_STATE_WAITING;
      delay(300);
      clearSegments();
      digitalWrite(GREEN_LED, LOW);
      lcd.clear();
    }
    else if (customKey == 'B' || (Serial1.available() && Serial1.peek() == 'B'))
    {
      if (Serial1.available() && Serial1.peek() == 'B')
      {
        Serial1.read();
      }
      currentMode = TESTING;
      currentTestState = TEST_STATE_INIT;
      testStateStartTime = millis();
      updateMovementState("Testing: Initializing");
      displayDigit(8);
      lcd.clear();
      Beep();
      delay(500);
    }
    else if (customKey == 'A' || (Serial1.available() && Serial1.peek() == 'A'))
    {
      if (Serial1.available() && Serial1.peek() == 'A')
      {
        Serial1.read();
      }
      Beep();
      currentMode = FOLLOW_ME;
      currentFollowState = FOLLOW_STATE_FOLLOWING;
      stopMotors();
    }
    else if (customKey == '6' || (Serial1.available() && Serial1.peek() == '6'))
    {
      if (Serial1.available() && Serial1.peek() == '6')
      {
        Serial1.read();
      }
      Beep();
      lcd.clear();
      currentMode = MAN_TST;
      manTstJustEntered = true;
      updateMovementState("MANUAL TEST READY");
    }
    else if (customKey == '5' || (Serial1.available() && Serial1.peek() == '5'))
    {
      if (Serial1.available() && Serial1.peek() == '5')
      {
        Serial1.read();
      }
      if (currentMode == NONE)
      {
        Beep();
        lcd.clear();
        currentMode = CREDITS;
        isCreditsActive = true;
        currentCreditMessage = 0;
        lastMessageTime = millis();
        updateMovementState(" ");
      }
      else if (currentMode == CREDITS)
      {
        isServoMovementEnabled = !isServoMovementEnabled;
        if (isServoMovementEnabled)
        {
          Serial.println("Servo movement ON");
          myservo.write(45);
        }
        else
        {
          Serial.println("Servo movement OFF");
          myservo.write(0);
        }
      }
    }
  }
  if (((analogRead(VRX) < 150) || (Serial1.available() && Serial1.peek() == '7')) && currentMode != JOYSTICK_TEST && currentMode == NONE)
  {
    currentMode = JOYSTICK_TEST;
    lcd.clear();
    Beep();
    clearSegments();
    stopMotors();
  }
  if (currentMode == JOYSTICK_TEST)
  {
    handleJoyTestMode();
  }
  else if ((customKey == '3' || (Serial1.available() && Serial1.peek() == '3')) && currentMode == NONE)
  {
    if (Serial1.available() && Serial1.peek() == '3')
    {
      Serial1.read();
    }
    currentMode = MORSE_COM;
    handleMorseCom();
  }
  else if (currentMode == MORSE_COM)
  {
    handleMorseCom();
  }
  else if ((customKey == '4' || (Serial1.available() && Serial1.peek() == '4')) && currentMode == NONE)
  {
    if (Serial1.available() && Serial1.peek() == '4')
    {
      Serial1.read();
    }
    currentMode = DICE;
    handleDice();
  }
  else if (currentMode == DICE)
  {
    handleDice();
  }
  else if (currentMode == BLUETOOTH)
  {
    if (!isMazePaused)
    {
      controlBluetooth();
    }
  }
  else if (currentMode == MAZE)
  {
    if (!isMazePaused)
    {
      handleMazeMode();
    }
  }
  else if (currentMode == MAN_TST)
  {
    handleManTstMode(customKey);
  }
  else if (currentMode == TESTING)
  {
    handleTestingMode();
  }
  else if (currentMode == CREDITS)
  {
    handleCreditsMode();
  }
  else if (currentMode == FOLLOW_ME)
  {
    if (!isMazePaused)
    {
      handleFollowMode();
    }
  }
  if (currentMode == BLUETOOTH)
  {
    if (!isMazePaused)
    {
      static unsigned long lastBlinkTime = 0;
      static bool showDigit = true;
      if (millis() - lastBlinkTime >= 300)
      {
        lastBlinkTime = millis();
        showDigit = !showDigit;
        if (!isMazePaused)
        {
          if (showDigit)
          {
            displayDigit(1);
          }
          else
          {
            clearSegments();
          }
        }
      }
      lcd.setCursor(0, 0);
      lcd.print("Distance : ");
      long distance = measureDistance();
      if (distance < 10)
        lcd.print("0");
      if (distance < 100)
        lcd.print(distance);
      else
        lcd.print("99");
      lcd.print(" cm");
      if (distance <= 99 && distance != 0)
      {
        Serial1.print("Distance : ");
        Serial1.print(distance);
        Serial1.println(" cm");
        Serial.print("Distance : ");
        Serial.print(distance);
      }
      else if (distance == 0)
      {
        Serial1.print("Distance : MIN\n");
        Serial.print("Distance : MIN\n");
      }
      else
      {
        Serial1.print("Distance : MAX\n");
        Serial.print("Distance : MAX\n");
      }
    }
  }
  else if (currentMode == NONE || currentMode == CREDITS)
  {
    animateSegmentMenu();
    animateLcdMenu();
  }
}
void handleDice()
{
  static enum DiceState { WAITING,
                          ROLLING,
                          RESULT } currentDiceState = WAITING;
  static unsigned long rollStartTime = 0;
  const unsigned long ROLL_DURATION = 2500;
  if (variablechanger == 1)
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    Serial.println(" -- DICE MODE TRIGGERED !");
    Serial1.println(" -- DICE MODE TRIGGERED !");
    lcd.clear();
    clearSegments();
    stopMotors();
    displayCentered("RANDOM DICE MODE", 0);
    displayCentered("~Press to Roll", 1);
    Beep(100);
    displayLetter('D');
    digitalWrite(BLUE_LED, HIGH);
    delay(100);
    digitalWrite(BLUE_LED, LOW);
    clearSegments();
    Beep(100);
    digitalWrite(BLUE_LED, HIGH);
    displayLetter('D');
    stopMotors();
    variablechanger = 0;
    currentDiceState = WAITING;
  }
  char customKey = keypad.getKey();
  if (customKey == 'D')
  {
    handleEmergency();
    currentMode = NONE;
    showModeSelection();
    return;
  }
  switch (currentDiceState)
  {
  case WAITING:
  {
    if ((digitalRead(CLICK) == LOW) || (customKey == '4'))
    {
      currentDiceState = ROLLING;
      rollStartTime = millis();
      randomSeed(analogRead(A0));
      Beep(50);
    }
    break;
  }
  case ROLLING:
  {
    Serial.println("\n -- Rolling...");
    Serial1.println("\n -- Rolling...");
    unsigned long currentTime = millis();
    if (currentTime - rollStartTime < ROLL_DURATION)
    {
      int currentDigit = (currentTime / 50) % 10;
      displayDigit(currentDigit);
      Beep();
      if ((currentTime / 100) % 2 == 0)
      {
        lcd.setCursor(0, 1);
        lcd.print("   ROLLING...   ");
        digitalWrite(BLUE_LED, HIGH);
      }
      else
      {
        lcd.setCursor(0, 1);
        lcd.print("                ");
        digitalWrite(BLUE_LED, LOW);
      }
    }
    else
    {
      digitalWrite(BLUE_LED, LOW);
      int final_roll = random(0, 10);
      Serial.println("\n -- Result is : \n");
      Serial1.println("\n -- Result is : \n");
      Serial.println(final_roll);
      Serial1.println(final_roll);
      displayDigit(final_roll);
      lcd.clear();
      displayCentered("ROLL COMPLETE!", 0);
      displayCentered("Result: " + String(final_roll), 1);
      digitalWrite(GREEN_LED, HIGH);
      Beep(300);
      currentDiceState = RESULT;
      rollStartTime = millis();
    }
    break;
  }
  case RESULT:
  {
    if (millis() - rollStartTime >= 3000)
    {
      clearSegments();
      lcd.clear();
      Serial.println("\n -- DICE MODE READY !");
      Serial1.println("\n -- DICE MODE READY !");
      displayCentered("RANDOM DICE MODE", 0);
      displayCentered("~Press to Roll", 1);
      digitalWrite(GREEN_LED, LOW);
      Beep(100);
      displayLetter('D');
      digitalWrite(BLUE_LED, HIGH);
      delay(100);
      digitalWrite(BLUE_LED, LOW);
      clearSegments();
      Beep(100);
      digitalWrite(BLUE_LED, HIGH);
      displayLetter('D');
      currentDiceState = WAITING;
    }
    break;
  }
  }
}
void handleJoyTestMode()
{
  if (variablechanger == 1)
  {
    variablechanger = 0;
    Serial.println(" -- JOYSTICK TESTING MODE TRIGGERED !");
    Serial1.println(" -- JOYSTICK TESTING MODE TRIGGERED !");
  }
  int vrxValue = analogRead(VRX);
  int vryValue = analogRead(VRY);
  int clickState = digitalRead(CLICK);
  Serial.print("VRX: ");
  Serial.print(vrxValue);
  Serial.print(" | VRY: ");
  Serial.print(vryValue);
  Serial.print(" | CLICK: ");
  Serial.println(clickState);
  lcd.setCursor(0, 0);
  lcd.print("X-Axis:");
  lcd.print(vrxValue);
  if (vrxValue < 1000)
    lcd.print("   ");
  if (vrxValue < 100)
    lcd.print("  ");
  if (vrxValue < 10)
    lcd.print(" ");
  lcd.setCursor(11, 0);
  lcd.print("|");
  lcd.setCursor(0, 1);
  lcd.print("Y-Axis:");
  lcd.print(vryValue);
  if (vryValue < 1000)
    lcd.print("   ");
  if (vryValue < 100)
    lcd.print("  ");
  if (vryValue < 10)
    lcd.print(" ");
  lcd.setCursor(11, 1);
  lcd.print("|");
  lcd.setCursor(12, 0);
  lcd.print("CLCK ");
  if (clickState == LOW)
  {
    lcd.setCursor(12, 1);
    lcd.print("~ON");
  }
  else
  {
    lcd.setCursor(12, 1);
    lcd.print("OFF");
  }
  unsigned long currentTime = millis();
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
}
void displayLetter(char letter)
{
  byte segments = 0;
  switch (letter)
  {
  case 'A':
    segments = 0b01110111;
    break;
  case 'B':
    segments = 0b01111100;
    break;
  case 'C':
    segments = 0b00111001;
    break;
  case 'D':
    segments = 0b01011110;
    break;
  case 'E':
    segments = 0b01111001;
    break;
  case 'F':
    segments = 0b01110001;
    break;
  case 'G':
    segments = 0b01111101;
    break;
  case 'H':
    segments = 0b01110110;
    break;
  case 'I':
    segments = 0b00000110;
    break;
  case 'J':
    segments = 0b00011110;
    break;
  case 'K':
    segments = 0b01110000;
    break;
  case 'L':
    segments = 0b00111000;
    break;
  case 'M':
    segments = 0b00110111;
    break;
  case 'N':
    segments = 0b01010100;
    break;
  case 'O':
    segments = 0b00111111;
    break;
  case 'P':
    segments = 0b01110011;
    break;
  case 'Q':
    segments = 0b01100111;
    break;
  case 'R':
    segments = 0b01010000;
    break;
  case 'S':
    segments = 0b01101101;
    break;
  case 'T':
    segments = 0b01111000;
    break;
  case 'U':
    segments = 0b00111110;
    break;
  case 'V':
    segments = 0b00111110;
    break;
  case 'W':
    segments = 0b00011100;
    break;
  case 'X':
    segments = 0b01110110;
    break;
  case 'Y':
    segments = 0b01101110;
    break;
  case 'Z':
    segments = 0b01011011;
    break;
  case '0':
    segments = 0b00111111;
    break;
  case '1':
    segments = 0b00000110;
    break;
  case '2':
    segments = 0b01011011;
    break;
  case '3':
    segments = 0b01001111;
    break;
  case '4':
    segments = 0b01100110;
    break;
  case '5':
    segments = 0b01101101;
    break;
  case '6':
    segments = 0b01111101;
    break;
  case '7':
    segments = 0b00000111;
    break;
  case '8':
    segments = 0b01111111;
    break;
  case '9':
    segments = 0b01101111;
    break;
  }
  for (int i = 0; i < 9; i++)
  {
    digitalWrite(segmentPins[i], (segments >> i) & 0x01);
  }
}
void morseElement(char element)
{
  int duration;
  if (element == '.')
  {
    duration = DOT_DURATION;
    tone(BUZZER, NOTE_A3);
  }
  else if (element == '-')
  {
    duration = DASH_DURATION;
    tone(BUZZER, NOTE_E3);
  }
  else
  {
    return;
  }
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  delay(duration);
  noTone(BUZZER);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
}
void morseChar(char character)
{
  char upperChar = toupper(character);
  displayLetter(upperChar);
  if (upperChar == 'A')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("A");
    Serial1.println("A");
  }
  else if (upperChar == '-')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("-");
    Serial1.println("-");
  }
  else if (upperChar == 'B')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("B");
    Serial1.println("B");
  }
  else if (upperChar == 'C')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("C");
    Serial1.println("C");
  }
  else if (upperChar == 'D')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("D");
    Serial1.println("D");
  }
  else if (upperChar == 'E')
  {
    morseElement('.');
    Serial.println("E");
    Serial1.println("E");
  }
  else if (upperChar == 'F')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("F");
    Serial1.println("F");
  }
  else if (upperChar == 'G')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("G");
    Serial1.println("G");
  }
  else if (upperChar == 'H')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("H");
    Serial1.println("H");
  }
  else if (upperChar == 'I')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("I");
    Serial1.println("I");
  }
  else if (upperChar == 'J')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("J");
    Serial1.println("J");
  }
  else if (upperChar == 'K')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("K");
    Serial1.println("K");
  }
  else if (upperChar == 'L')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("L");
    Serial1.println("L");
  }
  else if (upperChar == 'M')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("M");
    Serial1.println("M");
  }
  else if (upperChar == 'N')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("N");
    Serial1.println("N");
  }
  else if (upperChar == 'O')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("O");
    Serial1.println("O");
  }
  else if (upperChar == 'P')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("P");
    Serial1.println("P");
  }
  else if (upperChar == 'Q')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("Q");
    Serial1.println("Q");
  }
  else if (upperChar == 'R')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("R");
    Serial1.println("R");
  }
  else if (upperChar == 'S')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("S");
    Serial1.println("S");
  }
  else if (upperChar == 'T')
  {
    morseElement('-');
    Serial.println("T");
    Serial1.println("T");
  }
  else if (upperChar == 'U')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("U");
    Serial1.println("U");
  }
  else if (upperChar == 'V')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("V");
    Serial1.println("V");
  }
  else if (upperChar == 'W')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("W");
    Serial1.println("W");
  }
  else if (upperChar == 'X')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("X");
    Serial1.println("X");
  }
  else if (upperChar == 'Y')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("Y");
    Serial1.println("Y");
  }
  else if (upperChar == 'Z')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("Z");
    Serial1.println("Z");
  }
  else if (upperChar == '1')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("1");
    Serial1.println("1");
  }
  else if (upperChar == '2')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("2");
    Serial1.println("2");
  }
  else if (upperChar == '3')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("3");
    Serial1.println("3");
  }
  else if (upperChar == '4')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("4");
    Serial1.println("4");
  }
  else if (upperChar == '5')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("5");
    Serial1.println("5");
  }
  else if (upperChar == '6')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("6");
    Serial1.println("6");
  }
  else if (upperChar == '7')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("7");
    Serial1.println("7");
  }
  else if (upperChar == '8')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("8");
    Serial1.println("8");
  }
  else if (upperChar == '9')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("9");
    Serial1.println("9");
  }
  else if (upperChar == '0')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("0");
    Serial1.println("0");
  }
  else if (upperChar == '!')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println("!");
    Serial1.println("!");
  }
  else if (upperChar == '?')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println("?");
    Serial1.println("?");
  }
  else if (upperChar == '.')
  {
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println(".");
    Serial1.println(".");
  }
  else if (upperChar == ',')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    Serial.println(",");
    Serial1.println(",");
  }
  else if (upperChar == ':')
  {
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('-');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    delay(ELEMENT_SPACE);
    morseElement('.');
    Serial.println(":");
    Serial1.println(":");
  }
  else if (upperChar == ' ')
  {
    Serial.println("\n -End of message- \n");
    Serial1.println("\n -End of message- \n");
    lcd.setCursor(0, 1);
    lcd.print("~                ");
    morseRepeater = 1;
  }
  delay(CHAR_SPACE);
}
void handleMorseCom()
{
  if (variablechanger == 1)
  {
    variablechanger = 0;
    Serial.println(" -- MORSE COMMUNICATION MODE TRIGGERED !");
    Serial1.println(" -- MORSE COMMUNICATION MODE TRIGGERED !");
    lcd.clear();
    clearSegments();
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    Beep(100);
    displayLetter('M');
    digitalWrite(BLUE_LED, HIGH);
    delay(100);
    digitalWrite(BLUE_LED, LOW);
    clearSegments();
    Beep(100);
    digitalWrite(BLUE_LED, HIGH);
    displayLetter('M');
    stopMotors();
    Serial.println("\n -- Ready for Serial Input or Keypad Command !\n");
    Serial1.println("\n -- Ready for Serial Input or Keypad Command !\n");
    Serial.println(" -- Transmitted Message :\n");
    Serial1.println(" -- Transmitted Message :\n");
    lcd.setCursor(0, 0);
    lcd.print("MORSE COMMT MODE");
    lcd.setCursor(0, 1);
    lcd.print("USE COOLTERM /BT");
    delay(1000);
    digitalWrite(BLUE_LED, LOW);
    clearSegments();
    delay(1000);
    lcd.clear();
    Beep(100);
    displayLetter('8');
    digitalWrite(RED_LED, HIGH);
    delay(100);
    clearSegments();
    lcd.setCursor(0, 0);
    lcd.print("SEND YOUR LETTER");
    lcd.setCursor(0, 1);
    lcd.print("~");
  }
  char command = 0;
  while (Serial1.available())
  {
    command = Serial1.read();
    char receivedChar = Serial1.read();
    Serial.println(command);
    switch (command)
    {
    case 'A':
      morseChar('A');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('A');
      morseRepeater++;
      break;
    case 'B':
      morseChar('B');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('B');
      morseRepeater++;
      break;
    case 'C':
      morseChar('C');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('C');
      morseRepeater++;
      break;
    case 'D':
      morseChar('D');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('D');
      morseRepeater++;
      break;
    case 'E':
      morseChar('E');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('E');
      morseRepeater++;
      break;
    case '-':
      morseChar('-');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('-');
      morseRepeater++;
      break;
    case 'F':
      morseChar('F');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('F');
      morseRepeater++;
      break;
    case 'G':
      morseChar('G');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('G');
      morseRepeater++;
      break;
    case 'H':
      morseChar('H');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('H');
      morseRepeater++;
      break;
    case 'I':
      morseChar('I');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('I');
      morseRepeater++;
      break;
    case 'J':
      morseChar('J');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('J');
      morseRepeater++;
      break;
    case 'K':
      morseChar('K');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('K');
      morseRepeater++;
      break;
    case 'L':
      morseChar('L');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('L');
      morseRepeater++;
      break;
    case '!':
      morseChar('!');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('!');
      morseRepeater++;
      break;
    case '?':
      morseChar('?');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('?');
      morseRepeater++;
      break;
    case '.':
      morseChar('.');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('.');
      morseRepeater++;
      break;
    case ',':
      morseChar(',');
      lcd.setCursor(morseRepeater, 1);
      lcd.print(',');
      morseRepeater++;
      break;
    case ':':
      morseChar(':');
      lcd.setCursor(morseRepeater, 1);
      lcd.print(':');
      morseRepeater++;
      break;
    case 'M':
      morseChar('M');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('M');
      morseRepeater++;
      break;
    case 'N':
      morseChar('N');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('N');
      morseRepeater++;
      break;
    case 'O':
      morseChar('O');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('O');
      morseRepeater++;
      break;
    case 'P':
      morseChar('P');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('P');
      morseRepeater++;
      break;
    case 'Q':
      morseChar('Q');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('Q');
      morseRepeater++;
      break;
    case 'R':
      morseChar('R');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('R');
      morseRepeater++;
      break;
    case 'S':
      morseChar('S');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('S');
      morseRepeater++;
      break;
    case 'T':
      morseChar('T');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('T');
      morseRepeater++;
      break;
    case 'U':
      morseChar('U');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('U');
      morseRepeater++;
      break;
    case 'V':
      morseChar('V');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('V');
      morseRepeater++;
      break;
    case 'W':
      morseChar('W');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('W');
      morseRepeater++;
      break;
    case 'X':
      morseChar('X');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('X');
      morseRepeater++;
      break;
    case 'Y':
      morseChar('Y');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('Y');
      morseRepeater++;
      break;
    case 'Z':
      morseChar('Z');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('Z');
      morseRepeater++;
      break;
    case '0':
      morseChar('0');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('0');
      morseRepeater++;
      break;
    case '1':
      morseChar('1');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('1');
      morseRepeater++;
      break;
    case '2':
      morseChar('2');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('2');
      morseRepeater++;
      break;
    case '3':
      morseChar('3');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('3');
      morseRepeater++;
      break;
    case '4':
      morseChar('4');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('4');
      morseRepeater++;
      break;
    case '5':
      morseChar('5');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('5');
      morseRepeater++;
      break;
    case '6':
      morseChar('6');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('6');
      morseRepeater++;
      break;
    case '7':
      morseChar('7');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('7');
      morseRepeater++;
      break;
    case '8':
      morseChar('8');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('8');
      morseRepeater++;
      break;
    case '9':
      morseChar('9');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('9');
      morseRepeater++;
      break;
    case ' ':
      morseChar(' ');
      break;
    default:
      break;
    }
  }
  delay(50);
}
void handleManTstMode(char customKey)
{
  if (variablechanger == 1)
  {
    variablechanger = 0;
    Serial.println(" -- MANUAL TESTING MODE TRIGGERED !");
    Serial1.println(" -- MANUAL TESTING MODE TRIGGERED !");
  }
  if (manTstJustEntered)
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("     Ready!");
    stopMotors();
    myservo.write(90);
    clearSegments();
    digitalWrite(BUZZER, LOW);
    sevenSegIsOn = false;
    manTstJustEntered = false;
  }
  unsigned long currentTime = millis();
  if (!customKey && currentTime - lastDistanceUpdateTime >= DISTANCE_UPDATE_INTERVAL)
  {
    lastDistanceUpdateTime = currentTime;
    long distanceCm = measureDistance();
    if (distanceCm > 99)
    {
      distanceCm = 99;
    }
    char distBuffer[17];
    snprintf(distBuffer, sizeof(distBuffer), "Distance : %02ld cm", distanceCm);
    lcd.setCursor(0, 0);
    lcd.print(distBuffer);
  }
  if (customKey)
  {
    if (customKey == '8')
    {
      forward();
      updateMovementState(" MOTORS FORWARD");
    }
    else if (customKey == '4')
    {
      turnRight();
      updateMovementState("MOTORS RIGHT");
    }
    else if (customKey == '6')
    {
      turnLeft();
      updateMovementState("MOTORS LEFT");
    }
    else if (customKey == '2')
    {
      backward();
      updateMovementState("MOTORS BACKWARD");
    }
    else if (customKey == '5')
    {
      stopMotors();
      updateMovementState("ROBOT IDLE");
    }
    else if (customKey == 'A')
    {
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      updateMovementState("RED LED: ON");
    }
    else if (customKey == 'B')
    {
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
      updateMovementState(" GREEN LED: ON");
    }
    else if (customKey == 'C')
    {
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
      updateMovementState("BLUE LED: ON");
    }
    if (customKey == '9')
    {
      sevenSegIsOn = !sevenSegIsOn;
      if (sevenSegIsOn)
      {
        for (int i = 0; i < 7; i++)
        {
          digitalWrite(segmentPins[i], HIGH);
        }
        lcd.setCursor(0, 1);
        lcd.print("   7-SEG: ON       ");
      }
      else
      {
        clearSegments();
        lcd.setCursor(0, 1);
        lcd.print("   7-SEG: OFF      ");
      }
    }
    unsigned long actionEndTime = millis();
    while (millis() - actionEndTime < 50)
    {
    }
  }
  if (customKey == '0' || analogRead(VRY) > 950 || analogRead(VRY) < 150)
  {
    myservo.write(90);
    updateMovementState("SERVO CENTERED");
  }
  else if (customKey == '*' || analogRead(VRX) > 950)
  {
    myservo.write(0);
    updateMovementState("SERVO RIGHT");
  }
  else if (customKey == '#' || analogRead(VRX) < 150)
  {
    myservo.write(180);
    updateMovementState("SERVO LEFT");
  }
  else if (customKey == '3')
  {
    myservo.write(135);
    updateMovementState("SERVO left");
  }
  else if (customKey == '1')
  {
    myservo.write(45);
    updateMovementState("SERVO right");
  }
  if (customKey == '7' || digitalRead(CLICK) == LOW)
  {
    buzzerIsOn = !buzzerIsOn;
    if (buzzerIsOn)
    {
      displayCentered("BUZZER Testing", 1);
      for (int m = 0; m < 2000; m++)
      {
        tone(BUZZER, m);
        delay(2);
      }
    }
    displayCentered("BUZZER Test Done", 1);
    buzzerIsOn = !buzzerIsOn;
    noTone(BUZZER);
  }
}
void displayF()
{
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}
void displayP()
{
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}
void handleFollowMode()
{
  if (variablechanger == 1)
  {
    variablechanger = 0;
    Serial.println(" -- FOLLOW ME MODE TRIGGERED !");
    Serial1.println(" -- FOLLOW ME MODE TRIGGERED !");
  }
  unsigned long currentTime = millis();
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  if (currentTime - lastPauseBlinkTime >= MAZE_PAUSE_BLINK_INTERVAL)
  {
    lastPauseBlinkTime = currentTime;
    isPauseBlinkOn = !isPauseBlinkOn;
    if (isPauseBlinkOn)
    {
      displayLetter('F');
      digitalWrite(BLUE_LED, HIGH);
    }
    else
    {
      clearSegments();
      digitalWrite(BLUE_LED, LOW);
    }
  }
  long currentDistance = measureDistance();
  lcd.setCursor(0, 0);
  lcd.print("Distance : ");
  if (currentDistance < 10)
    lcd.print("0");
  if (currentDistance < 100)
    lcd.print(currentDistance);
  else
    lcd.print("99");
  lcd.print(" cm");
  lcd.setCursor(0, 1);
  lcd.print("Threshold: 20 cm      ");
  const int MIN_DISTANCE = FOLLOW_DISTANCE_CM - FOLLOW_TOLERANCE_CM;
  const int MAX_DISTANCE = FOLLOW_DISTANCE_CM + FOLLOW_TOLERANCE_CM;
  switch (currentFollowState)
  {
  case FOLLOW_STATE_WAITING:
    stopMotors();
    break;
  case FOLLOW_STATE_FOLLOWING:
    if (currentDistance < MIN_DISTANCE)
    {
      backward();
    }
    else if (currentDistance > MAX_DISTANCE)
    {
      forward();
    }
    else
    {
      stopMotors();
    }
    break;
  }
}
void showWelcome()
{
  Serial.println(" -- Reseting everything");
  Serial1.println(" -- Reseting everything");
  stopMotors();
  myservo.write(90);
  servoPos = 90;
  lcd.clear();
  displayCentered("Reseting all", 0);
  displayCentered("Components..", 1);
  clearSegments();
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED_1, LOW);
  digitalWrite(RED_LED_2, LOW);
  digitalWrite(WHITE_LED, LOW);
  digitalWrite(blueLedPin, LOW);
  digitalWrite(orangeLedPin, LOW);
  delay(1100);
  Serial.println(" -- Hello! I'm DAMI 3000!");
  Serial1.println(" -- Hello! I'm DAMI 3000!");
  lcd.clear();
  displayCentered("Hello!", 0);
  displayCentered("I'm DAMI 3000!", 1);
  welcomeBeep();
  delay(100);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);
  digitalWrite(blueLedPin, LOW);
  digitalWrite(orangeLedPin, LOW);
  digitalWrite(segmentPins[6], LOW);
  delay(50);
  delay(1000);
}
void animateSegmentMenu()
{
  if (currentMode != NONE)
    return;
  static unsigned long lastSegmentUpdate = 0;
  static unsigned long lastLedHoverUpdate = 0;
  const unsigned long LED_HOVER_INTERVAL = 5;
  static int segmentIndex = 0;
  if (millis() - lastSegmentUpdate >= 50)
  {
    lastSegmentUpdate = millis();
    clearSegments();
    if (segmentIndex < 6)
    {
      digitalWrite(segmentPins[segmentIndex], HIGH);
    }
    segmentIndex = (segmentIndex + 1) % 6;
  }
  if (millis() - lastLedHoverUpdate >= LED_HOVER_INTERVAL)
  {
    lastLedHoverUpdate = millis();
    static int rgbLedCycle = 0;
    ledBrightness += fadeAmount;
    if (ledBrightness <= 0 || ledBrightness >= 255)
    {
      fadeAmount = -fadeAmount;
      ledBrightness = constrain(ledBrightness, 0, 255);
      if (ledBrightness == 0 && fadeAmount > 0)
      {
        rgbLedCycle = (rgbLedCycle + 1) % 3;
      }
    }
    analogWrite(LEFT_LED, ledBrightness);
    analogWrite(RIGHT_LED, ledBrightness);
    analogWrite(RED_LED_1, ledBrightness);
    analogWrite(RED_LED_2, ledBrightness);
    analogWrite(blueLedPin, ledBrightness);
    analogWrite(orangeLedPin, ledBrightness);
    analogWrite(RED_LED, (rgbLedCycle == 0) ? ledBrightness : LOW);
    analogWrite(GREEN_LED, (rgbLedCycle == 1) ? ledBrightness : LOW);
    analogWrite(BLUE_LED, (rgbLedCycle == 2) ? ledBrightness : LOW);
  }
}
void animateLcdMenu()
{
  if (currentMode != NONE)
    return;
  if (millis() - lastLcdAnimationUpdate >= LCD_ANIMATION_DELAY)
  {
    lastLcdAnimationUpdate = millis();
    int prevRowLeft = (lcdAnimationPhase - 1 + 2) % 2;
    int currentRowLeft = lcdAnimationPhase % 2;
    int prevRowRight = (prevRowLeft == 0) ? 1 : 0;
    int currentRowRight = (currentRowLeft == 0) ? 1 : 0;
    lcd.setCursor(0, prevRowLeft);
    lcd.print(" ");
    lcd.setCursor(4, prevRowRight);
    lcd.print(" ");
    lcd.setCursor(8, prevRowLeft);
    lcd.print(" ");
    lcd.setCursor(12, prevRowRight);
    lcd.print(" ");
    lcd.setCursor(0, currentRowLeft);
    lcd.print("~");
    lcd.setCursor(4, currentRowRight);
    lcd.print("~");
    lcd.setCursor(8, currentRowLeft);
    lcd.print("~");
    lcd.setCursor(12, currentRowRight);
    lcd.print("~");
    lcdAnimationPhase++;
  }
}
void showModeSelection()
{
  stopMotors();
  currentMode = NONE;
  clearSegments();
  lcd.clear();
  lcd.clear();
  Serial.println(" -- Choose a mode :");
  Serial1.println(" -- Choose a mode :");
  displayCentered(" BTH MZE MRS FLM", 0);
  displayCentered(" DCE CRD MTT ATT", 1);
  menuBeep();
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  myservo.write(90);
}
void handleCreditsMode()
{
  if (variablechanger == 1)
  {
    variablechanger = 0;
    Serial.println(" -- CREDITS MODE TRIGGERED !");
    Serial1.println(" -- CREDITS MODE TRIGGERED !");
  }
  stopMotors();
  servoSweep();
  digitalWrite(RIGHT_LED, LOW);
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RED_LED_1, LOW);
  digitalWrite(RED_LED_2, LOW);
  digitalWrite(WHITE_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  String creditsMessages[] = {
      "Made by :\nNizar EL Idrysy",
      "Contains the\nmodified .ino",
      "3241 lines of\ndedicated coding",
      "Use CoolTerm on\nWindows for HC06",
      "Shout out to my\nbro Gemini ;",
      "1500+ Dirhams of\nmaterials",
      "Presented at 9AM\n18 June 2025",
      "EMSI-T - 2AP-G3\nGraded 17/20",
      "Rated the best\nrobot among all",
      "Funded by aunt\nHanane Belhaj <3",
      "Named after my\ndad Imad XD",
      "Groupe members :\nthat contributed",
      "Zerrari Ziyad &\nAmine Habz &",
      "Bilal Mesbahi.\nJury Members :",
      "Pr.Bouazizi Omar\nand",
      "Pr.Bendaoud\nKaoutar and",
      "Pr.Errezgouny\nAbderrachid.",
      "Shout out to my\nparents for",
      "their support &\nhelp & care <3",
      "More on LinkedIn :&\n@Nizar EL Idrysy",
      "DAMI 3000 !\nLoves you all :D"};
  const int numMessages = 22;
  static unsigned long lastSegmentBlinkTime = 0;
  static bool showSegmentC = true;
  if (millis() - lastSegmentBlinkTime >= 300)
  {
    lastSegmentBlinkTime = millis();
    showSegmentC = !showSegmentC;
    if (showSegmentC)
    {
      displayLetter('C');
    }
    else
    {
      clearSegments();
    }
  }
  if (millis() - lastMessageTime >= 2500)
  {
    lastMessageTime = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    int newlinePos = creditsMessages[currentCreditMessage].indexOf('\n');
    if (newlinePos != -1)
    {
      digitalWrite(BLUE_LED, HIGH);
      lcd.print(creditsMessages[currentCreditMessage].substring(0, newlinePos));
      lcd.setCursor(0, 1);
      lcd.print(creditsMessages[currentCreditMessage].substring(newlinePos + 1));
    }
    else
    {
      displayCentered(creditsMessages[currentCreditMessage], 0);
      digitalWrite(BLUE_LED, LOW);
    }
    currentCreditMessage++;
    if (currentCreditMessage >= numMessages)
    {
      currentCreditMessage = 0;
    }
  }
}
void handleEmergency()
{
  char customKey = '^^';
  variablechanger = 1;
  Serial.println(" -- EMERGENCY MODE TRIGGERED !");
  Serial1.println(" -- EMERGENCY MODE TRIGGERED !");
  stopMotors();
  isMazePaused = false;
  myservo.write(90);
  servoPos = 90;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" EMERGENCY MODE");
  lcd.setCursor(0, 1);
  lcd.print(" TRIGGERED !!!!");
  clearSegments();
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED_1, LOW);
  digitalWrite(RED_LED_2, LOW);
  digitalWrite(WHITE_LED, LOW);
  digitalWrite(blueLedPin, LOW);
  digitalWrite(orangeLedPin, LOW);
  for (int i = 0; i < 7; i++)
  {
    displayLetter('E');
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    alertBeep();
    clearSegments();
    digitalWrite(RED_LED, LOW);
    delay(50);
  }
  delay(1000);
}
void controlBluetooth()
{
  static bool isForwardToggled = false;
  static bool isBackwardToggled = false;
  static bool isLeftToggled = false;
  static bool isRightToggled = false;
  static String lastNonMovementState = "";
  static unsigned long lastLedHoverUpdateTime = 0;
  static int ledHoverBrightness = 0;
  static int ledHoverFadeAmount = 60;
  const unsigned long LED_HOVER_INTERVAL_BT = 1;
  static unsigned long lastWhiteLedHoverUpdateTime = 0;
  static int whiteLedHoverBrightness = 100;
  static int whiteLedHoverFadeAmount = 60;
  const unsigned long WHITE_LED_HOVER_INTERVAL_BT = 10000;
  auto resetMovementToggles = [&]()
  {
    isForwardToggled = false;
    isBackwardToggled = false;
    isLeftToggled = false;
    isRightToggled = false;
    stopMotors();
    analogWrite(LEFT_LED, LOW);
    analogWrite(RIGHT_LED, LOW);
    analogWrite(RED_LED_1, LOW);
    analogWrite(RED_LED_2, LOW);
    analogWrite(WHITE_LED, LOW);
    analogWrite(BLUE_LED, LOW);
    ledHoverBrightness = 0;
    ledHoverFadeAmount = abs(ledHoverFadeAmount);
    whiteLedHoverBrightness = 0;
    whiteLedHoverFadeAmount = abs(whiteLedHoverFadeAmount);
    lastNonMovementState = "";
  };
  if (btModeJustEntered)
  {
    resetMovementToggles();
    lcd.setCursor(0, 1);
    lcd.print("                ");
    updateMovementState("Idle", true);
    btModeJustEntered = false;
  }
  char command = 0;
  while (Serial1.available())
  {
    command = Serial1.read();
  }
  if (command != 0 && command != '\n' && command != '\r')
  {
    if (command == 'Z' || command == 'S' || command == 'Q' || command == 'D' || command == ' ' || command == 'X')
    {
      lastNonMovementState = "";
    }
    switch (command)
    {
    case 'Z':
      if (isForwardToggled)
      {
        resetMovementToggles();
      }
      else
      {
        resetMovementToggles();
        isForwardToggled = true;
        forward();
        updateMovementState("Forward");
      }
      break;
    case 'S':
      if (isBackwardToggled || isForwardToggled)
      {
        resetMovementToggles();
      }
      else
      {
        backward();
        isBackwardToggled = true;
        backward();
        updateMovementState("Backward");
      }
      break;
    case 'Q':
      if (isLeftToggled)
      {
        resetMovementToggles();
      }
      else
      {
        resetMovementToggles();
        isLeftToggled = true;
        turnLeft();
        updateMovementState("Left");
      }
      break;
    case 'D':
      if (isRightToggled)
      {
        resetMovementToggles();
      }
      else
      {
        resetMovementToggles();
        isRightToggled = true;
        turnRight();
        updateMovementState("Right");
      }
      break;
    case ' ':
      resetMovementToggles();
      updateMovementState("Idle", true);
      break;
    case 'M':
      myservo.write(0);
      servoPos = 0;
      lastNonMovementState = "Servo RIGHT";
      updateMovementState(lastNonMovementState);
      break;
    case 'K':
      myservo.write(180);
      servoPos = 180;
      lastNonMovementState = "Servo LEFT";
      updateMovementState(lastNonMovementState);
      break;
    case 'O':
    case 'L':
      myservo.write(90);
      servoPos = 90;
      lastNonMovementState = "Servo Middle";
      updateMovementState(lastNonMovementState);
      break;
    case 'I':
      myservo.write(135);
      servoPos = 135;
      lastNonMovementState = "Servo left";
      updateMovementState(lastNonMovementState);
      break;
    case 'P':
      myservo.write(45);
      servoPos = 45;
      lastNonMovementState = "Servo right";
      updateMovementState(lastNonMovementState);
      break;
    case 'B':
      buzzerEnabled = !buzzerEnabled;
      if (buzzerEnabled)
      {
        tone(BUZZER, NOTE_A4);
        lastNonMovementState = "BUZZER: ON";
        updateMovementState(lastNonMovementState);
      }
      else
      {
        noTone(BUZZER);
        lastNonMovementState = "BUZZER: OFF";
        updateMovementState(lastNonMovementState);
      }
      break;
    case 'X':
      handleEmergency();
      showModeSelection();
      return;
    }
  }
  if (isForwardToggled)
  {
    forward();
    updateMovementState("Motors Forward");
    analogWrite(LEFT_LED, LOW);
    analogWrite(RIGHT_LED, LOW);
    analogWrite(BLUE_LED, LOW);
    analogWrite(RED_LED_1, 50);
    analogWrite(RED_LED_2, 50);
    analogWrite(WHITE_LED, LOW);
  }
  else if (isBackwardToggled)
  {
    backward();
    updateMovementState("Motors Backward");
    analogWrite(LEFT_LED, LOW);
    analogWrite(RIGHT_LED, LOW);
    analogWrite(BLUE_LED, LOW);
    analogWrite(RED_LED_1, LOW);
    analogWrite(RED_LED_2, LOW);
    if (millis() - lastWhiteLedHoverUpdateTime >= WHITE_LED_HOVER_INTERVAL_BT)
    {
      lastWhiteLedHoverUpdateTime = millis();
      whiteLedHoverBrightness += whiteLedHoverFadeAmount;
      if (whiteLedHoverBrightness <= 0 || whiteLedHoverBrightness >= 255)
      {
        whiteLedHoverFadeAmount = -whiteLedHoverFadeAmount;
        whiteLedHoverBrightness = constrain(whiteLedHoverBrightness, 0, 255);
      }
      analogWrite(WHITE_LED, whiteLedHoverBrightness);
    }
  }
  else if (isLeftToggled)
  {
    turnLeft();
    updateMovementState("Motors Left");
    if (millis() - lastLedHoverUpdateTime >= LED_HOVER_INTERVAL_BT)
    {
      lastLedHoverUpdateTime = millis();
      ledHoverBrightness += ledHoverFadeAmount;
      if (ledHoverBrightness <= 0 || ledHoverBrightness >= 255)
      {
        ledHoverFadeAmount = -ledHoverFadeAmount;
        ledHoverBrightness = constrain(ledHoverBrightness, 0, 255);
      }
      analogWrite(LEFT_LED, ledHoverBrightness);
      analogWrite(RIGHT_LED, LOW);
    }
    analogWrite(RED_LED_1, 50);
    analogWrite(RED_LED_2, 50);
    analogWrite(WHITE_LED, LOW);
  }
  else if (isRightToggled)
  {
    turnRight();
    updateMovementState("Motors Right");
    if (millis() - lastLedHoverUpdateTime >= LED_HOVER_INTERVAL_BT)
    {
      lastLedHoverUpdateTime = millis();
      ledHoverBrightness += ledHoverFadeAmount;
      if (ledHoverBrightness <= 0 || ledHoverBrightness >= 255)
      {
        ledHoverFadeAmount = -ledHoverFadeAmount;
        ledHoverBrightness = constrain(ledHoverBrightness, 0, 255);
      }
      analogWrite(RIGHT_LED, ledHoverBrightness);
    }
    analogWrite(RED_LED_1, 50);
    analogWrite(RED_LED_2, 50);
    analogWrite(WHITE_LED, LOW);
  }
  else
  {
    stopMotors();
    analogWrite(LEFT_LED, LOW);
    analogWrite(RIGHT_LED, LOW);
    analogWrite(WHITE_LED, LOW);
    if (millis() - lastLedHoverUpdateTime >= LED_HOVER_INTERVAL_BT)
    {
      lastLedHoverUpdateTime = millis();
      ledHoverBrightness += ledHoverFadeAmount;
      if (ledHoverBrightness <= 0 || ledHoverBrightness >= 255)
      {
        ledHoverFadeAmount = -ledHoverFadeAmount;
        ledHoverBrightness = constrain(ledHoverBrightness, 0, 255);
      }
      analogWrite(RED_LED_1, ledHoverBrightness);
      analogWrite(RED_LED_2, ledHoverBrightness);
    }
    if (lastNonMovementState != "")
    {
      updateMovementState(lastNonMovementState);
    }
    else
    {
      updateMovementState("Motors Idle");
    }
  }
  int potValue = analogRead(POT_PIN);
  int motorSpeed = map(potValue, 0, 1023, 0, 255);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}
void displayCentered(String msg, int row)
{
  int len = msg.length();
  int startCol = max(0, (16 - len) / 2);
  lcd.setCursor(0, row);
  for (int i = 0; i < 16; ++i)
    lcd.print(" ");
  lcd.setCursor(startCol, row);
  lcd.print(msg);
}
void updateMovementState(String state, bool forceUpdate = false)
{
  if (isMazePaused)
  {
    return;
  }
  static String lastDisplayedState = "";
  if (forceUpdate || state != lastDisplayedState)
  {
    displayCentered(state, 1);
    lastDisplayedState = state;
    if (currentMode == BLUETOOTH)
    {
      bool motorsAreMoving = (state.indexOf("Forward") != -1 ||
                              state.indexOf("Backward") != -1 ||
                              state.indexOf("Left") != -1 ||
                              state.indexOf("Right") != -1);
      if (motorsAreMoving)
      {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(BLUE_LED, LOW);
      }
      else if (state.indexOf("Idle") != -1)
      {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
      }
    }
    else if (currentMode == MAZE)
    {
      if (state.indexOf("Obstacle!") != -1)
      {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
      }
      else
      {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(BLUE_LED, LOW);
      }
    }
    else if (currentMode == TESTING)
    {
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
    }
  }
}
void forward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void backward()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void turnLeft()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, MAZE_TURN_LEFT_SPEED);
  analogWrite(ENB, MAZE_TURN_LEFT_SPEED);
}
void turnRight()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, MAZE_TURN_RIGHT_SPEED);
  analogWrite(ENB, MAZE_TURN_RIGHT_SPEED);
}
void stopMotors()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
void servoSweep()
{
  if (millis() - lastSweepTime >= SWEEP_SPEED_DELAY)
  {
    lastSweepTime = millis();
    servoPosition += servoDirection;
    if (servoPosition >= 135)
    {
      servoDirection = -1;
      servoPosition = 135;
    }
    else if (servoPosition <= 45)
    {
      servoDirection = 1;
      servoPosition = 45;
    }
    myservo.write(servoPosition);
  }
}
long measureDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 25000);
  long distance = duration * 0.0343 / 2;
  return distance;
}
void displayDigit(int digit)
{
  if (digit < 0 || digit > 9)
    return;
  byte numbers[10] = {
      0b00111111, 0b00000110, 0b01011011, 0b01001111,
      0b01100110, 0b01101101, 0b01111101, 0b00000111,
      0b01111111, 0b01101111};
  byte segments = numbers[digit];
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(segmentPins[i], (segments >> i) & 0x01);
  }
}
void clearSegments()
{
  for (int i = 0; i < 7; i++)
    digitalWrite(segmentPins[i], LOW);
}
void executeMazeTurn(String direction)
{
  if (direction == "left")
  {
    turnLeft();
    delay(MAZE_TURN_LEFT_DURATION);
  }
  else if (direction == "right")
  {
    turnRight();
    delay(MAZE_TURN_RIGHT_DURATION);
  }
  else if (direction == "around")
  {
    turnLeft();
    delay(MAZE_TURN_LEFT_DURATION * 2);
  }
  stopMotors();
  delay(100);
}
void handleMazeMode()
{
  if (variablechanger == 1)
  {
    variablechanger = 0;
    Serial.println(" -- MAZE MODE TRIGGERED !");
    Serial1.println(" -- MAZE MODE TRIGGERED !");
  }
  static unsigned long lastSegmentBlinkTime = 0;
  static bool showSegmentDigit = true;
  static unsigned long lastBeepTimeMoving = 0;
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);
  digitalWrite(RED_LED_1, LOW);
  digitalWrite(RED_LED_2, LOW);
  digitalWrite(WHITE_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  if (millis() - lastSegmentBlinkTime >= 300)
  {
    lastSegmentBlinkTime = millis();
    showSegmentDigit = !showSegmentDigit;
    if (showSegmentDigit)
    {
      displayDigit(2);
    }
    else
    {
      clearSegments();
    }
  }
  int potValue = analogRead(POT_PIN);
  int motorSpeed = map(potValue, 0, 1023, 0, 255);
  lcd.setCursor(0, 0);
  lcd.print("Distance : ");
  long currentDistance = measureDistance();
  if (currentDistance < 10)
    lcd.print("0");
  if (currentDistance < 100)
    lcd.print(currentDistance);
  else
    lcd.print("99");
  lcd.print(" cm      ");
  switch (currentMazeState)
  {
  case MAZE_STATE_MOVING_FORWARD:
    forward();
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed);
    updateMovementState("Forward...");
    if (millis() - lastBeepTimeMoving >= 800)
    {
      lastBeepTimeMoving = millis();
      Beep(50);
    }
    if (measureDistance() <= OBSTACLE_DISTANCE)
    {
      currentMazeState = MAZE_STATE_OBSTACLE_DETECTED;
      stateStartTime = millis();
      stopMotors();
      updateMovementState("Obstacle!!!!");
      for (int m = 0; m < 5; m++)
      {
        tone(BUZZER, NOTE_B3);
        delay(100);
        noTone(BUZZER);
      }
      delay(1000);
    }
    break;
  case MAZE_STATE_OBSTACLE_DETECTED:
    currentMazeState = MAZE_STATE_SCANNING;
    stateStartTime = millis();
    break;
  case MAZE_STATE_SCANNING:
    updateMovementState("Scanning...");
    myservo.write(0);
    delay(300);
    distRight = measureDistance();
    Serial.print("Right: ");
    Serial.println(distRight);
    myservo.write(180);
    delay(300);
    distLeft = measureDistance();
    Serial.print("Left: ");
    Serial.println(distLeft);
    myservo.write(90);
    delay(100);
    if (distRight > TURN_CLEARANCE_DISTANCE)
    {
      currentMazeState = MAZE_STATE_TURNING_RIGHT;
    }
    else if (distLeft > TURN_CLEARANCE_DISTANCE)
    {
      currentMazeState = MAZE_STATE_TURNING_LEFT;
    }
    else
    {
      currentMazeState = MAZE_STATE_TURNING_AROUND;
    }
    stateStartTime = millis();
    break;
  case MAZE_STATE_TURNING_LEFT:
    tone(BUZZER, NOTE_C7);
    updateMovementState("Turning Left");
    executeMazeTurn("left");
    currentMazeState = MAZE_STATE_MOVING_FORWARD;
    stateStartTime = millis();
    break;
  case MAZE_STATE_TURNING_RIGHT:
    tone(BUZZER, NOTE_C7);
    updateMovementState("Turning Right");
    executeMazeTurn("right");
    currentMazeState = MAZE_STATE_MOVING_FORWARD;
    stateStartTime = millis();
    break;
  case MAZE_STATE_TURNING_AROUND:
    tone(BUZZER, NOTE_C7);
    updateMovementState("Turning Around");
    executeMazeTurn("around");
    currentMazeState = MAZE_STATE_MOVING_FORWARD;
    stateStartTime = millis();
    break;
  }
  noTone(BUZZER);
}
void handleTestingMode()
{
  if (variablechanger == 1)
  {
    variablechanger = 0;
    Serial.println(" -- AUTOMATIC TESTING MODE TRIGGERED !");
    Serial1.println(" -- AUTOMATIC TESTING MODE TRIGGERED !");
  }
  static unsigned long lastLedBlinkTime = 0;
  static bool ledState = LOW;
  static unsigned long lastBuzzerBeapTime = 0;
  static int rgbLedCycle = 0;
  int potValue = analogRead(POT_PIN);
  int motorSpeed = map(potValue, 0, 1023, 0, 255);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  if (millis() - lastLedBlinkTime >= 100)
  {
    lastLedBlinkTime = millis();
    ledState = !ledState;
    digitalWrite(LEFT_LED, ledState);
    digitalWrite(RIGHT_LED, ledState);
    digitalWrite(RED_LED_1, ledState);
    digitalWrite(RED_LED_2, ledState);
    digitalWrite(WHITE_LED, ledState);
  }
  if (millis() - lastBuzzerBeapTime >= 500)
  {
    lastBuzzerBeapTime = millis();
    displayDigit(8);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    if (rgbLedCycle == 0)
    {
      digitalWrite(RED_LED, HIGH);
    }
    else if (rgbLedCycle == 1)
    {
      digitalWrite(GREEN_LED, HIGH);
    }
    else if (rgbLedCycle == 2)
    {
      digitalWrite(BLUE_LED, HIGH);
    }
    Beep(50);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    rgbLedCycle = (rgbLedCycle + 1) % 3;
  }
  lcd.setCursor(0, 0);
  lcd.print("Distance : ");
  long distance = measureDistance();
  if (distance < 10)
  {
    lcd.print("0");
  }
  if (distance < 100)
    lcd.print(distance);
  else
    lcd.print("99");
  lcd.print(" cm");
  switch (currentTestState)
  {
  case TEST_STATE_INIT:
    stopMotors();
    myservo.write(90);
    updateMovementState("Testing...");
    currentTestState = TEST_STATE_FORWARD;
    testStateStartTime = millis();
    break;
  case TEST_STATE_FORWARD:
    forward();
    if (millis() - testStateStartTime == 0)
      updateMovementState("Forward");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      stopMotors();
      currentTestState = TEST_STATE_BACKWARD;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_BACKWARD:
    backward();
    if (millis() - testStateStartTime == 0)
      updateMovementState("Backward");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      stopMotors();
      currentTestState = TEST_STATE_TURN_LEFT;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_TURN_LEFT:
    turnLeft();
    if (millis() - testStateStartTime == 0)
      updateMovementState("Left");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      stopMotors();
      currentTestState = TEST_STATE_TURN_RIGHT;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_TURN_RIGHT:
    turnRight();
    if (millis() - testStateStartTime == 0)
      updateMovementState("Right");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      stopMotors();
      currentTestState = TEST_STATE_SERVO_LEFT;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_SERVO_LEFT:
    myservo.write(0);
    if (millis() - testStateStartTime == 0)
      updateMovementState("Servo Left");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      currentTestState = TEST_STATE_SERVO_CENTER;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_SERVO_CENTER:
    myservo.write(90);
    if (millis() - testStateStartTime == 0)
      updateMovementState("Servo Centered");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      currentTestState = TEST_STATE_SERVO_RIGHT;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_SERVO_RIGHT:
    myservo.write(180);
    if (millis() - testStateStartTime == 0)
      updateMovementState("Servo Right");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      currentTestState = TEST_STATE_CYCLE_COMPLETE;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_CYCLE_COMPLETE:
    stopMotors();
    myservo.write(90);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    updateMovementState("Test Finished.");
    tone(BUZZER, NOTE_C3);
    currentTestState = TEST_STATE_INIT;
    testStateStartTime = millis();
    delay(1000);
    noTone(BUZZER);
    delay(1500);
    break;
  }
}
void alertBeep()
{
  tone(BUZZER, NOTE_C6, 100);
  delay(70);
  noTone(BUZZER);
}
void Beep(int duration)
{
  tone(BUZZER, NOTE_C5, 100);
  delay(120);
  noTone(BUZZER);
}
void welcomeBeep()
{
  const int PACMAN_TEMPO = 200;
  int pacmanMelody[] = {
      NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5,
      NOTE_G4, NOTE_E4, NOTE_C4,
      NOTE_REST, NOTE_C5, NOTE_C5, NOTE_REST};
  int pacmanDurations[] = {
      8, 8, 8, 8,
      8, 8, 8,
      8, 8, 8};
  int notes = sizeof(pacmanMelody) / sizeof(pacmanMelody[0]);
  for (int i = 0; i < notes - 1; i++)
  {
    int duration = PACMAN_TEMPO / (pacmanDurations[i] / 4);
    tone(BUZZER, pacmanMelody[i], duration);
    if (i == 0)
    {
      myservo.write(70);
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(segmentPins[i], HIGH);
      delay(50);
    }
    else if (i == 1)
    {
      myservo.write(110);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(segmentPins[i], HIGH);
      delay(50);
    }
    else if (i == 2)
    {
      myservo.write(70);
      digitalWrite(BLUE_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(segmentPins[i], HIGH);
      delay(50);
    }
    else if (i == 3)
    {
      myservo.write(110);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(segmentPins[i], HIGH);
      delay(50);
    }
    else if (i == 4)
    {
      myservo.write(70);
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(segmentPins[i], HIGH);
      delay(50);
    }
    else if (i == 5)
    {
      myservo.write(110);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(segmentPins[i], HIGH);
      delay(50);
    }
    else if (i == 6)
    {
      myservo.write(70);
      digitalWrite(BLUE_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(segmentPins[i], HIGH);
      delay(50);
    }
    else if (i == 7)
    {
      myservo.write(70);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
      digitalWrite(segmentPins[0], LOW);
      digitalWrite(segmentPins[1], LOW);
      digitalWrite(segmentPins[2], LOW);
      digitalWrite(segmentPins[3], LOW);
      digitalWrite(segmentPins[4], LOW);
      digitalWrite(segmentPins[5], LOW);
      digitalWrite(segmentPins[6], LOW);
      digitalWrite(segmentPins[7], LOW);
      delay(50);
    }
    else if (i == 8)
    {
      myservo.write(110);
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(segmentPins[0], HIGH);
      digitalWrite(segmentPins[1], HIGH);
      digitalWrite(segmentPins[2], HIGH);
      digitalWrite(segmentPins[3], HIGH);
      digitalWrite(segmentPins[4], HIGH);
      digitalWrite(segmentPins[5], HIGH);
      digitalWrite(segmentPins[6], HIGH);
      digitalWrite(segmentPins[7], HIGH);
      delay(35);
      digitalWrite(segmentPins[0], LOW);
      digitalWrite(segmentPins[1], LOW);
      digitalWrite(segmentPins[2], LOW);
      digitalWrite(segmentPins[3], LOW);
      digitalWrite(segmentPins[4], LOW);
      digitalWrite(segmentPins[5], LOW);
      digitalWrite(segmentPins[6], LOW);
      digitalWrite(segmentPins[7], LOW);
      digitalWrite(GREEN_LED, LOW);
      delay(15);
    }
    else if (i == 9)
    {
      myservo.write(90);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(segmentPins[0], HIGH);
      digitalWrite(segmentPins[1], HIGH);
      digitalWrite(segmentPins[2], HIGH);
      digitalWrite(segmentPins[3], HIGH);
      digitalWrite(segmentPins[4], HIGH);
      digitalWrite(segmentPins[5], HIGH);
      digitalWrite(segmentPins[6], HIGH);
      digitalWrite(segmentPins[7], HIGH);
      delay(35);
      digitalWrite(segmentPins[0], LOW);
      digitalWrite(segmentPins[1], LOW);
      digitalWrite(segmentPins[2], LOW);
      digitalWrite(segmentPins[3], LOW);
      digitalWrite(segmentPins[4], LOW);
      digitalWrite(segmentPins[5], LOW);
      digitalWrite(segmentPins[6], LOW);
      digitalWrite(segmentPins[7], LOW);
      digitalWrite(GREEN_LED, LOW);
      delay(15);
    }
    int pauseTime = duration;
    delay(pauseTime);
    noTone(BUZZER);
  }
}
void mazeMessageBeep()
{
  const int LEVEL_CLEAR_TEMPO = 150;
  int creditsMelody[] = {
      NOTE_C5, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
      NOTE_REST,
      NOTE_E5, NOTE_C5};
  int creditsDurations[] = {
      8, 8, 8, 8, 8, 8, 8,
      8,
      4, 2};
  int notes = sizeof(creditsMelody) / sizeof(creditsMelody[0]);
  for (int i = 0; i < notes; i++)
  {
    long duration = (LEVEL_CLEAR_TEMPO * 4) / creditsDurations[i];
    if (creditsMelody[i] != NOTE_REST)
    {
      tone(BUZZER, creditsMelody[i], duration);
    }
    long pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER);
  }
}
void menuBeep()
{
  const int PACMAN_TEMPO = 200;
  int pacmanMelody[] = {
      NOTE_C4, NOTE_C4, NOTE_G4, NOTE_REST};
  int pacmanDurations[] = {
      8, 8, 8, 8,
      8};
  int notes = sizeof(pacmanMelody) / sizeof(pacmanMelody[0]);
  for (int i = 0; i < notes - 1; i++)
  {
    int duration = PACMAN_TEMPO / (pacmanDurations[i] / 4);
    tone(BUZZER, pacmanMelody[i], duration);
    if (i == 0)
    {
      myservo.write(100);
      digitalWrite(segmentPins[0], HIGH);
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
    }
    if (i == 1)
    {
      myservo.write(80);
      digitalWrite(segmentPins[6], HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
    }
    if (i == 2)
    {
      myservo.write(90);
      digitalWrite(segmentPins[3], HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
    }
    int pauseTime = duration;
    delay(pauseTime);
    noTone(BUZZER);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    clearSegments();
  }
}