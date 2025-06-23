#ifndef RELES_H
#define RELES_H

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "buzzer.h"

// Definição do pino do relé
#define rele1 12
#define BOTAO_A 5
#define BOTAO_JOY 22
#define BOTAO_B 6

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

bool estado_matriz = false;

//modificações (intervalo de limites)
void alerta_volume() {
    if (!estado_matriz) {
        buzzer_off(); // Garante que o buzzer fique desligado se a matriz estiver desligada
        return;
    }
    pwm_init_buzzer(BUZZER_PIN);
    if (volume >= 90.0) {
        gpio_put(rele1, 0); // Desliga o relé automaticamente
        beep(BUZZER_PIN, 250); // Bipe de 250ms
    } else if (volume <= 85.0 && volume >= 75.0) {
        printf("ALERTA: Volume do reservatório em %.0f%%!\n", volume);
        beep(BUZZER_PIN, 500); // Bipe de 500ms
    } else if(volume < 20) {
        gpio_put(rele1, 1); // Liga o relé automaticamente
        printf("Reservatório vazio!\n");
        beep(BUZZER_PIN, 1000); // Bipe de 1000ms
    }
}

// Configuração do debounce para os botões
#define debounce_delay 300
volatile uint tempo_interrupcao = 0;
volatile bool flag_toggle_rele = false;
volatile bool flag_toggle_matriz = false;

void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint tempo_atual = to_ms_since_boot(get_absolute_time());
    if (tempo_atual - tempo_interrupcao > debounce_delay)
    {
        if(gpio == BOTAO_B)
        {
            reset_usb_boot(0, 0);
        }
        else if (gpio == BOTAO_JOY)
        {
            flag_toggle_rele = true;
        }
        else if (gpio == BOTAO_A)
        {
            flag_toggle_matriz = true;
        }
        tempo_interrupcao = tempo_atual;
    }
}

#endif