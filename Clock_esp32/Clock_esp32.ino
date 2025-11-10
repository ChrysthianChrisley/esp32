#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "time.h" // <-- BIBLIOTECA DE HORA

// --- CONFIGURAÇÃO DO WIFI ---
const char* ssid = "CHRYSTHIAN_EXT";
const char* password = "7366712361883681";
// --- FIM DA CONFIGURAÇÃO DO WIFI ---

// --- CONFIGURAÇÃO NTP (HORA PELA INTERNET) ---
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800; // GMT-3 (Rio/Brasília) = -3 * 3600
const int   daylightOffset_sec = 0;   // Brasil não usa horário de verão
// --- FIM DA CONFIGURAÇÃO NTP ---

// --- CONFIGURAÇÕES DO DISPLAY (JÁ CORRETAS) ---
#define SDA_PIN 5
#define SCL_PIN 4
#define I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1    
// --- Fim das Configurações ---

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
struct tm timeinfo; // Estrutura para armazenar a hora

void setup() {
  Serial.begin(115200);
  
  // 1. INICIA O DISPLAY
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS)) {
    Serial.println(F("Falha ao iniciar SSD1306"));
    while (true); 
  }

  // 2. MOSTRA "CONECTANDO..." NO DISPLAY
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Conectando ao WiFi...");
  display.display();
  Serial.println("Conectando ao WiFi...");

  // 3. CONECTA AO WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.print(".");
    display.display();
  }

  // 4. MOSTRA QUE CONECTOU E BUSCA HORA
  Serial.println("\nConectado!");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Conectado!");
  display.println(WiFi.localIP());
  display.println("Buscando hora...");
  display.display();
  
  // 5. INICIA O CLIENTE DE HORA (NTP)
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  // Espera a hora ser sincronizada
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Aguardando sincronia da hora...");
    delay(1000);
  }

  // 6. MOSTRA AVISO DE SUCESSO
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Hora Sincronizada!");
  display.display();
  delay(2000); 
}

void loop() {
  // 1. Tenta obter a hora
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Falha ao obter hora");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Erro ao ler hora!");
    display.display();
    delay(1000);
    return; // Tenta de novo no próximo loop
  }

  // 2. Formata a string da hora (HH:MM:SS)
  char timeString[10]; 
  sprintf(timeString, "%02d:%02d:%02d", 
          timeinfo.tm_hour, 
          timeinfo.tm_min, 
          timeinfo.tm_sec);

  // 3. Limpa o display e prepara para desenhar
  display.clearDisplay();
  
  // 4. Define a fonte e a cor
  display.setTextSize(2); // <-- MUDADO! (Tamanho 2 agora)
  display.setTextColor(SSD1306_WHITE);

  // 5. Lógica para centralizar o texto
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(timeString, 0, 0, &x1, &y1, &w, &h);
  int x_pos = (SCREEN_WIDTH - w) / 2;
  int y_pos = (SCREEN_HEIGHT - h) / 2;

  // 6. Define o cursor e imprime a hora
  display.setCursor(x_pos, y_pos);
  display.print(timeString);

  // 7. Envia para o display
  display.display();
  
  delay(1000); // Atualiza a cada 1 segundo
}
