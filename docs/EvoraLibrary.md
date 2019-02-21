# API Reference

La  clase `EvoraBoard` permite que dispositivos Arduino con modulos de LoRaWAN puedan cominicarse a The Things Network. Por el momento solo son soportados los modulos de Microchip RN2903 y RN2483.

## Class: `EvoraBoard`
Incluye y crea una instancia de la clase EvoraBoard. El constructor inicia la la librería con el Streams que usará para comunicarse. Ademas configura el valor del Spreading Factor, el plan de frecuencia, y las sub banda de frecuencia.

```c
#include <EvoraBoard.h>

EvoraBoard ttn(Stream& modemStream, Stream& debugStream, fp_ttn_t fp, uint8_t sf = 7, uint8_t fsb = 2);
```

- `Stream& modemStream`: Stream para el modulo LoRa ([ver notas](https://www.thethingsnetwork.org/docs/devices/arduino/usage.html)).
- `Stream& debugStream`: Stream para la depuracion ([ver notas](https://www.thethingsnetwork.org/docs/devices/arduino/usage.html)).
- `fp_ttn_fp fp`: Plan de frecuancias: `TTN_FP_EU868`, `TTN_FP_US915`, `TTN_FP_AS920_923`, `TTN_FP_AS923_925` or `TTN_FP_KR920_923`depdendiendo dela region donde se esté aplicando [ver notas](https://www.thethingsnetwork.org/wiki/LoRaWAN/Frequencies/Frequency-Plans).
- `uint8_t sf = 7`: Configuracion opcional del Spreading Factor. Puede ser de `7` a `10` para `TTN_FP_US915` y `7` a `12` para otros planes de frecuencia. Por defecto es `7`.
- `uint8_t fsb = 2`: Configuaración opcional de las sub bandas de frecuencia. Puede ser de `1` a `8`. Por defecto es `2` (para US915).

## Method: `reset`

Realiza el reinicio del software del modulo RN2xx3. Esto no borra los parámetros guardados, por ejemplo: credenciales.

```c
void reset(bool adr);
```

- `bool adr`: Activa/Desactiva el Adaptive Data Rate (ADR).

## Method: `getHardwareEui`

Obtiene el hardware EUI, comunmente usado como DevEUI.

```c
size_t getHardwareEui(char *buffer, size_t size);
```

## Method: `getAppEui`

Obtiene el AppEUI guardada en el modulo. El AppEUI puede ser configurada usando `provision()` o `join()`.

```c
size_t getAppEui(char *buffer, size_t size);
```

## Method: `showStatus`

Despliega informacion sobre el dispositivo y el modulo de LoRa por medio del `debugStream`.

```c
void showStatus();
```

Se mostrará algo como lo siguiente:

```bash
EUI: 0004A30B001B7AD2
Battery: 3223
AppEUI: 70B3D57EF000001C
DevEUI: 0004A30B001B7AD2
Data Rate: 0
RX Delay 1: 1000
RX Delay 2: 2000
```

Mira el  ejemplo [Device_EUI](https://github.com/EvoraBoard/EvoraLibrary/blob/master/examples/Device_EUI/Device_EUI.ino).

## Method: `onMessage`

Establece una funcion  que será llamada para procesar los mensajes entrantes. Se recomienda hacer esto en la función `setup()` y luego definir la función `void (*cb)(const byte* payload, size_t length, port_t port)` en cualquier otra parte del programa.


```c
void onMessage(void (*cb)(const byte* payload, size_t length, port_t port));
```

- `const byte* payload`: Bytes received.
- `size_t length`: Number of bytes.
- `port_t port`: The port addressed.

Mira el ejemplo [Receive_Data](https://github.com/EvoraBoard/EvoraLibrary/blob/master/examples/Receive_Data/Receive_Data.ino).

## Method: `join`

Activación via OTAA (default).

```c
bool join(const char *appEui, const char *appKey, int8_t retries = -1, uint32_t retryDelay = 10000);
bool join(int8_t retries = -1, uint32_t retryDelay = 10000);
```

- `const char *appEui`: AppEUI del dispositivo registrado.
- `const char *appKey`: AppKey asignada al dispositivo.
- `int8_t retries = -1`: Numero de veces para reinterar despues de fallar o no confirmar el join. Por defecto es`-1` que significa infinito
- `uint32_t retryDelay = 10000`: Delay in ms between attempts. Defaults to 10 seconds.

Returns `true` or `false` depending on whether it received confirmation that the activation was successful before the maximum number of attempts.

Call the method without the first two arguments if the device's LoRa module comes with pre-flashed values.

## Method: `personalize`

Activate the device via ABP.

```c
bool personalize(const char *devAddr, const char *nwkSKey, const char *appSKey);
bool personalize();
```

- `const char *devAddr`: Device Address assigned to the device.
- `const char *nwkSKey`: Network Session Key assigned to the device for identification.
- `const char *appSKey`: Application Session Key assigned to the device for encryption.

Returns `true` or `false` depending on whether the activation was successful.

Call the method with no arguments if the device's LoRa module comes with pre-flashed values.

See the [SendABP](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/SendABP/SendABP.ino) example.

## Method: `sendBytes`

Send a message to the application using raw bytes.

```c
ttn_response_t sendBytes(const byte* payload, size_t length, port_t port = 1, bool confirm = false, uint8_t sf = 0);
```

- `const byte* payload `: Bytes to send.
- `size_t length`: The number of bytes. Use `sizeof(payload)` to get it.
- `port_t port = 1`: The port to address. Defaults to `1`.
- `bool confirm = false`: Whether to ask for confirmation. Defaults to `false`. If confirmation fails, the method will return error code `TTN_ERROR_UNEXPECTED_RESPONSE`.
- `uint8_t sf = 0`: Override the spreading factor (SF). If the default value `0` is passed, the SF is not changed from the constructor or previous value.

Returns a success or error code and logs the related error message:

* `TTN_ERROR_SEND_COMMAND_FAILED`: Send command failed.
* `TTN_SUCCESSFUL_TRANSMISSION`: Successful transmission.
* `TTN_SUCCESSFUL_RECEIVE`: Successful transmission. Received \<N> bytes
* `TTN_ERROR_UNEXPECTED_RESPONSE`: Unexpected response: \<Response>

See the [SendOTAA](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/SendOTAA/SendOTAA.ino) example.

## Method: `poll`

Calls `sendBytes()` with `{ 0x00 }` as payload to poll for incoming messages.

```c
int8_t poll(port_t port = 1, bool confirm = false);
```

- `port_t port = 1`: The port to address. Defaults to `1`.
- `bool confirm = false`: Whether to ask for confirmation.

Returns the result of `sendBytes()`.

See the [Receive](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/Receive/Receive.ino) example.

## Method: `provision`

Sets the information needed to activate the device via OTAA, without actually activating. Call join() without the first 2 arguments to activate.

```c
bool provision(const char *appEui, const char *appKey);
```

- `const char *appEui`: Application Identifier for the device.
- `const char *appKey`: Application Key assigned to the device.

## Method: `sleep`

Sleep the LoRa module for a specified number of milliseconds.

```c
void sleep(unsigned long mseconds);
```

- `unsigned long mseconds`: number of milliseconds to sleep.

## Method: `wake`

Wake up the LoRa module from sleep before the expiration of the defined time.

```c
void wake();
```

## Method: `linkCheck`

Sets the time interval for the link check process to be triggered.

```c
void linkCheck(uint16_t seconds);
```

- `uint16_t seconds`: the time interval in seconds. A value of 0 will disable the link check process.

## Method: `getLinkCheckGateways`

Gets the number of gateways that successfully received the last Link Check Request frame.

```c
uint8_t getLinkCheckGateways();
```

## Method: `getLinkCheckMargin`

Gets the demodulation margin as received in the last Link Check Answer frame.

```c
uint8_t getLinkCheckMargin();
```

## Method: `getVDD`

Returns the voltage in millivolt (mV) measured by the RN2xxx LoRa module. It's for information only since we don't know how it's measured but looks like accurate.

```c
uint16_t getVDD();
```
