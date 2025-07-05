from pymavlink import mavutil
import time

# Establish a connection (usually to PX4 SITL or another MAVLink compatible component)
master = mavutil.mavlink_connection('tcp:192.168.1.155:14550')

# Send a simple HEARTBEAT message
master.mav.heartbeat_send(
    type=mavutil.mavlink.MAV_TYPE_GCS,        # Specify the component type (e.g., GCS, drone, etc.)
    autopilot=mavutil.mavlink.MAV_AUTOPILOT_GENERIC,
    base_mode=0,
    custom_mode=0,
    system_status=mavutil.mavlink.MAV_STATE_ACTIVE
)

print("Heartbeat sent!")

# Wait for a heartbeat message to confirm connection
print("Waiting for heartbeat...")
master.wait_heartbeat()
print("Heartbeat received!")

# Now we can start reading messages
while True:
    # Read the next MAVLink message
    msg = master.recv_match(blocking=True)  # blocking=True waits for a message
    
    time.sleep(2)
        
    if msg:
        # Print the message type and content
        print(f"Received message: {msg.get_type()}")
        print(msg)
        
        # Example: If it's a heartbeat message, print specific fields
        if msg.get_type() == "HEARTBEAT":
            print(f"System Status: {msg.system_status}")
            print(f"MAVLink Version: {msg.mavlink_version}")
            print(f"Base Mode: {msg.base_mode}")
            print(f"Custom Mode: {msg.custom_mode}")

            
