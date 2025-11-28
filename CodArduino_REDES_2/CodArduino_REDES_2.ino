#include <WiFi.h>
#include <ESPping.h>
#include <Wire.h>

//Configura tu red WiFi
const char* ssid = "ulwifiI1";
const char* password = "";

// configurar numero de envios de paquetes
int enviados = 5;


void setup() {
  Serial.begin(115200);

  
  // Conectar a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Mostrar datos en Serial
  Serial.println("\n✅ Conectado a WiFi.");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());

  
}

void loop() {
  //Medir RSSI
  int rssi = WiFi.RSSI();
  Serial.print("RSSI: ");
  Serial.print(rssi);
  Serial.println(" dBm");

  // Medir latencia con ping (4 paquetes)
  IPAddress destino(8, 8, 8, 8);  // Google DNS
  int promedio = -1;
  int latencias[enviados];
  int suma = 0;
  int recibidos= 0;
  
  Serial.print("Se ingresaran: ");
  Serial.print(enviados);
  Serial.print(" paquetes");
  Serial.println();

  if( enviados > 3 and enviados <7){
    for (int i = 0; i < enviados; i++) {
      int tiempo = Ping.ping(destino, 1);// devuelve tiempo en ms o -1 si falla
      if (tiempo >= 0) {
        latencias[i]= Ping.averageTime();
        suma += latencias[i];
        recibidos++;
        Serial.printf("Paquete %d: ✅ %d ms\n", i + 1, latencias);
      }else {
        latencias[i] = -1;  // usamos -1 para marcar que no hubo respuesta
        Serial.printf("Paquete %d: ❌ sin respuesta\n", i + 1);
      }
      delay(500);
    }  
    if (recibidos > 0) {
      promedio = suma / recibidos;
    }
    Serial.println("Latencia promedio: " + String(promedio) + " ms");

    //porcentaje de perdida 
    int perdidos = enviados - recibidos;
    Serial.printf("Paquetes enviados : %d\n", enviados);
    Serial.printf("Paquetes recibidos: %d\n", recibidos);
    Serial.printf("Paquetes perdidos : %d\n", perdidos);
    

      
  }else{
    Serial.print("Nunmero de paquetes no valido");
  }

  delay(6000); // Actualiza cada 6 segundos
}
