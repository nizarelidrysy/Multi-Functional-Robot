# DAMI 3000: Operational Manual

This guide outlines the complete operational lifecycle, interface navigation, and safety protocols for the DAMI 3000 modular robotics platform.

---

## 1. Startup Sequence and Hardware Calibration

When the power source is connected, the robot executes an automated calibration routine to ensure structural and electrical safety:

* **Hardware Reset:** The firmware forces all hardware components to their default safety configurations. This corrects any manual shifts (such as a forced change to the servo motor position while the robot was stored on a shelf).
* **Standby Lock:** The system locks and waits for user acknowledgment. The LCD remains idle until you click the **Joystick**.
* **Welcome Sequence:** Once clicked, a one-time introductory message sequence plays, stating the robot's name (**DAMI 3000**), before transitioning to the main selection environment.

---

## 2. Menu Navigation Logic

The interface displays 16 core operational modes split across two distinct menu variants for clear readability.

* **Switching Menu Pages:** Move the **Joystick Left or Right** to cycle back and forth between the two menu variants. Each page presents 8 distinct operational modes.
* **Selecting a Mode:** Use the 4x4 Matrix Keypad to launch a mode. The keys correspond directly to the operational modes:
  * **S1 through S16:** Pressing a key maps directly to launching its assigned mode.

---

## 3. In-Mode Interactions and Runtime Controls

Once a mode is running, user interaction depends on the specific logic of that mode (e.g., joystick steering, calculator inputs, or game reflexes). However, two global overrides are hardcoded into the keypad layout:

* **Execution Pause (S15):** Supported modes can be paused instantly mid-execution by pressing the **S15** button, freezing current movements or timers until resumed by clicking s15 again.
* **Emergency Reset & Mode Exit (S16):** Pressing the **S16** button triggers the 17th mode (**Emergency Mode**). This functions as both a safety kill-switch and a global "Back" button from current modes:
  * It overrides any active process or runaway state.
  * It kills motor power and resets all components back to their original baseline positions.
  * It terminates the active mode and returns you directly back to the main selection menu.
  * If inside an active mode, press **S16** to safely exit back to the main menu.

---

## 4. Safe Shutdown Procedure

To prevent gear damage, electrical arcs, or memory corruption before storing the robot back on a shelf, follow this sequence:

*. If inside an active mode, press **S16** to safely exit back to the main menu.
2. While in the main menu, click the **Joystick** inward.
3. The robot will initiate a final teardown routine: resetting all actuators to baseline positions, turning off the LCD, and powering down all peripheral matrices.
4. Once the system indicators turn off, it is completely safe to disconnect the batteries and store the robot away.

---

<div align="center">
  <p>Engineered with dedication by Nizar EL IDRYSY ❤️</p>
</div>