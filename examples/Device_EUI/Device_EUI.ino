#include <EvoraBoard.h>

#define loraSerial Serial1
#define debugSerial Serial
#define freqPlan TTN_FP_US915 // Reemplazar conTTN_FP_EU868 or TTN_FP_US915

EvoraBoard ttn(loraSerial, debugSerial, freqPlan);

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);
}

void loop()
{
  debugSerial.println("Device Information");
  debugSerial.println();
  ttn.showStatus();
  debugSerial.println();
  debugSerial.println("-------------------------------------------");
  debugSerial.println();

  delay(10000);
}