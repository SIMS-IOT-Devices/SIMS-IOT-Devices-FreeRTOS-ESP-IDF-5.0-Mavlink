Interaction with ESP32 in ESP IDF 5.0 environment. <br><br>

1 - Send Heartbeat message in MAVLink 1.0 protocol: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1 - MAVLink_1_Heartbeat.py - The Python file for a PC to send the MAVLink 1.0 Heartbeat message to a specific IP<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1 - MAVLink_1_ESP32.c - The C file runs on ESP32 and receives the MAVLink 1.0 Heartbeat message on a specific IP<br><br>

2 - Send Heartbeat message in MAVLink 2.0: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2 - MAVLink_2_Heartbeat.py - The Python file for a PC to send the MAVLink 2.0 Heartbeat message to a specific IP<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2 - MAVLink_2_ESP32.c - The C file runs on ESP32 and receives the MAVLink 2.0 Heartbeat message on a specific IP<br><br>

3 - Send 5 Heartbeat messages in MAVLink 2.0: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3 - MAVLink_2_Heartbeat_5_messages.py - Sends 5 consecutive MAVLink 2.0 Heartbeat messages from the PC<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3 - MAVLink_2_ESP32_5_messages.c - The C file runs on ESP32 and receives 5 MAVLink 2.0 Heartbeat messages<br><br>

4 - Send Heartbeat message in MAVLink 2.0 from ESP32 to ESP32: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4 - MAVLink_2_Heartbeat.py - The Python file for a PC to send the MAVLink 2.0 Heartbeat message to a specific IP<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4 - MAVLink_2_ESP32_Server.c - ESP32 file receives the MAVLink 2.0 Heartbeat message on a specific IP<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4 - MAVLink_2_ESP32_Client.c - ESP32 file sends the MAVLink 2.0 Heartbeat message to a specific IP<br><br>

5 - Send 5 Heartbeat messages in MAVLink 2.0 from ESP32 to ESP32: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5 - MAVLink_2_Heartbeat.py - The Python file for a PC to send 5 MAVLink 2.0 Heartbeat messages to a specific IP<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5 - MAVLink_2_ESP32_Server.c - ESP32 file receives 5 MAVLink 2.0 Heartbeat messages on a specific IP<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5 - MAVLink_2_ESP32_Client.c - ESP32 file sends 5 MAVLink 2.0 Heartbeat messages to a specific IP<br><br>

6 - Connect a PC running Python to a PC running Mission Planner: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;6 - Mission_Planner_Connection.py - Establish a connection from the PC running this Python code to <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;another Windows PC running Mission Planner with a known IP and Port.<br><br>

7 - Connect a ESP32 running ESP IDF 5.3 to a PC running Mission Planner: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;7 - Python_Mission_Planner_Connection.py - Reference code to establish a connection from the PC running this Python code to <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;another Windows PC running Mission Planner with a known IP and Port.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;7 - ESP_IDF_Mission_Planner_Connection.py - Establish a connection from the ESP32 running this code to <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;another Windows PC running Mission Planner with a known IP and Port.<br><br>
