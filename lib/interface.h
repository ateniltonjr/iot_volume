#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "reles.h" 

int minLevel = 20; // Nível mínimo padrão
int maxLevel = 80; // Nível máximo padrão

const char HTML_BODY[] =
    "<!DOCTYPE html>"
    "<html lang='pt-BR'>"
    "<head><meta charset='UTF-8'>"
    "<title> Controle de Nível </title>"

    "<style>"
        "body{font-family:sans-serif;"
        "text-align:center;margin:0;"
        "background:#f0f0f0}.tank{width:150px;"
        "height:250px;background:#b0c4de;border:2px solid #bdc3c7;"
        "margin:20px auto;position:relative}.water-level{width:100%;"
        "background:#3498db;position:absolute;"
        "bottom:0}.level-display{position:absolute;bottom:5px;"
        "width:100%;color:white;font-weight:bold;"
        "text-shadow:1px 1px 2px black}.pump{width:80px;"
        "height:40px;margin:10px auto;background:#e74c3c;"
        "border:2px solid #bdc3c7;text-align:center;"
        "line-height:40px}.pump.on{background:#2ecc71}.pump-status{margin:5px;"
        "font-weight:bold}.config{margin:10px}.config label{margin-right:5px}.config input{padding:5px;"
        "border:1px solid #bdc3c7}"
    "</style>"

    "<script>"
        "function sendCommand(cmd){fetch('/control?cmd='+cmd)}"
        "function atualizar(){fetch('/estado').then(res=>res.json()).then(data=>"
        "{document.getElementById('waterLevel').style.height=data.nivel+'%';"
        "document.getElementById('currentLevel').innerText='Nível: '+data.nivel+'%';"
        "const pump=document.getElementById('pump');pump.className='pump'+(data.pump?' on':'');"
        "document.getElementById('pumpStatus').innerText='Bomba: '+(data.pump?'Ligada':'Desligada');"
        "pump.innerText=data.pump?'Bomba ON':'Bomba'})}setInterval(atualizar,500)"
    "</script>"

        "</head>"
            "<body>"
                "<h1>"
                    "Controle de Nível</h1>"
                    "<div class='tank'>"
                    "<div class='water-level' id='waterLevel'>"
                    "</div><div class='level-display' id='currentLevel'>Nível: 0%</div>"
                    "</div><div class='pump' id='pump'>Bomba"
                    "</div>"
                    "<div class='pump-status' id='pumpStatus'>Bomba: Desligada</div>"
                    "<div class='config'><label for='minLevel'>Mínimo (%):</label>"
                    "<input type='number' id='minLevel' min='0' max='100' value='20' onchange=\"sendCommand('min='+this.value)\">"
                    "<label for='maxLevel'>Máximo (%):</label>"
                    "<input type='number' id='maxLevel' min='0' max='100' value='80' onchange=\"sendCommand('max='+this.value)\">"
            "</div>"
        "</body>"
    "</html>";

struct http_state
{
    char response[2048]; // Reduzido para aliviar memória
    size_t len;
    size_t sent;
};

static err_t http_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    struct http_state *hs = (struct http_state *)arg;
    hs->sent += len;
    if (hs->sent >= hs->len)
    {
        tcp_close(tpcb);
        free(hs);
    }
    return ERR_OK;
}

static err_t http_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (!p)
    {
        tcp_close(tpcb);
        return ERR_OK;
    }

    char *req = (char *)p->payload;
    struct http_state *hs = malloc(sizeof(struct http_state));
    if (!hs)
    {
        pbuf_free(p);
        tcp_close(tpcb);
        return ERR_MEM;
    }
    hs->sent = 0;

    printf("Requisição recebida: %s\n", req ? req : "NULL"); // Debug: Exibe a requisição completa

    char *controlStart = strstr(req, "GET /control?");
    if (controlStart)
    {
        printf("Encontrou /control na requisição\n");
        char *paramStart = strstr(controlStart, "min=");
        if (paramStart)
        {
            printf("Encontrou parâmetro min\n");
            paramStart += 4; // Pula "min="
            char *paramEnd = strchr(paramStart, ' ');
            if (paramEnd)
            {
                *paramEnd = '\0';
                minLevel = atoi(paramStart);
                if (minLevel < 0) minLevel = 0;
                if (minLevel > 100) minLevel = 100;
                printf("Atualizado minLevel para: %d\n", minLevel); // Debug: Confirma o novo valor
                const char *txt = "Mínimo Atualizado";
                hs->len = snprintf(hs->response, sizeof(hs->response),
                                   "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
                                   (int)strlen(txt), txt);
            }
            else
            {
                printf("Erro: Não encontrou fim do parâmetro min\n");
                const char *txt = "Erro no parâmetro";
                hs->len = snprintf(hs->response, sizeof(hs->response),
                                   "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
                                   (int)strlen(txt), txt);
            }
        }
        else
        {
            paramStart = strstr(controlStart, "max=");
            if (paramStart)
            {
                printf("Encontrou parâmetro max\n");
                paramStart += 4; // Pula "max="
                char *paramEnd = strchr(paramStart, ' ');
                if (paramEnd)
                {
                    *paramEnd = '\0';
                    maxLevel = atoi(paramStart);
                    if (maxLevel < 0) maxLevel = 0;
                    if (maxLevel > 100) maxLevel = 100;
                    printf("Atualizado maxLevel para: %d\n", maxLevel); // Debug: Confirma o novo valor
                    const char *txt = "Máximo Atualizado";
                    hs->len = snprintf(hs->response, sizeof(hs->response),
                                       "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
                                       (int)strlen(txt), txt);
                }
                else
                {
                    printf("Erro: Não encontrou fim do parâmetro max\n");
                    const char *txt = "Erro no parâmetro";
                    hs->len = snprintf(hs->response, sizeof(hs->response),
                                       "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
                                       (int)strlen(txt), txt);
                }
            }
        }
    }
    else if (strstr(req, "GET /estado"))
{
    alerta_volume(); // Verifica alertas e desliga relé se necessário
    adc_gpio28(); // Lê o ADC do GPIO 28
    int nivel = (adc_value_x * 100) / 4095;
    int pumpState = gpio_get(rele1); // Estado do relé (bomba)

    char json_payload[64];
    int json_len = snprintf(json_payload, sizeof(json_payload), "{\"nivel\":%d,\"pump\":%d}", nivel, pumpState);
    hs->len = snprintf(hs->response, sizeof(hs->response),
                       "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
                       json_len, json_payload);
}
    else if (strstr(req, "GET /config"))
    {
        char json_payload[32];
        int json_len = snprintf(json_payload, sizeof(json_payload), "{\"minLevel\":%d,\"maxLevel\":%d}", minLevel, maxLevel);
        hs->len = snprintf(hs->response, sizeof(hs->response),
                           "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
                           json_len, json_payload);
    }
    else
    {
        hs->len = snprintf(hs->response, sizeof(hs->response),
                           "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
                           (int)strlen(HTML_BODY), HTML_BODY);
    }

    tcp_arg(tpcb, hs);
    tcp_sent(tpcb, http_sent);
    tcp_write(tpcb, hs->response, hs->len, TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);
    pbuf_free(p);
    return ERR_OK;
}

static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    tcp_recv(newpcb, http_recv);
    return ERR_OK;
}

static void start_http_server(void)
{
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb)
        return;
    if (tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK)
        return;
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, connection_callback);
    printf("Servidor HTTP rodando na porta 80...\n");
}
#endif