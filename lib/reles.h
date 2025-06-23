#ifndef RELES_H
#define RELES_H

#include "pico/stdlib.h"
#include "pico/bootrom.h"

//Modificações:
#include "hardware/pwm.h"
#include "hardware/clocks.h"
//#include "hardware/irq.h"

// Configuração do pino do buzzer
<<<<<<< Updated upstream

#define rele1 12
#define BOTAO_A 5
#define JOYSTICK_SW 22
#define BOTAO_B 6
#define JOYSTICK_Y 27
#define LED_G 11 
#define LED_B 12 
#define LED_R 13
#define BUZZER_PIN 10 
=======
#define BUZZER_PIN 21
#define rele1 12
#define BOTAO_A 5
#define BOTAO_B 6

// Declaração dos botões do Joytick (JVRS)

#define JOYSTICK_Y 27
#define JOYSTICK_SW 22 // botão do joystick para voltar ao menu do display oled

>>>>>>> Stashed changes

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY 3500

// Variáveis globais
enum { MENU_PRINCIPAL, TELA_MONITORAMENTO, TELA_LIMITES } estado_menu = MENU_PRINCIPAL;


// Definição de uma função para inicializar o PWM no pino do buzzer


void init_led_rgb() {
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    printf("LED RGB inicializado\n");
}

void set_led_rgb(bool ligada) {
    if (ligada) {
        gpio_put(LED_R, 0);
        gpio_put(LED_G, 1); 
        gpio_put(LED_B, 0);
        if (log_counter % 50 == 0) {
            printf("LED RGB: Bomba ligada (verde)\n");
        }
    } else {
        gpio_put(LED_R, 0);
        gpio_put(LED_G, 0);
        gpio_put(LED_B, 0);
        if (log_counter % 50 == 0) {
            printf("LED RGB: Bomba desligada\n");
        }
    }
}

<<<<<<< Updated upstream
void init_buzzer() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint clk_div = clock_get_hz(clk_sys) / (1000 * 4096); // Frequência ~1000Hz
    pwm_set_clkdiv(slice_num, clk_div);
    pwm_set_wrap(slice_num, 4095); // Resolução PWM
    pwm_set_enabled(slice_num, true);
    printf("Buzzer inicializado\n");
}
=======
//


>>>>>>> Stashed changes

void set_buzzer(bool ligado) {
    pwm_set_gpio_level(BUZZER_PIN, ligado ? 256 : 0); 
    if (log_counter % 50 == 0) {
        printf("Buzzer: %s\n", ligado ? "ligado" : "desligado");
    }
}


void iniciar_botoes()
{
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);

    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);

<<<<<<< Updated upstream
    gpio_init(JOYSTICK_SW);
    gpio_set_dir(JOYSTICK_SW, GPIO_IN);
    gpio_pull_up(JOYSTICK_SW);

=======
    // Relé concetado na GPIO16, adicionar lógica no .C
>>>>>>> Stashed changes
    gpio_init(rele1);
    gpio_set_dir(rele1, GPIO_OUT); 

    gpio_init(JOYSTICK_SW);
    gpio_set_dir(JOYSTICK_SW, GPIO_IN);
    gpio_pull_up(JOYSTICK_SW);
}

#include "hardware/adc.h"
#define potenciometro 26

uint16_t adc_value_x;
float volume;

void adc_gpio28()
{
    // Leitura dos valores analógicos
    adc_select_input(1);
    adc_value_x = adc_read();
    volume = (adc_value_x / 4095.0) * 100.0;
}
//modificações (intervalo de limites)

#endif