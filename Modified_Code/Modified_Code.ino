#include <Wire.h> // I2C Library
#include <LiquidCrystal_I2C.h> // LCD Library
#include <Servo.h> // Servo-motor Library
#include <Keypad.h> // Keypad Library - NEW
int variablechanger=1;
bool isCreditsActive = false;
int currentCreditMessage = 0;
unsigned long lastMessageTime = 0;

// ⭐ NEW: Variables for 2-second menu switch
unsigned long lastMenuSwitchTime = 0;
bool isMenuSetOne = true;
const unsigned long MENU_SWITCH_INTERVAL = 2000; // 2 seconds
// New variable for servo toggle
bool isServoMovementEnabled = false;
/* Pins & functions */
// LEDs
int morseRepeater=1;
bool diceRollerJustEntered = false;
enum Mode { NONE, BLUETOOTH, MAZE, TESTING, CREDITS, FOLLOW_ME, MAN_TST , MORSE_COM, JOYSTICK_TEST, DICE};
// NEW: Global flag for manual test mode entry
// ⭐ NEW: Variables for MORSE_COM Mode
bool morseComJustEntered = false;
// ⭐ NEW: Variables for T9 (Multi-tap) Input Logic
unsigned long lastKeyPressTime = 0;
const unsigned long T9_TIMEOUT = 800; // 800ms to cycle letters or confirm input
char lastKey = '\0'; // Stores the last number key pressed
int cycleIndex = 0;
// ⭐ NEW: T9 Key Map
// A simple array of strings where index 0 is key '0', index 1 is key '1', etc.
// The characters are in the order they should cycle. 
// Note: We'll skip index 1 and 7, 9 since they typically have punctuation.
// We'll use your standard keypad map: 
// 1: A, B, C
// 2: 2, D, E, F
// 3: 3, G, H, I
// 4: 4, J, K, L
// 5: 5, M, N, O
// 6: 6, P, Q, R, S
// 7: 7, T, U, V
// 8: 8, W, X, Y, Z
// 9: 9, Punctuation
// 0: 0, Space
const char* T9_MAP[] = {
    " 0",      // Key '0' -> Space, 0
    "1",       // Key '1' -> 1
    "D2E",     // Key '2' -> 2, D, E, F (Using D, E from your known keys)
    "3F",      // Key '3' -> 3, F
    "4G",      // Key '4' -> 4, G
    "5H",      // Key '5' -> 5, H
    "6I",      // Key '6' -> 6, I
    "7J",      // Key '7' -> 7, J
    "8K",      // Key '8' -> 8, K
    "9L"       // Key '9' -> 9, L
    // We will handle A, B, C separately for the non-number keys if needed
}; // Tracks the current position in the character cycle (A, B, C...)
#define MAX_MESSAGE_LENGTH 16
char morseMessage[MAX_MESSAGE_LENGTH + 1] = ""; // 16 characters + null terminator
int messageIndex = 0; // Current position in the message array
bool isTransmitting = false; // Flag to indicate when buzzer is active
int currentMorseCharIndex = 0; // Index of the character currently being transmitted
int currentMorsePatternIndex = 0; // Index of the dot/dash in the current character's pattern
unsigned long lastMorseSignalTime = 0; // Timer for non-blocking timing
// ⭐ NEW: Morse Code Map for Keypad Characters
// Using '/' as the word separator (for simplicity, but could be ' ' or another key)
// 'x' is a simple stop marker
const char* getMorsePattern(char character) {
    switch (character) {
        // Keypad numbers 0-9
        case '1': return ".----";
        case '2': return "..---";
        case '3': return "...--";
        case '4': return "....-";
        case '5': return ".....";
        case '6': return "-....";
        case '7': return "--...";
        case '8': return "---..";
        case '9': return "----.";
        case '0': return "-----";

        // Keypad letters (Assuming your keypad has A-D, *, #)
        case 'A': return ".-";
        case 'B': return "-...";
        case 'C': return "-.-.";
        case 'D': return "-..";

        // Space/Word Separator (Using a simple code, or you could use a dedicated symbol)
        case ' ': // We will treat the '*' key as a space when inputting
        case '/': return "/"; // Special character for word/char space

        default: return ""; // Non-printable character
    }
}
// Morse timing constants (standard)
const int UNIT_TIME = 100; // 100ms is one 'dot' duration
const int DOT_DURATION = UNIT_TIME;
const int DASH_DURATION = UNIT_TIME * 3;
const int ELEMENT_SPACE = UNIT_TIME; // Space between dots/dashes
const int CHAR_SPACE = UNIT_TIME * 3; // Space between characters
const int WORD_SPACE = UNIT_TIME * 7; // Space between words
bool manTstJustEntered = false;
bool isMazePaused = false; 
// NEW: Global flag for motor movement state
 // State for toggling 7-segment display
int number = 5;
// NEW: Global state variable for the 7-segment display
bool sevenSegIsOn = false;
bool whiteLedsOn = false;
// NEW: Global state variable for the Buzzer
// Timer to control how often the distance is updated on the LCD
unsigned long lastDistanceUpdateTime = 0; 
const int DISTANCE_UPDATE_INTERVAL = 200; // Update every 200ms
bool buzzerIsOn = false;
bool customBOI = false;
#define GREEN_LED 8
#define RED_LED 7
#define LEFT_LED 6   // CHANGED: Left LED connected to pin 6
#define RIGHT_LED 5  // No change: Right LED connected to pin 5
#define BLUE_LED 13 
const int CLICK = 41;
const int VRX = A12;
const int VRY = A8;
const int blueLedPin = 9;  // Pin for the blue LED
const int orangeLedPin = 10; // Pin for the orange LED
#define WHITE_LED_LEFT 42
#define WHITE_LED_RIGHT 47
// New LEDs
#define RED_LED_1 12 // New Red LED on PWM pin 12
#define RED_LED_2 11 // New Red LED on PWM pin 11
#define WHITE_LED 10 // New White LED on PWM pin 10
// After isMazePaused definition:

// ⭐ NEW: Variables for Servo Radar Sweep
int servoPosition = 0;             // Current position (0 to 180)
int servoDirection = 1;            // 1 for sweeping right, -1 for sweeping left
const int SWEEP_SPEED_DELAY = 20;  // Milliseconds to wait between steps (controls speed)
unsigned long lastSweepTime = 0;   // Tracks the last time the servo moved
// ⭐ NEW: Variables for blinking 'P'
// Adjust the 500 value to match the blink speed of your '2'
const unsigned long MAZE_PAUSE_BLINK_INTERVAL = 300; 
unsigned long lastPauseBlinkTime = 0;
bool isPauseBlinkOn = true; // State of the blink: true = 'P' shown, false = clear/off
// Buzzer
#define BUZZER 49
void beep(int duration = 100);
bool buzzerEnabled = false; // Used for Bluetooth mode buzzer toggle
// ⭐ MORSE MODE ENHANCEMENTS ⭐

// Keypad Definitions - NEW
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
// Define the keypad layout. Assuming S1='1', S2='2', S3='3', S4='A', S16='D'
char keys[ROWS][COLS] = {
  {'1','2','3','A'}, // S1, S2, S3, S4
  {'4','5','6','B'}, // S5, S6, S7, S8
  {'7','8','9','C'}, // S9, S10, S11, S12
  {'*','0','#','D'}  // S13, S14, S15, S16
};
// Pin definitions for the keypad rows and columns
// Your provided pin definitions:
// C4 : 30, C3 : 31, C2 : 32, C1 : 34
// R1 : 35, R2 : 36, R3 : 37, R4 : 38
byte rowPins[ROWS] = {35, 36, 37, 38}; // Connect to R1, R2, R3, R4 pins of the keypad
byte colPins[COLS] = {34, 32, 31, 30}; // Connect to C1, C2, C3, C4 pins of the keypad (order adjusted to match typical keypad layout)

// Create the Keypad object
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Removed old button definitions: BTN_BT, BTN_MAZE, BTN_EMERGENCY
void handleManTstMode(); // NEW: Declaration for the Manual Test Mode handler
void handleEmergency();
void controlBluetooth(); // MODIFIED: No char argument, it will read Serial1 internally

// Motors
#define IN1 50
#define IN2 51
#define IN3 52 // Reverted from 55 - This was changed from your original working code
#define IN4 53 // Reverted from 54 - This was changed from your original working code
#define ENA 3 // Speed Control for the motors
#define ENB 4 // Speed Control for the motors
void handleRandomizerMode(char customKey);
void forward();
void backward();
void turnLeft();
void turnRight();
void stopMotors();
void servoSweep();
// Servo
#define SERVO_PIN 2
int servoPos = 90; // Initial centered servo-ultrasonic position
Servo myservo;

// Ultrasonic Sensor
#define TRIG_PIN 43
#define ECHO_PIN 44
long measureDistance();

// 7 segment pins (A->G)
int segmentPins[7] = { 22, 23, 24, 25, 26, 27, 28 };
void clearSegments();
void animateSegmentMenu(); // Now handles 7-segment animation, LCD blinking, and LED hover
void displayDigit(int digit);
void displayLetter(char letter);

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void displayCentered(String msg, int row);

// Potentiometer
#define POT_PIN A0

/* Other functions declarations */
void showWelcome();
void showModeSelection();
void updateMovementState(String state, bool forceUpdate = false); // MODIFIED: Added forceUpdate parameter
void handleMazeMode(); // Maze Mode handler
void handleTestingMode(); // Testing Mode handler

/* States */

Mode currentMode = NONE; // Initial mode is set to none when in the menu


// Maze Mode Variables
const long OBSTACLE_DISTANCE = 15; // Distance in cm to consider an obstacle. MODIFIED to 15cm.
const int TURN_CLEARANCE_DISTANCE = 18; // Distance in cm required for a clear turn. MODIFIED to 7cm.
const int TURN_ACTION_DURATION = 350; // Duration in milliseconds for turning (0.35 seconds). MODIFIED.
const int TURN_DELAY = 500;       // MODIFIED: Turn delay set to 0.53 seconds (530ms).
const int HALF_TURN_DELAY = 850; // Delay for half turns (not used in new logic, but kept for reference)
const int RECLINE_DELAY = 200;   // Delay for backing up. MODIFIED: 0.2 seconds = 200ms.
bool obstacleDetected = false;
int turnDirection = 0; // 0: none, 1: left, 2: right (not used in new logic, but kept for reference)
bool isTurning = false; // (not used in new logic, but kept for reference)
unsigned long lastTurnStartTime = 0;     // Timer to track turn duration (not used in new logic, but kept for reference)
String mazeMovementState = "Moving";     // To store the current action in Maze mode
// 7-Segment Display Pin Definitions (YOU MUST VERIFY THESE PINS)
#define SEG_A 22
#define SEG_B 23
#define SEG_C 24
#define SEG_D 25
#define SEG_E 26
#define SEG_F 27
#define SEG_G 28
// You may also need a common pin definition, e.g., #define COMMON_PIN 29
// NEW: Separate speeds for left and right turns in Maze Mode
// Adjust these values (0-255) to calibrate your robot's turns
const int MAZE_TURN_LEFT_SPEED = 255;  // Example speed for left turn
const int MAZE_TURN_RIGHT_SPEED = 255; // Example speed for right turn
enum FollowState { FOLLOW_STATE_WAITING, FOLLOW_STATE_FOLLOWING };
FollowState currentFollowState = FOLLOW_STATE_WAITING;

// ⭐ NEW: Follow Me Mode Variables (for functionality later)
const int FOLLOW_DISTANCE_CM = 20; // Target distance to maintain
const int FOLLOW_TOLERANCE_CM = 5; // Acceptable window (28cm - 32cm)
const int FOLLOW_MOTOR_SPEED = 100; // Base speed for following
// NEW: Separate durations for left and right turns in Maze Mode
// Adjust these values (in milliseconds) to calibrate your robot's turn duration
const int MAZE_TURN_LEFT_DURATION = 310; // Example duration for left turn
const int MAZE_TURN_RIGHT_DURATION = 330; // Example duration for right turn


// Maze Mode States for Left-Hand Rule Algorithm
enum MazeState {
  MAZE_STATE_MOVING_FORWARD,      // Robot is moving straight
  MAZE_STATE_OBSTACLE_DETECTED,   // Robot has hit an obstacle and is reacting
  MAZE_STATE_SCANNING,            // Robot is scanning its surroundings with the servo
  MAZE_STATE_TURNING_LEFT,        // Robot is turning left
  MAZE_STATE_TURNING_RIGHT,       // Robot is turning right
  MAZE_STATE_TURNING_AROUND,      // Robot is turning 180 degrees (dead end)
  // MAZE_STATE_BACKING_UP and MAZE_STATE_ADJUSTING_SERVO removed as per new logic
};

MazeState currentMazeState = MAZE_STATE_MOVING_FORWARD; // Initial maze state
unsigned long stateStartTime = 0; // Timer to track how long we've been in the current state

// Distances from scanning
long distLeft = 0;
long distFront = 0;
long distRight = 0;


