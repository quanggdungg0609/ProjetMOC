const char index_html[] PROGMEM = R"rawliteral(
  <html>
    <head>
      <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
    </head>
    <body>
      <h1>Hello from ESP32!</h1>
      <div class="card">
        <p><i class="" style="color:#059e8a;"></i> ETAT CAPTEUR</p>
        <p><span class="reading"><span id="etatCapteur">%ETATCAPTEUR%</span></span></p>
      </div>

       <div class="card">
        <p><i class="" style="color:#059e8a;"></i> ETAT HARDWARE</p>
        <p><span class="reading"><span id="etatHW">%ETATHW%</span></span></p>
      </div>

      <div class="card">
        <p><i class="" style="color:#059e8a;"></i>DISTANCE</p>
        <p><span class="reading"><span id="distance">%DISTANCE%</span></span></p>
      </div>

      <script>
        function updateValues(etatCapteur, etatHardware, distanceValue) {
            document.getElementById('etatCapteur').innerText = etatCapteur;
            document.getElementById('etatHW').innerText = etatHardware;
            document.getElementById('distance').innerText = distanceValue;
        }

        function fetchData() {
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (xhr.readyState == 4 && xhr.status == 200) {
                    var data = JSON.parse(xhr.responseText);
                    updateValues(data.etatCapteur, data.etatHardware, data.distanceValue);
                }
            };
            xhr.open("GET", "/data", true);
            xhr.send();
        }

        // Update values every second
        setInterval(fetchData, 1000);
      </script>
    </body>
  </html>
)rawliteral";