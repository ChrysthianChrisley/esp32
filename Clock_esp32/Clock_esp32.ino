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
    display.setTextColor(SSD1306_WHITE); // <-- CORRIGIDO AQUI
    display.setCursor(0,0);
    display.println("Erro ao ler hora!");
    display.display();
    delay(1000);
    return; // Tenta de novo no próximo loop
  }

  // 2. Formata as strings de HORA e DATA
  char timeString[10]; // Buffer para "HH:MM:SS\0"
  sprintf(timeString, "%02d:%02d:%02d", 
          timeinfo.tm_hour, 
          timeinfo.tm_min, 
          timeinfo.tm_sec);
          
  char dateString[6]; // Buffer para "DD/MM\0"
  sprintf(dateString, "%02d/%02d",
          timeinfo.tm_mday,
          timeinfo.tm_mon + 1); // +1 porque os meses são de 0-11

  // 3. Limpa o display
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // Já estava correto aqui

  // 4. Desenha a HORA (Tamanho 2, Centralizada)
  display.setTextSize(2);
  int16_t x1_time, y1_time;
  uint16_t w_time, h_time;
  display.getTextBounds(timeString, 0, 0, &x1_time, &y1_time, &w_time, &h_time);
  int x_pos_time = (SCREEN_WIDTH - w_time) / 2;
  int y_pos_time = 18; // Posição Y (mais para cima)
  
  display.setCursor(x_pos_time, y_pos_time);
  display.print(timeString);

  // 5. Desenha a DATA (Tamanho 1, Centralizada)
  display.setTextSize(1);
  int16_t x1_date, y1_date;
  uint16_t w_date, h_date;
  display.getTextBounds(dateString, 0, 0, &x1_date, &y1_date, &w_date, &h_date);
  int x_pos_date = (SCREEN_WIDTH - w_date) / 2;
  int y_pos_date = 40; // Posição Y (abaixo da hora)

  display.setCursor(x_pos_date, y_pos_date);
  display.print(dateString);

  // 7. Envia tudo para o display
  display.display();
  
  delay(1000); // Atualiza a cada 1 segundo
}
