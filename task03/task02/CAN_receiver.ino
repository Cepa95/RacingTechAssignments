#include <SPI.h>
#include "mcp_can.h"

const byte SPI_CS_PIN = 10; 

MCP_CAN CAN(SPI_CS_PIN );

void setup() {
    Serial.begin(115200);
 
    while (CAN_OK != CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ)) {
        Serial.println("denied");
        delay(100);
    }
    Serial.println("success!");
}

void loop() {
    const byte len = 0;
    unsigned char buf[8];
    unsigned long canId;

    if (CAN_MSGAVAIL == CAN.checkReceive()) {
        CAN.readMsgBuf(&canId, &len, buf);

        Serial.println("-----------------------------");
        Serial.print("Data from ID: 0x");
        Serial.println(canId, HEX);

        for (int i = 0; i < len; i++) {
            Serial.print(buf[i]);
            Serial.print("\t");
            delay(500);
        }
        Serial.println();
    }
}