// Testing Mode Variables
enum TestState {
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
TestState currentTestState = TEST_STATE_INIT;
unsigned long testStateStartTime = 0; // Timer for testing phase state durations
const int TEST_ACTION_DURATION = 1000; // 1 second for each motor/servo action

// Variables for sequential button press for Testing Mode (REMOVED - no longer needed)
enum NoneModeState {
  NONE_STATE_WAITING,
  NONE_STATE_BT_PRESSED_FIRST
};
NoneModeState currentNoneModeState = NONE_STATE_WAITING; // This enum is now simplified to just WAITING
unsigned long lastBtPressTime = 0;
const unsigned long RAPID_PRESS_WINDOW = 500; // 500ms for rapid press to activate testing mode (REMOVED - no longer needed)

// Global variables for LED Hover Effect (Breathing)
int ledBrightness = 0; // Starting brightness (off)
int fadeAmount = 2;    // How much to change brightness each step (adjust for speed)

// NEW GLOBAL FLAG: To signal first entry into Bluetooth mode
bool btModeJustEntered = false;

// LCD Animation Variables
int lcdAnimationPhase = 0;
unsigned long lastLcdAnimationUpdate = 0;
const int LCD_ANIMATION_DELAY = 300; // Adjust for animation speed (increased for slower animation)


/* setup */
void setup() {
  lcd.init();
  lcd.backlight();
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
  myservo.attach(SERVO_PIN);
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(POT_PIN, INPUT); // Potentiometer

  // Initialize LEFT_LED and RIGHT_LED as outputs
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  // Initialize new LEDs as outputs
  pinMode(RED_LED_1, OUTPUT);
  pinMode(RED_LED_2, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT); // Initialize BLUE_LED as output

  Serial.begin(9600); // Initialize Serial for Arduino IDE
  Serial1.begin(9600); // Initialize Serial1 for Bluetooth

  showWelcome();
  showModeSelection();
}

/* loop */
void loop() {
 // Read keypad input - NEW

  // Read keypad input - NEW
  char customKey = keypad.getKey(); //

  // ⭐ MODIFIED: S15 ('#') Pause/Resume in MAZE Mode ONLY
  if (customKey == '#') { // S15 is '#'
  
    if (currentMode == MAZE || currentMode == BLUETOOTH || currentMode == FOLLOW_ME) {
      beep(50);
      isMazePaused = !isMazePaused;
      if (isMazePaused) {
        stopMotors(); // Stop motors immediately
        // Display PAUSED message
        
        lcd.clear();
        lcd.setCursor(2, 0);
        displayCentered("ROBOT PAUSED" , 0);
        lcd.setCursor(0, 1);
        displayCentered("~S15 TO CONTINUE" , 1);

        // ⭐ MODIFIED: Initialize blink state
        displayP(); // Show 'P' immediately on pause
        lastPauseBlinkTime = millis();
        isPauseBlinkOn = true;

        
        digitalWrite(GREEN_LED, LOW);
      } else {
        // Resumed: clear pause message and restore mode visual/state
        lcd.clear();
        // Re-display the current movement state (handleMazeMode will update next)
        updateMovementState(mazeMovementState, true); // Assuming updateMovementState is defined

        digitalWrite(RED_LED, LOW); // Turn off pause cue LED
      }
    } else {
      // Original white LED toggle logic (if not in MAZE mode)
      whiteLedsOn = !whiteLedsOn; //
      if (whiteLedsOn) {
        digitalWrite(WHITE_LED_LEFT, HIGH);
        digitalWrite(WHITE_LED_RIGHT, HIGH); //
      } else {
        digitalWrite(WHITE_LED_LEFT, LOW);
        digitalWrite(WHITE_LED_RIGHT, LOW); //
      }
    }
  }
  // Master control for the white LEDs.
  if ((currentMode == MAZE || currentMode == BLUETOOTH || currentMode == FOLLOW_ME) && isMazePaused) {
    unsigned long currentTime = millis();
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    // Check if the interval has passed (e.g., 500ms)
    if (currentTime - lastPauseBlinkTime >= MAZE_PAUSE_BLINK_INTERVAL) {
      lastPauseBlinkTime = currentTime; // Reset the timer
      
      isPauseBlinkOn = !isPauseBlinkOn; // Toggle the blink state
      
      if (isPauseBlinkOn) {
        displayP(); // Show 'P' (ON state)
        digitalWrite(RED_LED, HIGH);
      } else {
        clearSegments(); // Turn 7-segment display off (OFF state)
        digitalWrite(RED_LED, LOW);
      }
    }
  }
  // --- GLOBAL EMERGENCY BUTTON HANDLING (S16 / 'D' ) ---
// ... rest of the loop function ...//
  // Master control for the white LEDs.

  // --- GLOBAL EMERGENCY BUTTON HANDLING (S16 / 'D' ) ---
  // Note: 'X' from Serial for emergency is handled directly within controlBluetooth now.
  if ((currentMode != NONE && customKey == 'D') || (currentMode == NONE && customKey == 'D')) { // S16 is 'D'
    
    handleEmergency();
    showModeSelection(); // Go back to mode selection
    return; // Exit loop iteration as mode has changed
  }


  if (currentMode == NONE) {
    // Instant mode selection for BT and Maze (Bluetooth '1' also handled here for initial entry)
    if (customKey == '1' || (Serial1.available() && Serial1.peek() == '1')) { // S1 is '1' for BT mode
      // Consume the '1' from Serial1 if it was peeked
      if (Serial1.available() && Serial1.peek() == '1') {
          Serial1.read();
      }
      Serial.println(" -- BLUETOOTH MODE TRIGGERED !");
      Serial1.println(" -- BLUETOOTH MODE TRIGGERED !"); //lmaooo
      beep(); // Single beep for BT mode
      displayDigit(1);
      lcd.clear();
      currentMode = BLUETOOTH;
      btModeJustEntered = true; // SET THE FLAG HERE
      // Initial LED state for BT mode when entering (robot is idle)
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      currentNoneModeState = NONE_STATE_WAITING; // Reset state for next time
      // Removed: delay(100); // Small debounce for mode transition - not needed here
    } else if (customKey == '2' || (Serial1.available() && Serial1.peek() == '2')) { // S2 is '2' for Maze mode
      // Consume the '2' from Serial1 if it was peeked
      if (Serial1.available() && Serial1.peek() == '2') {
          Serial1.read();
      }
      

      // ⭐ MODIFIED: Pre-sequence for Maze Mode: LCD message, 6 rapid beeps with G segment, and servo movements
      lcd.clear();
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      Serial.println(" -- DAMI 3000 uses a Right-Hand Algorithm !");
      Serial1.println(" -- DAMI 3000 uses a Right-Hand Algorithm !");
      displayCentered("DAMI 3000 uses a", 0);
      displayCentered("Right-Hand Alg !", 1);
      stopMotors(); // Ensure motors are off during this sequence
      myservo.write(90); // Initial servo center for the sequence start

      for (int i = 0; i < 8; i++) { // ⭐ MODIFIED: Loop 6 times for 6 beeps
        clearSegments();
        digitalWrite(segmentPins[6], HIGH); // Light up segment G
        digitalWrite(BUZZER, HIGH); // Buzzer on
digitalWrite(BLUE_LED, HIGH);
        // ⭐ NEW: Servo movement pattern: 0, 45, 0, 45, 0, 45
        if (i % 2 == 0) { // On 0th, 2nd, 4th beep (even iterations)
          myservo.write(0); // Move servo to 0 degrees
        } else { // On 1st, 3rd, 5th beep (odd iterations)
          myservo.write(45); // Move servo to 45 degrees
        }
        
        delay(100); // Same speed as initial welcome beeps
        digitalWrite(BUZZER, LOW); // Buzzer off
        digitalWrite(BLUE_LED, LOW);
        digitalWrite(segmentPins[6], LOW); // Turn off segment G
        delay(50); // Same delay between beeps
      }
      clearSegments(); // Ensure G is off after sequence
      
      // ⭐ NEW: Servo resets to center a moment before countdown
      delay(700); // Pause before resetting servo
      myservo.write(90); // Reset servo to center
      delay(1300); // Pause after servo reset and before countdown

      // Fast Countdown from 3
      lcd.clear();
      displayCentered("Starting in...", 0);
      Serial.println(" -- Starting in 3, 2, 1...");
      Serial1.println(" -- Starting in 3, 2, 1...");
      for (int count = 3; count >= 1; count--) {
        displayCentered(String(count), 1); // Display count on LCD
        digitalWrite(RED_LED, HIGH);
        displayDigit(count); // Display count on 7-segment
        beep(150); // Beep for countdown
        clearSegments(); // Clear 7-segment after beep
        delay(300); // Fast countdown speed
        digitalWrite(RED_LED, LOW);
      }
      clearSegments();
      
      // Now actually enter Maze Mode
      lcd.clear(); // Clear LCD before entering mode
      Serial.println(" -- GO !");
      Serial1.println(" -- GO !");
      displayCentered("GO !", 0); // Display "GO !" on LCD
      digitalWrite(GREEN_LED, HIGH);
              displayDigit(0); // Display count on 7-segment
      beep(1000); // Long beep for "GO!"
            clearSegments();
      currentMode = MAZE;
      currentMazeState = MAZE_STATE_MOVING_FORWARD; // Initialize maze state
      stateStartTime = millis();
      updateMovementState(""); // Initial message for Maze Mode
      currentNoneModeState = NONE_STATE_WAITING; // Reset state for next time      
      delay(300); // Debounce for mode transition
      clearSegments(); // Clear 7-segment after beep
      digitalWrite(GREEN_LED, LOW);
      lcd.clear(); // Clear LCD again after "GO!" sequence
    }
    
    // S3 ('3') for entering Testing Mode from NONE mode
    else if (customKey == '3') { // S3 is '3'
     
      currentMode = TESTING;
      currentTestState = TEST_STATE_INIT;
      testStateStartTime = millis();
      updateMovementState("Testing: Initializing");
      displayDigit(8); // Show 8 for Testing mode
      lcd.clear();
      beep(); // Beep for mode entry
      delay(500); // Debounce for mode transition
    }
    else if (customKey == '4') { 

      beep();
      currentMode = FOLLOW_ME;
      currentFollowState = FOLLOW_STATE_FOLLOWING; // Initialize state
      stopMotors(); // Ensure robot is stationary
    }
    else if (customKey == '5') {

      beep();
      lcd.clear();
      currentMode = MAN_TST;
      manTstJustEntered = true; // Set the flag for one-time setup
      updateMovementState("MANUAL TEST READY");
      
      }// S7 ('7') for entering RANDOMIZER Mode from NONE mode
    // Debounce
    // NEW: S4 ('A') for entering VC Mode from NONE mode
// NEW: S4 ('A') for entering VC Mode from NONE mode
// Handle 'A' key press
else if (customKey == 'A') {
  // If in NONE mode, enter CREDITS mode
  if (currentMode == NONE) {

    beep();
    lcd.clear();
    currentMode = CREDITS;
    isCreditsActive = true;
    currentCreditMessage = 0;
    lastMessageTime = millis(); // Initialize the timer
    updateMovementState(" ");
  }
  // If in CREDITS mode, toggle servo movement
  else if (currentMode == CREDITS) {
    isServoMovementEnabled = !isServoMovementEnabled;
    if (isServoMovementEnabled) {
      Serial.println("Servo movement ON");
      myservo.write(45); // Start at 45 degrees
    } else {
      Serial.println("Servo movement OFF");
      myservo.write(0); // Move to 0 degrees to stop
    }
  }
  
    
    
}

  }
  if (analogRead(VRX) < 150 && currentMode != JOYSTICK_TEST && currentMode == NONE) {
    currentMode = JOYSTICK_TEST;
    // Clear the screen and give feedback
    lcd.clear();
    beep();
    clearSegments();
    stopMotors();
     // Brief pause for user to see the change
  }
  if (currentMode == JOYSTICK_TEST) {
    handleJoyTestMode();
  }
  else if (analogRead(VRX) > 900 && currentMode == NONE) {
    currentMode = MORSE_COM;
    handleMorseCom();
 
  }
  else if (currentMode == MORSE_COM) {
    
    handleMorseCom();
 
  }
  else if (analogRead(VRY) > 900 && currentMode == NONE) {
    currentMode = DICE;
    handleDice();
 
  }
  else if (currentMode == DICE) {
    
    handleDice();
 
  }
  else if (currentMode == BLUETOOTH) { // Call handler for BT mode
    if (!isMazePaused) { 
        
      controlBluetooth(); 
    } // No argument passed
  }
  else if (currentMode == MAZE) { // Call handler for MAZE mode
    if (!isMazePaused) {
      handleMazeMode();
    }
  }
  else if (currentMode == MAN_TST) {
      handleManTstMode(customKey);
  }
  else if (currentMode == TESTING) { // Call handler for TESTING mode
    handleTestingMode();
  }
  // NEW: Handle VC mode
else if (currentMode == CREDITS) {
  handleCreditsMode();
}

else if (currentMode == FOLLOW_ME) { 
    if (!isMazePaused) {

    handleFollowMode(); }
  }
  // Update 7-segment display and LCD distance for Bluetooth mode
  if (currentMode == BLUETOOTH) {
    if (!isMazePaused) {
    static unsigned long lastBlinkTime = 0;
    static bool showDigit = true;

    if (millis() - lastBlinkTime >= 300) {
      lastBlinkTime = millis();
      showDigit = !showDigit;
      if (!isMazePaused) {
      if (showDigit) {
        displayDigit(1);
      } else {
        clearSegments();
      }
    }
    }

    lcd.setCursor(0, 0);
    lcd.print("Distance : ");
    long distance = measureDistance();
    if (distance < 10) lcd.print("0");
    if (distance < 100) lcd.print(distance);
    else lcd.print("99");
    lcd.print(" cm");
    if (distance<=99&&distance!=0){
      Serial1.print("Distance : ");
    Serial1.print(distance);
    Serial1.println(" cm");
    Serial.print("Distance : ");
    Serial.print(distance);
    }
    else if (distance==0){
      Serial1.print("Distance : MIN\n");
    Serial.print("Distance : MIN\n");
    }
    else{
Serial1.print("Distance : MAX\n");
    Serial.print("Distance : MAX\n");
    }
    

  }}
  // If no specific mode is active (i.e., currentMode == NONE), run segment animation and LCD blinking
else if (currentMode == NONE || currentMode == CREDITS) {
  animateSegmentMenu();
  animateLcdMenu();
}
}
/* New Mode Handler: RANDOMIZER Mode */
/* New Mode Handler: RANDOMIZER Mode */
/* New Mode Handler: RANDOMIZER Mode */
void handleDice() {
    // Note: The global 'diceRollerJustEntered' flag and 'variablechanger' are used for 
    // initial mode setup, consistent with other handlers in your code[cite: 2164, 2168].
    
    // Dice State Enum: Tracks the current phase of the dice roller
    static enum DiceState { WAITING, ROLLING, RESULT } currentDiceState = WAITING;
    static unsigned long rollStartTime = 0;
    const unsigned long ROLL_DURATION = 2500; // 1.5 seconds for the quick number sequence
    
    // --- Mode Entry Setup ---
    if (variablechanger==1) {
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
        Serial.println(" -- DICE MODE TRIGGERED !");
        Serial1.println(" -- DICE MODE TRIGGERED !");
        lcd.clear();
        clearSegments(); // Clear the 7-segment display
        stopMotors();
        displayCentered("RANDOM DICE MODE", 0);
                displayCentered("~Press to Roll", 1);
                beep(100);
    displayLetter('D');
    digitalWrite(BLUE_LED, HIGH);
    delay(100);
    digitalWrite(BLUE_LED, LOW);
    clearSegments();
    beep(100); 
    digitalWrite(BLUE_LED, HIGH);
    displayLetter('D');
       
    stopMotors();
        
        variablechanger = 0; // Reset variablechanger after mode entry
        
        currentDiceState = WAITING; // Start in the waiting state
    }

    // --- Keypad Input ---
    char customKey = keypad.getKey();

    // --- Global Exit Check (S16 / 'D') ---
    if (customKey == 'D') { 
        handleEmergency();
        currentMode = NONE;
        showModeSelection();
      
        return;
    }

    // --- Dice State Machine ---
    switch (currentDiceState) {
        case WAITING: {
            // Await a click on button 'A'
            if ((digitalRead(CLICK) == LOW) || (customKey == 'A')) { // Key 'A' is S4
                currentDiceState = ROLLING;
                rollStartTime = millis();
                // Seed the random number generator for a more random result
                randomSeed(analogRead(A0)); 
                beep(50);
            }
            break;
        }
        case ROLLING: {
          Serial.println("\n -- Rolling...");
        Serial1.println("\n -- Rolling...");
            unsigned long currentTime = millis();
            if (currentTime - rollStartTime < ROLL_DURATION) {
                // Rapidly cycle numbers (0-9) - Update every 50ms
                // The delay is implicitly controlled by the loop duration
                int currentDigit = (currentTime / 50) % 10; 
                displayDigit(currentDigit); // Display the number (0-9)
                beep();
                // Blink the LCD to show activity
                if ((currentTime / 100) % 2 == 0) {
                    lcd.setCursor(0, 1);
                    lcd.print("   ROLLING...   ");
                    digitalWrite(BLUE_LED, HIGH);
                } else {
                    lcd.setCursor(0, 1);
                    lcd.print("                ");
                    digitalWrite(BLUE_LED, LOW);
                }
            } else {
              digitalWrite(BLUE_LED, LOW);
                // End of roll sequence - Land on a random number
                int final_roll = random(0, 10);
                Serial.println("\n -- Result is : \n");
        Serial1.println("\n -- Result is : \n");
                 // Random number from 0 to 9
                 Serial.println(final_roll);
        Serial1.println(final_roll);
        
                displayDigit(final_roll);
                
                lcd.clear();
                displayCentered("ROLL COMPLETE!", 0);
                displayCentered("Result: " + String(final_roll), 1);
                digitalWrite(GREEN_LED, HIGH);
                beep(300); // Long beep for the result

                currentDiceState = RESULT;
                rollStartTime = millis(); // Reuse timer for result display duration
            }
            break;
        }
        case RESULT: {
            // Stay in result state for 3 seconds, then return to WAITING
            if (millis() - rollStartTime >= 3000) {
                clearSegments();
                lcd.clear();
                Serial.println("\n -- DICE MODE READY !");
        Serial1.println("\n -- DICE MODE READY !");
                displayCentered("RANDOM DICE MODE", 0);
                displayCentered("~Press to Roll", 1);
                digitalWrite(GREEN_LED, LOW);
                 beep(100);
    displayLetter('D');
    digitalWrite(BLUE_LED, HIGH);
    delay(100);
    digitalWrite(BLUE_LED, LOW);
    clearSegments();
    beep(100); 
    digitalWrite(BLUE_LED, HIGH);
    displayLetter('D');
                currentDiceState = WAITING;
            }
            break;
        }
    }
}
/* New Mode Handler */
/* New Mode Handler: Manual Test (Keypad Control) */
/* New Mode Handler: Manual Test (Keypad Control) */
/* New Mode Handler: Manual Test (Keypad Control) */
// Requires your LCD object to be globally defined (e.g., LiquidCrystal_I2C lcd(0x27, 16, 2);)
void handleJoyTestMode() {
    // You may need to add includes for Wire.h and LiquidCrystal_I2C.h if not present.
    // Ensure VRX, VRY, and CLICK are defined correctly (e.g., const int VRX = A0;).
    if(variablechanger==1){
        variablechanger=0;
            Serial.println(" -- JOYSTICK TESTING MODE TRIGGERED !");
        Serial1.println(" -- JOYSTICK TESTING MODE TRIGGERED !");}
    int vrxValue = analogRead(VRX);
    int vryValue = analogRead(VRY);
    int clickState = digitalRead(CLICK);

    // Print to Serial Monitor
    Serial.print("VRX: "); Serial.print(vrxValue);
    Serial.print(" | VRY: "); Serial.print(vryValue);
    Serial.print(" | CLICK: "); Serial.println(clickState);

    // Print to LCD
    lcd.setCursor(0, 0); // Line 1
    lcd.print("X-Axis:");
    lcd.print(vrxValue);
    if (vrxValue < 1000) lcd.print("   ");
    if (vrxValue < 100) lcd.print("  ");
    if (vrxValue < 10) lcd.print(" ");
    

    lcd.setCursor(11,0); // Line 2
    lcd.print("|");
    // Pad spaces to clear previous digits
    
    lcd.setCursor(0, 1);
    lcd.print("Y-Axis:");
    lcd.print(vryValue);
    if (vryValue < 1000) lcd.print("   ");
    if (vryValue < 100) lcd.print("  ");
    if (vryValue < 10) lcd.print(" ");
    
    lcd.setCursor(11,1); // Line 2
    lcd.print("|");
    

    lcd.setCursor(12,0); // Line 2
    lcd.print("CLCK ");
    
    // Display "ON" when button is pressed (LOW state)
    if (clickState == LOW) {
        lcd.setCursor(12,1);
        lcd.print("~ON");
    } else {
      lcd.setCursor(12,1);
        lcd.print("OFF");
    }
    
    unsigned long currentTime = millis();
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    // Check if the interval has passed (e.g., 500ms)
    if (currentTime - lastPauseBlinkTime >= MAZE_PAUSE_BLINK_INTERVAL) {
      lastPauseBlinkTime = currentTime; // Reset the timer
      
      isPauseBlinkOn = !isPauseBlinkOn; // Toggle the blink state
      
      if (isPauseBlinkOn) {
        displayDigit(7);
        digitalWrite(GREEN_LED, HIGH);
      } else {
        clearSegments(); // Turn 7-segment display off (OFF state)
        digitalWrite(GREEN_LED, LOW);
      }
    }
    // Note: The delay(100) at the end of your original snippet is usually bad
    // practice inside loop/mode functions, as it slows down your whole program.
    // If you need to slow down the display rate, use a non-blocking timer.
}

// --- MORSE CODE HELPER FUNCTIONS ---
// These rely on the existing timing constants (UNIT_TIME, DOT_DURATION, DASH_DURATION, 
// ELEMENT_SPACE, CHAR_SPACE) and segment pins (SEG_A - SEG_G).

// Assuming SEG_A through SEG_G are defined as per your code [cite: 1993]


    
void displayLetter(char letter) {
  byte segments = 0;
  switch (letter) {
    case 'A': segments = 0b01110111; break;
        case 'B': segments = 0b01111100; break; // Looks like lowercase 'b'
        case 'C': segments = 0b00111001; break;
        case 'D': segments = 0b01011110; break; // Looks like lowercase 'd'
        case 'E': segments = 0b01111001; break;
        case 'F': segments = 0b01110001; break;
        case 'G': segments = 0b01111101; break; // Looks like a '9'
        case 'H': segments = 0b01110110; break; 
        case 'I': segments = 0b00000110; break; // Looks like a '1'
        case 'J': segments = 0b00011110; break;
        case 'K': segments = 0b01110000; break;
        case 'L': segments = 0b00111000; break;
        case 'M': segments = 0b00110111; break;
        case 'N': segments = 0b01010100; break; // Looks like lowercase 'n'
        case 'O': segments = 0b00111111; break; // Looks like a '0'
        case 'P': segments = 0b01110011; break;
        case 'Q': segments = 0b01100111; break;
        case 'R': segments = 0b01010000; break; // Looks like lowercase 'r'
        case 'S': segments = 0b01101101; break; // Looks like a '5'
        case 'T': segments = 0b01111000; break; // Looks like lowercase 't'
        case 'U': segments = 0b00111110; break;
        case 'V': segments = 0b00111110; break;
        case 'W': segments = 0b00011100; break;
        case 'X': segments = 0b01110110; break;
        case 'Y': segments = 0b01101110; break;
        case 'Z': segments = 0b01011011; break; // Looks like a '2'

        // K, M, V, W, X are generally unrepresentable on a 7-segment display.
        // --- Numbers (for completeness) ---
        case '0': segments = 0b00111111; break;
        case '1': segments = 0b00000110; break;
        case '2': segments = 0b01011011; break;
        case '3': segments = 0b01001111; break;
        case '4': segments = 0b01100110; break;
        case '5': segments = 0b01101101; break;
        case '6': segments = 0b01111101; break;
        case '7': segments = 0b00000111; break;
        case '8': segments = 0b01111111; break;
        case '9': segments = 0b01101111; break;
  }
  for (int i = 0; i < 9; i++) {
    digitalWrite(segmentPins[i], (segments >> i) & 0x01);
  }
}


// Assuming BLUE_LED and BUZZER pins are defined globally.
// Assuming BUZZER_FREQUENCY (e.g., 500) is defined for the tone.
const int BUZZER_FREQUENCY = 500; 

void morseElement(char element) {
    int duration;

    if (element == '.') {
        duration = DOT_DURATION;
    } else if (element == '-') {
        duration = DASH_DURATION;
    } else {
        return; // Exit if not dot or dash
    }
    
    // 1. Activate the Blue LED and Buzzer simultaneously
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    beep(duration);
    
    // 2. Wait for the element duration (Dot or Dash time)
    // The delay() function ensures the LED and tone last for the exact same time.
    
    
    // 3. Deactivate the LED and Buzzer
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
     
}

void morseChar(char character) {
  
   
    // Convert to uppercase to handle both 's' and 'S', 'o' and 'O', etc.
    char upperChar = toupper(character);
    
    // 1. Display the current character on the 7-segment display
    displayLetter(upperChar);
    
    // 2. Transmit the Morse Code via buzzer and send character over Serial
    if (upperChar == 'A') {
        // A: Dot, Dash
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        Serial.println("A");
        Serial1.println("A");
    } else if (upperChar == '-') {
        // - (Hyphen/Minus sign): Dash, Dot, Dot, Dot, Dot, Dash
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
    else if (upperChar == 'B') {
        // B: Dash, Dot, Dot, Dot
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        Serial.println("B");
        Serial1.println("B");
    } else if (upperChar == 'C') {
        // C: Dash, Dot, Dash, Dot
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        Serial.println("C");
        Serial1.println("C");
    } else if (upperChar == 'D') {
        // D: Dash, Dot, Dot
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        Serial.println("D");
        Serial1.println("D");
    } else if (upperChar == 'E') {
        // E: Dot
        morseElement('.');
        Serial.println("E");
        Serial1.println("E");
    } else if (upperChar == 'F') {
        // F: Dot, Dot, Dash, Dot
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        Serial.println("F");
        Serial1.println("F");
    } else if (upperChar == 'G') {
        // G: Dash, Dash, Dot
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        Serial.println("G");
        Serial1.println("G");
    } else if (upperChar == 'H') {
        // H: Dot, Dot, Dot, Dot
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        Serial.println("H");
        Serial1.println("H");
    } else if (upperChar == 'I') {
        // I: Dot, Dot
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        Serial.println("I");
        Serial1.println("I");
    } else if (upperChar == 'J') {
        // J: Dot, Dash, Dash, Dash
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('-');
        Serial.println("J");
        Serial1.println("J");
    } else if (upperChar == 'K') {
        // K: Dash, Dot, Dash
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        Serial.println("K");
        Serial1.println("K");
    } else if (upperChar == 'L') {
        // L: Dot, Dash, Dot, Dot
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        Serial.println("L");
        Serial1.println("L");
    } else if (upperChar == 'M') {
        // M: Dash, Dash
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('-');
        Serial.println("M");
        Serial1.println("M");
    } else if (upperChar == 'N') {
        // N: Dash, Dot
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        Serial.println("N");
        Serial1.println("N");
    } else if (upperChar == 'O') {
        // O: Dash, Dash, Dash
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('-');
        Serial.println("O");
        Serial1.println("O");
    } else if (upperChar == 'P') {
        // P: Dot, Dash, Dash, Dot
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        Serial.println("P");
        Serial1.println("P");
    } else if (upperChar == 'Q') {
        // Q: Dash, Dash, Dot, Dash
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        Serial.println("Q");
        Serial1.println("Q");
    } else if (upperChar == 'R') {
        // R: Dot, Dash, Dot
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        Serial.println("R");
        Serial1.println("R");
    } else if (upperChar == 'S') {
        // S: Dot, Dot, Dot
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        Serial.println("S");
        Serial1.println("S");
    } else if (upperChar == 'T') {
        // T: Dash
        morseElement('-');
        Serial.println("T");
        Serial1.println("T");
    } else if (upperChar == 'U') {
        // U: Dot, Dot, Dash
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        Serial.println("U");
        Serial1.println("U");
    } else if (upperChar == 'V') {
        // V: Dot, Dot, Dot, Dash
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        Serial.println("V");
        Serial1.println("V");
    } else if (upperChar == 'W') {
        // W: Dot, Dash, Dash
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('-');
        Serial.println("W");
        Serial1.println("W");
    } else if (upperChar == 'X') {
        // X: Dash, Dot, Dot, Dash
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        Serial.println("X");
        Serial1.println("X");
    } else if (upperChar == 'Y') {
        // Y: Dash, Dot, Dash, Dash
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('.');
        delay(ELEMENT_SPACE);
        morseElement('-');
        delay(ELEMENT_SPACE);
        morseElement('-');
        Serial.println("Y");
        Serial1.println("Y");
    } else if (upperChar == 'Z') {
        // Z: Dash, Dash, Dot, Dot
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
    
    // --- Numbers ---
    else if (upperChar == '1') {
        // 1: Dot, Dash, Dash, Dash, Dash
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
    } else if (upperChar == '2') {
        // 2: Dot, Dot, Dash, Dash, Dash
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
    } else if (upperChar == '3') {
        // 3: Dot, Dot, Dot, Dash, Dash
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
    } else if (upperChar == '4') {
        // 4: Dot, Dot, Dot, Dot, Dash
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
    } else if (upperChar == '5') {
        // 5: Dot, Dot, Dot, Dot, Dot
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
    } else if (upperChar == '6') {
        // 6: Dash, Dot, Dot, Dot, Dot
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
    } else if (upperChar == '7') {
        // 7: Dash, Dash, Dot, Dot, Dot
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
    } else if (upperChar == '8') {
        // 8: Dash, Dash, Dash, Dot, Dot
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
    } else if (upperChar == '9') {
        // 9: Dash, Dash, Dash, Dash, Dot
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
    } else if (upperChar == '0') {
        // 0: Dash, Dash, Dash, Dash, Dash
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
    } else if (upperChar == '!') {
        // ! (Exclamation Mark): Dash, Dot, Dash, Dot, Dash, Dash
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
    else if (upperChar == '?') {
        // ? (Question Mark): Dot, Dot, Dash, Dash, Dot, Dot
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
    else if (upperChar == '.') {
        // . (Period): Dot, Dash, Dot, Dash, Dot, Dash
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
    else if (upperChar == ',') {
        // , (Comma): Dash, Dash, Dot, Dot, Dash, Dash
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
    else if (upperChar == ':') {
        // : (Colon): Dash, Dash, Dash, Dot, Dot, Dot
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
    // Handle the space character
    else if (upperChar == ' ') {
      Serial.println("\n -End of message- \n");
        Serial1.println("\n -End of message- \n");
        lcd.setCursor(0, 1);
        lcd.print("~                ");
        morseRepeater=1;
      // Space between words is 7 times the UNIT_TIME.
      
      // Do NOT print anything to Serial for a space character to prevent extra lines.
    }
    delay(CHAR_SPACE);
}

// --- TARGET FUNCTION: handleMorseCom ---
// --- TARGET FUNCTION: handleMorseCom ---
void handleMorseCom() {
    // Initial Mode Entry Setup
    if(variablechanger == 1){
        variablechanger = 0;
        Serial.println(" -- MORSE COMMUNICATION MODE TRIGGERED !");
        Serial1.println(" -- MORSE COMMUNICATION MODE TRIGGERED !");
        lcd.clear();
    clearSegments();
    
    // Safety & Initialization (assuming these are defined elsewhere)
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    
    beep(100);
    displayLetter('M');
    digitalWrite(BLUE_LED, HIGH);
    delay(100);
    digitalWrite(BLUE_LED, LOW);
    clearSegments();
    beep(100); 
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
    
    beep(100);
    displayLetter('8');
    digitalWrite(RED_LED, HIGH);
    delay(100);
    clearSegments();
    
    
    lcd.setCursor(0, 0); 
    lcd.print("SEND YOUR LETTER");
    
    lcd.setCursor(0, 1);
    lcd.print("~"); 
    
    }

    // Initial setup and LCD message
    
    
    // Display interactive prompt
    
    
    
    
    // Define Keys (Use your actual keypad characters)
          // Placeholder for Exit button
    
    // Main Communication Loop
        
        // --- 1. Keypad Input Check (for Replay/Exit) ---
        
        
    
            char command = 0;
  while (Serial1.available()) {
    command = Serial1.read(); // Read the latest character
  
            

        // --- 2. Serial Input Check (Keyboard/Bluetooth Command) ---
        
            char receivedChar = Serial1.read();
            // Convert to uppercase so the switch only needs capital letters
            

            Serial.println(command);
            // Echo the received command on the LCD
           
            
            switch (command) {
                // --- Letters A-Z ---
                case 'A': morseChar('A');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('A');
          morseRepeater++; break;

case 'B': morseChar('B');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('B');
          morseRepeater++; break;

case 'C': morseChar('C');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('C');
          morseRepeater++; break;

case 'D': morseChar('D');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('D');
          morseRepeater++; break;

case 'E': morseChar('E');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('E');
          morseRepeater++; break;
case '-': morseChar('-');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('-');
          morseRepeater++; break;
case 'F': morseChar('F');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('F');
          morseRepeater++; break;

case 'G': morseChar('G');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('G');
          morseRepeater++; break;

case 'H': morseChar('H');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('H');
          morseRepeater++; break;

case 'I': morseChar('I');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('I');
          morseRepeater++; break;

case 'J': morseChar('J');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('J');
          morseRepeater++; break;

case 'K': morseChar('K');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('K');
          morseRepeater++; break;

case 'L': morseChar('L');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('L');
          morseRepeater++; break;
case '!': morseChar('!');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('!');
          morseRepeater++; break;

case '?': morseChar('?');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('?');
          morseRepeater++; break;

case '.': morseChar('.');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('.');
          morseRepeater++; break;

case ',': morseChar(',');
          lcd.setCursor(morseRepeater, 1);
          lcd.print(',');
          morseRepeater++; break;

case ':': morseChar(':');
          lcd.setCursor(morseRepeater, 1);
          lcd.print(':');
          morseRepeater++; break;
case 'M': morseChar('M');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('M');
          morseRepeater++; break;

case 'N': morseChar('N');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('N');
          morseRepeater++; break;

case 'O': morseChar('O');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('O');
          morseRepeater++; break;

case 'P': morseChar('P');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('P');
          morseRepeater++; break;

case 'Q': morseChar('Q');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('Q');
          morseRepeater++; break;

case 'R': morseChar('R');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('R');
          morseRepeater++; break;

case 'S': morseChar('S');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('S');
          morseRepeater++; break;

case 'T': morseChar('T');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('T');
          morseRepeater++; break;

case 'U': morseChar('U');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('U');
          morseRepeater++; break;

case 'V': morseChar('V');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('V');
          morseRepeater++; break;

case 'W': morseChar('W');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('W');
          morseRepeater++; break;

case 'X': morseChar('X');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('X');
          morseRepeater++; break;

case 'Y': morseChar('Y');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('Y');
          morseRepeater++; break;

case 'Z': morseChar('Z');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('Z');
          morseRepeater++; break;
                
                // --- Numbers 0-9 ---
                case '0': morseChar('0');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('0');
          morseRepeater++; break;

case '1': morseChar('1');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('1');
          morseRepeater++; break;

case '2': morseChar('2');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('2');
          morseRepeater++; break;

case '3': morseChar('3');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('3');
          morseRepeater++; break;

case '4': morseChar('4');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('4');
          morseRepeater++; break;

case '5': morseChar('5');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('5');
          morseRepeater++; break;

case '6': morseChar('6');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('6');
          morseRepeater++; break;

case '7': morseChar('7');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('7');
          morseRepeater++; break;

case '8': morseChar('8');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('8');
          morseRepeater++; break;

case '9': morseChar('9');
          lcd.setCursor(morseRepeater, 1);
          lcd.print('9');
          morseRepeater++; break;
                case ' ': morseChar(' '); break;
                
                // Default Case: Handle Newline, Carriage Return, or other non-Morse characters
                default:
                    // Only process letters/numbers. Ignore other characters (like the 'Enter' key)
                    break;
            }
        }
        
        delay(50); // Small delay for loop stability
    
}
void handleManTstMode(char customKey) {
  if(variablechanger==1){
        variablechanger=0;
          Serial.println(" -- MANUAL TESTING MODE TRIGGERED !");
        Serial1.println(" -- MANUAL TESTING MODE TRIGGERED !");}
  // --- ONE-TIME SETUP --;-
  if (manTstJustEntered) {
    digitalWrite(RED_LED,LOW);
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(BLUE_LED,LOW);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("     Ready!");
    stopMotors(); 
    myservo.write(90); 
    clearSegments(); 
    digitalWrite(BUZZER, LOW); 
    
    // Initialize the state variable
    sevenSegIsOn = false; 

    // Reset the flag
    manTstJustEntered = false;
  }
  unsigned long currentTime = millis();

  // ⭐ DISTANCE UPDATE LOGIC ⭐
  // Only update distance when no key is currently pressed, and the interval has passed.
  if (!customKey && currentTime - lastDistanceUpdateTime >= DISTANCE_UPDATE_INTERVAL) {
    lastDistanceUpdateTime = currentTime;

    long distanceCm = measureDistance(); // Call your existing distance function
    
    // FORMAT: "Distance : XXcm" on the top row (0)
    // 1. Cap the distance at 99cm
    if (distanceCm > 99) {
      distanceCm = 99;
    }
    
    // 2. Create a buffer to format the output string
    char distBuffer[17]; // Buffer size for "Distance : XXcm" (15 chars) + null terminator

    // 3. Use snprintf() to format the string with zero-padding and capping
    // %02ld: Use long decimal (ld), pad with a leading zero (0) to a width of 2 (2).
    snprintf(distBuffer, sizeof(distBuffer), "Distance : %02ld cm", distanceCm);
    
    // 4. Print the formatted string to the LCD
    lcd.setCursor(0, 0); 
    lcd.print(distBuffer);// Clear the rest of the line (16 characters total)
  }
  // --- CONTINUOUS LOOP LOGIC ---
// Read keypad input inside the mode
  if (customKey) {
    // --- Motor Control ---
    if (customKey == '8') { // S10: Forward
      forward();
      updateMovementState(" MOTORS FORWARD");
    } else if (customKey == '4') { // S5: Turn Right
      turnRight();
      updateMovementState("MOTORS RIGHT");
    } else if (customKey == '6') { // S7: Turn Left
      turnLeft();
      updateMovementState("MOTORS LEFT");
    } else if (customKey == '2') { // S2: Backward
      backward();
      updateMovementState("MOTORS BACKWARD");
    } else if (customKey == '5') { // S6: Stop Motors
      stopMotors();
      updateMovementState("ROBOT IDLE");
    }

    // --- Servo Control ---
    
    else if (customKey == 'A') { // S15: Servo Right (180 degrees)
      digitalWrite(RED_LED,HIGH);
    digitalWrite(GREEN_LED,LOW);
    digitalWrite(BLUE_LED,LOW);
    updateMovementState("RED LED: ON");
    }else if (customKey == 'B') { // S15: Servo Right (180 degrees)
      digitalWrite(RED_LED,LOW);
    digitalWrite(GREEN_LED,HIGH);
    digitalWrite(BLUE_LED,LOW);
    updateMovementState(" GREEN LED: ON");
    }
    else if (customKey == 'C') { // S15: Servo Right (180 degrees)
      digitalWrite(RED_LED,LOW);
    digitalWrite(GREEN_LED,LOW);
    digitalWrite(BLUE_LED,HIGH);
    updateMovementState("BLUE LED: ON");
    }
    
    // --- Buzzer Control (S12: Toggle ON/OFF) ---
    

    // --- 7-Segment Control (S8: Toggle ALL ON/OFF) ---
    if (customKey == '9') { // S8 is 'B'
        // TOGGLE the state variable first
        sevenSegIsOn = !sevenSegIsOn; 
        
        if (sevenSegIsOn) {
            // Apply the ON state based on the variable
            for (int i = 0; i < 7; i++) {
                digitalWrite(segmentPins[i], HIGH);
            }
            lcd.setCursor(0, 1);
            lcd.print("   7-SEG: ON       ");
        } else {
            // Apply the OFF state based on the variable
            clearSegments();
            lcd.setCursor(0, 1);
            lcd.print("   7-SEG: OFF      ");
        }
    }
    unsigned long actionEndTime = millis();
    while (millis() - actionEndTime < 50) {
      // Wait non-blockingly for 50ms, allowing the loop to continue
      // but slowing down the re-read rate.
    }
  }
  if (customKey == '0' || analogRead(VRY) > 950 || analogRead(VRY) < 150) { // S14: Center Servo
      myservo.write(90);
      updateMovementState("SERVO CENTERED");
      
    } else if (customKey == '*' || analogRead(VRX) > 950) { // S13: Servo Left (0 degrees)
      myservo.write(0);
      
      updateMovementState("SERVO RIGHT");
    } else if (customKey == '#' || analogRead(VRX) < 150) { // S15: Servo Right (180 degrees)
      myservo.write(180);
      
      updateMovementState("SERVO LEFT");
    }else if (customKey == '3'){ // S15: Servo Right (180 degrees)
      myservo.write(135);
      updateMovementState("SERVO left");
    }
    else if (customKey == '1') { // S15: Servo Right (180 degrees)
      myservo.write(45);
      updateMovementState("SERVO right");
    }
    if (customKey == '7' || digitalRead(CLICK) == LOW) { // S12 is 'C'
        
        buzzerIsOn = !buzzerIsOn;
        
        
if (buzzerIsOn) { // Check the state variable
digitalWrite(BUZZER, HIGH);
lcd.setCursor(0, 1);
lcd.print("   BUZZER: ON      ");
} else {
digitalWrite(BUZZER, LOW);
lcd.setCursor(0, 1);
lcd.print("   BUZZER: OFF      ");
 }
}
}
void displayF() {
    // Assuming you have functions like setSegment(pin, state) 
    // or direct digitalWrite calls for each segment pin.
    
    // Example using assumed segment definitions (A, B, C, D, E, F, G)
    // You will need to replace these segment pin constants and HIGH/LOW
    // states with the actual constants/logic for your hardware.
    
    // P requires: A, B, E, F, G (C, D are OFF)
    
    digitalWrite(SEG_A, HIGH); // Segment A (top)
    digitalWrite(SEG_B, LOW); // Segment B (top right)
    digitalWrite(SEG_C, LOW);  // Segment C (bottom right) - OFF
    digitalWrite(SEG_D, LOW);  // Segment D (bottom) - OFF
    digitalWrite(SEG_E, HIGH); // Segment E (bottom left)
    digitalWrite(SEG_F, HIGH); // Segment F (top left)
    digitalWrite(SEG_G, HIGH); // Segment G (middle)
    
    // Ensure you call clearSegments() or handle the decimal point (DP) if necessary.
}
// ⭐ NEW FUNCTION: Display the letter 'P' on the 7-segment display
void displayP() {
    // Assuming you have functions like setSegment(pin, state) 
    // or direct digitalWrite calls for each segment pin.
    
    // Example using assumed segment definitions (A, B, C, D, E, F, G)
    // You will need to replace these segment pin constants and HIGH/LOW
    // states with the actual constants/logic for your hardware.
    
    // P requires: A, B, E, F, G (C, D are OFF)
    
    digitalWrite(SEG_A, HIGH); // Segment A (top)
    digitalWrite(SEG_B, HIGH); // Segment B (top right)
    digitalWrite(SEG_C, LOW);  // Segment C (bottom right) - OFF
    digitalWrite(SEG_D, LOW);  // Segment D (bottom) - OFF
    digitalWrite(SEG_E, HIGH); // Segment E (bottom left)
    digitalWrite(SEG_F, HIGH); // Segment F (top left)
    digitalWrite(SEG_G, HIGH); // Segment G (middle)
    
    // Ensure you call clearSegments() or handle the decimal point (DP) if necessary.
}

// ⭐ NEW: Handler for Follow Me Mode
// Handler for Follow Me Mode

void handleFollowMode() {
  if(variablechanger==1){
        variablechanger=0;
        Serial.println(" -- FOLLOW ME MODE TRIGGERED !");
        Serial1.println(" -- FOLLOW ME MODE TRIGGERED !");}
  unsigned long currentTime = millis();

    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    // Check if the interval has passed (e.g., 500ms)
    if (currentTime - lastPauseBlinkTime >= MAZE_PAUSE_BLINK_INTERVAL) {
      lastPauseBlinkTime = currentTime; // Reset the timer
      
      isPauseBlinkOn = !isPauseBlinkOn; // Toggle the blink state
      
      if (isPauseBlinkOn) {
        displayDigit(5); // Show 'P' (ON state)
        digitalWrite(BLUE_LED, HIGH);
      } else {
        clearSegments(); // Turn 7-segment display off (OFF state)
        digitalWrite(BLUE_LED, LOW);
      }
    }
  // Read the current distance using the function defined in your code.
  long currentDistance = measureDistance();

lcd.setCursor(0, 0);
  lcd.print("Distance : ");
  if (currentDistance < 10) lcd.print("0");
  if (currentDistance < 100) lcd.print(currentDistance);
  else lcd.print("99");
  lcd.print(" cm");
  lcd.setCursor(0, 1);

lcd.print("Threshold: 20 cm      "); // Print 16 spaces
  // Define the target range (e.g., 28 cm to 32 cm)
  const int MIN_DISTANCE = FOLLOW_DISTANCE_CM - FOLLOW_TOLERANCE_CM; 
  const int MAX_DISTANCE = FOLLOW_DISTANCE_CM + FOLLOW_TOLERANCE_CM; 

  switch (currentFollowState) {
    case FOLLOW_STATE_WAITING:
      stopMotors(); // Stays stopped in the waiting state [cite: 17]
      break;

    case FOLLOW_STATE_FOLLOWING:
      
      // 1. TOO CLOSE (Distance < 28 cm): Drive backward
      if (currentDistance < MIN_DISTANCE) {
        // Use the exact function from your code
        backward(); 

      // 2. TOO FAR (Distance > 32 cm): Drive forward
      } else if (currentDistance > MAX_DISTANCE) {
        // Use the exact function from your code
        forward(); 

      // 3. JUST RIGHT (Distance is within 28-32 cm): Stop
      } else {
        stopMotors();
      }
      break;
  }
}
/* Functions */
void showWelcome() {
  Serial.println(" -- Hello! I'm DAMI 3000!");
      Serial1.println(" -- Hello! I'm DAMI 3000!");
  lcd.clear();
  displayCentered("Hello!", 0);
  displayCentered("I'm DAMI 3000!", 1);

  for (int i = 0; i < 5; i++) {
    clearSegments();
    digitalWrite(segmentPins[6], HIGH); // Segment G (always on for welcome beeps)
    digitalWrite(BUZZER, HIGH); // Turn buzzer on for beep

    // Turn off all standard LEDs before conditional blinking
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW); // This is the BLUE_LED on pin 13
    digitalWrite(LEFT_LED, LOW);
    digitalWrite(RIGHT_LED, LOW);


    // Turn off the new blue and orange LEDs before conditional blinking for this iteration
    digitalWrite(blueLedPin, LOW);   // Ensure off
    digitalWrite(orangeLedPin, LOW); // Ensure off

    // Control existing LEDs based on beep iteration
    if (i == 0) { // First beep (0-indexed)
      digitalWrite(RED_LED, HIGH);

    } else if (i == 1) { // Second beep
      digitalWrite(GREEN_LED, HIGH);

    } else if (i == 2) { // Third beep
      digitalWrite(BLUE_LED, HIGH); // This is the BLUE_LED on pin 13

    } else if (i == 3) { // Fourth beep
      digitalWrite(GREEN_LED, HIGH);

    } else if (i == 4) { // Fifth beep
      digitalWrite(RED_LED, HIGH);

    }
    
    // ⭐ NEW: Control the `blueLedPin` (pin 46) and `orangeLedPin` (pin 47) based on the "B O B O & Both" pattern
    if (i == 0 || i == 2) { // Blue for the 1st and 3rd blinks (0-indexed)
      digitalWrite(blueLedPin, HIGH);
    } else if (i == 1 || i == 3) { // Orange for the 2nd and 4th blinks (0-indexed)
      digitalWrite(orangeLedPin, HIGH);
    } else if (i == 4) { // Both for the 5th blink (0-indexed)
      digitalWrite(blueLedPin, HIGH);
      digitalWrite(orangeLedPin, HIGH);
    }

    // Servo movement based on beep iteration: right, left, right, left, center
    if (i == 0 || i == 2) { // First and third beep (0-indexed)
      myservo.write(120); // Slightly right
    } else if (i == 1 || i == 3) { // Second and fourth beep (0-indexed)
      myservo.write(60);  // Slightly left
    } else if (i == 4) { // Last beep (5th iteration, 0-indexed)
      myservo.write(90);  // Recenter
    }

    delay(100); // Delay for the beep duration (unchanged)
    digitalWrite(BUZZER, LOW); // Turn buzzer off

    // Turn off all standard LEDs after the blink
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW); // This is the BLUE_LED on pin 13
    digitalWrite(LEFT_LED, LOW);
    digitalWrite(RIGHT_LED, LOW);

    // ⭐ NEW: Turn off the new blue and orange LEDs after the blink
    digitalWrite(blueLedPin, LOW);    // Turn off the new blue LED
    digitalWrite(orangeLedPin, LOW);  // Turn off the new orange LED

    digitalWrite(segmentPins[6], LOW); // Turn off segment G
    delay(50); // Delay between beeps (unchanged)
  }
  delay(1000); // Final delay after welcome sequence
  lcd.clear();
  Serial.println(" -- Made with love by Nizar <3");
      Serial1.println(" -- Made with love by Nizar <3");
  displayCentered("Made with love", 0);
  displayCentered("by Nizar <3", 1);

  for (int i = 0; i < 3; i++) {
    clearSegments();
    digitalWrite(segmentPins[6], HIGH); // Segment G (always on for welcome beeps)
    digitalWrite(BUZZER, HIGH); // Turn buzzer on for beep

    // Turn off all standard LEDs before conditional blinking
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW); // This is the BLUE_LED on pin 13
    digitalWrite(LEFT_LED, LOW);
    digitalWrite(RIGHT_LED, LOW);

    // Turn off the new blue and orange LEDs before conditional blinking for this iteration
    digitalWrite(blueLedPin, LOW);   // Ensure off
    digitalWrite(orangeLedPin, LOW); // Ensure off

    // Control existing LEDs based on beep iteration
    if (i == 0) { // First beep (0-indexed)
      digitalWrite(RED_LED, HIGH);
    } else if (i == 1) { // Second beep
      digitalWrite(GREEN_LED, HIGH);
    } else if (i == 2) { // Third beep
      digitalWrite(BLUE_LED, HIGH); // This is the BLUE_LED on pin 13
    } else if (i == 3) { // Fourth beep
      digitalWrite(GREEN_LED, HIGH);
    } else if (i == 4) { // Fifth beep
      digitalWrite(RED_LED, HIGH);
    }
    
    // ⭐ NEW: Control the `blueLedPin` (pin 46) and `orangeLedPin` (pin 47) based on the "B O B O & Both" pattern
    if (i == 0 || i == 2) { // Blue for the 1st and 3rd blinks (0-indexed)
      digitalWrite(blueLedPin, HIGH);
    } else if (i == 1 || i == 3) { // Orange for the 2nd and 4th blinks (0-indexed)
      digitalWrite(orangeLedPin, HIGH);
    } else if (i == 4) { // Both for the 5th blink (0-indexed)
      digitalWrite(blueLedPin, HIGH);
      digitalWrite(orangeLedPin, HIGH);
    }

    // Servo movement based on beep iteration: right, left, right, left, center
    if (i == 0 ) { // First and third beep (0-indexed)
      myservo.write(60); // Slightly right
    } else if (i == 1) { // Second and fourth beep (0-indexed)
      myservo.write(120);  // Slightly left
    } else if (i == 2) { // Last beep (5th iteration, 0-indexed)
      myservo.write(90);  // Recenter
    }

    delay(100); // Delay for the beep duration (unchanged)
    digitalWrite(BUZZER, LOW); // Turn buzzer off

    // Turn off all standard LEDs after the blink
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW); // This is the BLUE_LED on pin 13
    digitalWrite(LEFT_LED, LOW);
    digitalWrite(RIGHT_LED, LOW);

    // ⭐ NEW: Turn off the new blue and orange LEDs after the blink
    digitalWrite(blueLedPin, LOW);    // Turn off the new blue LED
    digitalWrite(orangeLedPin, LOW);  // Turn off the new orange LED

    digitalWrite(segmentPins[6], LOW); // Turn off segment G
    delay(50); // Delay between beeps (unchanged)
  }
  delay(1000);
}
void animateSegmentMenu() {
  // Skip if not in NONE mode (mode selection)
  if (currentMode != NONE) return;

  static unsigned long lastSegmentUpdate = 0;
  
  static unsigned long lastLedHoverUpdate = 0; // NEW: Timer for LED hover
  const unsigned long LED_HOVER_INTERVAL = 5; // CHANGED: Interval for LED hover updates (even faster)


  // 7-segment animation logic (original)
  static int segmentIndex = 0;
  if (millis() - lastSegmentUpdate >= 50) {
    lastSegmentUpdate = millis();
    clearSegments();
    if (segmentIndex < 6) {
      digitalWrite(segmentPins[segmentIndex], HIGH);
    }
    segmentIndex = (segmentIndex + 1) % 6;
  }

  // NEW: LED Hover Effect for Menu (Left, Right, Red, Green, and Blue LEDs)
  if (millis() - lastLedHoverUpdate >= LED_HOVER_INTERVAL) {
    lastLedHoverUpdate = millis();

    static int rgbLedCycle = 0; // 0: Red, 1: Green, 2: Blue

    ledBrightness += fadeAmount; // Change brightness

    // Reverse the direction of the fade when reaching min or max brightness
    if (ledBrightness <= 0 || ledBrightness >= 255) {
      fadeAmount = -fadeAmount; // Reverse the direction
      // Ensure brightness stays within valid 0-255 range
      ledBrightness = constrain(ledBrightness, 0, 255);

      // If brightness is at 0 and about to fade in, advance the RGB LED cycle
      if (ledBrightness == 0 && fadeAmount > 0) {
        rgbLedCycle = (rgbLedCycle + 1) % 3; // Cycle through 0, 1, 2
      }
    }

    // Apply brightness to Left and Right LEDs (always hover)
    analogWrite(LEFT_LED, ledBrightness);
    analogWrite(RIGHT_LED, ledBrightness);
    analogWrite(RED_LED_1, ledBrightness);
    analogWrite(RED_LED_2, ledBrightness);
    analogWrite(blueLedPin, ledBrightness);
    analogWrite(orangeLedPin, ledBrightness);

    // Apply brightness to the currently active RGB LED, turn others off
    analogWrite(RED_LED, (rgbLedCycle == 0) ? ledBrightness : LOW);
    analogWrite(GREEN_LED, (rgbLedCycle == 1) ? ledBrightness : LOW);
    analogWrite(BLUE_LED, (rgbLedCycle == 2) ? ledBrightness : LOW);
  }
}

// LCD Animation Function - Simple up/down movement on left and right edges, reversed on right
void animateLcdMenu() {
  if (currentMode != NONE) return; // Only animate in menu mode

  if (millis() - lastLcdAnimationUpdate >= LCD_ANIMATION_DELAY) {
    lastLcdAnimationUpdate = millis();

    // Calculate previous and current rows for left side
    int prevRowLeft = (lcdAnimationPhase - 1 + 2) % 2; // Ensure positive result for modulo
    int currentRowLeft = lcdAnimationPhase % 2;

    // Calculate previous and current rows for right side (reversed)
    int prevRowRight = (prevRowLeft == 0) ? 1 : 0; // If left was top, right was bottom
    int currentRowRight = (currentRowLeft == 0) ? 1 : 0; // If left is top, right is bottom

    // Clear previous characters on both sides
    lcd.setCursor(0, prevRowLeft);
    lcd.print(" ");
    lcd.setCursor(5, prevRowRight);
    lcd.print(" ");
    lcd.setCursor(11, prevRowLeft);
    lcd.print(" ");

    // Draw current characters on both sides
    lcd.setCursor(0, currentRowLeft); // Left edge
    lcd.print("~");
  
    lcd.setCursor(5, currentRowRight); // Left edge
    lcd.print("~");
    lcd.setCursor(11, currentRowLeft); // Right edge (reversed row)
    lcd.print("~");

    // Increment phase for the next frame
    lcdAnimationPhase++; 
  }
}
void showModeSelection() {
  currentMode = NONE; // Reset mode
  stopMotors();       // Ensure motors are stopped
  digitalWrite(BUZZER, LOW); // Ensure buzzer is off
  clearSegments();         // Clear 7-segment
  lcd.clear();

  lcd.clear(); // Clear LCD after transition message
  Serial.println(" -- Choose a mode :");
      Serial1.println(" -- Choose a mode :");
  displayCentered(" :BTH :MAZE :TSA", 0); // Display "1:BT 2:Maze" centered on the top line
  displayCentered(" :CRD :FLWM :TSM", 1);
 
  
  // Display the main menu
// Display "3:Test A:VC" on the second line - MODIFIED
  // The Emergency button (S16/'D') is always active and not explicitly listed in the menu.
  
  // Ensure all LEDs are off when in mode selection
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(LEFT_LED, LOW);  // Ensure these are OFF when starting menu
  digitalWrite(RIGHT_LED, LOW); // Ensure these are OFF when starting menu
  digitalWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off
  myservo.write(90); // Center servo

  // The blinking logic is removed from here as per request
}

void beep(int duration) {
  digitalWrite(BUZZER, HIGH);
  delay(duration);
  digitalWrite(BUZZER, LOW);
}

void handleCreditsMode() {
  if(variablechanger==1){
        variablechanger=0;
          Serial.println(" -- CREDITS MODE TRIGGERED !");
        Serial1.println(" -- CREDITS MODE TRIGGERED !");}
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
  // Define the credit messages
  String creditsMessages[] = {
    "Made by :\nNizar EL Idrysy",
    "Contains the\nmodified .ino",
    "3244 lines of\ndedicated coding",
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
    "DAMI 3000 !\nLoves you all :D"
  };
  
  const int numMessages = 22;
  
  // Blink 'C' on the 7-segment display
  static unsigned long lastSegmentBlinkTime = 0;
  static bool showSegmentC = true;
  if (millis() - lastSegmentBlinkTime >= 300) {
    lastSegmentBlinkTime = millis();
    showSegmentC = !showSegmentC;
    if (showSegmentC) {
      displayDigit(4);
  
    } else {
      clearSegments();

    }
  }

  // Display messages every 2 seconds
  if (millis() - lastMessageTime >= 2500) {
    lastMessageTime = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    int newlinePos = creditsMessages[currentCreditMessage].indexOf('\n');
    if (newlinePos != -1) {
      digitalWrite(BLUE_LED, HIGH);

      lcd.print(creditsMessages[currentCreditMessage].substring(0, newlinePos));
      lcd.setCursor(0, 1);
      lcd.print(creditsMessages[currentCreditMessage].substring(newlinePos + 1));
    } else {
      displayCentered(creditsMessages[currentCreditMessage], 0);
            digitalWrite(BLUE_LED, LOW);

    }
    
    currentCreditMessage++;
    if (currentCreditMessage >= numMessages) {
      currentCreditMessage = 0; // Loop back to the start
    }
  }
  

}


// This function now handles 7-segment display animation and LED hover effect.



void handleEmergency() {
  variablechanger=1;
          Serial.println(" -- EMERGENCY MODE TRIGGERED !");
        Serial1.println(" -- EMERGENCY MODE TRIGGERED !");
  stopMotors();
  isMazePaused = false;
  digitalWrite(BUZZER, LOW); // Ensure buzzer is OFF during emergency
  myservo.write(90);
  servoPos = 90;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" EMERGENCY MODE");
  lcd.setCursor(0, 1);
  lcd.print(" TRIGGERED !!!!");
  clearSegments();

  // Ensure turn signal LEDs are off during emergency
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW); // Turn off BLUE_LED
  digitalWrite(RED_LED_1, LOW); // Turn off new red LEDs
  digitalWrite(RED_LED_2, LOW);
  digitalWrite(WHITE_LED, LOW); // Turn off white LED
  digitalWrite(blueLedPin, LOW); // Turn off white LED
  digitalWrite(orangeLedPin, LOW); // Turn off white LED

  for (int i = 0; i < 7; i++) {
    displayLetter('E');
    digitalWrite(RED_LED, HIGH);
    // Green LED is kept off or turned off
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off during emergency blinks
    beep(50);
    clearSegments();
    digitalWrite(RED_LED, LOW);
    delay(50);
  }
  delay(1000);
  // After emergency, it will go back to mode selection via loop()
}

// MODIFIED controlBluetooth function for toggle movement and no auto-stop
void controlBluetooth() { // MODIFIED: Now accepts the character read in loop()
  static bool isForwardToggled = false;
  static bool isBackwardToggled = false;
  static bool isLeftToggled = false;
  static bool isRightToggled = false;
  static String lastNonMovementState = ""; // NEW: To hold persistent non-movement states

  // NEW: Variables for LED Hover Effect in BT mode
  static unsigned long lastLedHoverUpdateTime = 0;
  static int ledHoverBrightness = 0; // Starting brightness (off)
  static int ledHoverFadeAmount = 60;    // How much to change brightness each step (adjust for speed)
  const unsigned long LED_HOVER_INTERVAL_BT = 1; // Interval for LED hover updates in BT mode

  // New variables for white LED hovering
  static unsigned long lastWhiteLedHoverUpdateTime = 0;
  static int whiteLedHoverBrightness = 100;
  static int whiteLedHoverFadeAmount = 60; // Same speed as turn signals
  const unsigned long WHITE_LED_HOVER_INTERVAL_BT = 10000; // Same interval as turn signals


  // Helper lambda to reset all movement toggles and stop motors
  // This ensures only one movement is active at a time and provides a clean stop.
  auto resetMovementToggles = [&]() {
    isForwardToggled = false;
    isBackwardToggled = false;
    isLeftToggled = false;
    isRightToggled = false;
    stopMotors();
    // Ensure turn signals are off and reset hover state when stopping or changing movement
    analogWrite(LEFT_LED, LOW);
    analogWrite(RIGHT_LED, LOW);
    analogWrite(RED_LED_1, LOW); // Turn off new red LEDs
    analogWrite(RED_LED_2, LOW);
    analogWrite(WHITE_LED, LOW); // Turn off white LED
    analogWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off
    ledHoverBrightness = 0; // Reset brightness for next hover
    ledHoverFadeAmount = abs(ledHoverFadeAmount); // Ensure fade starts increasing
    whiteLedHoverBrightness = 0; // Reset white LED brightness
    whiteLedHoverFadeAmount = abs(whiteLedHoverFadeAmount); // Ensure white LED fade starts increasing
    lastNonMovementState = ""; // Clear lastNonMovementState when resetting toggles
  };

  // Check if BT mode was just entered (global flag)
  if (btModeJustEntered) {
    resetMovementToggles(); // This will clear lastNonMovementState
    // Explicitly clear the second line before setting "Idle"
    lcd.setCursor(0, 1);
    lcd.print("                "); // Clear the entire line 1
    updateMovementState("Idle", true); // Force "Idle" display
    btModeJustEntered = false; // Reset the flag
    // Removed: delay(100); // Small debounce for mode transition - not needed here
  }

  // --- Read all available characters from Serial1 and process only the last one ---
  char command = 0;
  while (Serial1.available()) {
    command = Serial1.read(); // Read the latest character
  }

  // Process command if it's a new, non-null character and not a newline/carriage return
  if (command != 0 && command != '\n' && command != '\r') {
    // Only clear lastNonMovementState if the command is a *movement* or *explicit stop* or *emergency*
    // This prevents clearing it when a non-movement command (like servo/buzzer) is given,
    // ensuring those states persist.
    if (command == 'Z' || command == 'S' || command == 'Q' || command == 'D' || command == ' ' || command == 'X') {
        lastNonMovementState = "";
    }

    switch (command) {
      case 'Z':
        if (isForwardToggled) { // If already moving forward, toggle off
          resetMovementToggles();
          // No explicit updateMovementState here, will fall to final else block for "Idle"
        } else { // Start moving forward
          resetMovementToggles(); // Stop any other active movement
          isForwardToggled = true;
          forward();
          updateMovementState("Forward"); // Direct update
        }
        break;
      case 'S':
        if (isBackwardToggled || isForwardToggled) { // If already moving backward, toggle off
          resetMovementToggles();
        } else { // Start moving backward
          backward();
          isBackwardToggled = true;
          backward();
          updateMovementState("Backward");
        }
        break;
      case 'Q':
        if (isLeftToggled) { // If already turning left, toggle off
          resetMovementToggles();
        } else { // Start turning left
          resetMovementToggles();
          isLeftToggled = true;
          turnLeft();
          updateMovementState("Left");
        }
        break;
      case 'D':
        if (isRightToggled) { // If already turning right, toggle off
          resetMovementToggles();
        } else { // Start turning right
          resetMovementToggles();
          isRightToggled = true;
          turnRight();
          updateMovementState("Right");
        }
        break;
      case ' ': // Unconditional Stop command (spacebar)
        resetMovementToggles(); // This clears lastNonMovementState
        updateMovementState("Idle", true); // Explicitly set to Idle immediately and force update
        break;
      case 'M': // Servo controls - MODIFIED: Removed blocking delay()
        myservo.write(0);
        servoPos = 0;
        // delay(300); // REMOVED BLOCKING DELAY
        lastNonMovementState = "Servo RIGHT"; // Set persistent state
        updateMovementState(lastNonMovementState); // No change to RED/GREEN LEDs for servo
        break;
      case 'K': // Servo controls - MODIFIED: Removed blocking delay()
        myservo.write(180);
        servoPos = 180;
        // delay(300); // REMOVED BLOCKING DELAY
        lastNonMovementState = "Servo LEFT";
        updateMovementState(lastNonMovementState); // No change to RED/GREEN LEDs for servo
        break;
      case 'O':
      case 'L': // Servo controls - MODIFIED: Removed blocking delay()
        myservo.write(90);
        servoPos = 90;
        // delay(300); // REMOVED BLOCKING DELAY
        lastNonMovementState = "Servo Middle";
        updateMovementState(lastNonMovementState); // No change to RED/GREEN LEDs for servo
        break;
      case 'I': // Servo controls - MODIFIED: Removed blocking delay()
        myservo.write(135);
        servoPos = 135;
        // delay(300); // REMOVED BLOCKING DELAY
        lastNonMovementState = "Servo left";
        updateMovementState(lastNonMovementState); // No change to RED/GREEN LEDs for servo
        break;
            case 'P': // Servo controls - MODIFIED: Removed blocking delay()
        myservo.write(45);
        servoPos = 45;
        // delay(300); // REMOVED BLOCKING DELAY
        lastNonMovementState = "Servo right";
        updateMovementState(lastNonMovementState); // No change to RED/GREEN LEDs for servo
        break;
      case 'B': // Buzzer toggle
        buzzerEnabled = !buzzerEnabled;
        digitalWrite(BUZZER, buzzerEnabled ? HIGH : LOW);
        lastNonMovementState = buzzerEnabled ? "Buzzer On" : "Buzzer Off";
        updateMovementState(lastNonMovementState); // No change to RED/GREEN LEDs for buzzer
        break;
      case 'X': // Emergency from Bluetooth
        handleEmergency(); // Call emergency handler
        showModeSelection(); // Go back to mode selection
        return; // Exit function early as mode has changed
      // No 'default' case here to prevent unintended stops from unknown characters
    }
  }

  // --- Execute current toggled movement or display persistent non-movement state ---
  if (isForwardToggled) {
    forward();
    updateMovementState("Motors Forward");
    analogWrite(LEFT_LED, LOW); // Ensure turn signals are off
    analogWrite(RIGHT_LED, LOW);
    analogWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off
    analogWrite(RED_LED_1, 50); // Dim new red LEDs
    analogWrite(RED_LED_2, 50); // Dim new red LEDs
    analogWrite(WHITE_LED, LOW); // Ensure white LED is off
  } else if (isBackwardToggled) {
    backward();
    updateMovementState("Motors Backward");
    analogWrite(LEFT_LED, LOW); // Ensure turn signals are off
    analogWrite(RIGHT_LED, LOW);
    analogWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off
    analogWrite(RED_LED_1, LOW); // Ensure red LEDs are off
    analogWrite(RED_LED_2, LOW);
    // Hovering logic for WHITE_LED
    if (millis() - lastWhiteLedHoverUpdateTime >= WHITE_LED_HOVER_INTERVAL_BT) {
      lastWhiteLedHoverUpdateTime = millis();
      whiteLedHoverBrightness += whiteLedHoverFadeAmount;
      if (whiteLedHoverBrightness <= 0 || whiteLedHoverBrightness >= 255) {
        whiteLedHoverFadeAmount = -whiteLedHoverFadeAmount;
        whiteLedHoverBrightness = constrain(whiteLedHoverBrightness, 0, 255);
      }
      analogWrite(WHITE_LED, whiteLedHoverBrightness);
    }
  } else if (isLeftToggled) {
    turnLeft();
    updateMovementState("Motors Left");
    // Hovering logic for LEFT_LED
    if (millis() - lastLedHoverUpdateTime >= LED_HOVER_INTERVAL_BT) {
      lastLedHoverUpdateTime = millis();
      ledHoverBrightness += ledHoverFadeAmount;
      if (ledHoverBrightness <= 0 || ledHoverBrightness >= 255) {
        ledHoverFadeAmount = -ledHoverFadeAmount;
        ledHoverBrightness = constrain(ledHoverBrightness, 0, 255);
      }
      analogWrite(LEFT_LED, ledHoverBrightness);
      analogWrite(RIGHT_LED, LOW); // Ensure other LED is off
    }
    analogWrite(RED_LED_1, 50); // Dim new red LEDs
    analogWrite(RED_LED_2, 50); // Dim new red LEDs
    analogWrite(WHITE_LED, LOW); // Ensure white LED is off
  } else if (isRightToggled) {
    turnRight();
    updateMovementState("Motors Right");
    // Hovering logic for RIGHT_LED
    if (millis() - lastLedHoverUpdateTime >= LED_HOVER_INTERVAL_BT) {
      lastLedHoverUpdateTime = millis();
      ledHoverBrightness += ledHoverFadeAmount;
      if (ledHoverBrightness <= 0 || ledHoverBrightness >= 255) {
        ledHoverFadeAmount = -ledHoverFadeAmount;
        ledHoverBrightness = constrain(ledHoverBrightness, 0, 255);
      }
      analogWrite(RIGHT_LED, ledHoverBrightness);
    }
    analogWrite(RED_LED_1, 50); // Dim new red LEDs
    analogWrite(RED_LED_2, 50); // Dim new red LEDs
    analogWrite(WHITE_LED, LOW); // Ensure white LED is off
  } else {
    // If no movement is toggled (robot is essentially idle or performing non-motor actions)
    stopMotors(); // Ensure motors are off
    analogWrite(LEFT_LED, LOW); // Ensure turn signals are off
    analogWrite(RIGHT_LED, LOW);
    analogWrite(WHITE_LED, LOW); // Ensure white LED is off

    // Hovering logic for RED_LED_1 and RED_LED_2 (full bright) when idle
    if (millis() - lastLedHoverUpdateTime >= LED_HOVER_INTERVAL_BT) {
      lastLedHoverUpdateTime = millis();
      ledHoverBrightness += ledHoverFadeAmount;
      if (ledHoverBrightness <= 0 || ledHoverBrightness >= 255) {
        ledHoverFadeAmount = -ledHoverFadeAmount;
        ledHoverBrightness = constrain(ledHoverBrightness, 0, 255);
      }
      analogWrite(RED_LED_1, ledHoverBrightness); // Full bright hovering
      analogWrite(RED_LED_2, ledHoverBrightness); // Full bright hovering
    }

    // Update LCD state based on last non-movement command or default to "Idle"
    if (lastNonMovementState != "") {
        updateMovementState(lastNonMovementState); // Keep displaying the last non-movement state
    } else {
        updateMovementState("Motors Idle"); // Default to "Idle"
    }
  }

  // Potentiometer for Speed Control (always active in BT mode)
  int potValue = analogRead(POT_PIN);
  int motorSpeed = map(potValue, 0, 1023, 0, 255);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void displayCentered(String msg, int row) {
  int len = msg.length();
  int startCol = max(0, (16 - len) / 2);
  lcd.setCursor(0, row);
  for (int i = 0; i < 16; ++i) lcd.print(" "); // Clear line
  lcd.setCursor(startCol, row);
  lcd.print(msg);
}

// MODIFIED updateMovementState to prevent flickering and implement new LED logic
void updateMovementState(String state, bool forceUpdate = false) {
  if (isMazePaused) {
        return; 
    } // MODIFIED: Added forceUpdate parameter
  static String lastDisplayedState = ""; // Stores the last state displayed on LCD

  // Only update LCD if the state has actually changed OR if force update is requested
  if (forceUpdate || state != lastDisplayedState) { // MODIFIED: Added forceUpdate condition
    displayCentered(state, 1); // Update LCD
    lastDisplayedState = state; // Store the new state

    // Main LED logic (RED/GREEN) for Bluetooth mode based on movement or state
    if (currentMode == BLUETOOTH) {
      // Determine if motors are currently moving based on the state string
      bool motorsAreMoving = (state.indexOf("Forward") != -1 ||
                               state.indexOf("Backward") != -1 ||
                               state.indexOf("Left") != -1 ||
                               state.indexOf("Right") != -1);

      // Only change RED/GREEN LEDs if the state indicates motor movement or idle
      if (motorsAreMoving) {
        digitalWrite(RED_LED, LOW);   // Red LED OFF when motors are moving
        digitalWrite(GREEN_LED, HIGH); // Green LED ON when motors are moving
        digitalWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off
      } else if (state.indexOf("Idle") != -1) { // Explicitly handle "Idle" state
        digitalWrite(RED_LED, HIGH);  // Red LED ON when motors are not moving (idle)
        digitalWrite(GREEN_LED, LOW); // Green LED OFF when motors are not moving
        digitalWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off
      }
      // For Servo or Buzzer states, the LEDs retain their previous state (no change here)
    }
    // Other modes' LED logic remains unchanged
    else if (currentMode == MAZE){
          if (state.indexOf("Obstacle!") != -1) { // Check for "Obstacle!" in the state string
          digitalWrite(RED_LED, HIGH);
          digitalWrite(GREEN_LED, LOW);
          digitalWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off
        } else {
          digitalWrite(RED_LED, LOW);
          digitalWrite(GREEN_LED, HIGH);
          digitalWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off
        }
    }
    else if (currentMode == TESTING){
          // For testing mode, we want both LEDs on for general activity,
          // but specific turn signals will blink independently.
          // The blinking for RED_LED and GREEN_LED will now be handled directly
          // in handleTestingMode() for more control.
          // So, we ensure they are off here to avoid conflicts.
          digitalWrite(RED_LED, LOW);
          digitalWrite(GREEN_LED, LOW);
          digitalWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off
    }
  }
}


// ⭐ N
void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}
void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void turnLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); // Left motor forward, Right motor backward
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  // Apply specific speeds for left turn
  analogWrite(ENA, MAZE_TURN_LEFT_SPEED);  // Speed for motor A
  analogWrite(ENB, MAZE_TURN_LEFT_SPEED);  // Speed for motor B
}
void turnRight() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); // Left motor backward, Right motor forward
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  // Apply specific speeds for right turn
  analogWrite(ENA, MAZE_TURN_RIGHT_SPEED); // Speed for motor A
  analogWrite(ENB, MAZE_TURN_RIGHT_SPEED); // Speed for motor B
}
void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
void servoSweep() {
    // Only run if the sweep delay has passed
    if (millis() - lastSweepTime >= SWEEP_SPEED_DELAY) {
        lastSweepTime = millis(); // Reset the timer

        // 1. Update the position
        servoPosition += servoDirection;

        // 2. Check limits and reverse direction
        if (servoPosition >= 135) {
            servoDirection = -1; // Reverse to sweep left
            servoPosition = 135; // Clamp to 180 (for accuracy)
        } else if (servoPosition <= 45) {
            servoDirection = 1;  // Reverse to sweep right
            servoPosition = 45;   // Clamp to 0
        }

        // 3. Write the new position
        myservo.write(servoPosition);
    }
}
// These slow movement functions are not currently used by Maze or BT mode in this version
void forwardSlow() { /* ... */
}
void backwardSlow() { /* ... */
}
void turnLeftSlow() { /* ... */
}
void turnRightSlow() { /* ... */
}

