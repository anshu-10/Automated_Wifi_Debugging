from machine import UART, Pin
from time import sleep

# --- PIN DEFINITIONS ---
uart_pico = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))

# Relay Control (GPIO 15)
RELAY_PIN = Pin(15, Pin.OUT) 
RELAY_PIN.value(1)           # Default to OFF (Router ON via NC)

# Status LEDs (220 Ohm Resistor Mandatory)
LED_GREEN = Pin(13, Pin.OUT) # Network OK LED
LED_RED = Pin(14, Pin.OUT)   # Rebooting LED


# --- CONTROL VARIABLES ---
REBOOT_DURATION = 10         
REBOOT_COUNT = 0             

# --- LED STATUS CONTROL ---
def set_status_leds(green, red):
    LED_GREEN.value(green)
    LED_RED.value(red)

# --- SIMPLE REBOOT FUNCTION (Core Logic) ---
def simple_reboot():
    # 1. Turn Router Power OFF (BREAK the NC link)
    RELAY_PIN.value(0) # Signal 0 = Coil ON = BREAKS NC connection
    
    # 2. Status Update (Red LED ON)
    set_status_leds(0, 1) # Red ON

    print(f"Router Power OFF for {REBOOT_DURATION} seconds...")
    sleep(REBOOT_DURATION)

    # 3. Turn Router Power ON (Restore the NC link)
    RELAY_PIN.value(1) # Signal 1 = Coil OFF = RESTORES NC connection
    
    print("Reboot successful. Monitoring resumed.")
    set_status_leds(1, 0) # Green ON

    return False # No fault detected in simplified mode


# --- MAIN LOOP LOGIC ---
def loop():
    global REBOOT_COUNT

    set_status_leds(1, 0) # Green ON (Network OK Status)
    
    # Check for command from ESP32
    if uart_pico.any():
        data = uart_pico.read()
        if data:
            command = data.decode('utf-8').strip()
            
            if 'R' in command:
                print("\n--- COMMAND RECEIVED: REBOOT (R) ---")
                
                # 1. Send ACK back to ESP32 immediately
                uart_pico.write(b"A\n")
                
                # 2. Run Simple Reboot
                simple_reboot()
                
    sleep(0.01)

# --- SETUP AND RUN ---
print("--- RPi Pico Control System READY ---")
set_status_leds(1, 0) # Initial Green LED Status
while True:
    loop()
