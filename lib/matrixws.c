#include "lib/matrixws.h"
#include "hardware/pio.h"
#include "lib/ws2818b.pio.h"

npLED_t leds[NUM_LEDS];  
PIO np_pio;             
uint sm;                 
uint8_t brilho_global = BRILHO_PADRAO; 


void set_brilho(uint8_t brilho) {
    brilho_global = (brilho > BRILHO_MAX) ? BRILHO_MAX : brilho;
}


int getIndex(int x, int y) {
    if (x % 2 == 0) { 
        return 24 - (x * 5 + y); 
    } else { 
        return 24 - (x * 5 + (4 - y)); 
    }
}


void bf() {
    for (uint i = 0; i < NUM_LEDS; ++i) {
        
        uint8_t r = (leds[i].R * brilho_global) / BRILHO_MAX;
        uint8_t g = (leds[i].G * brilho_global) / BRILHO_MAX;
        uint8_t b = (leds[i].B * brilho_global) / BRILHO_MAX;
        
        pio_sm_put_blocking(np_pio, sm, r);
        pio_sm_put_blocking(np_pio, sm, g);
        pio_sm_put_blocking(np_pio, sm, b);
    }
    sleep_us(100); 
}


void controle(uint pino) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, true);
    ws2818b_program_init(np_pio, sm, offset, pino, 800000.f);

    desliga(); 
}


void cores(const uint indice, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[indice].R = r;
    leds[indice].G = g;
    leds[indice].B = b;
}


void desliga() {
    for (uint i = 0; i < NUM_LEDS; ++i) {
        cores(i, 0, 0, 0);
    }
    bf();
}


void desenhaMatriz(int mat[5][5][3]) {
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cores(posicao, mat[linha][cols][0], mat[linha][cols][1], mat[linha][cols][2]);
        }
    }
    bf();
}


void sequencia_rgb() {
    
    desliga();
    
    // Sequência vermelho
    for (int i = 0; i < NUM_LEDS; i++) {
        cores(i, BRILHO_MAX, 0, 0);
        bf();
        sleep_ms(100);
    }
    
    // Sequência verde
    for (int i = 0; i < NUM_LEDS; i++) {
        cores(i, 0, BRILHO_MAX, 0);
        bf();
        sleep_ms(100);
    }
    
    // Sequência azul
    for (int i = 0; i < NUM_LEDS; i++) {
        cores(i, 0, 0, BRILHO_MAX);
        bf();
        sleep_ms(100);
    }
    
    // Desliga no final
    desliga();
}