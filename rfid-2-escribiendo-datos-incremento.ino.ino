
#include <SPI.h> // Serial Perispherical Interface  
#include <MFRC522.h> // lector/escritor RC522

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // MFRC522 instancia.
MFRC522::MIFARE_Key key;             //llave de acceso

void setup() {
   Serial.begin(9600); // Comunicación serial 
    SPI.begin();            // comunicación SPI
    mfrc522.PCD_Init(); // inicia MFRC522
        Serial.println(F("Escribiendo datos MIFARE PICC "));
         for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
          dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
}

void loop() {
        
      
        if ( ! mfrc522.PICC_IsNewCardPresent())   return;
        if ( ! mfrc522.PICC_ReadCardSerial())    return;
        
 // Detalles del PICC (tag/card)
    Serial.print(F("Card UID:"));
   dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

//Chequea compatibilidad
        if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("This sample only works with MIFARE Classic cards."));
        return;
    }

       /* 

       //variables iniciales escritura 
        byte buffer[]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,};
        byte block;
        
        byte len;

        //interface de datos
        Serial.setTimeout(20000L) ;     // espera 20 seg
        // Pregunta por data
      */
byte  sector = 1; // numero de sector     
byte  blockA = 5; // numero de bloqueA
byte  blockB = 6; // numero de bloqueB
byte  trailerblock = 7; // bloque de transacción
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);
    long value;
    //autenticación llaveA
 Serial.println(F("Autenticación con key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerblock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    // Show the whole sector as it currently is
    Serial.println(F("data actual sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();
   
    byte trailerBuffer[] = {
        255, 255, 255, 255, 255, 255,       // Keep default key A
        0, 0, 0, 
        0,
        255, 255, 255, 255, 255, 255};      // Keep default key B
    mfrc522.MIFARE_SetAccessBits(&trailerBuffer[6], 0, 6, 6, 3);     

  Serial.println(F("leyendo sector trailer..."));
    status = mfrc522.MIFARE_Read(trailerblock, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
       // Check if it matches the desired access pattern already;
    // because if it does, we don't need to write it again...
    if (    buffer[6] != trailerBuffer[6]
        &&  buffer[7] != trailerBuffer[7]
        &&  buffer[8] != trailerBuffer[8]) {
        // They don't match (yet), so write it to the PICC
        Serial.println(F("Writing new sector trailer..."));
        status = mfrc522.MIFARE_Write(trailerblock, trailerBuffer, 16);
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Write() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
        }
    }

    // Authenticate using key B
    Serial.println(F("Autenticación con key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerblock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
 
    formatValueBlock(blockA);
    //formatValueBlock(blockB);
        Serial.print("+1 en bloque "); Serial.println(blockA);
    status = mfrc522.MIFARE_Increment(blockA, 1);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Increment() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    status = mfrc522.MIFARE_Transfer(blockA);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Transfer() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    // Show the new value of valueBlockA
    status = mfrc522.MIFARE_GetValue(blockA, &value);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("mifare_GetValue() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    Serial.print("nuevo dato en bloque "); Serial.print(blockA);
    Serial.print(" = "); Serial.println(value);
    
   }  
        /*
         }
        // Escribir en el bloque 62
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("fallo escritura "));
      Serial.println(mfrc522.GetStatusCodeName(status));
            return;
  }
        else Serial.println(F("escribiendo... "));
        Serial.println("hecho ");
         // cargar sector data
         byte sector=15;
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();
    //leer bloque 
    Serial.println(F("leyendo bloque 62 sector 15 ..."));
byte bufferead[18];
  byte size = sizeof(bufferead);
    status = mfrc522.MIFARE_Read(block, bufferead, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        
        Serial.print(mfrc522.GetStatusCodeName(status));
    }
      Serial.print(F("datos: "));
      for(int i=0;i<15;i++) Serial.print(bufferead[i],HEX);       
        mfrc522.PICC_HaltA(); // Halt PICC
        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    
}*/

/*
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

/**
 * Ensure that a given block is formatted as a Value Block.
 */
void formatValueBlock(byte blockAddr) {
    byte buffer[18];
    byte size = sizeof(buffer);
    MFRC522::StatusCode status;

    Serial.print(F("Leyendo bloque ")); Serial.println(blockAddr);
    status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    if (    (buffer[0] == (byte)~buffer[4])
        &&  (buffer[1] == (byte)~buffer[5])
        &&  (buffer[2] == (byte)~buffer[6])
        &&  (buffer[3] == (byte)~buffer[7])

        &&  (buffer[0] == buffer[8])
        &&  (buffer[1] == buffer[9])
        &&  (buffer[2] == buffer[10])
        &&  (buffer[3] == buffer[11])

        &&  (buffer[12] == (byte)~buffer[13])
        &&  (buffer[12] ==        buffer[14])
        &&  (buffer[12] == (byte)~buffer[15])) {
        Serial.println(F("El formato del bloque es correcto..."));
    }
    else {
        Serial.println(F("Formatting as Value Block..."));
        byte valueBlock[] = {
            0, 0, 0, 0,
            255, 255, 255, 255,
            0, 0, 0, 0,
            blockAddr, ~blockAddr, blockAddr, ~blockAddr };
        status = mfrc522.MIFARE_Write(blockAddr, valueBlock, 16);
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Write() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
        }
    }
}
