# Proof of concept to monitor warehouse capacity

- frontend: svelte
- backend: Go/Gin
- microcontrollers: ESP32
- sensors: HC-SR04

ESP32 communicates distance with mosquite MQTT broker on the same network.

Go app prints any differences greater than 2 cm
