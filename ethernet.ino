#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// UDP instance
EthernetUDP Udp;

// Network configuration
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC address
IPAddress ip(192, 168, 1, 177);                      // Arduino's IP address
unsigned int localPort = 8888;                       // Local port for UDP
IPAddress remoteIp(192, 168, 1, 2);                  // Replace with your computer's IP
unsigned int remotePort = 8888;                      // Port on your computer

void ethernet_setup() {
  // Start Ethernet and UDP
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
}

// Send a message via UDP
void ethernet_send(String message) {
  // Check the network status before sending
  if (Ethernet.linkStatus()==LinkON) {
    Udp.beginPacket(remoteIp, remotePort);
    Udp.print(message);
    Udp.endPacket();
  }
}
