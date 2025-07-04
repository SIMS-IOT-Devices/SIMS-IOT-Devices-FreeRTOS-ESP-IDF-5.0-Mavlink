Interaction with ESP32 in ESP IDF 5.0 environment. <br><br>

Send Heartbeat message in MAVLink 1.0 protocol: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;mavlink_heartbeat.py - Python file run on PC <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Mavlink_on_ESP32.c - C file run on ESP32 <br><br>

Send Heartbeat message in MAVLink 2.0: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;mavlink_2_heartbeat.py - Python file run on PC. Sends the MAVLink 2.0 Heartbeat message to a specific IP <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Mavlink_2_on_ESP32.c - C file run on ESP32. Receives the MAVLink 2.0 Heartbeat message on a specific IP by opening a socket<br>
