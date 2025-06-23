// Bibliotecas

#include "lwip/tcp.h"
#include "display.h"
#include "interface.h"
#include "wifi_init.h"
#include "desenho_matriz.h"
#include "matrixws.h"
#include "reles.h"

#define WIFI_SSID "KLAZ"
#define WIFI_PASS "10213250"

char str_x[5], str_v[5]; // Buffer para armazenar a string

void exibicoes_display()
{
    adc_gpio28(); // Chama a função para ler o ADC do GPIO 28

    sprintf(str_x, "%d", adc_value_x);            // Converte o inteiro em string
    sprintf(str_v, "%.0f", volume);
    ssd1306_fill(&ssd, !cor);                     // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor); // Desenha um retângulo
    ssd1306_line(&ssd, 3, 25, 123, 25, cor);      // Desenha uma linha
    ssd1306_line(&ssd, 3, 37, 123, 37, cor);      // Desenha uma linha

    ssd1306_draw_string(&ssd, "CEPEDI   TIC37", 8, 6); // Desenha uma string
    ssd1306_draw_string(&ssd, "EMBARCATECH", 20, 16);  // Desenha uma string
    ssd1306_draw_string(&ssd, ip_str, 10, 28);
    ssd1306_draw_string(&ssd, "X  VOLUME PB", 20, 41);           // Desenha uma string
    ssd1306_line(&ssd, 44, 37, 44, 60, cor);                     // Desenha uma linha vertical
    ssd1306_draw_string(&ssd, str_x, 8, 52);                     // Desenha uma string
    ssd1306_draw_string(&ssd, str_v, 64, 52);        // Desenha uma string
    ssd1306_rect(&ssd, 52, 90, 8, 8, cor, !gpio_get(BOTAO_JOY)); // Desenha um retângulo
    ssd1306_rect(&ssd, 52, 102, 8, 8, cor, !gpio_get(BOTAO_A));  // Desenha um retângulo
    ssd1306_rect(&ssd, 52, 114, 8, 8, cor, !cor);                // Desenha um retângulo
    ssd1306_send_data(&ssd);                                     // Atualiza o display
}

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
        else if (gpio == BOTAO_A)
        {
            flag_toggle_rele = true;
        }
        else if (gpio == BOTAO_JOY)
        {
            flag_toggle_matriz = true;
        }
        tempo_interrupcao = tempo_atual;
    }
}

int main()
{   
    iniciar_display();
    iniciar_botoes();
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BOTAO_JOY, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    stdio_init_all();
    controle(PINO_MATRIZ); // Inicializa a matriz de LEDs
    iniciar_wifi(WIFI_SSID, WIFI_PASS);
    sleep_ms(2000);

    //Buzzer:
    pwm_init_buzzer(BUZZER_PIN); // inicializa o buzzer

    adc_init();
    adc_gpio_init(potenciometro);

    start_http_server(); 
    
    // Ativando as funções
    while (true)
    {
        cyw43_arch_poll();
        exibicoes_display();
        //atualizar_nivel_na_matriz((int)volume);
        alerta_volume();
        // --- Ações dos botões ---
        if (flag_toggle_rele) {
            gpio_put(rele1, !gpio_get(rele1)); // Inverte o estado do relé
            flag_toggle_rele = false;
        }
        if (flag_toggle_matriz) {
            if(estado_matriz == false)
            {
                estado_matriz = true;
                atualizar_nivel_na_matriz((int)volume);
                buzzer_on(); // Ativa o buzzer junto com a matriz
            }
            else
            {
                estado_matriz = false;
                desliga();
                buzzer_off(); // Desativa o buzzer junto com a matriz
            }
            flag_toggle_matriz = false;
        }
        sleep_ms(300);
    }

    cyw43_arch_deinit(); // Desativa o Wi-Fi
    return 0; // Finaliza o programa
}