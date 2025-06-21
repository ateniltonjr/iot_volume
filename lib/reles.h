#ifndef RELES_H
#define RELES_H

#include "pico/stdlib.h"
#include "pico/bootrom.h"

//Modificações:
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Configuração do pino do buzzer
#define BUZZER_PIN 21

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY 3500

// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    // Configurar o pino como saída de PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096)); // Divisor de clock
    pwm_init(slice_num, &config, true);

    // Iniciar o PWM no nível baixo
    pwm_set_gpio_level(pin, 0);
}

// Definição de uma função para emitir um beep com duração especificada
void beep(uint pin, uint duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(pin, 2048);

    // Temporização
    sleep_ms(duration_ms);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin, 0);

    // Pausa entre os beeps
    sleep_ms(100); // Pausa de 100ms
}

//

#define rele1 12
#define BOTAO_A 5
#define BOTAO_JOY 22
#define BOTAO_B 6

void gpio_irq_handler(uint gpio, uint32_t events)
{
    reset_usb_boot(0, 0);
}

void iniciar_botoes()
{
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);

    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);

    gpio_init(BOTAO_JOY);
    gpio_set_dir(BOTAO_JOY, GPIO_IN);
    gpio_pull_up(BOTAO_JOY);

    gpio_init(rele1);
    gpio_set_dir(rele1, GPIO_OUT);
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
void alerta_volume() {
     pwm_init_buzzer(BUZZER_PIN);


    if (volume >= 90.0) {
        gpio_put(rele1, 0); // Desliga o relé automaticamente
        beep(BUZZER_PIN, 250); // Bipe de 500ms
        
    } else if (volume <= 85.0 && volume >= 75.0) {
        printf("ALERTA: Volume do reservatório em %.0f%%!\n", volume);
        beep(BUZZER_PIN, 500); // Bipe de 500ms
    }
}

#endif