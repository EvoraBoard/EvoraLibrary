# API Reference

La  clase `EvoraBoard` permite que dispositivos Arduino con modulos de LoRaWAN puedan cominicarse a [The Things Network](https://www.thethingsnetwork.org/). Por el momento solo son soportados los modulos de Microchip RN2903 y RN2483.

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

Obtiene el AppEUI guardado en el módulo. El AppEUI puede ser configurado usando `provision()` o `join()`.

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

Mira el ejemplo [Device_EUI](https://github.com/EvoraBoard/EvoraLibrary/blob/master/examples/Device_EUI/Device_EUI.ino).

## Method: `onMessage`

Establece una funcion  que será llamada para procesar los mensajes entrantes. Se recomienda hacer esto en la función `setup()` y luego definir la función `void (*cb)(const byte* payload, size_t length, port_t port)` en cualquier otra parte del codigo.


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
- `uint32_t retryDelay = 10000`: Tiempo en milisegundos entre los intentos. Por defecto son 10 segundos.

Regresa un `true` o `false` dependiendo de si se recibió la confirmación de que la activación se realizó correctamente antes del número máximo de intentos.

Llama al metodo sin los primeros dos argumentos si el modulo LoRa viene con credenciales predefinidas.

## Method: `personalize`

Activación via ABP.

```c
bool personalize(const char *devAddr, const char *nwkSKey, const char *appSKey);
bool personalize();
```

- `const char *devAddr`: Device Address asignado al dispositivo.
- `const char *nwkSKey`: Network Session Key asignado al dispositivo para su identificación.
- `const char *appSKey`: Application Session Key asignado al dispositivo para la encriptación.

Regresa un `true` o `false` dependiendo de si la activación fue exitosa.  

Llama al metodo sin los argumentos si el modulo LoRa viene con credenciales predefinidas.

Mira el ejemplo [Send_ABP](https://github.com/EvoraBoard/EvoraLibrary/blob/master/examples/Send_ABP/Send_ABP.ino).

## Method: `sendBytes`

Envía un mensaje a la aplicacion utilizando bytes.

```c
ttn_response_t sendBytes(const byte* payload, size_t length, port_t port = 1, bool confirm = false, uint8_t sf = 0);
```

- `const byte* payload `: Bytes por enviar.
- `size_t length`: Numero de bytes. Use `sizeof(payload)` para obtenerlo.
- `port_t port = 1`: El puerto por donde enviar el mensaje. Por defecto es `1`.
- `bool confirm = false`: Solicitar confirmación. Por defecto es `false`. Si la confirmación falla, el metodo regresará un codigo de error `TTN_ERROR_UNEXPECTED_RESPONSE`.
- `uint8_t sf = 0`: Anular el spreading factor (SF) definido en el constructor. Si el valor de `0` no es editado, el SF no se cambia del constructor o del valor anterior.

Regresa un mensaje de error o de exito 
Returns a success or error code and logs the related error message:

* `TTN_ERROR_SEND_COMMAND_FAILED`: Fallo al enviar el comando
* `TTN_SUCCESSFUL_TRANSMISSION`: Transmisión exitosa.
* `TTN_SUCCESSFUL_RECEIVE`: Transmisión exitosa. Se recibieron \<N> bytes
* `TTN_ERROR_UNEXPECTED_RESPONSE`: Respuesta inesperada: \<Response>

Mira el ejemplo [Send_OTAA](https://github.com/EvoraBoard/EvoraLibrary/blob/master/examples/Send_OTAA/Send_OTAA.ino).



## Method: `provision`

Establece la información necesaria para activar el dispositivo a través de OTAA, sin activarlo realmente. Llame la función join () sin los primeros 2 argumentos para activar.

```c
bool provision(const char *appEui, const char *appKey);
```

- `const char *appEui`: AppEUI del dispositivo registrado.
- `const char *appKey`: AppKey asignada al dispositivo.

## Method: `sleep`

Duerme el modulo LoRa por un tiempo en específico en milisegundos.

```c
void sleep(unsigned long mseconds);
```

- `unsigned long mseconds`: Numero en milisegundos para dormir.

## Method: `wake`

Activa el módulo LoRa desde el modo de espera antes de que finalice el tiempo definido.

```c
void wake();
```

## Method: `linkCheck`

Establece el intervalo de tiempo para que se active el proceso de `Link Check`.

```c
void linkCheck(uint16_t seconds);
```

- `uint16_t seconds`: El intervalo de tiempo en segundos. Un valor de 0 deshabilitará el proceso `Link Check`.

## Method: `getLinkCheckGateways`

Obtiene el numero de gateways que recibieron exitosamente la ultima solicitud de `Link Check Request`.

```c
uint8_t getLinkCheckGateways();
```

## Method: `getLinkCheckMargin`

Obtiene el marjen de demodulación tal como se recibió en el ultimo `Link Check Answer`.

```c
uint8_t getLinkCheckMargin();
```

## Method: `getVDD`

Regresa el voltaje en milivolts (mV) medido por el modulo RN2xx3. Es solo por información.

```c
uint16_t getVDD();
```
