# 🤖 DAMI 3000: An Advanced Modular Arduino Robot

**DAMI 3000** is an expanded and highly versatile Arduino-based robot designed to demonstrate advanced robotics, embedded systems, and user interaction concepts. This repository contains the latest iteration of the source code, featuring **ten distinct operational modes**, an intuitive menu system, and new communication capabilities.

---

## 🚀 Key Features

The project has evolved into a comprehensive platform with the following core functionalities:

### 1. **Expanded Multi-Mode Operation (7 Modes)**

The robot now supports a robust menu system, with the following modes defined in the firmware:

* **Navigation & Control:**
    * **BLUETOOTH:** Remote control of movement, servo, and buzzer via a Bluetooth connection.
    * **MAZE:** Autonomous navigation using the left-hand rule algorithm and obstacle avoidance.
    * **FOLLOW_ME:** A new mode likely for object tracking or simple follow behavior.
    * **JOYSTICK_TEST:** Dedicated mode for testing joystick input or controls.
* **Utility & Diagnostics:**
    * **AUTO-TST** Diagnostic mode for sequentially testing motors (Forward/Backward) and the servo (Left/Center/Right).
    * **MAN_TST (Manual Test):** A dedicated mode for manual, low-level testing of components.
    * **CREDITS:** Displays project information or development credits on the LCD.

### 2. **Advanced User Interaction & Input**

* **T9 (Multi-tap) Input Logic:** Implements a T9-style input system using the **4x4 Keypad** to allow users to cycle through letters/commands, enhancing the input capabilities beyond simple mode selection, specifically for modes like **MORSE_COM**.
* **Dynamic Menu System:** Utilizes a **2-second interval** menu switch (`MENU_SWITCH_INTERVAL = 2000`) to cycle through available mode options on the **16x2 I2C LCD**, making a large number of modes navigable.

### 3. **Core Robotics & Safety**

* **Safety Features:** A global emergency stop function, available via a dedicated keypad button and Bluetooth command, immediately halts all motors.
* **Dynamic Visual Feedback:** LEDs provide welcoming sequences, mode indicators, and dynamic light patterns based on the robot's state.

---

## 🛠️ Hardware Requirements

The core hardware remains essential, with the **4x4 Keypad** and **16x2 I2C LCD** being central to the new menu and input system.

* **Microcontroller:** Arduino Mega 2560 or similar board
* **Motor Control:** L298N Motor Driver, 2x DC Motors
* **Actuators:** 1x Micro Servo Motor (SG90)
* **Sensing & Comms:** 1x HC-SR04 Ultrasonic Sensor, Bluetooth Module (e.g., HC-05)
* **User Interface:** 1x 4x4 Keypad, 1x 16x2 LCD with I2C Module, 1x 7-Segment Display, Buzzer, LEDs.

---

## ⚙️ Installation & Setup

### 1. Libraries
Ensure the following libraries are installed via the Arduino IDE's Library Manager:

* `Wire.h` (for I2C communication)
* `LiquidCrystal_I2C.h`
* `Servo.h`
* `Keypad.h`

### 2. Wiring
Follow the pin definitions in the code for component connections:

| Component | Pin(s) (Arduino Mega) |
| :--- | :--- |
| **Motors (L298N)** | ENA (pin 3), ENB (pin 4) |
| **Servo** | Pin 2 |
| **Ultrasonic Sensor** | TRIG (pin 43), ECHO (pin 44) |
| **Keypad** | Rows (pins 35-38), Columns (pins 34-30) |
| **LCD** | I2C (address is typically `0x27`) |

### 3. Code Upload
Upload the `Modified_Code.txt` file content to your Arduino board using the Arduino IDE.

---

## 🏃 Usage

1.  **Startup:** The robot performs a welcome sequence, and the LCD displays the current menu page.
2.  **Mode Selection:** The menu options will cycle every **2 seconds**. Use the keypad to select the desired mode when it is displayed.
3.  **Advanced Input:** For modes like **MORSE_COM**, utilize the T9 logic on the keypad: press a number key repeatedly to cycle through the associated characters (`T9 Key Map` logic is defined in the code).
4.  **Emergency Stop:** Press the designated emergency stop key (`D` in the original specification) on the keypad at any time to immediately halt all operations.

---

## 🤝 Contribution

This project is a continuous work in progress. Contributions, especially those expanding on the new modes (`FOLLOW_ME`, `MORSE_COM`, etc.) or refining the T9 input system, are highly welcome!

1.  Fork the repository.
2.  Create your feature branch (`git checkout -b feature/new-mode`).
3.  Commit your changes.
4.  Open a Pull Request.

---

## 📄 License

This project is open-source, and open for contributions.