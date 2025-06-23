#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "reles.h" 


// Variáveis globais

// Definição de HTML_BODY 
const char *HTML_BODY =
"<!DOCTYPE html>"
"<html lang='pt-BR'>\n"
"<head>\n"
"  <meta charset='UTF-8'>\n"
"  <title>Controle e Monitoramento de Nível de Água</title>\n"
"  <style>\n"
"    body { font-family: sans-serif; text-align: center; padding: 10px; margin: 0; background: #f0f0f0; color: #333; }\n"
"    .container { display: flex; flex-direction: column; align-items: center; }\n"
"    .content { display: flex; justify-content: center; width: 100%; }\n"
"    .tanque { width: 100px; height: 300px; background: #ccc; border: 2px solid #333; position: relative; overflow: hidden; margin: 20px; }\n"
"    .nivel { width: 100%; background: #007BFF; position: absolute; bottom: 0; transition: height 0.5s ease; z-index: 1; }\n"
"    .boia { width: 20px; height: 20px; background: #ffff00; border-radius: 50%; position: absolute; left: 40px; transition: transform 0.5s ease; z-index: 2; }\n"
"    .info { margin-left: 20px; text-align: left; }\n"
"    .botao { font-size: 18px; padding: 8px 20px; margin: 5px; border: none; border-radius: 5px; background: #007BFF; color: white; cursor: pointer; }\n"
"    .label { font-weight: bold; margin: 5px 0; }\n"
"    input { padding: 5px; width: 80px; margin: 5px; }\n"
"  </style>\n"
"  <script>\n"
"    function saveLimits() {\n"
"      const min = document.getElementById('min_limit').value;\n"
"      const max = document.getElementById('max_limit').value;\n"
"      fetch(`/set_limits?min=${min}&max=${max}`);\n"
"    }\n"
"    function atualizar() {\n"
"      fetch('/update')\n"
"        .then(res => res.json())\n"
"        .then(data => {\n"
"          document.getElementById('nivel').innerText = data.nivel + '%';\n"
"          const bombaStatus = document.getElementById('bomba_status');\n"
"          bombaStatus.innerText = data.bomba ? 'Ligada' : 'Desligada';\n"
"          const nivelHeight = (data.nivel / 100) * 300;\n"
"          document.getElementById('nivel_barra').style.height = nivelHeight + 'px';\n"
"          document.getElementById('boia').style.transform = `translateY(${300 - nivelHeight - 10}px)`;\n"
"        });\n"
"    }\n"
"    setInterval(atualizar, 1000);\n"
"  </script>\n"
"</head>\n"
"<body>\n"
"  <div class='container'>\n"
"    <h2>Controle e Monitoramento de Nível de Água</h2>\n"
"    <div class='content'>\n"
"      <div>\n"
"        <div class='label'>Nível: <span id='nivel'>--%</span></div>\n"
"        <div class='tanque'>\n"
"          <div id='nivel_barra' class='nivel'></div>\n"
"          <div id='boia' class='boia'></div>\n"
"        </div>\n"
"      </div>\n"
"      <div class='info'>\n"
"        <div class='label'>Bomba: <span id='bomba_status'>--</span></div>\n"
"        <div class='label'>Limite Mínimo: <input type='number' id='min_limit' min='0' max='100' value='10'></div>\n"
"        <div class='label'>Limite Máximo: <input type='number' id='max_limit' min='0' max='100' value='90'></div>\n"
"        <button class='botao' onclick='saveLimits()'>Salvar</button>\n"
"      </div>\n"
"    </div>\n"
"  </div>\n"
"</body>\n"
"</html>";


// Webserver
struct http_state {
    char response[4096];
    size_t len;
    size_t sent;
};

static err_t http_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    struct http_state *hs = (struct http_state *)arg;
    hs->sent += len;
    if (hs->sent >= hs->len) {
        tcp_close(tpcb);
        free(hs);
    }
    return ERR_OK;
}

static err_t http_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        return ERR_OK;
    }
    char *req = (char *)p->payload;
    struct http_state *hs = malloc(sizeof(struct http_state));
    if (!hs) {
        pbuf_free(p);
        tcp_close(tpcb);
        return ERR_MEM;
    }
    hs->sent = 0;

    if (strstr(req, "GET /set_limits")) {
        char *min_str = strstr(req, "min=");
        char *max_str = strstr(req, "max=");
        if (min_str && max_str) {
            min_str += 4;
            max_str += 4;
            lim_min = atoi(min_str);
            lim_max = atoi(max_str);
            if (log_counter % 50 == 0) {
                printf("Requisição: limites atualizados - Min: %d%%, Max: %d%%\n");
            }
        }
        const char *txt = "Limites atualizados";
        hs->len = snprintf(hs->response, sizeof(hs->response),
                           "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
                           (int)strlen(txt), txt);
    } else if (strstr(req, "GET /update")) {
        char json_payload[96];
        int json_len = snprintf(json_payload, sizeof(json_payload), "{\"nivel\":%d,\"bomba\":%d}\r\n", nivel_agua, bomba_ligada);
        hs->len = snprintf(hs->response, sizeof(hs->response),
                           "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
                           json_len, json_payload);
    } else {
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

static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, http_recv);
    return ERR_OK;
}

static void start_http_server(void) {
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb || tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) {
        printf("Erro ao iniciar servidor\n");
        return;
    }
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, connection_callback);
    printf("Servidor HTTP iniciado na porta 80\n");
}

#endif