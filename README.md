Interaction with ESP32 in ESP IDF 5.0 environment. <br><br>

1 - Send Heartbeat message in MAVLink 1.0 protocol: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1 - MAVLink_1_Heartbeat.py - The Python file for a PC to send the MAVLink 1.0 Heartbeat message to a specific IP.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1 - MAVLink_1_ESP32.c - The C file runs on ESP32 and receives the MAVLink 1.0 Heartbeat message on a specific IP<br><br>

2 - Send Heartbeat message in MAVLink 2.0: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2 - MAVLink_2_Heartbeat.py - The Python file for a PC to send the MAVLink 2.0 Heartbeat message to a specific IP.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2 - MAVLink_2_ESP32.c - The C file runs on ESP32 and receives the MAVLink 2.0 Heartbeat message on a specific IP<br><br>

3 - Send 5 Heartbeat messages in MAVLink 2.0: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3 - MAVLink_2_Heartbeat_5_messages.py - The Python file runs on a PC and sends 5 consecutive MAVLink 2.0 Heartbeat messages.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3 - MAVLink_2_ESP32_5_messages.c - The C file runs on ESP32 and receives 5 MAVLink 2.0 Heartbeat messages.<br><br>
