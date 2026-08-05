# 🔋🚗 c2d-ev-learning-kit
An interactive IoT EV educational platform built with ESP32 and web telemetry for SKEE3733 Integrated Design Project at University Teknologi Malaysia. The system uses sequential LED paths and a pedal simulator to demonstrate charging and driving dynamics.

## 🛠️ Features

Here's what you can do with this hands-on EV educational kit:

* **Interactive Touchscreen Quiz:** Work through guided multiple-choice questions on a 3.2" SPI TFT display (ILI9341 Driver + XPT2046 Touch) to master EV concepts.
* **Sequential LED Power Flow:** Watch WS2812B addressable LEDs light up step-by-step across 3D-printed infrastructure models as you answer correctly, using color conventions (Red for AC, Green for DC) to trace energy paths.
* **Single-Pedal & Regen Simulation:** Use a custom 3D-printed spring-return pedal with a rack-and-pinion potentiometer mechanism to control a DC motor and observe real-time regenerative braking LED feedback.
* **Multi-Module Coverage:** Explore three distinct EV energy pathways: Home AC Charging, DC Fast Charging (with Solar/BESS integration), and Driving & Regenerative Energy Recovery.
* **Live IoT Dashboard:** Experience real-time synchronization over Wi-Fi/MQTT (HiveMQ Cloud TLS) to a MySQL database and web platform that tracks speed, node states, and quiz performance.
* **Multi-Sensory Feedback:** Receive instant audio feedback (ascending/descending tones) via a passive buzzer alongside screen highlights for correct and incorrect answers.

## 🕹️ How to Play & Use the Kit

1. **Power On & Link Kit:** Connect the battery pack and switch the power ON. Note the kit's MAC address displayed on the welcome screen, open the web portal, select your kit from the active list, and click "Link" until the status dot turns green.
2. **Select a Module:** Tap the TFT touchscreen to choose between **Module 1 (Home AC Charging)**, **Module 2 (DC Fast Charging)**, or **Module 3 (Driving & Regenerative Braking)**.
3. **Review Content:** View the module preview and notes on the website to understand the underlying electrical concepts before attempting the quiz.
4. **Answer Quiz Questions:** Tap choice A, B, or C on the touchscreen:
   * *Correct Answer:* The screen flashes green, a success chime plays, and the next LED segment illuminates to extend the power flow path.
   * *Incorrect Answer:* The screen prompts you to try again without locking the system.
   * *Skip Step:* Press the `SKIP >>` button in the corner if you already understand a step to fast-forward the LED path.
5. **Drive & Regenerate (Module 3):** Press the physical pedal to accelerate the DC motor (observing energy flowing from Battery $\rightarrow$ Inverter $\rightarrow$ Motor). Release the pedal to simulate regenerative braking, where kinetic energy reverses back into the battery.
6. **Complete & Review:** Complete all questions to trigger the victory fanfare and light up the full energy path. Visit your student dashboard on the web platform to review your final score, percentage, and detailed question breakdown.

## 📅 The Process

Our team began by identifying the core learning challenges students face when trying to visualize hidden EV energy flows. We designed a Google Forms survey and clustered feedback from over 50 respondents to define our persona and design requirements. 

Once the conceptual architecture was established, we split development into parallel hardware and software tasks:
* **Hardware & Circuitry:** We breadboarded the ESP32, TFT touchscreen, LED strips, motor driver, and potentiometer. After verifying component compatibility, we designed a custom PCB in EasyEDA to eliminate jumper wires and stepped down battery voltage using a 5V buck converter to safely power logic circuits.
* **Mechanical Design:** We modeled scenery components (transmission towers, substations, chargers, BESS) and a custom pedal assembly in Fusion 360/SolidWorks, printing them on a 3D printer.
* **Firmware & State Machine:** We wrote non-blocking C++ firmware in Arduino IDE using a Finite State Machine (FSM) so touchscreen interactions, LED animations, buzzer tones, and MQTT telemetry could operate concurrently without freezing.
* **Web Platform & IoT Middleware:** We built a PHP/MySQL web platform paired with a background daemon (`mqtt-bridge.php`) that listens to HiveMQ Cloud MQTT topics and updates the database in real time.

