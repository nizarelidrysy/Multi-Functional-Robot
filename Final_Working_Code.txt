#include <Wire.h> // I2C Library
#include <LiquidCrystal_I2C.h> // LCD Library
#include <Servo.h> // Servo-motor Library
#include <Keypad.h> // Keypad Library - NEW

/* Pins & functions */
// LEDs
#define GREEN_LED 8
#define RED_LED 7
#define LEFT_LED 6   // CHANGED: Left LED connected to pin 6
#define RIGHT_LED 5  // No change: Right LED connected to pin 5
#define BLUE_LED 13  // CHANGED: MIDDLE_LED renamed to BLUE_LED

// New LEDs
#define RED_LED_1 12 // New Red LED on PWM pin 12
#define RED_LED_2 11 // New Red LED on PWM pin 11
#define WHITE_LED 10 // New White LED on PWM pin 10

// Buzzer
#define BUZZER 49
void beep(int duration = 100);
bool buzzerEnabled = false; // Used for Bluetooth mode buzzer toggle

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

void handleEmergency();
void controlBluetooth(); // MODIFIED: No char argument, it will read Serial1 internally

// Motors
#define IN1 50
#define IN2 51
#define IN3 52 // Reverted from 55 - This was changed from your original working code
#define IN4 53 // Reverted from 54 - This was changed from your original working code
#define ENA 3 // Speed Control for the motors
#define ENB 4 // Speed Control for the motors
void forward();
void backward();
void turnLeft();
void turnRight();
void stopMotors();

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
enum Mode { NONE, BLUETOOTH, MAZE, TESTING, VC }; // NEW: Added VC mode
Mode currentMode = NONE; // Initial mode is set to none when in the menu
int number = 5; // Countdown (related to unused COUNTDOWN mode)

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

// NEW: Separate speeds for left and right turns in Maze Mode
// Adjust these values (0-255) to calibrate your robot's turns
const int MAZE_TURN_LEFT_SPEED = 255;  // Example speed for left turn
const int MAZE_TURN_RIGHT_SPEED = 255; // Example speed for right turn

