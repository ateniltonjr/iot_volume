#ifndef DISPLAY_H
#define DISPLAY_H
#include "ssd1306.h"
#include "font.h"

#define I2C_PORT_DISP i2c1
#define I2C_SDA_DISP 14
#define I2C_SCL_DISP 15
#define endereco 0x3C
ssd1306_t ssd;
bool cor = true;


uint16_t nivel_agua = 0; // 0-100%
bool bomba_ligada = false;
uint16_t lim_min = 10;
uint16_t lim_max = 90;
static uint32_t log_counter = 0;
char ip_str[24];

enum { MENU_PRINCIPAL, TELA_MONITORAMENTO, TELA_LIMITES } estado_menu = MENU_PRINCIPAL;
    
void iniciar_display()
{
    i2c_init(I2C_PORT_DISP, 400 * 1000);
    gpio_set_function(I2C_SDA_DISP, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_DISP, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_DISP);
    gpio_pull_up(I2C_SCL_DISP);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT_DISP);
    ssd1306_config(&ssd);
}

void draw_menu_principal(ssd1306_t *ssd) {
    ssd1306_fill(ssd, false);
    ssd1306_rect(ssd, 0, 0, 128, 64, true, false); // Borda retangular
    ssd1306_draw_string(ssd, "Menu", 48, 8); 
    ssd1306_draw_string(ssd, "Monitorar: A", 10, 32);
    ssd1306_draw_string(ssd, "Limites: B", 10, 48);
}

void draw_tela_monitoramento(ssd1306_t *ssd) {
    ssd1306_fill(ssd, false);
    ssd1306_rect(ssd, 0, 0, 128, 64, true, false); // Borda retangular
    char bomba_str[16];
    snprintf(bomba_str, sizeof(bomba_str), "Bomba: %s", bomba_ligada ? "ON" : "OFF");
    ssd1306_draw_string(ssd, bomba_str, 4, 8); 
    char nivel_str[16];
    snprintf(nivel_str, sizeof(nivel_str), "Nivel: %d%%", nivel_agua);
    ssd1306_draw_string(ssd, nivel_str, 4, 24); 
    uint8_t barra_width = (nivel_agua * 120) / 100; 
    ssd1306_rect(ssd, 40, 4, barra_width, 8, true, true); 
}

void draw_tela_limites(ssd1306_t *ssd) {
    ssd1306_fill(ssd, false);
    ssd1306_rect(ssd, 0, 0, 128, 64, true, false); // Borda retangular
    char min_str[18];
    snprintf(min_str, sizeof(min_str), "Lim MIN: %d%%", lim_min);
    ssd1306_draw_string(ssd, min_str, 4, 8); 
    char max_str[18];
    snprintf(max_str, sizeof(max_str), "Lim MAX: %d%%", lim_max);
    ssd1306_draw_string(ssd, max_str, 4, 24);
    ssd1306_draw_string(ssd, ip_str, 4, 40); 
}

void update_display(ssd1306_t *ssd) {
    switch (estado_menu) {
        case MENU_PRINCIPAL: draw_menu_principal(ssd); break;
        case TELA_MONITORAMENTO: draw_tela_monitoramento(ssd); break;
        case TELA_LIMITES: draw_tela_limites(ssd); break;
    }
    ssd1306_send_data(ssd);
    if (log_counter % 50 == 0) {
        printf("Display OLED atualizado: %s\n", 
               estado_menu == MENU_PRINCIPAL ? "Menu Principal" : 
               estado_menu == TELA_MONITORAMENTO ? "Monitoramento" : "Limites");
        printf("--------------------------------\n");
    }
}

#endif