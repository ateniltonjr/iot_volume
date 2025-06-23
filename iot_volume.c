// Bibliotecas

#include "lwip/tcp.h"
#include "display.h"
#include "interface.h"
#include "wifi_init.h"
//#include "desenho_matriz.h"
#include "lib/matrixws.h"
#include "reles.h"



#define WS2812_PIN 7     // GPIO para matriz de LEDs WS2812

#define WIFI_SSID "RL"
#define WIFI_PASS "12345678"

char str_x[5], str_v[5]; // Buffer para armazenar a string

void gpio_irq_handler(uint gpio, uint32_t events) {
    static uint32_t last_press = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_press < 200) return;
    last_press = current_time;

    if (gpio == BOTAO_A && events == GPIO_IRQ_EDGE_FALL && estado_menu == MENU_PRINCIPAL) {
        estado_menu = TELA_MONITORAMENTO;
        printf("Botão A: pressionado\n");
    } else if (gpio == BOTAO_B && events == GPIO_IRQ_EDGE_FALL && estado_menu == MENU_PRINCIPAL) {
        estado_menu = TELA_LIMITES;
        printf("Botão B: pressionado\n");
    } else if (gpio == JOYSTICK_SW && events == GPIO_IRQ_EDGE_FALL) {
        estado_menu = MENU_PRINCIPAL;
        printf("Botão Joystick: pressionado, retornando ao menu principal\n");
    }
}

int main() {
    
    stdio_init_all();
    sleep_ms(2000);
    printf("Iniciando sistema de controle de nível...\n");

    iniciar_display();
    iniciar_wifi(WIFI_SSID, WIFI_PASS);
    init_rele(bomba_ligada);

    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled(BOTAO_B, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(JOYSTICK_SW, GPIO_IRQ_EDGE_FALL, true);
    printf("Interrupções dos botões configuradas\n");

    adc_init();
    adc_gpio_init(JOYSTICK_Y);
    printf("ADC e joystick inicializados\n");

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, 0, offset, WS2812_PIN, 800000, false);
    printf("PIO para WS2812 inicializado\n");

    init_led_rgb();
    init_buzzer();
    start_http_server();
    iniciar_botoes();
    printf("--------------------------------\n");

    while (true) {
        cyw43_arch_poll();

        adc_select_input(1);
        uint16_t adc_value = adc_read();
        nivel_agua = (adc_value * 100) / 4095;

        if (nivel_agua < lim_min) {
            bomba_ligada = true;
            if (log_counter % 50 == 0) {
                printf("Bomba ligada: nível abaixo de %d%%\n", lim_min);
            }
        } else if (nivel_agua > lim_max) {
            bomba_ligada = false;
            if (log_counter % 50 == 0) {
                printf("Bomba desligada: nível acima de %d%%\n", lim_max);
            }
        }

        if (log_counter % 50 == 0) {
            printf("Nível de água lido: %d%%\n", nivel_agua);
        }

        set_matriz_nivel(nivel_agua);
        set_led_rgb(bomba_ligada);
        set_buzzer(bomba_ligada);
        update_display(&ssd);

        log_counter++;
        sleep_ms(100);
    }
    cyw43_arch_deinit();
    return 0;
}
