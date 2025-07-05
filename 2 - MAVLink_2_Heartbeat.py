# Sending heartbeats in MAVLink 2.0 format 
# Start-of-frame: fe V1
# Start-of-frame: fd V2

import os
from pymavlink import mavutil

# Set the MAVLINK20 environment variable to '1' to force MAVLink 2.0 encoding
os.environ['MAVLINK20'] = '1'

master = mavutil.mavlink_connection(
    'tcp:192.168.1.154:14550',
    baud=0,                     # 0 - Not used for UDP
    source_system=255,          # MAV ID of this GCS/program - ff
    source_component=255,       # Component ID               - ff
    dialect='common',           # MAVLink dialect
    autoreconnect=True,
    mavlink2=True               # Enforce MAVLink 2.0
)

# Send a simple HEARTBEAT message
master.mav.heartbeat_send(
    type=mavutil.mavlink.MAV_TYPE_GCS,        # Specify the component type (e.g., GCS, drone, etc.)
    autopilot=mavutil.mavlink.MAV_AUTOPILOT_GENERIC,
    base_mode=0,
    custom_mode=0,
    system_status=mavutil.mavlink.MAV_STATE_ACTIVE
)

print("Heartbeat sent!")
