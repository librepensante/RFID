#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


byte ActualUID[4];
byte Usuario1[4]= {228, 35, 209, 181} ;
byte Usuario2[4]= {186, 90, 135, 180} ;

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
     for (byte i = 0; i < mfrc522.uid.size; i++) {
      ActualUID[i]=mfrc522.uid.uidByte[i];          
                  } 
      
      delay(1000);
      Serial.println(" ");
if(comparar(ActualUID,Usuario1))
   Serial.println("Acceso MASTER...");
else if(comparar(ActualUID,Usuario2))
   Serial.println("Acceso MEDIADOR...");   
else
   Serial.println("Acceso denegado...");       
 }       
//Función para comparar dos vectores
 boolean comparar(byte uid[],byte tarjeta[])
{
  if(uid[0] != tarjeta[0])return(false);
  if(uid[1] != tarjeta[1])return(false);
  if(uid[2] != tarjeta[2])return(false);
  if(uid[3] != tarjeta[3])return(false);
  return(true);
}
      
      
    
