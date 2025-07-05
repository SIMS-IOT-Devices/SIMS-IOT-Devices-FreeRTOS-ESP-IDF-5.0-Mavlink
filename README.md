Interaction with ESP32 in ESP IDF 5.0 environment. <br><br>

1 - Send Heartbeat message in MAVLink 1.0 protocol: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;mavlink_heartbeat.py - The Python file for a PC to send the MAVLink 1.0 Heartbeat message to a specific IP.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Mavlink_on_ESP32.c - C file run on ESP32 <br><br>

2 - Send Heartbeat message in MAVLink 2.0: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;mavlink_2_heartbeat.py - The Python file for a PC to send the MAVLink 2.0 Heartbeat message to a specific IP.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Mavlink_2_on_ESP32.c - C file run on ESP32. Receives the MAVLink 2.0 Heartbeat message on a specific IP<br><br>

3 - Send 5 Heartbeat messages in MAVLink 2.0: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;mavlink_2_heartbeat.py - The Python file runs on a PC and sends 5 consecutive MAVLink 2.0 Heartbeat messages.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Mavlink_2_on_ESP32.c - C file run on ESP32. Receives the MAVLink 2.0 Heartbeat message on a specific IP<br><br>
