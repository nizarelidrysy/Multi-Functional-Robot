static const int handleCredits_codeLinesCounter_firstLine = __LINE__;

// Libraries
// New libraries
#include <Keypad.h>            // "Keypad" by Marl Stanley, Alexander Brevig.
#include <LiquidCrystal_I2C.h> // "LiquidCrystal I2C" by Frank de Brabander.
// Built-in libraries
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
// Definitions
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;
// Function Definitions
void updateMovementState(String state, bool forceUpdate = false);
// DC Motors
void motorsSpeed();
void motorsForward();
void motorsBackward();
void motorsLeft();
void motorsRight();
void motorsStop();
// Buzzer
void buzzer_activeBeep(int duration = 120);
// Ultrasonic Sensor
long ultrasonicSensor_measureDistance();
// Reset mode
void handleReset(int handleReset_delay = 50);
// Emergency mode
void handleEmergency();
void handleEmergency_buzzer_passiveBeep();
// Bluetooth mode
void handleBluetooth();
// Maze mode
void handleMaze();
void handleMaze_motorsTurn(String direction);
void handleMaze_buzzer_passiveBeep();
// Morse Communication mode
void handleMorseCommunication();
void handleMorseCommunication_element(char element);
void handleMorseCommunication_character(char character);
// Follow Me mode
void handleFollowMe();
// Dice mode
void handleDice();
// Credits mode
void handleCredits();
void handleCredits_servoMotorSweep();
// Manual Test mode
void handleManualTest(char customKey);
// Automatic Test mode
void handleAutomaticTest();
// Joystick Test mode
void handleJoystickTest();
// LCD
void LCD_displayMessageCentered(String msg, int row);
void LCD_welcomeMessage();
void LCD_welcomeMessage_buzzer_passiveBeep();
void LCD_menuMessage();
void LCD_menuMessage_animation();
void LCD_menuMessage_buzzer_passiveBeep();
// 7 Segments Display
void sevenSegment_clearAllSegments();
void sevenSegment_animation();
void sevenSegment_displayDigit(int digit);
void sevenSegment_displayLetter(char letter);
// Components
int sevenSegmentPin[7] = {22, 23, 24, 25, 26, 27, 28};
byte keypadRowPin[4] = {35, 36, 37, 38};
byte keypadColumnPin[4] = {34, 32, 31, 30};
char keypadKey[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
Keypad keypad = Keypad(makeKeymap(keypadKey), keypadRowPin, keypadColumnPin, 4, 4);
#define servoMotor 4
#define motors_ENA 10
#define motors_ENB 11
#define buzzer 49
#define ultrasonicSensor_trigger 43
#define ultrasonicSensor_echo 44
#define motors_IN1 50
#define motors_IN2 51
#define motors_IN3 52
#define motors_IN4 53
#define LED_red 7
#define LED_green 8
#define LED_blue 12
#define joystick_button 41
#define joystick_VRY A8
#define joystick_VRX A12
#define potentiometer A13
// Modes
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
enum Mode_noneState
{
  NONE_STATE_WAITING,
  NONE_STATE_BT_PRESSED_FIRST
};
enum handleMaze_followState
{
  FOLLOW_STATE_WAITING,
  FOLLOW_STATE_FOLLOWING
};
enum handleMaze_mazeState
{
  MAZE_STATE_MOVING_FORWARD,
  MAZE_STATE_OBSTACLE_DETECTED,
  MAZE_STATE_SCANNING,
  MAZE_STATE_TURNING_LEFT,
  MAZE_STATE_TURNING_RIGHT,
  MAZE_STATE_TURNING_AROUND,
};
enum handleAutomaticTest_testState
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
// Logic variables
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
extern const int handleCredits_codeLinesCounter_endLine;
extern const int handleCredits_codeLinesCounter_calculator;
int menuPage = 1;
const unsigned long MAZE_PAUSE_BLINK_INTERVAL = 300;
const long OBSTACLE_DISTANCE = 15;
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
bool buzzerIsOn = false;
bool isPauseBlinkOn = true;
bool buzzerEnabled = false;
bool btModeJustEntered = false;
int servoPos = 90;
int ledBrightness = 0;
int fadeAmount = 2;
int lcdAnimationPhase = 0;
int ensureSingleModeAccess = 1;
int currentCreditMessage = 0;
int morseRepeater = 1;
int servoPosition = 0;
int servoDirection = 1;
Mode currentMode = NONE;
String mazeMovementState = "Moving";
handleMaze_followState currenthandleMaze_followState = FOLLOW_STATE_WAITING;
handleMaze_mazeState currenthandleMaze_mazeState = MAZE_STATE_MOVING_FORWARD;
handleAutomaticTest_testState currenthandleAutomaticTest_testState = TEST_STATE_INIT;
Mode_noneState currentMode_noneState = NONE_STATE_WAITING;
// Void setup
void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  lcd.init();
  lcd.backlight();
  myServo.attach(servoMotor);
  for (int i = 0; i < 7; i++)
  {
    pinMode(sevenSegmentPin[i], OUTPUT);
  }
  pinMode(joystick_button, INPUT_PULLUP);
  pinMode(joystick_VRX, INPUT);
  pinMode(joystick_VRY, INPUT);
  pinMode(LED_red, OUTPUT);
  pinMode(LED_green, OUTPUT);
  pinMode(LED_blue, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(motors_IN1, OUTPUT);
  pinMode(motors_IN2, OUTPUT);
  pinMode(motors_IN3, OUTPUT);
  pinMode(motors_IN4, OUTPUT);
  pinMode(motors_ENA, OUTPUT);
  pinMode(motors_ENB, OUTPUT);
  pinMode(potentiometer, INPUT);
  pinMode(ultrasonicSensor_trigger, OUTPUT);
  pinMode(ultrasonicSensor_echo, INPUT);
  LCD_welcomeMessage();
  LCD_menuMessage();
}
// Void loop
void loop()
{
  motorsSpeed();
  char customKey = keypad.getKey();
  if (customKey == '#')
  {
    if (currentMode == MAZE || currentMode == BLUETOOTH || currentMode == FOLLOW_ME)
    {
      buzzer_activeBeep(50);
      isMazePaused = !isMazePaused;
      if (isMazePaused)
      {
        motorsStop();
        lcd.clear();
        lcd.setCursor(2, 0);
        LCD_displayMessageCentered("ROBOT PAUSED", 0);
        lcd.setCursor(0, 1);
        LCD_displayMessageCentered("~S15 TO CONTINUE", 1);
        sevenSegment_displayLetter('P');
        lastPauseBlinkTime = millis();
        isPauseBlinkOn = true;
        digitalWrite(LED_green, LOW);
      }
      else
      {
        lcd.clear();
        updateMovementState(mazeMovementState, true);
        digitalWrite(LED_red, LOW);
      }
    }
  }
  if ((currentMode == MAZE || currentMode == BLUETOOTH || currentMode == FOLLOW_ME) && isMazePaused)
  {
    unsigned long currentTime = millis();
    digitalWrite(LED_blue, LOW);
    digitalWrite(LED_green, LOW);
    if (currentTime - lastPauseBlinkTime >= MAZE_PAUSE_BLINK_INTERVAL)
    {
      lastPauseBlinkTime = currentTime;
      isPauseBlinkOn = !isPauseBlinkOn;
      if (isPauseBlinkOn)
      {
        sevenSegment_displayLetter('P');
        digitalWrite(LED_red, HIGH);
      }
      else
      {
        sevenSegment_clearAllSegments();
        digitalWrite(LED_red, LOW);
      }
    }
  }
  if (currentMode == NONE && ( (customKey == 'D') || (analogRead(joystick_VRX) < 250) || (analogRead(joystick_VRX) > 800) ) )
  {
    
    sevenSegment_clearAllSegments();
    LCD_menuMessage_buzzer_passiveBeep();
    tone(buzzer, NOTE_C4);
    digitalWrite(LED_green, HIGH);
    menuPage++;
    if (menuPage > 2)
    {
      tone(buzzer, NOTE_C6);
      menuPage = 1;
    }
    LCD_menuMessage();
    noTone(buzzer);
    digitalWrite(LED_green, LOW);
    delay(200);
    return;
  }
  else if (currentMode != NONE && customKey == 'D')
  {
    handleEmergency();
    menuPage++;
    if (menuPage > 2)
    {
      menuPage = 1;
    }
    LCD_menuMessage();
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
      buzzer_activeBeep();
      sevenSegment_displayDigit(1);
      lcd.clear();
      currentMode = BLUETOOTH;
      btModeJustEntered = true;
      digitalWrite(LED_red, HIGH);
      digitalWrite(LED_green, LOW);
      currentMode_noneState = NONE_STATE_WAITING;
    }
    else if (customKey == '2' || (Serial1.available() && Serial1.peek() == '2'))
    {
      if (Serial1.available() && Serial1.peek() == '2')
      {
        Serial1.read();
      }
      sevenSegment_clearAllSegments();
      digitalWrite(sevenSegmentPin[4], HIGH);
      digitalWrite(sevenSegmentPin[5], HIGH);
      digitalWrite(sevenSegmentPin[6], HIGH);
      lcd.clear();
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_green, LOW);
      digitalWrite(LED_blue, HIGH);
      Serial.println(" -- DAMI 3000 uses a Right-Hand Algorithm !");
      Serial1.println(" -- DAMI 3000 uses a Right-Hand Algorithm !");
      LCD_displayMessageCentered("DAMI 3000 uses a", 0);
      LCD_displayMessageCentered("Right-Hand Alg !", 1);
      motorsStop();
      myServo.write(45);
      handleMaze_buzzer_passiveBeep();
      delay(1500);
      myServo.write(90);
      digitalWrite(LED_blue, LOW);
      lcd.clear();
      LCD_displayMessageCentered("Starting in...", 0);
      Serial.println(" -- Starting in 3, 2, 1...");
      Serial1.println(" -- Starting in 3, 2, 1...");
      for (int count = 3; count >= 1; count--)
      {
        LCD_displayMessageCentered(String(count), 1);
        digitalWrite(LED_red, HIGH);
        sevenSegment_displayDigit(count);
        if (count == 3)
        {
          tone(buzzer, NOTE_C3);
          delay(800);
        }
        if (count == 2)
        {
          tone(buzzer, NOTE_C4);
          delay(800);
        }
        if (count == 1)
        {
          tone(buzzer, NOTE_C5);
          delay(800);
        }
        sevenSegment_clearAllSegments();
        digitalWrite(LED_red, LOW);
      }
      sevenSegment_clearAllSegments();
      lcd.clear();
      sevenSegment_displayDigit(0);
      Serial.println(" -- GO !");
      Serial1.println(" -- GO !");
      LCD_displayMessageCentered("GO !", 0);
      digitalWrite(LED_green, HIGH);
      tone(buzzer, NOTE_C6);
      delay(1000);
      noTone(buzzer);
      currentMode = MAZE;
      currenthandleMaze_mazeState = MAZE_STATE_MOVING_FORWARD;
      stateStartTime = millis();
      updateMovementState("");
      currentMode_noneState = NONE_STATE_WAITING;
      delay(300);
      sevenSegment_clearAllSegments();
      digitalWrite(LED_green, LOW);
      lcd.clear();
    }
    else if (customKey == 'B' || (Serial1.available() && Serial1.peek() == 'B'))
    {
      if (Serial1.available() && Serial1.peek() == 'B')
      {
        Serial1.read();
      }
      currentMode = TESTING;
      currenthandleAutomaticTest_testState = TEST_STATE_INIT;
      testStateStartTime = millis();
      updateMovementState("Testing: Initializing");
      sevenSegment_displayDigit(8);
      lcd.clear();
      buzzer_activeBeep();
      delay(500);
    }
    else if (customKey == 'A' || (Serial1.available() && Serial1.peek() == 'A'))
    {
      if (Serial1.available() && Serial1.peek() == 'A')
      {
        Serial1.read();
      }
      buzzer_activeBeep();
      currentMode = FOLLOW_ME;
      currenthandleMaze_followState = FOLLOW_STATE_FOLLOWING;
      motorsStop();
    }
    else if (customKey == '6' || (Serial1.available() && Serial1.peek() == '6'))
    {
      if (Serial1.available() && Serial1.peek() == '6')
      {
        Serial1.read();
      }
      buzzer_activeBeep();
      lcd.clear();
      currentMode = MAN_TST;
      customKey = '5';
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
        buzzer_activeBeep();
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
          myServo.write(45);
        }
        else
        {
          Serial.println("Servo movement OFF");
          myServo.write(0);
        }
      }
    }
  }
  if (((customKey == '7') || (Serial1.available() && Serial1.peek() == '7')) && currentMode != JOYSTICK_TEST && currentMode == NONE)
  {
    currentMode = JOYSTICK_TEST;
    lcd.clear();
    buzzer_activeBeep();
    sevenSegment_clearAllSegments();
    motorsStop();
  }
  if (currentMode == JOYSTICK_TEST)
  {
    handleJoystickTest();
  }
  else if ((customKey == '3' || (Serial1.available() && Serial1.peek() == '3')) && currentMode == NONE)
  {
    if (Serial1.available() && Serial1.peek() == '3')
    {
      Serial1.read();
    }
    currentMode = MORSE_COM;
    handleMorseCommunication();
  }
  else if (currentMode == MORSE_COM)
  {
    handleMorseCommunication();
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
      handleBluetooth();
    }
  }
  else if (currentMode == MAZE)
  {
    if (!isMazePaused)
    {
      handleMaze();
    }
  }
  else if (currentMode == MAN_TST)
  {
    handleManualTest(customKey);
  }
  else if (currentMode == TESTING)
  {
    handleAutomaticTest();
  }
  else if (currentMode == CREDITS)
  {
    handleCredits();
  }
  else if (currentMode == FOLLOW_ME)
  {
    if (!isMazePaused)
    {
      handleFollowMe();
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
            sevenSegment_displayDigit(1);
          }
          else
          {
            sevenSegment_clearAllSegments();
          }
        }
      }
      lcd.setCursor(0, 0);
      lcd.print("Distance : ");
      long distance = ultrasonicSensor_measureDistance();
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
  else if (currentMode == NONE)
  {
    sevenSegment_animation();
    LCD_menuMessage_animation();
  }
}
// Functions Definitions
void updateMovementState(String state, bool forceUpdate = false)
{
  if (isMazePaused)
  {
    return;
  }
  static String lastDisplayedState = "";
  if (forceUpdate || state != lastDisplayedState)
  {
    LCD_displayMessageCentered(state, 1);
    lastDisplayedState = state;
    if (currentMode == BLUETOOTH)
    {
      bool motorsAreMoving = (state.indexOf("Forward") != -1 ||
                              state.indexOf("Backward") != -1 ||
                              state.indexOf("Left") != -1 ||
                              state.indexOf("Right") != -1);
      if (motorsAreMoving)
      {
        digitalWrite(LED_red, LOW);
        digitalWrite(LED_green, HIGH);
        digitalWrite(LED_blue, LOW);
      }
      else if (state.indexOf("Idle") != -1)
      {
        digitalWrite(LED_red, HIGH);
        digitalWrite(LED_green, LOW);
        digitalWrite(LED_blue, LOW);
      }
    }
    else if (currentMode == MAZE)
    {
      if (state.indexOf("Obstacle!") != -1)
      {
        digitalWrite(LED_red, HIGH);
        digitalWrite(LED_green, LOW);
        digitalWrite(LED_blue, LOW);
      }
      else
      {
        digitalWrite(LED_red, LOW);
        digitalWrite(LED_green, HIGH);
        digitalWrite(LED_blue, LOW);
      }
    }
    else if (currentMode == TESTING)
    {
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_green, LOW);
      digitalWrite(LED_blue, LOW);
    }
  }
}
// DC Motors - Verified
void motorsSpeed(){
  int potentiometerValue = analogRead(potentiometer); 
  int speed = map(potentiometerValue, 0, 1023, 0, 255);
  analogWrite(motors_ENA, speed);
  analogWrite(motors_ENB, speed);
}
void motorsForward()
{
  digitalWrite(motors_IN1, HIGH);
  digitalWrite(motors_IN2, LOW);
  digitalWrite(motors_IN3, LOW);
  digitalWrite(motors_IN4, HIGH);
}
void motorsBackward()
{
  digitalWrite(motors_IN1, LOW);
  digitalWrite(motors_IN2, HIGH);
  digitalWrite(motors_IN3, HIGH);
  digitalWrite(motors_IN4, LOW);
}
void motorsLeft()
{
  digitalWrite(motors_IN1, HIGH);
  digitalWrite(motors_IN2, LOW);
  digitalWrite(motors_IN3, HIGH);
  digitalWrite(motors_IN4, LOW);
}
void motorsRight()
{
  digitalWrite(motors_IN1, LOW);
  digitalWrite(motors_IN2, HIGH);
  digitalWrite(motors_IN3, LOW);
  digitalWrite(motors_IN4, HIGH);
}
void motorsStop()
{
  analogWrite(motors_ENA, 0);
  analogWrite(motors_ENB, 0);
  digitalWrite(motors_IN1, LOW);
  digitalWrite(motors_IN2, LOW);
  digitalWrite(motors_IN3, LOW);
  digitalWrite(motors_IN4, LOW);
}
// Buzzer - Verified
void buzzer_activeBeep(int duration)
{
  tone(buzzer, NOTE_C5, 100);
  delay(duration);
  noTone(buzzer);
}
// Ultrasonic Sensor - Verified
long ultrasonicSensor_measureDistance()
{
  digitalWrite(ultrasonicSensor_trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicSensor_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicSensor_trigger, LOW);
  long duration = pulseIn(ultrasonicSensor_echo, HIGH, 25000);
  long distance = duration * 0.0343 / 2;
  return distance;
}
// Reset mode - Verified
void handleReset(int handleReset_delay)
{
  servoPos = 90;
  isMazePaused = false;
  ensureSingleModeAccess = 1;
  lcd.clear();
  motorsStop();
  myServo.write(90);
  noTone(buzzer);
  sevenSegment_clearAllSegments();
  digitalWrite(LED_red, LOW);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_blue, LOW);
  delay(handleReset_delay);
}
// Emergency mode - Verified
void handleEmergency()
{
  
  motorsStop();
  myServo.write(90);
  sevenSegment_clearAllSegments();
  digitalWrite(LED_red, LOW);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_blue, LOW);
  servoPos = 90;
  isMazePaused = false;
  menuPage = 1;
  ensureSingleModeAccess = 1;
  Serial.println(" -- EMERGENCY MODE TRIGGERED !");
  Serial1.println(" -- EMERGENCY MODE TRIGGERED !");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" EMERGENCY MODE");
  lcd.setCursor(0, 1);
  lcd.print(" TRIGGERED !!!!");
  for (int i = 0; i < 7; i++)
  {
    sevenSegment_displayLetter('E');
    digitalWrite(LED_red, HIGH);
    handleEmergency_buzzer_passiveBeep();
    sevenSegment_clearAllSegments();
    digitalWrite(LED_red, LOW);
    delay(50);
  }
  delay(1000);
}
void handleEmergency_buzzer_passiveBeep()
{
  tone(buzzer, NOTE_C6, 100);
  delay(70);
  noTone(buzzer);
}
// Bluetooth mode - Verified
void handleBluetooth()
{
  static bool isForwardToggled = false;
  static bool isBackwardToggled = false;
  static bool isLeftToggled = false;
  static bool isRightToggled = false;
  static bool isServoToggled = false;
  static String lastNonMovementState = "";
  auto resetMotorsToggle = [&]()
  {
    isForwardToggled = false;
    isBackwardToggled = false;
    isLeftToggled = false;
    isRightToggled = false;
    motorsStop();
    lastNonMovementState = "";
  };
  if (btModeJustEntered)
  {
    resetMotorsToggle();
    handleReset();
    updateMovementState("Waiting for inpt", true);
    btModeJustEntered = false;
  }
  char keyboardPress = 0;
  while (Serial1.available())
  {
    keyboardPress = Serial1.read();
  }
  if (keyboardPress != 0 && keyboardPress != '\n' && keyboardPress != '\r')
  {
    switch (keyboardPress)
    {
    // DC Motors
    case 'Z':
      if (isForwardToggled)
      {
        resetMotorsToggle();
      }
      else
      {
        resetMotorsToggle();
        isForwardToggled = true;
        motorsForward();
        updateMovementState("Motors Forward");
        lastNonMovementState = "Motors Forward";
      }
      break;
    case 'S':
      if (isBackwardToggled)
      {
        resetMotorsToggle();
      }
      else
      {
        motorsBackward();
        isBackwardToggled = true;
        motorsBackward();
        updateMovementState("Motors Backward");
        lastNonMovementState = "Motors Backward";
      }
      break;
    case 'Q':
      if (isLeftToggled)
      {
        resetMotorsToggle();
      }
      else
      {
        resetMotorsToggle();
        isLeftToggled = true;
        motorsLeft();
        updateMovementState("Motors Left");
        lastNonMovementState = "Motors Left";
      }
      break;
    case 'D':
      if (isRightToggled)
      {
        resetMotorsToggle();
      }
      else
      {
        resetMotorsToggle();
        isRightToggled = true;
        motorsRight();
        updateMovementState("Motors Right");
        lastNonMovementState = "Motors Right";
      }
      break;
    case ' ':
      resetMotorsToggle();
      updateMovementState("Motors Idle");
      lastNonMovementState = "Motors Idle";
      break;
    // Servo Motor
    case 'M':
      if (isServoToggled)
      {
        myServo.write(90);
        servoPos = 90;
        updateMovementState("Servo MIDDLE");
        lastNonMovementState = "Servo MIDDLE";
      }
      else
      {
        myServo.write(0);
        servoPos = 0;
        updateMovementState("Servo RIGHT");
        lastNonMovementState = "Servo RIGHT";
      }
      break;
    case 'K':
      if (isServoToggled)
      {
        myServo.write(90);
        servoPos = 90;
        updateMovementState("Servo MIDDLE");
        lastNonMovementState = "Servo MIDDLE";
      }
      else
      {
        myServo.write(180);
        servoPos = 180;
        updateMovementState("Servo LEFT");
        lastNonMovementState = "Servo LEFT";
      }
      break;
    case 'O':
    case 'L':
      myServo.write(90);
      servoPos = 90;
      updateMovementState("Servo MIDDLE");
      lastNonMovementState = "Servo MIDDLE";
      break;
    case 'I':
      if (isServoToggled)
      {
        myServo.write(90);
        servoPos = 90;
        updateMovementState("Servo MIDDLE");
        lastNonMovementState = "Servo MIDDLE";
      }
      else
      {
        myServo.write(135);
        servoPos = 135;
        updateMovementState("Servo left");
        lastNonMovementState = "Servo left";
      }
      break;
    case 'P':
      if (isServoToggled)
      {
        myServo.write(90);
        servoPos = 90;
        updateMovementState("Servo MIDDLE");
        lastNonMovementState = "Servo MIDDLE";
      }
      else
      {
        myServo.write(45);
        servoPos = 45;
        updateMovementState("Servo right");
        lastNonMovementState = "Servo right";
      }
      break;
    // Buzzer
    case 'B':
      buzzerEnabled = !buzzerEnabled;
      if (buzzerEnabled)
      {
        tone(buzzer, NOTE_A4);
        updateMovementState("Buzzer ON");
        lastNonMovementState = "Buzzer ON";
      }
      else
      {
        noTone(buzzer);
        updateMovementState("Buzzer OFF");
        lastNonMovementState = "Buzzer OFF";
      }
      break;
    // Emergency mode
    case 'X':
      handleEmergency();
      LCD_menuMessage();
      return;
    }
  }
}
// Maze mode
void handleMaze()
{
  if (ensureSingleModeAccess == 1)
  {
    ensureSingleModeAccess = 0;
    Serial.println(" -- MAZE MODE TRIGGERED !");
    Serial1.println(" -- MAZE MODE TRIGGERED !");
  }
  static unsigned long lastSegmentBlinkTime = 0;
  static bool showSegmentDigit = true;
  static unsigned long lastBeepTimeMoving = 0;
  digitalWrite(LED_blue, LOW);
  if (millis() - lastSegmentBlinkTime >= 300)
  {
    lastSegmentBlinkTime = millis();
    showSegmentDigit = !showSegmentDigit;
    if (showSegmentDigit)
    {
      sevenSegment_displayDigit(2);
    }
    else
    {
      sevenSegment_clearAllSegments();
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Distance : ");
  long currentDistance = ultrasonicSensor_measureDistance();
  if (currentDistance < 10)
    lcd.print("0");
  if (currentDistance < 100)
    lcd.print(currentDistance);
  else
    lcd.print("99");
  lcd.print(" cm      ");
  switch (currenthandleMaze_mazeState)
  {
  case MAZE_STATE_MOVING_FORWARD:
    motorsForward();
    updateMovementState("Forward...");
    if (millis() - lastBeepTimeMoving >= 800)
    {
      lastBeepTimeMoving = millis();
      buzzer_activeBeep(50);
    }
    if (ultrasonicSensor_measureDistance() <= OBSTACLE_DISTANCE)
    {
      currenthandleMaze_mazeState = MAZE_STATE_OBSTACLE_DETECTED;
      stateStartTime = millis();
      motorsStop();
      updateMovementState("Obstacle!!!!");
      for (int m = 0; m < 5; m++)
      {
        tone(buzzer, NOTE_B3);
        delay(100);
        noTone(buzzer);
      }
      delay(1000);
    }
    break;
  case MAZE_STATE_OBSTACLE_DETECTED:
    currenthandleMaze_mazeState = MAZE_STATE_SCANNING;
    stateStartTime = millis();
    break;
  case MAZE_STATE_SCANNING:
    updateMovementState("Scanning...");
    myServo.write(0);
    delay(300);
    distRight = ultrasonicSensor_measureDistance();
    Serial.print("Right: ");
    Serial.println(distRight);
    myServo.write(180);
    delay(300);
    distLeft = ultrasonicSensor_measureDistance();
    Serial.print("Left: ");
    Serial.println(distLeft);
    myServo.write(90);
    delay(100);
    if (distRight > TURN_CLEARANCE_DISTANCE)
    {
      currenthandleMaze_mazeState = MAZE_STATE_TURNING_RIGHT;
    }
    else if (distLeft > TURN_CLEARANCE_DISTANCE)
    {
      currenthandleMaze_mazeState = MAZE_STATE_TURNING_LEFT;
    }
    else
    {
      currenthandleMaze_mazeState = MAZE_STATE_TURNING_AROUND;
    }
    stateStartTime = millis();
    break;
  case MAZE_STATE_TURNING_LEFT:
    tone(buzzer, NOTE_C7);
    updateMovementState("Turning Left");
    handleMaze_motorsTurn("left");
    currenthandleMaze_mazeState = MAZE_STATE_MOVING_FORWARD;
    stateStartTime = millis();
    break;
  case MAZE_STATE_TURNING_RIGHT:
    tone(buzzer, NOTE_C7);
    updateMovementState("Turning Right");
    handleMaze_motorsTurn("right");
    currenthandleMaze_mazeState = MAZE_STATE_MOVING_FORWARD;
    stateStartTime = millis();
    break;
  case MAZE_STATE_TURNING_AROUND:
    tone(buzzer, NOTE_C7);
    updateMovementState("Turning Around");
    handleMaze_motorsTurn("around");
    currenthandleMaze_mazeState = MAZE_STATE_MOVING_FORWARD;
    stateStartTime = millis();
    break;
  }
  noTone(buzzer);
}
void handleMaze_motorsTurn(String direction)
{
  if (direction == "left")
  {
    motorsLeft();
    delay(MAZE_TURN_LEFT_DURATION);
  }
  else if (direction == "right")
  {
    motorsRight();
    delay(MAZE_TURN_RIGHT_DURATION);
  }
  else if (direction == "around")
  {
    motorsLeft();
    delay(MAZE_TURN_LEFT_DURATION * 2);
  }
  motorsStop();
  delay(100);
}
void handleMaze_buzzer_passiveBeep()
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
      tone(buzzer, creditsMelody[i], duration);
    }
    long pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzer);
  }
}
// Morse Communication mode
void handleMorseCommunication()
{
  if (ensureSingleModeAccess == 1)
  {
    ensureSingleModeAccess = 0;
    Serial.println(" -- MORSE COMMUNICATION MODE TRIGGERED !");
    Serial1.println(" -- MORSE COMMUNICATION MODE TRIGGERED !");
    lcd.clear();
    sevenSegment_clearAllSegments();
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_green, LOW);
    digitalWrite(LED_blue, LOW);
    buzzer_activeBeep(100);
    sevenSegment_displayLetter('M');
    digitalWrite(LED_blue, HIGH);
    delay(100);
    digitalWrite(LED_blue, LOW);
    sevenSegment_clearAllSegments();
    buzzer_activeBeep(100);
    digitalWrite(LED_blue, HIGH);
    sevenSegment_displayLetter('M');
    motorsStop();
    Serial.println("\n -- Ready for Serial Input or Keypad Command !\n");
    Serial1.println("\n -- Ready for Serial Input or Keypad Command !\n");
    Serial.println(" -- Transmitted Message :\n");
    Serial1.println(" -- Transmitted Message :\n");
    lcd.setCursor(0, 0);
    lcd.print("MORSE COMMT MODE");
    lcd.setCursor(0, 1);
    lcd.print("USE COOLTERM /BT");
    delay(1000);
    digitalWrite(LED_blue, LOW);
    sevenSegment_clearAllSegments();
    delay(1000);
    lcd.clear();
    buzzer_activeBeep(100);
    sevenSegment_displayLetter('8');
    digitalWrite(LED_red, HIGH);
    delay(100);
    sevenSegment_clearAllSegments();
    lcd.setCursor(0, 0);
    lcd.print("SEND YOUR LETTER");
    lcd.setCursor(0, 1);
    lcd.print("~");
  }
  char keyboardPress = 0;
  while (Serial1.available())
  {
    keyboardPress = Serial1.read();
    char receivedChar = Serial1.read();
    Serial.println(keyboardPress);
    switch (keyboardPress)
    {
    case 'A':
      handleMorseCommunication_character('A');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('A');
      morseRepeater++;
      break;
    case 'B':
      handleMorseCommunication_character('B');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('B');
      morseRepeater++;
      break;
    case 'C':
      handleMorseCommunication_character('C');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('C');
      morseRepeater++;
      break;
    case 'D':
      handleMorseCommunication_character('D');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('D');
      morseRepeater++;
      break;
    case 'E':
      handleMorseCommunication_character('E');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('E');
      morseRepeater++;
      break;
    case '-':
      handleMorseCommunication_character('-');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('-');
      morseRepeater++;
      break;
    case 'F':
      handleMorseCommunication_character('F');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('F');
      morseRepeater++;
      break;
    case 'G':
      handleMorseCommunication_character('G');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('G');
      morseRepeater++;
      break;
    case 'H':
      handleMorseCommunication_character('H');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('H');
      morseRepeater++;
      break;
    case 'I':
      handleMorseCommunication_character('I');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('I');
      morseRepeater++;
      break;
    case 'J':
      handleMorseCommunication_character('J');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('J');
      morseRepeater++;
      break;
    case 'K':
      handleMorseCommunication_character('K');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('K');
      morseRepeater++;
      break;
    case 'L':
      handleMorseCommunication_character('L');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('L');
      morseRepeater++;
      break;
    case '!':
      handleMorseCommunication_character('!');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('!');
      morseRepeater++;
      break;
    case '?':
      handleMorseCommunication_character('?');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('?');
      morseRepeater++;
      break;
    case '.':
      handleMorseCommunication_character('.');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('.');
      morseRepeater++;
      break;
    case ',':
      handleMorseCommunication_character(',');
      lcd.setCursor(morseRepeater, 1);
      lcd.print(',');
      morseRepeater++;
      break;
    case ':':
      handleMorseCommunication_character(':');
      lcd.setCursor(morseRepeater, 1);
      lcd.print(':');
      morseRepeater++;
      break;
    case 'M':
      handleMorseCommunication_character('M');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('M');
      morseRepeater++;
      break;
    case 'N':
      handleMorseCommunication_character('N');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('N');
      morseRepeater++;
      break;
    case 'O':
      handleMorseCommunication_character('O');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('O');
      morseRepeater++;
      break;
    case 'P':
      handleMorseCommunication_character('P');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('P');
      morseRepeater++;
      break;
    case 'Q':
      handleMorseCommunication_character('Q');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('Q');
      morseRepeater++;
      break;
    case 'R':
      handleMorseCommunication_character('R');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('R');
      morseRepeater++;
      break;
    case 'S':
      handleMorseCommunication_character('S');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('S');
      morseRepeater++;
      break;
    case 'T':
      handleMorseCommunication_character('T');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('T');
      morseRepeater++;
      break;
    case 'U':
      handleMorseCommunication_character('U');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('U');
      morseRepeater++;
      break;
    case 'V':
      handleMorseCommunication_character('V');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('V');
      morseRepeater++;
      break;
    case 'W':
      handleMorseCommunication_character('W');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('W');
      morseRepeater++;
      break;
    case 'X':
      handleMorseCommunication_character('X');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('X');
      morseRepeater++;
      break;
    case 'Y':
      handleMorseCommunication_character('Y');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('Y');
      morseRepeater++;
      break;
    case 'Z':
      handleMorseCommunication_character('Z');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('Z');
      morseRepeater++;
      break;
    case '0':
      handleMorseCommunication_character('0');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('0');
      morseRepeater++;
      break;
    case '1':
      handleMorseCommunication_character('1');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('1');
      morseRepeater++;
      break;
    case '2':
      handleMorseCommunication_character('2');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('2');
      morseRepeater++;
      break;
    case '3':
      handleMorseCommunication_character('3');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('3');
      morseRepeater++;
      break;
    case '4':
      handleMorseCommunication_character('4');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('4');
      morseRepeater++;
      break;
    case '5':
      handleMorseCommunication_character('5');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('5');
      morseRepeater++;
      break;
    case '6':
      handleMorseCommunication_character('6');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('6');
      morseRepeater++;
      break;
    case '7':
      handleMorseCommunication_character('7');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('7');
      morseRepeater++;
      break;
    case '8':
      handleMorseCommunication_character('8');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('8');
      morseRepeater++;
      break;
    case '9':
      handleMorseCommunication_character('9');
      lcd.setCursor(morseRepeater, 1);
      lcd.print('9');
      morseRepeater++;
      break;
    case ' ':
      handleMorseCommunication_character(' ');
      break;
    default:
      break;
    }
  }
  delay(50);
}
void handleMorseCommunication_element(char element)
{
  int duration;
  if (element == '.')
  {
    duration = DOT_DURATION;
    tone(buzzer, NOTE_A3);
  }
  else if (element == '-')
  {
    duration = DASH_DURATION;
    tone(buzzer, NOTE_E3);
  }
  else
  {
    return;
  }
  digitalWrite(LED_green, HIGH);
  digitalWrite(LED_red, LOW);
  delay(duration);
  noTone(buzzer);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_red, HIGH);
}
void handleMorseCommunication_character(char character)
{
  char upperChar = toupper(character);
  sevenSegment_displayLetter(upperChar);
  if (upperChar == 'A')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("A");
    Serial1.println("A");
  }
  else if (upperChar == '-')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("-");
    Serial1.println("-");
  }
  else if (upperChar == 'B')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("B");
    Serial1.println("B");
  }
  else if (upperChar == 'C')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("C");
    Serial1.println("C");
  }
  else if (upperChar == 'D')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("D");
    Serial1.println("D");
  }
  else if (upperChar == 'E')
  {
    handleMorseCommunication_element('.');
    Serial.println("E");
    Serial1.println("E");
  }
  else if (upperChar == 'F')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("F");
    Serial1.println("F");
  }
  else if (upperChar == 'G')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("G");
    Serial1.println("G");
  }
  else if (upperChar == 'H')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("H");
    Serial1.println("H");
  }
  else if (upperChar == 'I')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("I");
    Serial1.println("I");
  }
  else if (upperChar == 'J')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("J");
    Serial1.println("J");
  }
  else if (upperChar == 'K')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("K");
    Serial1.println("K");
  }
  else if (upperChar == 'L')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("L");
    Serial1.println("L");
  }
  else if (upperChar == 'M')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("M");
    Serial1.println("M");
  }
  else if (upperChar == 'N')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("N");
    Serial1.println("N");
  }
  else if (upperChar == 'O')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("O");
    Serial1.println("O");
  }
  else if (upperChar == 'P')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("P");
    Serial1.println("P");
  }
  else if (upperChar == 'Q')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("Q");
    Serial1.println("Q");
  }
  else if (upperChar == 'R')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("R");
    Serial1.println("R");
  }
  else if (upperChar == 'S')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("S");
    Serial1.println("S");
  }
  else if (upperChar == 'T')
  {
    handleMorseCommunication_element('-');
    Serial.println("T");
    Serial1.println("T");
  }
  else if (upperChar == 'U')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("U");
    Serial1.println("U");
  }
  else if (upperChar == 'V')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("V");
    Serial1.println("V");
  }
  else if (upperChar == 'W')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("W");
    Serial1.println("W");
  }
  else if (upperChar == 'X')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("X");
    Serial1.println("X");
  }
  else if (upperChar == 'Y')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("Y");
    Serial1.println("Y");
  }
  else if (upperChar == 'Z')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("Z");
    Serial1.println("Z");
  }
  else if (upperChar == '1')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("1");
    Serial1.println("1");
  }
  else if (upperChar == '2')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("2");
    Serial1.println("2");
  }
  else if (upperChar == '3')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("3");
    Serial1.println("3");
  }
  else if (upperChar == '4')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("4");
    Serial1.println("4");
  }
  else if (upperChar == '5')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("5");
    Serial1.println("5");
  }
  else if (upperChar == '6')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("6");
    Serial1.println("6");
  }
  else if (upperChar == '7')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("7");
    Serial1.println("7");
  }
  else if (upperChar == '8')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("8");
    Serial1.println("8");
  }
  else if (upperChar == '9')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("9");
    Serial1.println("9");
  }
  else if (upperChar == '0')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("0");
    Serial1.println("0");
  }
  else if (upperChar == '!')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println("!");
    Serial1.println("!");
  }
  else if (upperChar == '?')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    Serial.println("?");
    Serial1.println("?");
  }
  else if (upperChar == '.')
  {
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println(".");
    Serial1.println(".");
  }
  else if (upperChar == ',')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    Serial.println(",");
    Serial1.println(",");
  }
  else if (upperChar == ':')
  {
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('-');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
    delay(ELEMENT_SPACE);
    handleMorseCommunication_element('.');
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
// Follow Me mode
void handleFollowMe()
{
  if (ensureSingleModeAccess == 1)
  {
    ensureSingleModeAccess = 0;
    Serial.println(" -- FOLLOW ME MODE TRIGGERED !");
    Serial1.println(" -- FOLLOW ME MODE TRIGGERED !");
  }
  unsigned long currentTime = millis();
  digitalWrite(LED_red, LOW);
  digitalWrite(LED_green, LOW);
  if (currentTime - lastPauseBlinkTime >= MAZE_PAUSE_BLINK_INTERVAL)
  {
    lastPauseBlinkTime = currentTime;
    isPauseBlinkOn = !isPauseBlinkOn;
    if (isPauseBlinkOn)
    {
      sevenSegment_displayLetter('F');
      digitalWrite(LED_blue, HIGH);
    }
    else
    {
      sevenSegment_clearAllSegments();
      digitalWrite(LED_blue, LOW);
    }
  }
  long currentDistance = ultrasonicSensor_measureDistance();
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
  switch (currenthandleMaze_followState)
  {
  case FOLLOW_STATE_WAITING:
    motorsStop();
    break;
  case FOLLOW_STATE_FOLLOWING:
    if (currentDistance < MIN_DISTANCE)
    {
      motorsBackward();
    }
    else if (currentDistance > MAX_DISTANCE)
    {
      motorsForward();
    }
    else
    {
      motorsStop();
    }
    break;
  }
}
// Dice mode
void handleDice()
{
  static enum DiceState { WAITING,
                          ROLLING,
                          RESULT } currentDiceState = WAITING;
  static unsigned long rollStartTime = 0;
  const unsigned long ROLL_DURATION = 2500;
  if (ensureSingleModeAccess == 1)
  {
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_green, LOW);
    digitalWrite(LED_blue, LOW);
    Serial.println(" -- DICE MODE TRIGGERED !");
    Serial1.println(" -- DICE MODE TRIGGERED !");
    lcd.clear();
    sevenSegment_clearAllSegments();
    motorsStop();
    LCD_displayMessageCentered("RANDOM DICE MODE", 0);
    LCD_displayMessageCentered("~Press to Roll", 1);
    buzzer_activeBeep(100);
    sevenSegment_displayLetter('D');
    digitalWrite(LED_blue, HIGH);
    delay(100);
    digitalWrite(LED_blue, LOW);
    sevenSegment_clearAllSegments();
    buzzer_activeBeep(100);
    digitalWrite(LED_blue, HIGH);
    sevenSegment_displayLetter('D');
    motorsStop();
    ensureSingleModeAccess = 0;
    currentDiceState = WAITING;
  }
  char customKey = keypad.getKey();
  if (customKey == 'D')
  {
    handleEmergency();
    currentMode = NONE;
    LCD_menuMessage();
    return;
  }
  switch (currentDiceState)
  {
  case WAITING:
  {
    if ((digitalRead(joystick_button) == LOW) || (customKey == '4'))
    {
      currentDiceState = ROLLING;
      rollStartTime = millis();
      randomSeed(analogRead(A0));
      buzzer_activeBeep(50);
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
      sevenSegment_displayDigit(currentDigit);
      buzzer_activeBeep();
      if ((currentTime / 100) % 2 == 0)
      {
        lcd.setCursor(0, 1);
        lcd.print("   ROLLING...   ");
        digitalWrite(LED_blue, HIGH);
      }
      else
      {
        lcd.setCursor(0, 1);
        lcd.print("                ");
        digitalWrite(LED_blue, LOW);
      }
    }
    else
    {
      digitalWrite(LED_blue, LOW);
      int final_roll = random(0, 10);
      Serial.println("\n -- Result is : \n");
      Serial1.println("\n -- Result is : \n");
      Serial.println(final_roll);
      Serial1.println(final_roll);
      sevenSegment_displayDigit(final_roll);
      lcd.clear();
      LCD_displayMessageCentered("ROLL COMPLETE!", 0);
      LCD_displayMessageCentered("Result: " + String(final_roll), 1);
      digitalWrite(LED_green, HIGH);
      tone(buzzer, NOTE_C6);
      currentDiceState = RESULT;
      rollStartTime = millis();
      delay(1000);
    }
    noTone(buzzer);
    break;
  }
  case RESULT:
  {
    if (millis() - rollStartTime >= 3000)
    {
      sevenSegment_clearAllSegments();
      lcd.clear();
      Serial.println("\n -- DICE MODE READY !");
      Serial1.println("\n -- DICE MODE READY !");
      LCD_displayMessageCentered("RANDOM DICE MODE", 0);
      LCD_displayMessageCentered("~Press to Roll", 1);
      digitalWrite(LED_green, LOW);
      buzzer_activeBeep(100);
      sevenSegment_displayLetter('D');
      digitalWrite(LED_blue, HIGH);
      delay(100);
      digitalWrite(LED_blue, LOW);
      sevenSegment_clearAllSegments();
      buzzer_activeBeep(100);
      digitalWrite(LED_blue, HIGH);
      sevenSegment_displayLetter('D');
      currentDiceState = WAITING;
    }
    break;
  }
  }
}
// Credits mode
void handleCredits()
{
  if (ensureSingleModeAccess == 1)
  {
    ensureSingleModeAccess = 0;
    Serial.println(" -- CREDITS MODE TRIGGERED !");
    Serial1.println(" -- CREDITS MODE TRIGGERED !");
  }
  sevenSegment_clearAllSegments();
  motorsStop();
  myServo.write(90);
  digitalWrite(LED_red, LOW);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_blue, LOW);
  String creditsMessages[] = {
      "Made by :\nNizar EL IDRYSY",
      "Contains the\nmodified .ino",
      String(handleCredits_codeLinesCounter_calculator) + " lines of\ndedicated coding",
      "Use CoolTerm on\nWindows for HC06",
      "Shout out to my\nbro Gemini ;)",
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
      "DAMI 3000 !\nLoves you ALL :D"};
  const int numMessages = sizeof(creditsMessages) / sizeof(creditsMessages[0]);
  if (millis() - lastMessageTime >= 2500)
  {
    lastMessageTime = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    int newlinePos = creditsMessages[currentCreditMessage].indexOf('\n');
    if (newlinePos != -1)
    {
      lcd.print(creditsMessages[currentCreditMessage].substring(0, newlinePos));
      lcd.setCursor(0, 1);
      lcd.print(creditsMessages[currentCreditMessage].substring(newlinePos + 1));
    }
    else
    {
      LCD_displayMessageCentered(creditsMessages[currentCreditMessage], 0);
    }
    currentCreditMessage++;
    if (currentCreditMessage >= numMessages)
    {
      currentCreditMessage = 0;
    }
  }
}
void handleCredits_servoMotorSweep()
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
    myServo.write(servoPosition);
  }
}
// Manual Test mode
void handleManualTest(char customKey)
{
  if (ensureSingleModeAccess == 1)
  {
    ensureSingleModeAccess = 0;
    Serial.println(" -- MANUAL TESTING MODE TRIGGERED !");
    Serial1.println(" -- MANUAL TESTING MODE TRIGGERED !");
  }
  if (manTstJustEntered)
  {
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_green, LOW);
    digitalWrite(LED_blue, LOW);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("     Ready!");
    motorsStop();
    myServo.write(90);
    sevenSegment_clearAllSegments();
    digitalWrite(buzzer, LOW);
    sevenSegIsOn = false;
    manTstJustEntered = false;
  }
  unsigned long currentTime = millis();
  if (!customKey && currentTime - lastDistanceUpdateTime >= DISTANCE_UPDATE_INTERVAL)
  {
    lastDistanceUpdateTime = currentTime;
    long distanceCm = ultrasonicSensor_measureDistance();
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
      motorsForward();
      updateMovementState(" MOTORS FORWARD");
    }
    else if (customKey == '4')
    {
      motorsRight();
      updateMovementState("MOTORS RIGHT");
    }
    else if (customKey == '6')
    {
      motorsLeft();
      updateMovementState("MOTORS LEFT");
    }
    else if (customKey == '2')
    {
      motorsBackward();
      updateMovementState("MOTORS BACKWARD");
    }
    else if (customKey == '5')
    {
      motorsStop();
      updateMovementState("ROBOT IDLE");
    }
    else if (customKey == 'A')
    {
      digitalWrite(LED_red, HIGH);
      digitalWrite(LED_green, LOW);
      digitalWrite(LED_blue, LOW);
      updateMovementState("RED LED: ON");
    }
    else if (customKey == 'B')
    {
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_green, HIGH);
      digitalWrite(LED_blue, LOW);
      updateMovementState(" GREEN LED: ON");
    }
    else if (customKey == 'C')
    {
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_green, LOW);
      digitalWrite(LED_blue, HIGH);
      updateMovementState("BLUE LED: ON");
    }
    if (customKey == '9')
    {
      sevenSegIsOn = !sevenSegIsOn;
      if (sevenSegIsOn)
      {
        for (int i = 0; i < 7; i++)
        {
          digitalWrite(sevenSegmentPin[i], HIGH);
        }
        lcd.setCursor(0, 1);
        lcd.print("   7-SEG: ON       ");
      }
      else
      {
        sevenSegment_clearAllSegments();
        lcd.setCursor(0, 1);
        lcd.print("   7-SEG: OFF      ");
      }
    }
  }
  if (customKey == '0' || analogRead(joystick_VRY) > 950 || analogRead(joystick_VRY) < 150)
  {
    myServo.write(90);
    updateMovementState("SERVO CENTERED");
  }
  else if (customKey == '*' || analogRead(joystick_VRX) > 950)
  {
    myServo.write(0);
    updateMovementState("SERVO RIGHT");
  }
  else if (customKey == '#' || analogRead(joystick_VRX) < 150)
  {
    myServo.write(180);
    updateMovementState("SERVO LEFT");
  }
  else if (customKey == '3')
  {
    myServo.write(135);
    updateMovementState("SERVO left");
  }
  else if (customKey == '1')
  {
    myServo.write(45);
    updateMovementState("SERVO right");
  }
  if (customKey == '7' || digitalRead(joystick_button) == LOW)
  {
    buzzerIsOn = !buzzerIsOn;
    if (buzzerIsOn)
    {
      LCD_displayMessageCentered("buzzer Testing", 1);
      for (int m = 0; m < 2000; m++)
      {
        tone(buzzer, m);
        delay(2);
      }
    }
    LCD_displayMessageCentered("buzzer Test Done", 1);
    buzzerIsOn = !buzzerIsOn;
    noTone(buzzer);
  }
}
// Automatic Test mode
void handleAutomaticTest()
{
  if (ensureSingleModeAccess == 1)
  {
    ensureSingleModeAccess = 0;
    Serial.println(" -- AUTOMATIC TESTING MODE TRIGGERED !");
    Serial1.println(" -- AUTOMATIC TESTING MODE TRIGGERED !");
  }
  static unsigned long lastLedBlinkTime = 0;
  static bool ledState = LOW;
  static unsigned long lastBuzzerBeapTime = 0;
  static int rgbLedCycle = 0;
  if (millis() - lastBuzzerBeapTime >= 500)
  {
    lastBuzzerBeapTime = millis();
    sevenSegment_displayDigit(8);
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_green, LOW);
    digitalWrite(LED_blue, LOW);
    if (rgbLedCycle == 0)
    {
      digitalWrite(LED_red, HIGH);
    }
    else if (rgbLedCycle == 1)
    {
      digitalWrite(LED_green, HIGH);
    }
    else if (rgbLedCycle == 2)
    {
      digitalWrite(LED_blue, HIGH);
    }
    buzzer_activeBeep(50);
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_green, LOW);
    digitalWrite(LED_blue, LOW);
    rgbLedCycle = (rgbLedCycle + 1) % 3;
  }
  lcd.setCursor(0, 0);
  lcd.print("Distance : ");
  long distance = ultrasonicSensor_measureDistance();
  if (distance < 10)
  {
    lcd.print("0");
  }
  if (distance < 100)
    lcd.print(distance);
  else
    lcd.print("99");
  lcd.print(" cm");
  switch (currenthandleAutomaticTest_testState)
  {
  case TEST_STATE_INIT:
    motorsStop();
    myServo.write(90);
    updateMovementState("Testing...");
    currenthandleAutomaticTest_testState = TEST_STATE_FORWARD;
    testStateStartTime = millis();
    break;
  case TEST_STATE_FORWARD:
    motorsForward();
    if (millis() - testStateStartTime == 0)
      updateMovementState("Forward");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      motorsStop();
      currenthandleAutomaticTest_testState = TEST_STATE_BACKWARD;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_BACKWARD:
    motorsBackward();
    if (millis() - testStateStartTime == 0)
      updateMovementState("Backward");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      motorsStop();
      currenthandleAutomaticTest_testState = TEST_STATE_TURN_LEFT;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_TURN_LEFT:
    motorsLeft();
    if (millis() - testStateStartTime == 0)
      updateMovementState("Left");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      motorsStop();
      currenthandleAutomaticTest_testState = TEST_STATE_TURN_RIGHT;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_TURN_RIGHT:
    motorsRight();
    if (millis() - testStateStartTime == 0)
      updateMovementState("Right");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      motorsStop();
      currenthandleAutomaticTest_testState = TEST_STATE_SERVO_LEFT;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_SERVO_LEFT:
    myServo.write(0);
    if (millis() - testStateStartTime == 0)
      updateMovementState("Servo Left");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      currenthandleAutomaticTest_testState = TEST_STATE_SERVO_CENTER;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_SERVO_CENTER:
    myServo.write(90);
    if (millis() - testStateStartTime == 0)
      updateMovementState("Servo Centered");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      currenthandleAutomaticTest_testState = TEST_STATE_SERVO_RIGHT;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_SERVO_RIGHT:
    myServo.write(180);
    if (millis() - testStateStartTime == 0)
      updateMovementState("Servo Right");
    if (millis() - testStateStartTime >= TEST_ACTION_DURATION)
    {
      currenthandleAutomaticTest_testState = TEST_STATE_CYCLE_COMPLETE;
      testStateStartTime = millis();
    }
    break;
  case TEST_STATE_CYCLE_COMPLETE:
    motorsStop();
    myServo.write(90);
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_green, LOW);
    digitalWrite(LED_blue, LOW);
    updateMovementState("Test Finished.");
    tone(buzzer, NOTE_C3);
    currenthandleAutomaticTest_testState = TEST_STATE_INIT;
    testStateStartTime = millis();
    delay(1000);
    noTone(buzzer);
    delay(1500);
    break;
  }
}
// Joystick Test mode
void handleJoystickTest()
{
  if (ensureSingleModeAccess == 1)
  {
    ensureSingleModeAccess = 0;
    Serial.println(" -- JOYSTICK TESTING MODE TRIGGERED !");
    Serial1.println(" -- JOYSTICK TESTING MODE TRIGGERED !");
  }
  int vrxValue = analogRead(joystick_VRX);
  int vryValue = analogRead(joystick_VRY);
  int clickState = digitalRead(joystick_button);
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
  digitalWrite(LED_red, LOW);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_blue, LOW);
}
// LCD
void LCD_displayMessageCentered(String msg, int row)
{
  int len = msg.length();
  int startCol = max(0, (16 - len) / 2);
  lcd.setCursor(0, row);
  for (int i = 0; i < 16; ++i)
    lcd.print(" ");
  lcd.setCursor(startCol, row);
  lcd.print(msg);
}
void LCD_welcomeMessage()
{
  LCD_displayMessageCentered("Resetting all", 0);
  LCD_displayMessageCentered("Components...", 1);
  Serial.println(" -- Resetting all components...");
  Serial1.println(" -- Resetting all components...");
  delay(1500);
  handleReset();
  lcd.clear();
  LCD_displayMessageCentered("Click Joystick", 0);
  LCD_displayMessageCentered("to start !", 1);
  Serial.println(" -- Click Joystick to start !");
  Serial1.println(" -- Click Joystick to start !");
  bool readJostickClick = false;
  if (digitalRead(joystick_button) == HIGH)
  {
    readJostickClick = true;
  }
  while (readJostickClick)
  {
    if (digitalRead(joystick_button) == LOW)
    {
      Serial.println(" -- Hello! I'm DAMI 3000!");
      Serial1.println(" -- Hello! I'm DAMI 3000!");
      lcd.clear();
      LCD_displayMessageCentered("Hello!", 0);
      LCD_displayMessageCentered("I'm DAMI 3000!", 1);
      LCD_welcomeMessage_buzzer_passiveBeep();
      delay(100);
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_green, LOW);
      digitalWrite(LED_blue, LOW);
      digitalWrite(sevenSegmentPin[6], LOW);
      delay(50);
      delay(1000);
      break;
    }
  }
}
void LCD_welcomeMessage_buzzer_passiveBeep()
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
    tone(buzzer, pacmanMelody[i], duration);
    if (i == 0)
    {
      myServo.write(70);
      digitalWrite(LED_red, HIGH);
      digitalWrite(LED_green, LOW);
      digitalWrite(LED_blue, LOW);
      digitalWrite(sevenSegmentPin[i], HIGH);
      delay(50);
    }
    else if (i == 1)
    {
      myServo.write(110);
      digitalWrite(LED_green, HIGH);
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_blue, LOW);
      digitalWrite(sevenSegmentPin[i], HIGH);
      delay(50);
    }
    else if (i == 2)
    {
      myServo.write(70);
      digitalWrite(LED_blue, HIGH);
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_green, LOW);
      digitalWrite(sevenSegmentPin[i], HIGH);
      delay(50);
    }
    else if (i == 3)
    {
      myServo.write(110);
      digitalWrite(LED_green, HIGH);
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_blue, LOW);
      digitalWrite(sevenSegmentPin[i], HIGH);
      delay(50);
    }
    else if (i == 4)
    {
      myServo.write(70);
      digitalWrite(LED_red, HIGH);
      digitalWrite(LED_green, LOW);
      digitalWrite(LED_blue, LOW);
      digitalWrite(sevenSegmentPin[i], HIGH);
      delay(50);
    }
    else if (i == 5)
    {
      myServo.write(110);
      digitalWrite(LED_green, HIGH);
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_blue, LOW);
      digitalWrite(sevenSegmentPin[i], HIGH);
      delay(50);
    }
    else if (i == 6)
    {
      myServo.write(70);
      digitalWrite(LED_blue, HIGH);
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_green, LOW);
      digitalWrite(sevenSegmentPin[i], HIGH);
      delay(50);
    }
    else if (i == 7)
    {
      myServo.write(70);
      digitalWrite(LED_green, LOW);
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_blue, HIGH);
      digitalWrite(sevenSegmentPin[0], LOW);
      digitalWrite(sevenSegmentPin[1], LOW);
      digitalWrite(sevenSegmentPin[2], LOW);
      digitalWrite(sevenSegmentPin[3], LOW);
      digitalWrite(sevenSegmentPin[4], LOW);
      digitalWrite(sevenSegmentPin[5], LOW);
      digitalWrite(sevenSegmentPin[6], LOW);
      digitalWrite(sevenSegmentPin[7], LOW);
      delay(50);
    }
    else if (i == 8)
    {
      myServo.write(110);
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_green, HIGH);
      digitalWrite(LED_blue, LOW);
      digitalWrite(sevenSegmentPin[0], HIGH);
      digitalWrite(sevenSegmentPin[1], HIGH);
      digitalWrite(sevenSegmentPin[2], HIGH);
      digitalWrite(sevenSegmentPin[3], HIGH);
      digitalWrite(sevenSegmentPin[4], HIGH);
      digitalWrite(sevenSegmentPin[5], HIGH);
      digitalWrite(sevenSegmentPin[6], HIGH);
      digitalWrite(sevenSegmentPin[7], HIGH);
      delay(35);
      digitalWrite(sevenSegmentPin[0], LOW);
      digitalWrite(sevenSegmentPin[1], LOW);
      digitalWrite(sevenSegmentPin[2], LOW);
      digitalWrite(sevenSegmentPin[3], LOW);
      digitalWrite(sevenSegmentPin[4], LOW);
      digitalWrite(sevenSegmentPin[5], LOW);
      digitalWrite(sevenSegmentPin[6], LOW);
      digitalWrite(sevenSegmentPin[7], LOW);
      digitalWrite(LED_green, LOW);
      delay(15);
    }
    else if (i == 9)
    {
      myServo.write(90);
      digitalWrite(LED_green, HIGH);
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_blue, LOW);
      digitalWrite(sevenSegmentPin[0], HIGH);
      digitalWrite(sevenSegmentPin[1], HIGH);
      digitalWrite(sevenSegmentPin[2], HIGH);
      digitalWrite(sevenSegmentPin[3], HIGH);
      digitalWrite(sevenSegmentPin[4], HIGH);
      digitalWrite(sevenSegmentPin[5], HIGH);
      digitalWrite(sevenSegmentPin[6], HIGH);
      digitalWrite(sevenSegmentPin[7], HIGH);
      delay(35);
      digitalWrite(sevenSegmentPin[0], LOW);
      digitalWrite(sevenSegmentPin[1], LOW);
      digitalWrite(sevenSegmentPin[2], LOW);
      digitalWrite(sevenSegmentPin[3], LOW);
      digitalWrite(sevenSegmentPin[4], LOW);
      digitalWrite(sevenSegmentPin[5], LOW);
      digitalWrite(sevenSegmentPin[6], LOW);
      digitalWrite(sevenSegmentPin[7], LOW);
      digitalWrite(LED_green, LOW);
      delay(15);
    }
    int pauseTime = duration;
    delay(pauseTime);
    noTone(buzzer);
  }
}
void LCD_menuMessage()
{
  motorsStop();
  currentMode = NONE;
  sevenSegment_clearAllSegments();
  lcd.clear();
  lcd.clear();
  Serial.println(" -- Choose a mode :");
  Serial1.println(" -- Choose a mode :");
  if (menuPage == 1)
  {
  LCD_displayMessageCentered(" BTH MZE MRS FLM", 0);
  LCD_displayMessageCentered(" DCE CRD MTT ATT", 1);
  }
  else if (menuPage == 2){
    LCD_displayMessageCentered(" JTT xxx xxx xxx", 0);
    LCD_displayMessageCentered(" xxx xxx xxx xxx", 1);
    }
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_red, LOW);
  digitalWrite(LED_blue, LOW);
  myServo.write(90);
  }
