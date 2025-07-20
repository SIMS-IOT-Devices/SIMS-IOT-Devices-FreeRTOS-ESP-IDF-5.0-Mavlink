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

sys_status = mav.sys_status_encode(
        onboard_control_sensors_present=0,
        onboard_control_sensors_enabled=0,
        onboard_control_sensors_health=0,
        load=500,               # 0.5% CPU load
        voltage_battery=12000,  # in millivolts (12V)
        current_battery=3000,   # in 10mA (3A)
        battery_remaining=80,   # percent
        drop_rate_comm=0,
        errors_comm=0,
        errors_count1=0,
        errors_count2=0,
        errors_count3=0,
        errors_count4=0
    )

# Parameter list
param_list = [
    ("PARAM_1", 10.0, mavlink2.MAV_PARAM_TYPE_REAL32),
    ("PARAM_2", 42, mavlink2.MAV_PARAM_TYPE_INT32),
    ("ENABLE_FEATURE", 1, mavlink2.MAV_PARAM_TYPE_UINT8),
]

for i in range(50):
    msg_bytes = heartbeat.pack(mav) # Encode Heartbeat MAVLink message into bytes
    sock.sendto(msg_bytes, (MP_IP, MP_PORT)) # Send MAVLink packet via UDP
    print(f"Sent heartbeat {i}")
    time.sleep(1)

    sock.settimeout(0.1) # 0.1 second Set socket read timeout (non-blocking)
    # Waits to receive a UDP packet from the network recvfrom(1024) reads up to 1024 bytes
    # data - the binary payload; addr - tuple of (sender_ip, sender_port)
    data, addr = sock.recvfrom(1024) # Read incoming messages from Mission Planner
    msg = mav.parse_char(data)

    print(f"Received {len(data)} bytes Raw Data (hex):", data.hex())
    print(msg)
    
    if msg.get_type() == 'REQUEST_DATA_STREAM':
        print("Sending DATA_STREAM")
        sock.sendto(sys_status.pack(mav), (MP_IP, MP_PORT))

    if msg.get_type() == 'PARAM_REQUEST_LIST':
        print("Sending Parameters request")
        # Respond with PARAM_VALUE for each parameter
        for ii, (name, value, param_type) in enumerate(param_list):
            param_value_msg = mav.param_value_encode(
                param_id=name.encode('ascii'),  # Param name (up to 16 bytes)
                param_value=float(value),       # Always a float32 representation
                param_type=param_type,
                param_count=len(param_list),
                param_index=ii
            )
            sock.sendto(param_value_msg.pack(mav), addr)
            print(f"Sent PARAM_VALUE: {name} = {value}")
            time.sleep(0.1)  # Prevent flooding Mission Planner
    
    print("-------------------------------------------------------------------------------------------------------------------------------------------------------")

sock.close()
