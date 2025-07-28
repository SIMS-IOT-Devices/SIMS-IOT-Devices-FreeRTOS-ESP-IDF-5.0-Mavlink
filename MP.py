# Establish a connection from the PC running this Python code to 
# another Windows PC running Mission Planner with a known IP and Port.

import socket
import time
from pymavlink.dialects.v20 import common as mavlink2

# IP and port for Mission Planner UDP
MP_IP = '192.168.1.34'  # Replace with actual IP
MP_PORT = 14550

# Create UDP socket once and reuse
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Create MAVLink instance
mav = mavlink2.MAVLink(sock.makefile('wb'))
mav.srcSystem = 1      # System ID (e.g., drone ID)
mav.srcComponent = 1   # Component ID (e.g., autopilot)

heartbeat = mav.heartbeat_encode(
        type=mavlink2.MAV_TYPE_QUADROTOR,
        autopilot=mavlink2.MAV_AUTOPILOT_GENERIC,
        base_mode=0,
        custom_mode=0,
        system_status=mavlink2.MAV_STATE_ACTIVE
    )

parm_msg = mav.param_value_encode(
        param_id="PARAM_1".encode('ascii'),
        param_value=1.0,
        param_type=mavlink2.MAV_PARAM_TYPE_REAL32,
        param_count=0,
        param_index=0
    )

for i in range(10):
    # Send messages to Mission Planner:
    msg_bytes = heartbeat.pack(mav) # Encode Heartbeat MAVLink message into bytes
    sock.sendto(msg_bytes, (MP_IP, MP_PORT)) # Send MAVLink packet via UDP
    sock.sendto(parm_msg.pack(mav), (MP_IP, MP_PORT))
    print(f"Sent heartbeat and parameters {i}")
    time.sleep(1)

    # Receive incoming messages from Mission Planner:
    data, addr = sock.recvfrom(1024)
    msg = mav.parse_char(data)
    print(f"Received {len(data)} bytes Raw Data (hex):", data.hex())
    print(msg)
    
    print("-------------------------------------------------------------------------------------------------------------------------------------------------------")

sock.close()
