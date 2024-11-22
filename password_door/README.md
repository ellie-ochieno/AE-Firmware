

# SMART DOOR CONTROL SYSTEM

This project implements a secure and automated smart door system using an ESP32 microcontroller, a servo motor, an OLED display, a keypad, and a buzzer. The system connects to WiFi and communicates with a remote server for password verification and door status updates.
Features

1. Keypad-Based Password Input:
  - A 4x4 keypad allows the user to input passwords.
  - Supports clearing (*) and submitting (#) the password.

2. WiFi Connectivity:
  - The ESP32 connects to a WiFi network to fetch the password and door status from a server.

3. OLED Display:
  - Provides visual feedback for password validation.
  - Displays messages such as "Smart Home," "Password Error," and "Password Correct."

4. Buzzer Alerts:
  - Plays different tones for correct and incorrect passwords.

5. Secure HTTPS Communication:
  - Fetches the set password and door status securely using HTTPS.

6. Servo Motor Control:
  - Operates a door lock based on password validation and server commands.

7. Error Handling:
  - Provides error messages and retries in case of server connection failures.

## Libraries Used

1. Keypad- For handling keypad inputs.
2. Adafruit_GFX & SSD1306- For OLED display control.
3. WiFi & WiFiClientSecure- For WiFi connectivity and HTTPS communication.
4. ESP32_New_ISR_Servo- For precise servo motor control.
5. Tone32- For buzzer sound generation.

## Workflow

1. Initialization:
  - Connect to WiFi.
  - Display "Smart Home" on the OLED.
  - Initialize the servo and set it to the default position.

2. Password Entry:
  - Users input the password via the keypad.
  - Submit the password using #.

3. Validation:
  - Compare the input password with the server's password.
  - If correct:
    - Open/close the door based on the current state.
  - If incorrect:
    - Display an error message and prompt for re-entry.

4. Door Control:
  - Fetch the door status and password from the server.
  - Move the servo to open or close the door.