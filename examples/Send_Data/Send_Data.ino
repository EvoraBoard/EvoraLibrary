#include <EvoraBoard.h>

//Configuración de credenciales
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

// Seleccionar la plan de frecuencia que usa el modulo (TTN_FP_EU868 o TTN_FP_US915)
#define freqPlan TTN_FP_US915

EvoraBoard ttn(loraSerial, debugSerial, freqPlan);

int lect;
void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  
  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
}

void loop()
{
  lect=analogRead(A0);
  debugSerial.println("lectura: "+ String(lect));
  // Preparar datos
  byte payload[2];
  payload[0]=highByte(lect);
  payload[1]=lowByte(lect);

  // Enviar data
  ttn.sendBytes(payload, sizeof(payload));
    
  delay(10000);
}

/*-----Payload Format-----
function Decoder(bytes, port) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.
  var decoded = {};

  // if (port === 1) 
  decoded.lectura = (bytes[0]<<8)| bytes[1];
  

  return decoded;
}
*/ 