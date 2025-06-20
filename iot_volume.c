#include "lwip/tcp.h"
#include "display.h"
#include "interface.h"
#include "wifi_init.h"
#include "desenho_matriz.h"

#define WIFI_SSID "nome_wifi"
#define WIFI_PASS "senha_wifi"
#define SENSOR_PIN 26 // Pino ADC para o potenciômetro

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

int main()
{
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    stdio_init_all();
    sleep_ms(2000);

    gpio_init(PUMP_PIN);
    gpio_set_dir(PUMP_PIN, GPIO_OUT);

    adc_init();
    adc_gpio_init(SENSOR_PIN);

    i2c_init(I2C_PORT_DISP, 400 * 1000);
    gpio_set_function(I2C_SDA_DISP, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_DISP, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_DISP);
    gpio_pull_up(I2C_SCL_DISP);

    ssd1306_t ssd;
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT_DISP);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Iniciando Wi-Fi", 0, 0);
    ssd1306_draw_string(&ssd, "Aguarde...", 0, 30);
    ssd1306_send_data(&ssd);

    if (cyw43_arch_init())
    {
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, "WiFi => FALHA", 0, 0);
        ssd1306_send_data(&ssd);
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 200000))
    {
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, "WiFi => ERRO", 0, 0);
        ssd1306_send_data(&ssd);
        return 1;
    }

    uint8_t *ip = (uint8_t *)&(cyw43_state.netif[0].ip_addr.addr);
    char ip_str[24];
    snprintf(ip_str, sizeof(ip_str), "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "WiFi => OK", 0, 0);
    ssd1306_draw_string(&ssd, ip_str, 0, 10);
    ssd1306_send_data(&ssd);

    start_http_server();

    while (true)
    {
        cyw43_arch_poll();

        adc_select_input(0);
        uint16_t sensorValue = adc_read();
        int nivel = (sensorValue * 100) / 4095;
        int pumpState = gpio_get(PUMP_PIN);

        // Controle automático da bomba com base nos níveis atualizados
        if (nivel <= minLevel && !pumpState)
        {
            gpio_put(PUMP_PIN, 1);
            pumpState = 1;
        }
        else if (nivel >= maxLevel && pumpState)
        {
            gpio_put(PUMP_PIN, 0);
            pumpState = 0;
        }

        sleep_ms(500); // Reduzido para aliviar o processador
    }

    cyw43_arch_deinit();
    return 0;
}