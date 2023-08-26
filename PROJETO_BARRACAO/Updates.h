                   /* // // // // //   //          //
                          //          //          //
                         //          //          //
                        //          // // // // //
                       //          //          //
                      //          //          //
                     //          //          //     */
                  
                  
                  /* Configurações de Update/Atualizaçoes! */
                  
               /* CONFIGURAÇOES Updates/Atualizçoes code ! !*/

                  //   CRÉDITOS: © 2022 Thiago Macedo.   \\


unsigned long ultimoAcessoHost = 0;
ESP8266HTTPUpdateServer atualizadorOTA; //Este e o objeto que permite atualizacao do programa via wifi (OTA)
ESP8266WebServer server(80); //Servidor Web na porta 80


//Esta e a pagina enviada para o navegador de internet
String paginaWeb = ""
"<!DOCTYPE html><html><head><title>Controle de Fita de LED RGB</title>"
"<meta name='mobile-web-app-capable' content='yes' />"
"<meta name='viewport' content='width=device-width' />"
"</head><body style='margin: 0px; padding: 0px;'>"
"<canvas id='colorspace'></canvas>"
"</body>"
"<script type='text/javascript'>"
"(function () {"
" var canvas = document.getElementById('colorspace');"
" var ctx = canvas.getContext('2d');"
" function drawCanvas() {"
" var colours = ctx.createLinearGradient(0, 0, window.innerWidth, 0);"
" for(var i=0; i <= 360; i+=10) {"
" colours.addColorStop(i/360, 'hsl(' + i + ', 100%, 50%)');"
" }"
" ctx.fillStyle = colours;"
" ctx.fillRect(0, 0, window.innerWidth, window.innerHeight);"
" var luminance = ctx.createLinearGradient(0, 0, 0, ctx.canvas.height);"
" luminance.addColorStop(0, '#ffffff');"
" luminance.addColorStop(0.05, '#ffffff');"
" luminance.addColorStop(0.5, 'rgba(0,0,0,0)');"
" luminance.addColorStop(0.95, '#000000');"
" luminance.addColorStop(1, '#000000');"
" ctx.fillStyle = luminance;"
" ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);"
" }"
" var eventLocked = false;"
" function handleEvent(clientX, clientY) {"
" if(eventLocked) {"
" return;"
" }"
" function colourCorrect(v) {"
" return Math.round(1023-(v*v)/64);"
" }"
" var data = ctx.getImageData(clientX, clientY, 1, 1).data;"
" var params = ["
" 'r=' + colourCorrect(data[0]),"
" 'g=' + colourCorrect(data[1]),"
" 'b=' + colourCorrect(data[2])"
" ].join('&');"
" var req = new XMLHttpRequest();"
" req.open('POST', '?' + params, true);"
" req.send();"
" eventLocked = true;"
" req.onreadystatechange = function() {"
" if(req.readyState == 4) {"
" eventLocked = false;"
" }"
" }"
" }"
" canvas.addEventListener('click', function(event) {"
" handleEvent(event.clientX, event.clientY, true);"
" }, false);"
" canvas.addEventListener('touchmove', function(event){"
" handleEvent(event.touches[0].clientX, event.touches[0].clientY);"
"}, false);"
" function resizeCanvas() {"
" canvas.width = window.innerWidth;"
" canvas.height = window.innerHeight;"
" drawCanvas();"
" }"
" window.addEventListener('resize', resizeCanvas, false);"
" resizeCanvas();"
" drawCanvas();"
" document.ontouchmove = function(e) {e.preventDefault()};"
" })();"
"</script></html>";



void InicializaServicoAtualizacao() {
  atualizadorOTA.setup(&server);
  server.begin();
  Serial.println("O servico de atualizacao remota (OTA) Foi iniciado com sucesso! Abra http://");
  Serial.println(WiFi.softAPIP());
  Serial.println(".local/update no seu browser para iniciar a o serviço de atualizacao\n");
}