// NEW: Separate durations for left and right turns in Maze Mode
// Adjust these values (in milliseconds) to calibrate your robot's turn duration
const int MAZE_TURN_LEFT_DURATION = 335; // Example duration for left turn
const int MAZE_TURN_RIGHT_DURATION = 367; // Example duration for right turn


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
  char customKey = keypad.getKey();

  // --- GLOBAL EMERGENCY BUTTON HANDLING (S16 / 'D' ) ---
  // Note: 'X' from Serial for emergency is handled directly within controlBluetooth now.
  if (currentMode != NONE && customKey == 'D') { // S16 is 'D'
    Serial.println("EMERGENCY button triggered (Keypad)! ");
    Serial1.println("EMERGENCY button triggered (Keypad)! ");
    handleEmergency();
    showModeSelection(); // Go back to mode selection
    return; // Exit loop iteration as mode has changed
  }
  // If in NONE mode, S16 ('D') acts as emergency
  else if (currentMode == NONE && customKey == 'D') { // S16 is 'D'
      Serial.println("EMERGENCY button triggered (from NONE mode)! ");
      Serial1.println("EMERGENCY button triggered (from NONE mode)! ");
      handleEmergency(); // Still call emergency handler to show message and beep
      showModeSelection(); // Go back to mode selection immediately
      return; // Exit loop iteration as mode has changed
  }


  if (currentMode == NONE) {
    // Instant mode selection for BT and Maze (Bluetooth '1' also handled here for initial entry)
    if (customKey == '1' || (Serial1.available() && Serial1.peek() == '1')) { // S1 is '1' for BT mode
      // Consume the '1' from Serial1 if it was peeked
      if (Serial1.available() && Serial1.peek() == '1') {
          Serial1.read();
      }
      Serial.println("Button 1 triggered (BT Mode)");
      Serial1.println("Button 1 triggered (BT Mode)");
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
      Serial.println("Button 2 triggered (Maze Mode)");
      Serial1.println("Button 2 triggered (Maze Mode)");
      beep(); // Single beep for Maze mode
      displayDigit(2);
      lcd.clear();
      currentMode = MAZE;
      currentMazeState = MAZE_STATE_MOVING_FORWARD; // Initialize maze state
      stateStartTime = millis();
      updateMovementState("Moving");
      currentNoneModeState = NONE_STATE_WAITING; // Reset state for next time
      delay(500); // Debounce for mode transition
    }
    // S3 ('3') for entering Testing Mode from NONE mode
    else if (customKey == '3') { // S3 is '3'
      Serial.println("Keypad 3 triggered (Testing Mode)");
      Serial1.println("Keypad 3 triggered (Testing Mode)");
      currentMode = TESTING;
      currentTestState = TEST_STATE_INIT;
      testStateStartTime = millis();
      updateMovementState("Testing: Initializing");
      displayDigit(8); // Show 8 for Testing mode
      lcd.clear();
      beep(); // Beep for mode entry
      delay(500); // Debounce for mode transition
    }
    // NEW: S4 ('A') for entering VC Mode from NONE mode
    else if (customKey == 'A') { // S4 is 'A'
      Serial.println("Keypad A triggered (VC Mode)");
      Serial1.println("Keypad A triggered (VC Mode)");
      beep(); // Beep for mode entry
      displayDigit(4); // Display 4 for VC mode on 7-segment
      lcd.clear();
      currentMode = VC; // Set new mode
      updateMovementState("Mode VC Engaged", true); // Force update LCD
      delay(500); // Debounce for mode transition
    }
  }
  else if (currentMode == BLUETOOTH) { // Call handler for BT mode
    controlBluetooth(); // No argument passed
  }
  else if (currentMode == MAZE) { // Call handler for MAZE mode
    handleMazeMode();
  }
  else if (currentMode == TESTING) { // Call handler for TESTING mode
    handleTestingMode();
  }
  // NEW: Handle VC mode
  else if (currentMode == VC) { // Handle VC mode
    // For now, just keep the LCD message and allow emergency exit
    // No specific actions defined yet for VC mode
    // The emergency button (S16/'D') will still work as it's global
    // Keep the 7-segment blinking for VC mode (digit 4)
    static unsigned long lastSegmentBlinkTimeVC = 0;
    static bool showSegmentDigitVC = true;
    if (millis() - lastSegmentBlinkTimeVC >= 300) {
      lastSegmentBlinkTimeVC = millis();
      showSegmentDigitVC = !showSegmentDigitVC;
      if (showSegmentDigitVC) {
        displayDigit(4); // Display 4 for VC mode
      } else {
        clearSegments();
      }
    }
    // Ensure LEDs are off or in a specific state for VC mode
    digitalWrite(LEFT_LED, LOW);
    digitalWrite(RIGHT_LED, LOW);
    digitalWrite(RED_LED_1, LOW);
    digitalWrite(RED_LED_2, LOW);
    digitalWrite(WHITE_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off
  }


  // Update 7-segment display and LCD distance for Bluetooth mode
  if (currentMode == BLUETOOTH) {
    static unsigned long lastBlinkTime = 0;
    static bool showDigit = true;

    if (millis() - lastBlinkTime >= 300) {
      lastBlinkTime = millis();
      showDigit = !showDigit;
      if (showDigit) {
        displayDigit(1);
      } else {
        clearSegments();
      }
    }

    lcd.setCursor(0, 0);
    lcd.print("Distance : ");
    long distance = measureDistance();
    if (distance < 10) lcd.print("0");
    if (distance < 100) lcd.print(distance);
    else lcd.print("99");
    lcd.print(" cm");

    Serial1.print("Distance : ");
    Serial1.print(distance);
    Serial1.println(" cm");
    Serial.print("Distance : ");
    Serial.print(distance);
    Serial.print(" cm, Keypad: ");
    Serial.print(customKey); // Print the last pressed keypad key for debugging
    Serial.println();
  }
  // If no specific mode is active (i.e., currentMode == NONE), run segment animation and LCD blinking
  else if (currentMode == NONE) {
    animateSegmentMenu();
    animateLcdMenu(); // Call the new LCD animation function
  }
}

