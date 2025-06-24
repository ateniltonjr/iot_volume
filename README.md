# Sistema Inteligente de Monitoramento e Controle de Nível de Água :water_wave:

## Desenvolvedores :busts_in_silhouette:
- Atenilton Santos de Souza Junior
- Daniel Silva de Souza
- José Vinicius Rodrigues Soara
- Luiz Eduardo Pereira Gomes
- Ronaldo César Santos Rocha
- Polo: Bom Jesus da Lapa
- Data: 23/06/2025

## Descrição do Projeto :gear:

O **Sistema Inteligente de Monitoramento e Controle de Nível de Água** é um projeto desenvolvido por Atenilton Santos de Souza Junior, Daniel Silva de Souza, José Vinicius Rodrigues Soara, Luiz Eduardo Pereira Gomes, e Ronaldo César Santos Rocha, alunos do polo de Bom Jesus da Lapa. Este sistema embarcado, baseado na placa BitDogLab com o microcontrolador RP2040, monitora e controla remotamente o nível de água em um reservatório, automatizando o acionamento de uma bomba com base em limites configuráveis (padrão 10% e 90%). O sistema oferece uma interface web responsiva para ajustes em tempo real e feedback visual/sonoro local via periféricos da placa BitDogLab (OLED, LEDs WS2812B, buzzer, etc.), promovendo eficiência no uso da água em ambientes agrícolas ou residenciais.

## Objetivo Geral :target:

Desenvolver um sistema acessível que:
- Monitore o nível de água em tempo real usando uma boia caseira.
- Controle automaticamente uma bomba de 12V via relé.
- Forneça uma interface web para configuração remota.
- Utilize periféricos da placa BitDogLab para feedback local.

## Funcionamento :rocket:

### Componentes Principais
- **Placa BitDogLab com RP2040**: Processa leituras e controla os periféricos.
- **Boia Caseira**: Conectada ao GPIO28 (ADC2), usa um potenciômetro e uma bola de isopor para medir o nível de água (0--100%).
- **Relé 5V**: Acionado por um transistor BC337 (GPIO16) para controlar a bomba de 12V.
- **Display OLED (128x64)**: Exibe três modos (Menu Principal, Monitoramento, Limites) via I2C (GPIO14/15).
- **Matriz WS2812B (5x5)**: Mostra o nível com linhas azuis (GPIO7).
- **LED RGB**: Indica o estado da bomba (GPIO11, 12, 13).
- **Buzzer**: Emite som quando a bomba está ligada (GPIO10).
- **Wi-Fi (CYW43)**: Habilita a interface web na porta 80.

### Modos de Operação
- **Menu Principal**: Opções para "Monitorar" (Botão A, GPIO5) e "Limites" (Botão B, GPIO6), com retorno via Joystick (GPIO22).
- **Modo Monitoramento**: Exibe nível, estado da bomba e barra gráfica.
- **Modo Limites**: Configura limites e mostra o IP Wi-Fi.
- **Interface Web**: Atualiza dados a cada segundo e permite ajustes via `/set_limits`.

### Lógica de Controle
- Liga a bomba se o nível for < limite mínimo.
- Desliga a bomba se o nível for > limite máximo.

## Nome dos Componentes :label:

- **Microcontrolador**: RP2040 (placa BitDogLab).
- **Transistor**: BC337 NPN.
- **Resistor**: 10 kΩ.
- **Potenciômetro**: Modelo genérico (varia resistência com o nível de água).
- **Relé**: 5V.
- **Bomba**: 12V.
- **Bola de Isopor**: Material flutuante.
- **Display OLED**: SSD1306 (128x64, I2C).
- **Matriz de LEDs**: WS2812B (5x5).
- **LED RGB**: Verde, Azul, Vermelho.
- **Buzzer**: PWM 1000Hz.
- **Botões**: Botão A, Botão B, Joystick SW.
- **Fonte Externa**: 5V e 12V.

## Portas GPIO Utilizadas :electric_plug:

### Detalhamento das Conexões
- **GPIO28 (ADC2)**: Entrada analógica do potenciômetro da boia caseira.
- **GPIO16**: Saída para controle do transistor BC337 (aciona o relé).
- **GPIO14 (SDA)** e **GPIO15 (SCL)**: Comunicação I2C com o Display OLED (400 kHz).
- **GPIO7**: Controle da matriz WS2812B (5x5) via PIO.
- **GPIO11 (Verde)**, **GPIO12 (Azul)**, **GPIO13 (Vermelho)**: LEDs RGB para indicação.
- **GPIO10**: Saída PWM para o buzzer (1000Hz).
- **GPIO5 (Botão A)**, **GPIO6 (Botão B)**, **GPIO22 (Joystick SW)**: Entradas com interrupções e debounce (200ms).

## Requisitos :wrench:

- Placa BitDogLab com RP2040.
- Bomba de 12V e fontes de 5V/12V.
- Componentes eletrônicos listados acima.
- Ambiente de desenvolvimento: Pico SDK ou Arduino IDE.
- Conexão Wi-Fi para a interface web.

## Uso :play_button:

- Acesse o sistema via Wi-Fi usando o IP exibido no OLED.
- Use a interface web para monitorar o nível e ajustar limites (ex.: `/set_limits?lim_min=10&lim_max=90`).
- Interaja localmente com os botões (A, B, Joystick) e visualize o feedback no OLED e na matriz de LEDs.

## Estrutura do Código :page_facing_up:

- **main.c**: Lógica principal, leitura do sensor e controle da bomba.
- **display.h**: Gerencia o OLED e os modos de interface.
- **reles.h**: Controla o relé e a bomba.
- **interface.h**: Implementa o servidor web com lwip (endpoint `/update` retorna JSON, `/set_limits` ajusta limites).

## Demonstração :film_projector:

- [Vídeo de Demonstração][(https://youtu.be/](https://youtu.be/3ygbRdIc8zc))