long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Measure the duration of the echo pulse.
  // A timeout of 25000 microseconds (25 milliseconds) is added to ensure that
  // if no echo is received within a reasonable time (e.g., object is too far),
  // pulseIn will return 0, preventing spurious small distance readings.
  long duration = pulseIn(ECHO_PIN, HIGH, 25000); 
  // Calculate distance in cm based on the speed of sound (0.0343 cm/microsecond).
  // The duration is divided by 2 because the sound travels to the object and back.
  long distance = duration * 0.0343 / 2;
  return distance;
}

void displayDigit(int digit) {
  if (digit < 0 || digit > 9) return; // Basic validation
  byte numbers[10] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111,
    0b01100110, 0b01101101, 0b01111101, 0b00000111,
    0b01111111, 0b01101111
  };
  byte segments = numbers[digit];
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (segments >> i) & 0x01);
  }
}

void clearSegments() {
  for (int i = 0; i < 7; i++) digitalWrite(segmentPins[i], LOW);
}

// Helper function to execute a turn with a calibrated delay
void executeMazeTurn(String direction) {
  if (direction == "left") {
    turnLeft(); // This will now apply MAZE_TURN_LEFT_SPEED
    delay(MAZE_TURN_LEFT_DURATION); // Use the new constant for left turn duration
  } else if (direction == "right") {
    turnRight(); // This will now apply MAZE_TURN_RIGHT_SPEED
    delay(MAZE_TURN_RIGHT_DURATION); // Use the new constant for right turn duration
  } else if (direction == "around") {
    turnLeft(); // Assuming two left turns for 180 degrees, or a long right turn
    // This will use MAZE_TURN_LEFT_SPEED and MAZE_TURN_LEFT_DURATION. If you need a specific speed/duration for 180 turn,
    // you might define new variables or adjust this logic.
    delay(MAZE_TURN_LEFT_DURATION * 2); // Use the new constant for 180 degree turn (double left duration)
  }
  stopMotors(); // Ensure motors stop after the turn duration
  delay(100); // Short pause after turning
}

