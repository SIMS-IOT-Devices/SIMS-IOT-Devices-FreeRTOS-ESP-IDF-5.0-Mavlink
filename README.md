Interaction with ESP32 in ESP IDF 5.0 environment via Mavlink 1.0 protocol: <br>
mavlink_heartbeat.py - Python file runned on PC <br>
Mavlink_on_ESP32.c - C file runned on ESP32 <br><br>

Send Heartbeat message in MAVLink 2.0: <br>
mavlink_2_heartbeat.py - Python file runned on PC. Sends the Heartbeat message to spesific IP <br>
Mavlink_2_on_ESP32.c - C file runned on ESP32. Recives the Heartbeat message to spesific IP by opening a socket on network IP <br>
