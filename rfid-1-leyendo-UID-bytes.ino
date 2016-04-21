#include <SPI.h> // Serial Perispherical Interface  
#include <MFRC522.h> // lector/escritor RC522

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // MFRC522 instancia.

void setup() {
    Serial.begin(9600); // Comunicación serial 
    SPI.begin();            // comunicación SPI
    mfrc522.PCD_Init(); // inicia MFRC522
    Serial.println(""UID 4 bytes..."");
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
    
   for(int i=0;i<4;i++){
Serial.println(mfrc522.uid.uidByte[i]); //muestra información de los primeros 4 bytes (registros) del UID
      }
mfrc522.PICC_HaltA(); // se detiene la lectura
      }
    
