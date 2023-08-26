#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h> //Biblioteca que cria o servico de atualizacão via wifi (ou Over The Air - OTA)
#include <Servo.h>
#include "Updates.h"

// Define o nome e a senha do ponto de acesso Wi-Fi local
const char* ssid = "Chuva_do_Barracao";
const char* password = "darcy12345678";


// Define as variáveis para o controle do servo motor
unsigned long previousMillis = 0;
const long interval = 1000;
int angle = 0;



// Define o pino do servo
#define SERVO_PIN D2

// Define o pino do relé
#define RELAY_PIN D1

// Cria um objeto Servo
Servo motorServo;



// Define a página web para o controle do relé
String relayPage = "<!DOCTYPE html>\
<html>\
  <head>\
    <title>Controle da Chuva</title>\
    <style>\
        body {\
          display: flex;\
          justify-content: center;\
          align-items: center;\
          height: 100vh;\
          margin: 0;\
        }\
        .title {\
          position: fixed;\
          top: 0;\
          left: 0;\
          right: 0;\
          text-align: center;\
          font-size: 28px;\
          margin: 20px 0;\
        }\
        .container {\
          display: flex;\
          justify-content: center;\
          align-items: center;\
          flex-direction: column;\
          margin-top: calc(28px + 40px);\
        }\
        button {\
          background-color: black;\
          color: white;\
          font-size: 40px;\
          padding: 19px 92px;\
          border-radius: 20px;\
          border: 3px solid white;\
          box-sizing: border-box;\
          cursor: pointer;\
          transform: translate(0%, -141%);\
        }\
        button:hover {\
          \}\
        @media screen and (max-width: 1200px) {\
          button {\
            font-size: 114px;\
            padding: 28px 160px;\
            border-radius: 47px;\
            border: 5px solid white;\
          }\
          .title {\
            position: fixed;\
            top: 69px;\
            left: 0px;\
            right: 0;\
            text-align: center;\
            font-size: 72px;\
            margin: 26px 0;\
            text-decoration-line: underline;\
          }\
        }\
    </style>\
    <script>\
      function updateRelayButton() {\
        var httpRequest = new XMLHttpRequest();\
        httpRequest.onreadystatechange = function() {\
          if (httpRequest.readyState === XMLHttpRequest.DONE) {\
            if (httpRequest.status === 200) {\
              var button = document.getElementById('relayButton');\
              if (httpRequest.responseText === 'on') {\
                button.innerHTML = 'Desligar';\
              } else if (httpRequest.responseText === 'off') {\
                button.innerHTML = 'Ligar';\
              }\
            } else {\
              console.log('Erro ao atualizar o botão do relé');\
            }\
          }\
        };\
        httpRequest.open('GET', '/relaystate', true);\
        httpRequest.send();\
      }\
      setInterval(updateRelayButton, 1000);\
      function toggleRelay() {\
        var httpRequest = new XMLHttpRequest();\
        httpRequest.onreadystatechange = function() {\
          if (httpRequest.readyState === XMLHttpRequest.DONE) {\
            if (httpRequest.status === 200) {\
              var button = document.getElementById('relayButton');\
              if (httpRequest.responseText === 'on') {\
                button.innerHTML = 'Desligar';\
              } else if (httpRequest.responseText === 'off') {\
                button.innerHTML = 'Ligar';\
              }\
            } else {\
              console.log('Erro ao acionar o relé');\
            }\
          }\
        };\
        httpRequest.open('POST', '/relay', true);\
        httpRequest.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');\
        httpRequest.send('toggle');\
      }\
    </script>\
  </head>\
  <body onload='updateRelayButton()'>\
    <h1 class='title'>CONTROLE DE CHUVA</h1>\
    <div class='container'>\
      <button id='relayButton' onclick='toggleRelay()'></button>\
    </div>\
  </body>\
</html>";


void setup() {
  // Inicializa a porta serial
  Serial.begin(9600);

  // Cria o ponto de acesso Wi-Fi local
  WiFi.softAP(ssid, password);
  Serial.println("Ponto de acesso criado com sucesso!");

  // Mostra o endereço IP do ponto de acesso Wi-Fi local
  Serial.print("Endereço IP do ponto de acesso painel do botao de desligar e ligar: ");
  Serial.println(WiFi.softAPIP());

  // Inicializa o objeto Servo
  motorServo.attach(SERVO_PIN);

  // Configura o pino do relé como saída e desliga o relé
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("Relé desligado");




  // Configura a página web para o controle do relé
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", relayPage);
  });
  server.on("/relay", HTTP_POST, []() {
    if (server.hasArg("toggle")) {
      if (digitalRead(RELAY_PIN) == LOW) {
        digitalWrite(RELAY_PIN, HIGH);
        server.send(200, "text/plain", "on");
        Serial.println("Relé ligado");
      } else {
        digitalWrite(RELAY_PIN, LOW);
        server.send(200, "text/plain", "off");
        Serial.println("Relé desligado");
      }
    } else {
      server.send(400, "text/plain", "Solicitação inválida");
    }
  });
  server.on("/relaystate", HTTP_GET, []() {
    if (digitalRead(RELAY_PIN) ==  HIGH) {
      server.send(200, "text/plain", "off");
    } else {
      server.send(200, "text/plain", "on");
    }
  });



  // Inicia o servidor web
  InicializaServicoAtualizacao();  // SISTEMA DE ATUALIZAÇÃO WI-FI.
}

void loop() {
// Atualiza o servidor web
  server.handleClient();


  // Verifica se o tempo decorrido desde a última atualização do servo motor é maior que o intervalo definido
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Atualiza o valor do ângulo do servo motor
    angle = random(360) - 180;

    // Define a posição do servo motor
    motorServo.write(angle);

    // Armazena o tempo atual como a última atualização
    previousMillis = currentMillis;
  }
}
