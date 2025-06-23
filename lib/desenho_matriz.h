#ifndef DESENHO_MATRIZ_H
#define DESENHO_MATRIZ_H

#include "string.h"         
#include "matrixws.h"       
#include "pico/stdlib.h"

#define AL    70
#define VE   0
#define VO 0


static inline void atualizar_nivel_na_matriz(int nivel_porcentagem) {
    int mat_temp[5][5][3]; 


    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            mat_temp[y][x][0] = 0; 
            mat_temp[y][x][1] = 0; 
            mat_temp[y][x][2] = 0; 
        }
    }

    
    // 0-19% -> 0 
    // 20-39% -> 1 
    // 40-59% -> 2 
    // 60-79% -> 3 
    // 80-99% -> 4 
    // 100%   -> 5  (full matrix)
    int linhas_para_acender = 0; 

    if (nivel_porcentagem >= 95) {
        linhas_para_acender = 5; 
    } else if (nivel_porcentagem >= 80) {
        linhas_para_acender = 4; 
    } else if (nivel_porcentagem >= 60) {
        linhas_para_acender = 3; 
    } else if (nivel_porcentagem >= 40) {
        linhas_para_acender = 2; 
    } else if (nivel_porcentagem >= 20) {
        linhas_para_acender = 1; 
    }

    for (int i = 0; i < linhas_para_acender; i++) {
        int linha_atual = 4 - i; 
        for (int col = 0; col < 5; col++) {
            
            mat_temp[linha_atual][col][0] = VO; // Red
            mat_temp[linha_atual][col][1] = VE;    // Green
            mat_temp[linha_atual][col][2] = AL;     // Blue
        }
    }

    
    desenhaMatriz(mat_temp);
}

#endif 