#include <EvoraBoard.h>

//Configuración de credenciales
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

// Seleccionar la plan de frecuencia que usa el modulo (TTN_FP_EU868 o TTN_FP_US915)
#define freqPlan TTN_FP_US915

EvoraBoard ttn(loraSerial, debugSerial, freqPlan);
int cont=0;
void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
}

void loop()
{
  debugSerial.print("-- LOOP ");
  debugSerial.println(cont);
  // Preparar datos
  
  byte payload[] = "Hola, soy Evora :)";

  // Enviar datos
  ttn.sendBytes(payload, sizeof(payload)-1);
    cont++;
  delay(10000);
}

/*PAYLOAD FORMAT
function Decoder(bytes, port) {
  var result = "";
  for (var i = 0; i < bytes.length; i++) {
    result += (String.fromCharCode(bytes[i]));
  }

  return {text: result};
}
*/