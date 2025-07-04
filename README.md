Interaction with ESP32 in ESP IDF 5.0 environment via MAVLink 1.0 protocol: <br>
mavlink_heartbeat.py - Python file run on PC <br>
Mavlink_on_ESP32.c - C file run on ESP32 <br><br>

Send Heartbeat message in MAVLink 2.0: <br>
mavlink_2_heartbeat.py - Python file run on PC. Sends the Heartbeat message to a specific IP <br>
Mavlink_2_on_ESP32.c - C file run on ESP32. Receives the Heartbeat message to a specific IP by opening a socket on the network IP <br>
