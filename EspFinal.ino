#include <WiFi.h>
#include <ESPping.h> 
#include <HardwareSerial.h>

// --- WiFi Configuration ---
const char* WIFI_SSID = "TP-Link_9D88"; 
const char* WIFI_PASS = "SUM_1212@@"; 

// --- PING TARGETS (Fail-Safe Mechanism) ---
const IPAddress PING_TARGET_1(192, 168, 1, 1);   // Router Gateway IP (Local Test)
const IPAddress PING_TARGET_2(8, 8, 8, 8); // Google DNS
const IPAddress PING_TARGET_3(1, 1, 1, 1);   // Cloudflare DNS

// --- UART Configuration ---
#define TXD2 17 // ESP32 TX to Pico RX
#define RXD2 16 // ESP32 RX from Pico TX (for ACK)
HardwareSerial SerialPico(2);

// --- CONTROL VARIABLES ---
int reboot_fail_count = 0;
const int MAX_FAILS = 3; 
const int ACK_TIMEOUT = 500; // 500ms wait for ACK from Pico

void setup() {
  Serial.begin(115200); 
  SerialPico.begin(9600, SERIAL_8N1, RXD2, TXD2); 
  
  // 1. WiFi Connection
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. System Ready.");
  Serial.print("ESP32 Local IP: ");
  Serial.println(WiFi.localIP());
}

// Function to send command and check for ACK
void sendCommand(char command) {
    SerialPico.print(command);
    Serial.print("Command Sent: ");
    Serial.println(command);

    // Wait for ACK response from Pico
    unsigned long startTime = millis();
    while (millis() - startTime < ACK_TIMEOUT) {
        if (SerialPico.available()) {
            char response = SerialPico.read();
            if (response == 'A') {
                Serial.println("ACK received: Pico alive, rebooting router!");
                return; // Exit function upon success
            }
        }
    }
    // If timeout occurs without receiving 'A'
    Serial.println("!!! ERROR: ACK Timeout. Pico Fault Detected. !!!");
}

bool runPingTest() {
    bool fail_1 = !Ping.ping(PING_TARGET_1, 1);
    bool fail_2 = !Ping.ping(PING_TARGET_2, 1);
    bool fail_3 = !Ping.ping(PING_TARGET_3, 1);

    // Only if all three targets fail, we consider the network DOWN.
    return (fail_1 && fail_2 && fail_3);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected. Reconnecting...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    delay(5000);
    return;
  }
  
  bool all_targets_failed = runPingTest();

  if (all_targets_failed) {
    reboot_fail_count++;
    Serial.print("All Pings FAILED. Count: ");
    Serial.println(reboot_fail_count);
  } else {
    reboot_fail_count = 0; 
    Serial.println("Ping SUCCESS. Network OK.");
  }

  // --- REBOOT LOGIC ---
  if (reboot_fail_count >= MAX_FAILS) {
    Serial.println("--- CRITICAL FAILURE DETECTED ---");
    sendCommand('R'); // Send Reboot command
    reboot_fail_count = 0; // Reset counter
  }

  delay(10000); // Check every 10 seconds
}
