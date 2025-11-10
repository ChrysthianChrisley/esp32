# Hardware – ESP32 com Display OLED e Suporte para Bateria 18650

Este documento apresenta as especificações técnicas principais, mapeamento de pinos e informações relevantes sobre a placa de prototipagem **ESP32 da RoboCore**, utilizada neste projeto.

---

## Especificações Técnicas

| Componente                   | Descrição                                         |
|------------------------------|---------------------------------------------------|
| **Processador**              | Xtensa® Dual-Core 32-bit LX6                      |
| **Memória Flash**            | 4 MB                                              |
| **Memória RAM**              | 520 KB                                            |
| **Memória ROM**              | 448 KB                                            |
| **Clock Máximo**             | 240 MHz                                           |
| **Pinos Digitais GPIO**      | 11 (todos com suporte a PWM)                      |
| **Resolução PWM**            | até 16 bits (ajustável via código)                |
| **Conectividade Wi-Fi**      | 802.11 b/g/n - 2.4 GHz (antena integrada)         |
| **Modos de Operação**        | Access Point / Estação / Access Point + Estação   |
| **Bluetooth Low Energy**     | padrão 4.2 integrado                              |

---

## Hardware Principal

- Microcontrolador: ESP32 (Wi-Fi e Bluetooth integrados)
- Placa: Modelo RoboCore com display OLED e suporte para bateria 18650
- Display: OLED 128x64, controlador SSD1306
- Gerenciamento de energia: carregamento de bateria 18650 via USB (5V)

---

## Mapeamento de Pinos e Conexões

### Display OLED (SSD1306)
- Protocolo: I2C
- SDA (Dados): GPIO 5
- SCL (Relógio): GPIO 4
- Endereço I2C padrão: `0x3C`
- [Datasheet do Display OLED – SSD1306](https://d229kd5ey79jzj.cloudfront.net/1208/SSD1306.pdf)
- [Biblioteca OLED para ESP32 (Exemplo alternativo)](https://d229kd5ey79jzj.cloudfront.net/1208/esp8266-oled-ssd1306-master.zip)

### Conectividade (Wi-Fi e Bluetooth)
- As funções de rede sem fio são internas ao chip ESP32.
- Não há pinos GPIO externos dedicados ao Wi-Fi ou Bluetooth.
- A antena necessária já está integrada ao módulo.
- [Arquitetura do Bluetooth – ESP32](https://d229kd5ey79jzj.cloudfront.net/1208/ESP32_Bluetooth.Architecture.pdf)

---

## Leitura da Bateria

- Não há conexão de fábrica entre o terminal da bateria e um pino ADC do ESP32.
- A bateria é usada para alimentação e carregada via USB.
- **Limitação:** não é possível medir voltagem ou porcentagem da bateria por software nesta configuração.
- **Solução opcional:**
  - Adicionar um divisor de tensão entre o terminal da bateria (máx. 4.2V) e um pino ADC do ESP32 (ex: GPIO 32).
  - O divisor deve reduzir a tensão máxima para **3.3V** antes da conexão ao ADC.

---

## Instalação das Bibliotecas

Para uso do display OLED com ESP32 via Arduino IDE, é necessário instalar as bibliotecas [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) e [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library):

1. Abra o menu `Ferramentas > Gerenciar Bibliotecas…` na Arduino IDE.
2. Procure e instale **Adafruit SSD1306**.
3. Em seguida, procure e instale **Adafruit GFX**.
4. Reinicie a Arduino IDE após a instalação.

---

## Exemplo de Inicialização do Display (código)



> Definições da placa RoboCore: SDA no GPIO 5 e SCL no GPIO 4, com endereço I2C 0x3C. Modifique conforme necessário para sua aplicação.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
Serial.begin(115200);
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Endereço padrão 0x3C
Serial.println(F("SSD1306 allocation failed"));
for(;;); // Travar se falhar
}
delay(2000);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0, 10);
display.println("Hello, World!");
display.display();
}

void loop() {
// Código principal 
}

---

## Notas de Desenvolvimento

Durante a compilação na IDE do Arduino, podem aparecer avisos como:
- `AVISO: A categoria 'Sound' na biblioteca ESP_SR é inválida...`
- `AVISO: A categoria 'Security' na biblioteca Hash é inválida...`

Esses avisos são inofensivos e podem ser ignorados, pois referem-se apenas à formatação de categorias das bibliotecas do ESP-IDF e não impactam a compilação ou execução do código.

---

## Referências

- [Datasheet SSD1306](https://d229kd5ey79jzj.cloudfront.net/1208/SSD1306.pdf)
- [Arquitetura do Bluetooth ESP32](https://d229kd5ey79jzj.cloudfront.net/1208/ESP32_Bluetooth.Architecture.pdf)
- [Biblioteca SSD1306](https://d229kd5ey79jzj.cloudfront.net/1208/esp8266-oled-ssd1306-master.zip)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)

---

Para dúvidas ou sugestões, entre em contato com a equipe de desenvolvimento.
