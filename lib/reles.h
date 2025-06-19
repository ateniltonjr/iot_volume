#ifndef RELES_H
#define RELES_H

#include "pico/stdlib.h"
#include "pico/bootrom.h"

#define rele1 16
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

#endif