/* Functions */
void showWelcome() {
  lcd.clear();
  displayCentered("Hello!", 0);
  displayCentered("I'm DAMI 3000!", 1);

  for (int i = 0; i < 5; i++) {
    clearSegments();
    digitalWrite(segmentPins[6], HIGH); // Segment G (always on for welcome beeps)
    digitalWrite(BUZZER, HIGH); // Turn buzzer on for beep

    // Control LEDs based on beep iteration
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW); // Ensure blue LED is off by default for each iteration

    if (i == 0) { // First beep (0-indexed)
      digitalWrite(RED_LED, HIGH);
    } else if (i == 1) { // Second beep
      digitalWrite(GREEN_LED, HIGH);
    } else if (i == 2) { // Third beep
      digitalWrite(BLUE_LED, HIGH);
    } else if (i == 3) { // Fourth beep
      digitalWrite(GREEN_LED, HIGH);
    } else if (i == 4) { // Fifth beep
      digitalWrite(RED_LED, HIGH);
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
    digitalWrite(RED_LED, LOW);   // Turn off all LEDs after the blink
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(LEFT_LED, LOW);  // Ensure these are off during welcome sequence
    digitalWrite(RIGHT_LED, LOW); // Ensure these are off during welcome sequence
    digitalWrite(segmentPins[6], LOW); // Turn off segment G
    delay(50); // Delay between beeps (unchanged)
  }
  delay(1000); // Final delay after welcome sequence
}

void showModeSelection() {
  currentMode = NONE; // Reset mode
  stopMotors();       // Ensure motors are stopped
  digitalWrite(BUZZER, LOW); // Ensure buzzer is off
  clearSegments();         // Clear 7-segment
  lcd.clear();

  // Transition message: "Please select a mode" with 3 rapid beeps
  displayCentered("Which mode would", 0);
  displayCentered("you like ? :-)", 1);
  for (int i = 0; i < 3; i++) {
    // Ensure all LEDs are off before blinking the specific one
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(LEFT_LED, LOW);  
    digitalWrite(RIGHT_LED, LOW); 
    
    // Control LEDs based on beep iteration
    if (i == 0) { // First beep (0-indexed)
      digitalWrite(RED_LED, HIGH);
      displayDigit(1); // First beep, flash 1
    } else if (i == 1) { // Second beep
      digitalWrite(GREEN_LED, HIGH);
      displayDigit(2); // Second beep, flash 2
    } else if (i == 2) { // Third beep
      digitalWrite(BLUE_LED, HIGH);
      displayLetter('X'); // Third beep, flash X
    }
    
    beep(50); // Rapid beep (unchanged)
    delay(100); // Short delay between beeps (unchanged)
    
    clearSegments(); // Clear segments after each flash
    // Turn off all LEDs after the blink
    digitalWrite(RED_LED, LOW);  
    digitalWrite(GREEN_LED, LOW); 
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(LEFT_LED, LOW);  
    digitalWrite(RIGHT_LED, LOW); 
  }
 delay(1000);
  lcd.clear(); // Clear LCD after transition message

  // Display the main menu
  displayCentered("1:BT  2:Maze", 0); // Display "1:BT 2:Maze" centered on the top line
  displayCentered("X:Test", 1); // Display "3:Test A:VC" on the second line - MODIFIED
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

void displayLetter(char letter) {
  byte segments = 0;
  switch (letter) {
    case 'E': segments = 0b01111001; break;
    case 'H': segments = 0b01110110; break;
    case 'X': segments = 0b01110110; // Pattern for 'X' (using 'H' for now, can be customized)
  }
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (segments >> i) & 0x01);
  }
}

