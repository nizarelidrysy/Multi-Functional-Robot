# DAMI 3000: Advanced Modular Arduino 4x4 Robot

The DAMI 3000 is an ultra-versatile, mega-expanded robotics platform powered by an Arduino MEGA 2560. Designed to integrate autonomous navigation, sensory feedback, interactive entertainment, and utilities, this project packs massive functionality into a single modular codebase.

---

## Hardware Architecture

The firmware orchestrates a diverse array of hardware components integrated directly onto the robot:
* **Brain:** Arduino MEGA 2560
* **Locomotion:** L298N H-Bridge controlling dual DC motors (with analog potentiometer speed control)
* **Vision & Range:** HC-SR04 Ultrasonic Sensor mounted on a high-torque Servo Motor
* **User Interaction:** 16x2 I2C LCD, 4x4 Matrix Keypad, 2-Axis Joystick, and a 7-Segment Display
* **Feedback Matrix:** Active/Passive Buzzer and full-spectrum RGB LEDs

---

## Integrated Operational Modes

The DAMI 3000 firmware features 17 distinct execution modes categorized below. *No complex menu shortcuts required—just smooth, dynamic cycling directly through the I2C LCD.*

### Autonomous & Manual Control
* **BLUETOOTH:** Remote operation over serial connection (HC-06) supporting directional drive commands, servo sweeps, and custom horn triggers, all using android (BLE enabled) or *CoolTerm* on Windows 10.
* **MAZE:** Autonomous navigation driven by a smart *Right-Hand Algorithm* with automatic obstacle scanning and path clearing.
* **FOLLOW ME:** Real-time object tracking utilizing a customizable ultrasonic range threshold adjustable on-the-fly via the joystick.
* **OFF:** Enters a low-power, safe standby sequence accompanied by an off-tone alert.

### Interactive Mini-Games
* **CHROME DINAUSOR GAME:** An LCD-rendered endless side-scroller game using the joystick to jump over obstacles and track high scores in EEPROM.
* **SIMON SAYS:** A memory matching visual game testing reflexes and sequential recall.
* **REACTION:** A lightning-fast reflex testing mode logging milliseconds straight to onboard storage.

### Novelties & Simulations
* **SLEEPY RADAR:** Sweeps the servo to monitor surrounding vectors, scanning objects within immediate proximity.
* **MORSE COMMUNICATION:** Translates characters input over serial link directly into standard audio-visual Morse code via buzzer and RGB states.
* **DICE:** True randomized rolling numbers simulated dynamically across the 7-Segment screen.
* **MELODIES:** Pre-programmed music mode utilizing the passive buzzer to play classic multi-note chiptunes.

### Daily Utilities
* **CALCULATOR:** Full float-based mathematical terminal input directly via the 4x4 matrix keypad layout.
* **STOPWATCH AND TIMER:** Dual-purpose precision timepiece featuring countdown alarms and standalone run logs.
* **CREDITS:** Displays project milestones, lineage, development specs, and special acknowledgments across the screens.

### Hardware Diagnostics
* **AUTOMATIC TEST:** A safe sequential self-test pulsing motor directions, testing RGB limits, and running servo sweeps.
* **MANUAL TEST:** Low-level diagnostic state allowing devs to isolate single pin components via specific keystrokes.
* **JOYSTICK TEST:** Raw feedback monitoring showing XY boundaries directly on the telemetry terminal.

---

## Built-in Safety Protocols

> **Emergency State Reset:** 
> The firmware intercepts a global emergency key (`D`) during ALL active states. Triggering it instantly overrides execution, forcefully kills the L298N drive lines, initializes safe servo seating ($90^\circ$), flashes warning telemetry data, and cycles the buzzer aggressively until structural safety is restored.

---

## Contribution Workflow

Want to optimize the navigation algorithms or introduce an 18th operational mode? Contributions are welcome:
1. Fork this repository.
2. Setup your feature branch (`git checkout -b feature/amazing-new-mode`).
3. Document any newly added hardware pins or dependencies.
4. Open a clean Pull Request.

---

<div align="center">
  <p>Engineered with dedication by Nizar EL IDRYSY ❤️</p>
</div>