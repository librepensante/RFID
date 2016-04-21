#include <SPI.h> // Serial Perispherical Interface  
#include <MFRC522.h> // lector/escritor RC522

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // MFRC522 instancia.

byte ActualUID[4]; // 4 bytes con los datos del UID
byte Usuario1[4]= {228, 35, 209, 181} ; //datos del UID de la tarjeta 
byte Usuario2[4]= {186, 90, 135, 180} ;//datos del UID de una segunda tarjeta 


void setup() {
    Serial.begin(9600); // Comunicación serial 
    SPI.begin();            // comunicación SPI
    mfrc522.PCD_Init(); // inicia MFRC522
    Serial.println("Acceso por UID...");
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
    //Accede a los bytes del UID
     for (byte i = 0; i < mfrc522.uid.size; i++) {
      ActualUID[i]=mfrc522.uid.uidByte[i];           
                  } 
      Serial.println(" ");
// Compara UID de la tarjeta con los números de la tarjeta usuario y toma una decisión.
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
      
      
    
