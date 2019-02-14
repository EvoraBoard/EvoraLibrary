#include <EvoraBoard.h>

//Configuración de credenciales
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial
// Seleccionar la plan de frecuencia que usa el modulo (TTN_FP_EU868 o TTN_FP_US915)
#define freqPlan TTN_FP_US915
#define led1 13


EvoraBoard ttn(loraSerial, debugSerial, freqPlan);

int lect;
void setup()
{
  pinMode(led1,OUTPUT);
  
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  
  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  ttn.onMessage(message);
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

void message(const uint8_t *payload, size_t size, port_t port){

    analogWrite(payload[0],payload[1]);
}

/*-----Payload Format-----
----------------DECODER-------------------
function Decoder(bytes, port) {
  // Decodifica el mensaje entrante
  
  var decoded = {};


  decoded.lectura = (bytes[0]<<8)| bytes[1];
  

  return decoded;
}
------------------------------------------
----------------ENCODER-------------------
//Codifica el mensaje saliente
function Encoder(object, port) {
  var bytes=[];
  if(object.pin===13){
    bytes[0] = object.pin;
    bytes[1] = object.brillo;
  }
  return bytes;
}

------------------------------------------
*/ 