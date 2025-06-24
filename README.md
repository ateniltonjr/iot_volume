# Sistema Inteligente de Monitoramento e Controle de Nível de Água 🎉

## 🌟 Visão Geral
O projeto é um sistema embarcado desenvolvido com o RP2040 para monitorar e controlar remotamente o nível de água em reservatórios. Utiliza Wi-Fi para uma interface web responsiva, onde você ajusta limites mínimo e máximo em tempo real. Integra periféricos da placa BitDogLab, como display OLED, matriz de LEDs WS2812, LED externo e buzzer, para feedback visual e sonoro. 💧

## 🚀 Funcionalidades
- Monitoramento do nível via potenciômetro e ADC.
- Acionamento automático da bomba com base em limites configuráveis.
- Interface web na porta 80 para controle remoto.
- Visualização no OLED e LEDs, com alertas sonoros via buzzer. 🔊

## 🛠️ Pré-requisitos
- Placa RP2040 (BitDogLab).
- Conexão Wi-Fi (configure SSID/senha no código).
- Bibliotecas: `pico-sdk`, `lwip`, `ssd1306`, `ws2812.pio`.

## 📋 Como Usar
1. Clone o repositório;
2. Configure o SSID e senha Wi-Fi no arquivo principal;
3. Compile e grave no RP2040 usando o Pico SDK;
4. Acesse a interface web no IP exibido no OLED (exp: `192.168.1.100`).

## 🎥 Demonstração
- Vídeo: https://youtu.be/3ygbRdIc8zc
