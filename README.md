# AE-Firmware
SMART FARM project firmware
  - Language: Micropython
  - Microcontroller: Raspberry pi pico
  - Protocols: MQTT&Lora
  - /023

# Support Files 
----------------
1) secrets.py
  - All connection tokens for WiFi and MQTT broker are reserved here
2) topics.py
  - Contain all client(s) connection topics as defined in mqtt broker
3) mod_connect.py
  - This contains re-usable WiFi&MQTT broker connection handlers
  - It's imported in all sensors files that requires MQTT broker connectivity
