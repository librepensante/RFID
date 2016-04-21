
#include <SPI.h> // Serial Perispherical Interface  
#include <MFRC522.h> // lector/escritor RC522

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // MFRC522 instancia.

void setup() {
   Serial.begin(9600); // Comunicación serial 
    SPI.begin();            // comunicación SPI
    mfrc522.PCD_Init(); // inicia MFRC522
        Serial.println(F("Escribiendo datos MIFARE PICC "));
}

void loop() {
        
 // Preparando key (llave) - todos inician en FFFFFFFFFFFFh cuando el chip de la tarjeta viene de fábrica. 
        MFRC522::MIFARE_Key key;
        for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
        
        if ( ! mfrc522.PICC_IsNewCardPresent())   return;
        if ( ! mfrc522.PICC_ReadCardSerial())    return;
        
        Serial.print(F("Card UID:"));    //lee información en UID y lo muestra en hexadecimal
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
        } 
        Serial.print(F(" PICC type: "));   // PICC tipo
        MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
        Serial.println(mfrc522.PICC_GetTypeName(piccType));

       //variables iniciales escritura 
        byte buffer[]={255,12,1,1,1,1,1,1,1,1,1,1,1,1,1,1,};
        byte block;
        MFRC522::StatusCode status;
        byte len;

        //interface de datos
        Serial.setTimeout(20000L) ;     // espera 20 seg
        // Pregunta por data
      
        block = 1; // numero de bloque
        Serial.println(F("Autenticando usando la llave (key A)..."));
        //autenticando el acceso al sector bloque 1
        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
        //diálogo validación
        if (status != MFRC522::STATUS_OK) {
           Serial.print(F("Falla de autenticación: "));
           Serial.println(mfrc522.GetStatusCodeName(status));
           return;
        }
        else  { Serial.print(F("Acceso al bloque: "));Serial.println(block); }
        
        // Escribir en el bloque 1
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("fallo escritura "));
      Serial.println(mfrc522.GetStatusCodeName(status));
            return;
  }
        else Serial.println(F("escribiendo "));
        
        Serial.println(" ");
        Serial.println("hecho ");
        mfrc522.PICC_HaltA(); // Halt PICC
        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    
}
