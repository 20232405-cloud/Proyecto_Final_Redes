#include <WiFi.h>

const char* ssid = "ulwifiE3";
const char* password ="";

WiFiServer server(8080);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Conectado a WiFi");
  Serial.print("IP del ESP32: ");
  Serial.println(WiFi.localIP());

  server.begin();  // Inicia servidor TCP en puerto 8080
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("📥 Cliente conectado...");

    // --- SUBIDA: recibir datos desde PC ---
    size_t totalBytes = 0;
    unsigned long start = millis();

    while (client.connected() && client.available()) {
      client.read();
      totalBytes++;
    }

    unsigned long end = millis();
    float tiempo = (end - start) / 1000.0;
    float velocidadSubida = (totalBytes * 8.0) / (tiempo * 1e6);

    Serial.printf("Recibidos: %d bytes en %.2f s\n", totalBytes, tiempo);
    Serial.printf("Velocidad de subida: %.2f Mbps\n", velocidadSubida);

    // --- DESCARGA: enviar datos al PC ---
    Serial.println("📤 Enviando datos de prueba al cliente...");
    const size_t dataSize = 1000000; // 1 MB
    unsigned long startSend = millis();

    // Crear un buffer de 1024 bytes llenos de 'A'
    uint8_t buffer[1024];
    memset(buffer, 'A', sizeof(buffer));

    size_t sent = 0;
    while (sent < dataSize) {
      size_t chunk = min(sizeof(buffer), dataSize - sent);
    client.write(buffer, chunk);
    sent += chunk;
    }


    unsigned long endSend = millis();
    float tiempoSend = (endSend - startSend) / 1000.0;
    float velocidadDescarga = (dataSize * 8.0) / (tiempoSend * 1e6);

    Serial.printf("Enviados: %d bytes en %.2f s\n", dataSize, tiempoSend);
    Serial.printf("Velocidad de descarga: %.2f Mbps\n", velocidadDescarga);

    client.stop();
    Serial.println("🔌 Cliente desconectado");
  }
}