// Maze Mode Handler - Implements the Left-Hand Rule Algorithm
void handleMazeMode() {
      if(variablechanger==1){
        variablechanger=0;
Serial.println(" -- MAZE MODE TRIGGERED !");
        Serial1.println(" -- MAZE MODE TRIGGERED !");
        
      }
      
          
  static unsigned long lastSegmentBlinkTime = 0; // Timer for 7-segment blinking
  static bool showSegmentDigit = true;         // State for 7-segment blinking
  static unsigned long lastBeepTimeMoving = 0;  // Timer for beeping while moving

  // Ensure turn signal LEDs are off in Maze mode
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);
  digitalWrite(RED_LED_1, LOW); // Ensure new red LEDs are off
  digitalWrite(RED_LED_2, LOW);
  digitalWrite(WHITE_LED, LOW); // Ensure white LED is off
  digitalWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off

  // --- 7-Segment Display: Blink number 2 (indicating Maze Mode) ---
  if (millis() - lastSegmentBlinkTime >= 300) { // Blink rate for digit 2
    lastSegmentBlinkTime = millis();
    showSegmentDigit = !showSegmentDigit;
    if (showSegmentDigit) {
      displayDigit(2);
    } else {
      clearSegments();
    }
  }

  // --- Potentiometer for Speed Control (adjust robot speed when moving forward) ---
  // Only apply potentiometer speed when moving straight, turns have their own speed.
  int potValue = analogRead(POT_PIN);
  int motorSpeed = map(potValue, 0, 1023, 0, 255);

  // --- Display distance on LCD (top line) ---
  lcd.setCursor(0, 0);
  lcd.print("Distance : ");
  long currentDistance = measureDistance();
  if (currentDistance < 10) lcd.print("0");
  if (currentDistance < 100) lcd.print(currentDistance);
  else lcd.print("99"); // Cap display at 99cm
  lcd.print(" cm      "); // Clear remaining characters on the line

  // --- Maze Solving Logic (New Algorithm State Machine) ---
  switch (currentMazeState) {
    case MAZE_STATE_MOVING_FORWARD:
      forward(); // Move the robot straight
      analogWrite(ENA, motorSpeed); // Apply potentiometer speed for forward movement
      analogWrite(ENB, motorSpeed);
      updateMovementState("Forward..."); // Update LCD state
      digitalWrite(BUZZER, LOW); // Ensure buzzer is off while moving forward
      if (millis() - lastBeepTimeMoving >= 800) { // Beep periodically while moving
        lastBeepTimeMoving = millis();
        beep(50); // Small beep while moving
      }

      // Check for obstacle in front
      if (measureDistance() <= OBSTACLE_DISTANCE) {
        currentMazeState = MAZE_STATE_OBSTACLE_DETECTED; // Obstacle detected, change state
        stateStartTime = millis(); // Record time of state change
        stopMotors(); // Stop immediately
        digitalWrite(BUZZER, HIGH); // Beep when obstacle detected
        updateMovementState("Obstacle!!!!"); // Update LCD
        delay(1000);
      }
      break;

    case MAZE_STATE_OBSTACLE_DETECTED:
      // Immediately transition to scanning after detecting an obstacle
      currentMazeState = MAZE_STATE_SCANNING;
      stateStartTime = millis(); // Record time of state change
      digitalWrite(BUZZER, LOW); // Turn off buzzer
      break;

    case MAZE_STATE_SCANNING:
      updateMovementState("Scanning..."); // Update LCD state

      // Scan right first (servo at 0 degrees, which was previously used for left)
      myservo.write(0); // Move servo to the right (assuming 0 is right extreme)
      delay(300); // Give servo time to move
      distRight = measureDistance(); // Measure distance to the right
      Serial.print("Right: "); Serial.println(distRight); // Debugging output

      // Scan left (servo at 180 degrees, which was previously used for right)
      myservo.write(180); // Move servo to the left (assuming 180 is left extreme)
      delay(300); // Give servo time to move
      distLeft = measureDistance(); // Measure distance to the left
      Serial.print("Left: "); Serial.println(distLeft); // Debugging output

      myservo.write(90); // Move servo back to center
      delay(100); // Give servo time to move

      // Decision making based on new priority: Right -> Left -> Turn Around
      if (distRight > TURN_CLEARANCE_DISTANCE) { // Prioritize right turn if clear
        currentMazeState = MAZE_STATE_TURNING_RIGHT;
      } else if (distLeft > TURN_CLEARANCE_DISTANCE) { // If right is blocked, check left
        currentMazeState = MAZE_STATE_TURNING_LEFT;
      } else { // Both right and left are blocked, turn around (half a turn was requested, assuming 180 degrees)
        currentMazeState = MAZE_STATE_TURNING_AROUND;
      }
      stateStartTime = millis(); // Reset timer for next state
      break;

    case MAZE_STATE_TURNING_LEFT:
      updateMovementState("Turning Left"); // Update LCD state
      executeMazeTurn("left"); // Perform the left turn (speed and duration are applied inside turnLeft() and executeMazeTurn())
      currentMazeState = MAZE_STATE_MOVING_FORWARD; // After turn, continue moving forward
      stateStartTime = millis();
      break;

    case MAZE_STATE_TURNING_RIGHT:
      updateMovementState("Turning Right"); // Update LCD state
      executeMazeTurn("right"); // Perform the right turn (speed and duration are applied inside turnRight() and executeMazeTurn())
      currentMazeState = MAZE_STATE_MOVING_FORWARD; // After turn, continue moving forward
      stateStartTime = millis();
      break;

    case MAZE_STATE_TURNING_AROUND:
      updateMovementState("Turning Around"); // Update LCD state
      executeMazeTurn("around"); // Perform the 180-degree turn
      currentMazeState = MAZE_STATE_MOVING_FORWARD; // After turn, continue moving forward
      stateStartTime = millis();
      break;
  }
}

