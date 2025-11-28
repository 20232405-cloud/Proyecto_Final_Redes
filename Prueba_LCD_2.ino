#include <WiFi.h>
#include <ESPping.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Configura tu red WiFi
const char* ssid = "MOVISTAR6819";
const char* password = "zxfm5576";

//configurar numero de envios de paquetes
int enviados = 5;

//Configura la pantalla LCD (dirección I2C y tamaño)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección típica 0x27, pantalla 16x2


void setup() {
  Serial.begin(115200);

  // Inicializar comunicación I2C en los pines por defecto del ESP32
  Wire.begin(21, 22);  // SDA = GPIO21, SCL = GPIO22

  //Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Conectando...");

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

  // Mostrar mensaje inicial en LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi conectado");
  lcd.setCursor(0,1);
  lcd.print(WiFi.SSID());
  delay(2000);
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

  
  //Mostrar en LCD (solo RSSI y latencia)
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("RSSI: ");
  lcd.print(rssi);
  lcd.print(" dBm");

  lcd.setCursor(0,1);
  if (promedio != -1) {
    lcd.print("Lat: ");
    lcd.print(promedio);
    lcd.print(" ms");
  } else {
    lcd.print("Ping fallido");
  }

  delay(6000); // Actualiza cada 6 segundos
}
