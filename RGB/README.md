# RGB LED Control with ESP32

This project demonstrates how to control an RGB LED using an ESP32. By utilizing the ESP32's PWM capabilities, the RGB LED cycles through various colors such as red, green, blue, yellow, cyan, magenta, and deep pink.

---

## Features

- Control RGB LED colors using PWM (Pulse Width Modulation).
- Cycle through multiple predefined colors with smooth transitions.
- Adjust brightness and color intensities using PWM.

---

## Pin Configuration

1. **RGB LED Pin Definitions:**
   - `RED_PIN`: Connect to the red pin of the RGB LED.
   - `GREEN_PIN`: Connect to the green pin of the RGB LED.
   - `BLUE_PIN`: Connect to the blue pin of the RGB LED.

2. Define the pins in `pin_configurations.h` as follows:

   ```cpp
   #define RED_PIN  25 // Replace with your pin number
   #define GREEN_PIN 26 // Replace with your pin number
   #define BLUE_PIN  27 // Replace with your pin number

## How It Works

1. Setup:

  - Pins for red, green, and blue are attached to PWM channels.
  - PWM frequency and resolution are configured for each channel.

2. Color Control:

  - The RGB_Color() function adjusts the PWM duty cycle for each channel to create various colors.
  - Colors are specified as (Red, Green, Blue) intensity values ranging from 0 to 255.

3. Color Cycling:

  - The loop() function cycles through predefined colors with a 500ms delay between transitions.

## Example Colors

| Color             | RGB Code                                                                | 
| ----------------- |  |------------------------------------------------------------------ |
| Red | (255,0,0)| 
| Green |(0,255,0) | 
| Blue | (0,0,255)| 
| Yellow |(255,255,0) | 
| Magenta |(255,0,255) | 
| Deep Pink |(255,20,147) | 