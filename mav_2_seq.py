# Sending heartbeats in MAVLink 2.0 format 
# with Sequence Number update and CRC calculation

import os
from pymavlink import mavutil
import time

# Set the MAVLINK20 environment variable to '1' to force MAVLink 2.0 encoding
os.environ['MAVLINK20'] = '1'

master = mavutil.mavlink_connection(
    'tcp:192.168.1.153:14550',
    baud=0,                     # 0 - Not used for UDP
    source_system=255,          # MAV ID of this GCS/program - FF
    source_component=255,       # Component ID - FF
    dialect='common',           # MAVLink dialect
    autoreconnect=True,
    mavlink2=True               # Enforce MAVLink 2.0
)

# Send a simple HEARTBEAT message
for i in range(5): 
    master.mav.heartbeat_send(
        type=mavutil.mavlink.MAV_TYPE_GCS,        # Specify the component type (e.g., GCS, drone, etc.)
        autopilot=mavutil.mavlink.MAV_AUTOPILOT_GENERIC,
        base_mode=0,
        custom_mode=0,
        system_status=mavutil.mavlink.MAV_STATE_ACTIVE)
    print(f"The Heartbeat #{i} was sent!")
    time.sleep(1)