// This function now handles 7-segment display animation and LED hover effect.
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
    lcd.setCursor(15, prevRowRight);
    lcd.print(" ");

    // Draw current characters on both sides
    lcd.setCursor(0, currentRowLeft); // Left edge
    lcd.print("|");
    lcd.setCursor(15, currentRowRight); // Right edge (reversed row)
    lcd.print("|");

    // Increment phase for the next frame
    lcdAnimationPhase++; 
  }
}


void handleEmergency() {
  stopMotors();
  digitalWrite(BUZZER, LOW); // Ensure buzzer is OFF during emergency
  myservo.write(90);
  servoPos = 90;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" EMERGENCY MODE");
  lcd.setCursor(0, 1);
  lcd.print("!!!!!!!!!!!!!!!!");
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
  static int whiteLedHoverBrightness = 0;
  static int whiteLedHoverFadeAmount = 60; // Same speed as turn signals
  const unsigned long WHITE_LED_HOVER_INTERVAL_BT = 1; // Same interval as turn signals


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
        if (isBackwardToggled) { // If already moving backward, toggle off
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
        lastNonMovementState = "Servo Right"; // Set persistent state
        updateMovementState(lastNonMovementState); // No change to RED/GREEN LEDs for servo
        break;
      case 'K': // Servo controls - MODIFIED: Removed blocking delay()
        myservo.write(180);
        servoPos = 180;
        // delay(300); // REMOVED BLOCKING DELAY
        lastNonMovementState = "Servo Left";
        updateMovementState(lastNonMovementState); // No change to RED/GREEN LEDs for servo
        break;
      case 'O':
      case 'L': // Servo controls - MODIFIED: Removed blocking delay()
        myservo.write(90);
        servoPos = 90;
        // delay(300); // REMOVED BLOCKING DELAY
        lastNonMovementState = "Servo Centered";
        updateMovementState(lastNonMovementState); // No change to RED/GREEN LEDs for servo
        break;
      case 'B': // Buzzer toggle
        buzzerEnabled = !buzzerEnabled;
        digitalWrite(BUZZER, buzzerEnabled ? HIGH : LOW);
        lastNonMovementState = buzzerEnabled ? "Buzzer : ON" : "Buzzer : OFF";
        updateMovementState(lastNonMovementState); // No change to RED/GREEN LEDs for buzzer
        break;
      case 'X': // Emergency from Bluetooth
        Serial.println("EMERGENCY button triggered (Bluetooth)! ");
        Serial1.println("EMERGENCY button triggered (Bluetooth)! ");
        handleEmergency(); // Call emergency handler
        showModeSelection(); // Go back to mode selection
        return; // Exit function early as mode has changed
      // No 'default' case here to prevent unintended stops from unknown characters
    }
  }

  // --- Execute current toggled movement or display persistent non-movement state ---
  if (isForwardToggled) {
    forward();
    updateMovementState("Forward");
    analogWrite(LEFT_LED, LOW); // Ensure turn signals are off
    analogWrite(RIGHT_LED, LOW);
    analogWrite(BLUE_LED, LOW); // Ensure BLUE_LED is off
    analogWrite(RED_LED_1, 50); // Dim new red LEDs
    analogWrite(RED_LED_2, 50); // Dim new red LEDs
    analogWrite(WHITE_LED, LOW); // Ensure white LED is off
  } else if (isBackwardToggled) {
    backward();
    updateMovementState("Backward");
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
    updateMovementState("Left");
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
    updateMovementState("Right");
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
        updateMovementState("Idle"); // Default to "Idle"
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
void updateMovementState(String state, bool forceUpdate = false) { // MODIFIED: Added forceUpdate parameter
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
    0b01100110, 0b01111101, 0b01111101, 0b00000111,
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
      updateMovementState("Maze: Forward"); // Update LCD state
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
        updateMovementState("Maze: Obstacle!"); // Update LCD
      }
      break;

    case MAZE_STATE_OBSTACLE_DETECTED:
      // Immediately transition to scanning after detecting an obstacle
      currentMazeState = MAZE_STATE_SCANNING;
      stateStartTime = millis(); // Record time of state change
      digitalWrite(BUZZER, LOW); // Turn off buzzer
      break;

    case MAZE_STATE_SCANNING:
      updateMovementState("Maze: Scanning"); // Update LCD state

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
      updateMovementState("Maze: Turning Left"); // Update LCD state
      executeMazeTurn("left"); // Perform the left turn (speed and duration are applied inside turnLeft() and executeMazeTurn())
      currentMazeState = MAZE_STATE_MOVING_FORWARD; // After turn, continue moving forward
      stateStartTime = millis();
      break;

    case MAZE_STATE_TURNING_RIGHT:
      updateMovementState("Maze: Turning Right"); // Update LCD state
      executeMazeTurn("right"); // Perform the right turn (speed and duration are applied inside turnRight() and executeMazeTurn())
      currentMazeState = MAZE_STATE_MOVING_FORWARD; // After turn, continue moving forward
      stateStartTime = millis();
      break;

    case MAZE_STATE_TURNING_AROUND:
      updateMovementState("Maze: Turning Around"); // Update LCD state
      executeMazeTurn("around"); // Perform the 180-degree turn
      currentMazeState = MAZE_STATE_MOVING_FORWARD; // After turn, continue moving forward
      stateStartTime = millis();
      break;
  }
}

