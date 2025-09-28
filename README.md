# Wirelessly Controlled Robotic Arm (Arduino)

This project demonstrates a **wirelessly controlled robotic arm** using two Arduino UNO boards and Bluetooth modules.  
The controller reads potentiometer values and sends joint angles wirelessly.  
The robotic arm receives these commands and drives servo motors accordingly.

## 🚀 Features
- Wireless control using Bluetooth (HC-05/HC-06)
- Real-time servo motor actuation
- EEPROM initialization for safe startup positions
- Modular and scalable design

## 🛠️ Hardware Required
- Arduino UNO × 2
- Bluetooth Modules (HC-05/HC-06) × 2
- Servo Motors (per joint of the robotic arm)
- Potentiometers (one per joint on controller side)
- Power supply (regulated 5V for servos)
- Robotic arm frame/chassis

## ⚙️ System Overview
1. **Controller Side (Transmitter)**
   - Reads potentiometer values
   - Maps them to servo-compatible angles
   - Sends angles wirelessly via Bluetooth

2. **Robotic Arm Side (Receiver)**
   - Receives angles over Bluetooth
   - Drives servo motors to match received positions

3. **EEPROM Initialization**
   - Ensures the robotic arm always boots into a safe starting position

*(Insert your block diagram or schematic here)*

## 📂 Repository Structure
```
/code
  ├── ARM_TRANSMITTER/   -> Controller side code
  ├── ARM_RECEIVER/      -> Robotic arm side code
  └── EEPROM_SETUP/      -> EEPROM initialization sketch
```

## ▶️ Usage
1. Upload `EEPROM_SETUP/rom_data_set.ino` to the robotic arm Arduino **once** to set safe default positions.
2. Upload `ARM_TRANSMITTER/ARM_TRANSMITTER.ino` to the controller Arduino.
3. Upload `ARM_RECEIVER/arm_final.ino` to the robotic arm Arduino.
4. Pair Bluetooth modules (make sure one is set as Master, the other as Slave).
5. Power the system and move the potentiometers — the robotic arm should follow in real time.

## 📸 Demonstration
*(Insert images or GIF of the working robotic arm here)*

## 🔮 Future Improvements
- Upgrade to Wi-Fi/IoT for internet control
- Add feedback sensors for closed-loop control
- Implement inverse kinematics for advanced movement
- Improve payload capacity with stronger actuators

## 📜 License
This project is licensed under the [MIT License](LICENSE).
