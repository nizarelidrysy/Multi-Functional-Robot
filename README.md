## 🤖 DAMI 3000: An Advanced Modular Arduino Robot

DAMI 3000 is an expanded and highly versatile **Arduino-based robot** designed to demonstrate advanced robotics, embedded systems, and user interaction concepts. This repository contains the latest iteration of the source code, featuring **nine distinct operational modes**, an intuitive menu system, and new communication capabilities.

---

## 🚀 Key Features

The project has evolved into a comprehensive platform with the following core functionalities:

### 1. Nine Operational Modes

The robot now supports a robust menu system, with the following nine operational modes defined in the firmware:

#### Navigation & Control:
* **BLUETOOTH (1):** Remote control of movement, servo, and buzzer via a **Bluetooth connection**.
* **MAZE (2):** Autonomous navigation using the **Right-Hand Algorithm** and obstacle avoidance.
* **FOLLOW\_ME (A):** For object tracking or simple follow behavior.
* **JOYSTICK\_TEST (7):** Dedicated mode for testing joystick input and controls.

#### Utility & Diagnostics:
* **AUTO-TST (B):** Diagnostic mode (TESTING) for sequentially testing motors (Forward/Backward), servo (Left/Center/Right), and LEDs.
* **MAN\_TST (6):** A dedicated mode for manual, low-level testing of individual components via the keypad.
* **CREDITS (5):** Displays project information or development credits on the **LCD**, with an optional servo sweep feature.

#### Novelty & Communications:
* **MORSE\_COM (3):** Transmits characters via the **buzzer and LEDs** using Morse code.
* **DICE (4):** A simple rolling dice simulation displayed on the **7-Segment Display**.

### 2. Keypad-Driven Menu System

* **Menu Navigation:** The menu system is navigated by pressing the designated key on the **4x4 Keypad** (**D** in the default configuration) or by moving the joystick left/right. This cycles through available modes on the **16x2 I2C LCD**.
* **Mode Activation:** Modes are activated by pressing the corresponding number or letter key shown on the LCD menu.
* **Morse Input:** The **MORSE\_COM** mode primarily accepts characters for translation via the **Bluetooth/Serial connection** (**Serial1**).

### 3. Core Robotics & Safety
* **Safety Features:** A global **emergency stop function** is available via the designated keypad button (**D** key in operational modes) which immediately halts all motors and resets the robot state.
* **Dynamic Visual Feedback:** **RGB LEDs** and the **7-Segment Display** are utilized to provide welcoming sequences, mode indicators (e.g., blinking **1** for Bluetooth, **P** for pause), and dynamic light/segment patterns based on the robot's state.

---

## 🏃 Usage

* **Startup:** The robot performs a welcome sequence, and the **LCD** displays the current menu page.
* **Menu Navigation:** Press the **D key** (or move the joystick) to cycle through the menu pages. The LCD displays the mode name and the corresponding key to select it.
* **Mode Selection:** Press the associated key (**1** through **7**, **A**, or **B**) on the keypad to select the desired mode.
* **Pause Function:** In operational modes like **BLUETOOTH** or **MAZE**, press the **# key** to toggle the robot's pause state.
* **Emergency Stop:** Press the designated emergency stop key (**D** in an active mode) on the keypad at any time to immediately halt all operations and reset to the main menu.

---

## 🤝 Contribution

This project is a continuous work in progress. Contributions, especially those expanding on the new modes or refining existing logic, are highly welcome!

* Fork the repository.
* Create your feature branch (\`git checkout -b feature/new-mode\`).
* Commit your changes.
* Open a Pull Request.

---

## 📄 License

This project is open-source and open for contribution.

---

<div align="center">
  <p>Built with ❤️ by Nizar EL IDRYSY.</p>
</div>
