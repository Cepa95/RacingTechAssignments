#include <SPI.h>
#include <mcp_can.h>

const byte SPI_CS_PIN = 10;  // CS pin za MCP2515

MCP_CAN CAN(SPI_CS_PIN);    // Inicijalizacija objekta MCP_CAN

void setup() {

  Serial.begin(115200);

//MCP_ANY detektira razlicite brzine dok ne nadje odgovarajucu
// => MCP_NORMAL: Sets the mode to normal operation.
//CAN_500KBPS brzina prijenosa podataka
//MCP_16MHZ oscillator frequency
  while (CAN_OK != CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ)){ // Inicijalizacija CAN bus-a
        Serial.println("denied");
        delay(100);
    }
    Serial.println("success!");
}

void loop() {
// Kreiranje CAN poruke
 const byte MESSAGE_SIZE = 8;
 unsigned char message[MESSAGE_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7};

  unsigned int canId = 0xFF; 

  Serial.println(CAN.sendMsgBuf(canId, 0, MESSAGE_SIZE , message) == CAN_OK ? "sent" : "error sending");

  delay(500);  
}
