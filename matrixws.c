#include "matrixws.h"
#include "pico/stdlib.h" // Para sleep_us

// Variável estática para armazenar o último valor de abertura
static float ultima_abertura = -1.0f;

// Função auxiliar para enviar um pixel à matriz WS2812
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u); // Envia o valor RGB ao PIO
}

// Função auxiliar para converter valores RGB em formato de 32 bits para a matriz WS2812
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b); // Combina R, G e B em um único valor
}

void set_matriz_nivel(uint16_t valor) {
    int linhas_acesas = (valor * 5) / 100;
    uint32_t pixels[25] = {0};
    int pixel_map[5][5] = {
        {24, 23, 22, 21, 20},
        {15, 16, 17, 18, 19},
        {14, 13, 12, 11, 10},
        {5,  6,  7,  8,  9},
        {4,  3,  2,  1,  0}
    };
    for (int i = 4; i >= (4 - linhas_acesas); i--) {
        for (int j = 0; j < 5; j++) {
            pixels[pixel_map[i][j]] = urgb_u32(0, 0, 5); // 
        }
    }
    for (int i = 0; i < 25; i++) {
        put_pixel(pixels[i]);
    }
}