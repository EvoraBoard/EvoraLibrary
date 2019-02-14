#include <Wire.h>
#include <EvoraBoard.h>
#include <Adafruit_BMP085.h>

//Configuración de credenciales
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial
// Seleccionar la plan de frecuencia que usa el modulo (TTN_FP_EU868 o TTN_FP_US915)
#define freqPlan TTN_FP_US915

Adafruit_BMP085 bmp;
EvoraBoard ttn(loraSerial, debugSerial, freqPlan);

int Temp,Pres,Alti,SeaPres;
void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  
  if (!bmp.begin()) {
	debugSerial.println("Sensor no encontrado, revisar conexiones!");
	while (1) {}
  }

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
}

void loop() {

   Temp = bmp.readTemperature();
   debugSerial.println("Temp: " + String(Temp));
   Pres = bmp.readPressure();
   debugSerial.println("Pres: " + String(Pres));
   Alti = bmp.readAltitude(101500);
   debugSerial.println("Alti: " + String(Alti));


  // Preparar datos
  byte payload[6];
  payload[0]=highByte(Temp);
  payload[1]=lowByte(Temp);
  payload[2]=highByte(Pres);
  payload[3]=lowByte(Pres);
  payload[4]=highByte(Alti);
  payload[5]=lowByte(Alti);

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
  decoded.Temperatura = (bytes[0]<<8)| bytes[1];
  decoded.Presion = (bytes[2]<<8) | bytes[3];
  decoded.Altitud = (bytes[4]<<8) | bytes[5];

  return decoded;
}
*/ 