During testing, we encountered and resolved several engineering challenges, such as routing potentiometer signals from ADC2 pins (which conflict with Wi-Fi) to independent ADC1 pins (GPIO 34), implementing dead-zone filtering to eliminate motor whine at idle, and rewriting startup audio sequences to run non-blocking.

## 📈 Overall Growth

Leading the touchscreen UI design and quiz logic for this Integrated Design Project gave me invaluable technical confidence and practical engineering experience:

* **Embedded GUI Programming:** Mastered low-level graphics rendering and touch-coordinate mapping using `TFT_eSPI` and C++ on the ESP32.
* **Non-Blocking Architecture:** Learned how to write event-driven code and finite state machines (FSM) to handle touch interrupts, screen blinking, passive buzzer melodies, and MQTT data publishing concurrently.
* **User Experience (UX) Design for Micro-Screens:** Developed an eye for color-contrast, font hierarchy, and touch-target padding within a compact $320 \times 240$ pixel resolution.
* **Technical Problem Solving:** Successfully debugged resistive touch unresponsiveness and touch-bus crosstalk through methodical calibration and pin isolation.
* **Educational Content Curation:** Gained skill in simplifying complex technical topics (like OBC AC-to-DC rectification and BESS solar integration) into clear, concise multiple-choice quiz steps.

## 💭 How Can It Be Improved?

* **Cloud Migration:** Transition the website server from local host (Laragon) to a public cloud platform (e.g., AWS or Render) for global remote classroom access.
* **Wi-Fi Auto-Provisioning:** Implement `WiFiManager` auto-portal configuration to allow Wi-Fi connection updates without reflashing ESP32 firmware.
* **Real Sensor Telemetry:** Integrate physical current/voltage sensors (e.g., INA219) and temperature sensors to report actual live power readings alongside simulated values.
* **Expanded EV Architectures:** Add modular hardware plug-ins for Hybrid Electric Vehicles (HEVs), Fuel Cell EVs (FCEVs), and Vehicle-to-Grid (V2G) bidirectional charging.
* **Multiplayer Learning Mode:** Introduce head-to-head competitive quiz modes between multiple physical kits in a classroom setting.

## ▶️ Running the Project

### Prerequisites
* **Hardware:** ESP32 Dev Kit, 3.2" SPI TFT Display (ILI9341), WS2812B LED strip, L298N Motor Driver, DC Motor, 10k Potentiometer, Passive Buzzer, Buck Converter, and 7.4V Battery Pack.
* **Software:** Arduino IDE (for firmware), Laragon/XAMPP (Apache + PHP + MySQL for web portal), and a HiveMQ Cloud MQTT account.

Below is the complete PDF guidance for setting up, configuring, and operating the system:

[C2D EV Kit Guideline.pdf](https://github.com/user-attachments/files/30724692/C2D.EV.Kit.Guideline.pdf)

## 📝 Project Documentation & Reports

All weekly progress logs, individual contributions, and developmental reports are available below:

1. [Discussion Note (W1-W12).pdf](https://github.com/user-attachments/files/30724794/Discussion.Note.W1-W12.pdf)

2. [Documentation.pdf](https://github.com/user-attachments/files/30724847/Documentation.pdf)

3. [Individual Report 1.pdf](https://github.com/user-attachments/files/30724804/Individual.Report.1.pdf)

4. [Individual Report 2.pdf](https://github.com/user-attachments/files/30724817/Individual.Report.2.pdf)

5. [Individual Report 3.pdf](https://github.com/user-attachments/files/30724830/Individual.Report.3.pdf)

## 🎨 EES 2026 Showcase Materials

Presentation materials prepared for the Electrical Engineering Showcase (EES 2026) at Universiti Teknologi Malaysia (UTM)

[P1G13 Abstract EES2026.pdf](https://github.com/user-attachments/files/30724834/P1G13.Abstract.EES2026.pdf)

EES 2026 Presentation Poster:

<img width="2245" height="3179" alt="Image" src="https://github.com/user-attachments/assets/a6db46ea-d694-4cb2-8d51-b05373c1becb" />

## 📖 Final Technical Report & Verification

[Group P1G13 Technical Report (W14).pdf](https://github.com/user-attachments/files/30724845/Group.P1G13.Technical.Report.W14.pdf)
