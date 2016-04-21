#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup() {
    Serial.begin(9600); // Initialize serial communications with the PC
    SPI.begin();            // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card
    Serial.println("Scan PICC to see UID and type...");
}

void loop() {
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
    }
    
   for(int i=0;i<4;i++){
Serial.println(mfrc522.uid.uidByte[i]);
      }
      delay(1000);
      }
    