void LCD_menuMessage_animation()
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
void LCD_menuMessage_buzzer_passiveBeep()
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
    tone(buzzer, pacmanMelody[i], duration);
    if (i == 0)
    {
      myServo.write(100);
      digitalWrite(sevenSegmentPin[0], HIGH);
      digitalWrite(LED_red, HIGH);
      digitalWrite(LED_green, LOW);
      digitalWrite(LED_blue, LOW);
    }
    if (i == 1)
    {
      myServo.write(80);
      digitalWrite(sevenSegmentPin[6], HIGH);
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_green, HIGH);
      digitalWrite(LED_blue, LOW);
    }
    if (i == 2)
    {
      myServo.write(90);
      digitalWrite(sevenSegmentPin[3], HIGH);
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_green, LOW);
      digitalWrite(LED_blue, HIGH);
    }
    int pauseTime = duration;
    delay(pauseTime);
    noTone(buzzer);
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_green, LOW);
    digitalWrite(LED_blue, LOW);
    sevenSegment_clearAllSegments();
  }
}
// 7 Segments Display
void sevenSegment_clearAllSegments()
{
  for (int i = 0; i < 7; i++)
    digitalWrite(sevenSegmentPin[i], LOW);
}
void sevenSegment_animation()
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
    sevenSegment_clearAllSegments();
    if (segmentIndex < 6)
    {
      digitalWrite(sevenSegmentPin[segmentIndex], HIGH);
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
    analogWrite(LED_red, (rgbLedCycle == 0) ? ledBrightness : LOW);
    analogWrite(LED_green, (rgbLedCycle == 1) ? ledBrightness : LOW);
    analogWrite(LED_blue, (rgbLedCycle == 2) ? ledBrightness : LOW);
  }
}
void sevenSegment_displayDigit(int digit)
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
    digitalWrite(sevenSegmentPin[i], (segments >> i) & 0x01);
  }
}
void sevenSegment_displayLetter(char letter)
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
    digitalWrite(sevenSegmentPin[i], (segments >> i) & 0x01);
  }
}
static const int handleCredits_codeLinesCounter_endLine = __LINE__;
static const int handleCredits_codeLinesCounter_calculator = handleCredits_codeLinesCounter_endLine - handleCredits_codeLinesCounter_firstLine;