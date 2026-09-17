# 🏎️ ESP32 Line Follower Robot with PID & Dead-End Recovery

An autonomous 4-sensor line follower robot powered by ESP32/Arduino, featuring a hybrid PD control algorithm and logic for maze handling (dead-ends and junction priority).

---

## 📌 Features

* **Control Algorithm:** PD (Proportional-Derivative) line tracking ($K_p = 20$, $K_d = 7$).
* **Sensors:** 4-Array IR digital sensor suite for precise line position estimation.
* **Dead-End Detection:** Automatic 180° rotation when the line is lost for $>300\text{ ms}$.
* **Junction Handling:** Priority-based pivot turns for sharp corners and intersections.
* **Speed Management:** Dynamic speed adaptation based on line curvature.

---

## 🔌 Hardware Pin Mapping

### IR Sensor Array
| Sensor | Pin (ESP32) | Function |
| :--- | :--- | :--- |
| **IR_L2** | `35` | Far Left Sensor |
| **IR_L1** | `34` | Inner Left Sensor |
| **IR_R1** | `27` | Inner Right Sensor |
| **IR_R2** | `14` | Far Right Sensor |

### Motor Driver (H-Bridge)
| Motor Channel | Pin (ESP32) | Control Signal |
| :--- | :--- | :--- |
| **IN1** | `12` | Left Motor Forward |
| **IN2** | `26` | Left Motor Reverse |
| **IN3** | `25` | Right Motor Forward |
| **IN4** | `33` | Right Motor Reverse |

---

## ⚙️ PID Parameters

```cpp
float Kp = 20.0;
float Ki = 0.0;
float Kd = 7.0;