// New Testing Mode Handler
void handleTestingMode() {
  static unsigned long lastLedBlinkTime = 0;
  static bool ledState = LOW; // For blinking turn signals and new red/white LEDs
  static unsigned long lastBuzzerBeapTime = 0;
  static unsigned long lastSegmentBlinkTime = 0; // Timer for 7-segment blinking
  static bool showSegmentDigit = true;         // State for 7-segment blinking
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

  // Sequential RGB LED blinking with buzzer beeping
  if (millis() - lastBuzzerBeapTime >= 500) { // Beep every 500ms
    lastBuzzerBeapTime = millis();
    
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

  // --- 7-Segment Display: Blink number 8 ---
  if (millis() - lastSegmentBlinkTime >= 300) { // Blink rate for 7-segment
    lastSegmentBlinkTime = millis();
    showSegmentDigit = !showSegmentDigit;
    if (showSegmentDigit) {
      displayDigit(8); // Changed from displayLetter('X') to displayDigit(8)
    } else {
      clearSegments();
    }
  }

  // LCD display for distance and state
  lcd.setCursor(0, 0);
  lcd.print("Distance : ");
  long distance = measureDistance();
  if (distance < 10) lcd.print("0");
  if (distance < 100) lcd.print(distance);
  else lcd.print("99");
  lcd.print(" cm");

  // State machine for testing sequence
  switch (currentTestState) {
    case TEST_STATE_INIT:
      stopMotors();
      myservo.write(90); // Center servo
      updateMovementState("Testing Loop");
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
      digitalWrite(LEFT_LED, LOW); // Turn off blinking LEDs
      digitalWrite(RIGHT_LED, LOW);
      digitalWrite(RED_LED, LOW);   // Turn off RED_LED
      digitalWrite(GREEN_LED, LOW); // Turn off GREEN_LED
      digitalWrite(BLUE_LED, LOW); // Turn off BLUE_LED
      digitalWrite(BUZZER, LOW); // Turn off buzzer
      updateMovementState("Loop Finished");
      // Loop back to the beginning of the test cycle
      currentTestState = TEST_STATE_INIT;
      testStateStartTime = millis();
      delay(2000);
      break;
  }
}
