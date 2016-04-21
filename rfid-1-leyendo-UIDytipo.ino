#include <SPI.h> // Serial Perispherical Interface  
#include <MFRC522.h> // lector/escritor RC522

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // MFRC522 instancia.

void setup() {
    Serial.begin(9600); // Comunicación serial 
    SPI.begin();            // comunicación SPI
    mfrc522.PCD_Init(); // inicia MFRC522
    Serial.println("Escaniando: UID y tipo...");
}

void loop() {
    // busca nuevas tarjetas
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Selecciona una 
    if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    // busca la información de la tarjeta y termina el llamado.
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
      }
    
