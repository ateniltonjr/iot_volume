#ifndef RELES_H
#define RELES_H

#include "pico/stdlib.h"
#include "pico/bootrom.h"

//Modificações:
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Configuração do pino do buzzer

#define rele1 12
#define BOTAO_A 5
#define JOYSTICK_SW 22
#define BOTAO_B 6
#define JOYSTICK_Y 27
#define LED_G 11 
#define LED_B 12 
#define LED_R 13
#define BUZZER_PIN 10
#define RELE_PIN 16          // PINO PARA CONEXÃO COM O RELÉ NO PROJETO FISICO

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY 3500

// Definição de uma função para inicializar o PWM no pino do buzzer


// ADIÇÃO DO RELÉ PARA CONREXÃO FÍSICA
void init_rele(){
    gpio_init(RELE_PIN);
    gpio_set_dir(RELE_PIN, GPIO_OUT);  
}

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
        gpio_put(RELE_PIN, 1);              // AQUI DEVE SER INSERIDO A CONEXÃO COM O RELÉ PARA CONTROLE
        if (log_counter % 50 == 0) {
            printf("LED RGB: Bomba ligada (verde)\n");
        }
    } else {
        gpio_put(LED_R, 0);
        gpio_put(LED_G, 0);
        gpio_put(LED_B, 0);
        gpio_put(RELE_PIN, 0);              // AQUI DEVE SER INSERIDO A CONEXÃO COM O RELÉ PARA CONTROLE
        if (log_counter % 50 == 0) {
            printf("LED RGB: Bomba desligada\n");
        }
    }
}

void init_buzzer() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint clk_div = clock_get_hz(clk_sys) / (1000 * 4096); // Frequência ~1000Hz
    pwm_set_clkdiv(slice_num, clk_div);
    pwm_set_wrap(slice_num, 4095); // Resolução PWM
    pwm_set_enabled(slice_num, true);
    printf("Buzzer inicializado\n");
}

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

    gpio_init(JOYSTICK_SW);
    gpio_set_dir(JOYSTICK_SW, GPIO_IN);
    gpio_pull_up(JOYSTICK_SW);

    gpio_init(rele1);
    gpio_set_dir(rele1, GPIO_OUT);
}

#include "hardware/adc.h"
#define potenciometro 26
#define Relé 

uint16_t adc_value_x;
float volume;

void adc_gpio28()
{
    // Leitura dos valores analógicos
    adc_select_input(2);                  // Alteração do canal para utilizar o potenciometro na GPIO28
    adc_value_x = adc_read();
    volume = (adc_value_x / 4095.0) * 100.0;
}
//modificações (intervalo de limites)

#endif