// New Testing Mode Handler
void handleTestingMode() {
  if(variablechanger==1){
        variablechanger=0;
          Serial.println(" -- AUTOMATIC TESTING MODE TRIGGERED !");
        Serial1.println(" -- AUTOMATIC TESTING MODE TRIGGERED !");
  }
  static unsigned long lastLedBlinkTime = 0;
  static bool ledState = LOW; // For blinking turn signals and new red/white LEDs
  static unsigned long lastBuzzerBeapTime = 0;
  // Removed: static bool showSegmentDigit = true; // This is no longer needed for constant on
  static int rgbLedCycle = 0; // 0: Red, 1: Green, 2: Blue - for sequential RGB LED blinking

  // --- Potentiometer for Speed Control ---
  int potValue = analogRead(POT_PIN);
  int motorSpeed = map(potValue, 0, 1023, 0, 255);
  analogWrite(ENA, motorSpeed); // Apply speed
  analogWrite(ENB, motorSpeed);

  // Continuous LED blinking for LEFT_LED, RIGHT_LED, RED_LED_1, RED_LED_2, WHITE_LED
  if (millis() - lastLedBlinkTime >= 100) { // Blink every 100ms
    lastLedBlinkTime = millis();
    ledState = !ledState; // Toggle LED state
    digitalWrite(LEFT_LED, ledState);
    digitalWrite(RIGHT_LED, ledState);
    digitalWrite(RED_LED_1, ledState); // New Red LED 1
    digitalWrite(RED_LED_2, ledState); // New Red LED 2
    digitalWrite(WHITE_LED, ledState); // New White LED
  }

  // Synchronize RGB LED blinking, Buzzer beaping, and now constant 7-Segment display
  if (millis() - lastBuzzerBeapTime >= 500) { // Beep every 500ms /////////////////
    lastBuzzerBeapTime = millis();
    
    // ⭐ NEW: Ensure 7-Segment '8' is always displayed
    displayDigit(8); // Keep segment '8' ON, no blinking

    // Turn off all RGB LEDs first
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);

    // Turn on the current RGB LED based on the cycle
    if (rgbLedCycle == 0) {
      digitalWrite(RED_LED, HIGH);
    } else if (rgbLedCycle == 1) {
      digitalWrite(GREEN_LED, HIGH);
    } else if (rgbLedCycle == 2) {
      digitalWrite(BLUE_LED, HIGH);
    }
    
    beep(50); // Short beep

    // Turn off all RGB LEDs after the beep
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);

    // Advance the RGB LED cycle
    rgbLedCycle = (rgbLedCycle + 1) % 3; // Cycle through 0, 1, 2
  }

  // LCD display for distance and state
  lcd.setCursor(0, 0);
  lcd.print("Distance : ");
  long distance = measureDistance();
  if (distance < 10) {
    lcd.print("0");
    
  }
  
  if (distance < 100) lcd.print(distance);
  else lcd.print("99");
  lcd.print(" cm");

  
  
  // State machine for testing sequence
  switch (currentTestState) {
    case TEST_STATE_INIT:
      stopMotors();
      myservo.write(90); // Center servo
      updateMovementState("Testing...");
      currentTestState = TEST_STATE_FORWARD; // Move to the first test action
      testStateStartTime = millis();
      break;

    case TEST_STATE_FORWARD:
      forward();
      // Only update state string when entering this state
      if (millis() - testStateStartTime == 0) updateMovementState("Forward");
      if (millis() - testStateStartTime >= TEST_ACTION_DURATION) {
        stopMotors();
        currentTestState = TEST_STATE_BACKWARD;
        testStateStartTime = millis();
      }
      break;

    case TEST_STATE_BACKWARD:
      backward();
      // Only update state string when entering this state
      if (millis() - testStateStartTime == 0) updateMovementState("Backward");
      if (millis() - testStateStartTime >= TEST_ACTION_DURATION) {
        stopMotors();
        currentTestState = TEST_STATE_TURN_LEFT;
        testStateStartTime = millis();
      }
      break;

    case TEST_STATE_TURN_LEFT:
      turnLeft();
      // Only update state string when entering this state
      if (millis() - testStateStartTime == 0) updateMovementState("Left");
      if (millis() - testStateStartTime >= TEST_ACTION_DURATION) {
        stopMotors();
        currentTestState = TEST_STATE_TURN_RIGHT;
        testStateStartTime = millis();
      }
      break;

    case TEST_STATE_TURN_RIGHT:
      turnRight();
      // Only update state string when entering this state
      if (millis() - testStateStartTime == 0) updateMovementState("Right");
      if (millis() - testStateStartTime >= TEST_ACTION_DURATION) {
        stopMotors();
        currentTestState = TEST_STATE_SERVO_LEFT;
        testStateStartTime = millis();
      }
      break;

    case TEST_STATE_SERVO_LEFT:
      myservo.write(0); // Servo left
      // Only update state string when entering this state
      if (millis() - testStateStartTime == 0) updateMovementState("Servo Left");
      if (millis() - testStateStartTime >= TEST_ACTION_DURATION) {
        currentTestState = TEST_STATE_SERVO_CENTER;
        testStateStartTime = millis();
      }
      break;

    case TEST_STATE_SERVO_CENTER:
      myservo.write(90); // Servo center
      // Only update state string when entering this state
      if (millis() - testStateStartTime == 0) updateMovementState("Servo Centered");
      if (millis() - testStateStartTime >= TEST_ACTION_DURATION) {
        currentTestState = TEST_STATE_SERVO_RIGHT;
        testStateStartTime = millis();
      }
      break;

    case TEST_STATE_SERVO_RIGHT:
      myservo.write(180); // Servo right
      // Only update state string when entering this state
      if (millis() - testStateStartTime == 0) updateMovementState("Servo Right");
      if (millis() - testStateStartTime >= TEST_ACTION_DURATION) {
        currentTestState = TEST_STATE_CYCLE_COMPLETE;
        testStateStartTime = millis();
      }
      break;

    case TEST_STATE_CYCLE_COMPLETE:
      stopMotors();
      myservo.write(90); // Ensure servo is centered

  
      digitalWrite(RED_LED, LOW);   // Turn off RED_LED
      digitalWrite(GREEN_LED, LOW); // Turn off GREEN_LED
      digitalWrite(BLUE_LED, LOW); // Turn off BLUE_LED
      digitalWrite(BUZZER, LOW); // Turn off buzzer
      updateMovementState("Test Finished.");
      // Loop back to the beginning of the test cycle
      currentTestState = TEST_STATE_INIT;
      testStateStartTime = millis();
      delay(2500);
      break;
  }
}// I love yall, from Nizar :D
