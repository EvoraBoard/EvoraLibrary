#include <EvoraBoard.h>
#include <Servo.h>
//Configuración de credenciales
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial
// Seleccionar la plan de frecuencia que usa el modulo (TTN_FP_EU868 o TTN_FP_US915)
#define freqPlan TTN_FP_US915
#define servo 9


EvoraBoard ttn(loraSerial, debugSerial, freqPlan);
Servo myservo;
int lect=1;
void setup()
{
 
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  
  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  ttn.onMessage(message);
  myservo.attach(servo);
}

void loop()
{
  
  // Preparar datos
  byte payload[2];
  payload[0]=highByte(lect);
  payload[1]=lowByte(lect);

  // Enviar data
  ttn.sendBytes(payload, sizeof(payload));
    
  delay(10000);
}

void message(const uint8_t *payload, size_t size, port_t port){
    if(payload[0]== servo){
        if(payload[1]<=180 && payload[1]>=0){
        myservo.write(payload[1]);
        }
        else{
            debugSerial.println("Solo se pueden valores de 0 a 180 grados");
        }
    }
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
  if(object.PinServo===9){
    bytes[0] = object.PinServo;
    bytes[1] = object.grados;
  }
  return bytes;
}

------------------------------------------
*/ 