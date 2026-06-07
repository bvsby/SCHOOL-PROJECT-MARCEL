#include <WiFi.h>
#include <WebServer.h>

// Identifiants Wi-Fi
const char* ssid = "Mbassirou_sgn";
const char* password = "12345678";

// Création du serveur
WebServer server(80);

// === PAGES HTML (à remplir avec vos fichiers plus tard) ===
const char PAGE_INDEX[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>Marcel Controller</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background: linear-gradient(to right, #c1f7d5, #a7d4f5);
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
      padding: 20px;
    }

    .container {
      background: linear-gradient(to right, #a7d4f5, #c1f7d5);
      border-radius: 25px;
      padding: 40px;
      display: flex;
      align-items: center;
      gap: 40px;
      flex-wrap: wrap;
      justify-content: center;
      max-width: 750px;
      width: 90%;
    }

    .logo-block {
      text-align: center;
    }

    .logo-block img {
      width: 150px;
    }

    .logo-block .label {
      margin-top: 10px;
      font-family: Impact, sans-serif;
      font-size: 22px;
      font-style: italic;
    }

    .content-block {
      text-align: center;
    }

    .content-block h1 {
      font-size: 26px;
      font-weight: bold;
      margin-bottom: 10px;
    }

    .content-block p {
      margin: 10px 0 20px;
      font-size: 18px;
    }

    .btn {
      background-color: #004b4f;
      color: white;
      font-weight: bold;
      border: none;
      border-radius: 40px;
      padding: 12px 24px;
      font-size: 16px;
      margin: 10px;
      cursor: pointer;
      transition: background-color 0.3s;
    }

    .btn:hover {
      background-color: #026d73;
    }

    /* Portrait - Mobile */
    @media (max-width: 600px) and (orientation: portrait) {
      .container {
        flex-direction: column;
        padding: 30px 20px;
      }

      .logo-block img {
        width: 120px;
      }

      .content-block h1 {
        font-size: 22px;
      }

      .btn {
        width: 100%;
        max-width: 250px;
      }
    }

    /* Paysage - Mobile */
    @media (max-width: 900px) and (orientation: landscape) {
      .container {
        flex-direction: row;
        gap: 20px;
      }

      .logo-block img {
        width: 130px;
      }

      .btn {
        padding: 10px 20px;
        font-size: 15px;
      }
    }

  </style>
</head>
<body>
  <div class="container">
    <div class="logo-block">
      <img src="https://img.icons8.com/ios-filled/150/000000/truck.png" alt="truck" />
      <div class="label">MARCEL CONTROLLER</div>
    </div>

    <div class="content-block">
      <h1>HELLO<br>CONNECTEZ VOUS<br>A VOTRE ROBOT!!!</h1>
      <p>Connectez vous ici</p>
      <button class="btn" onclick="window.location.href='controller.html'">CONNECT</button>
      <!-- <button class="btn" onclick="window.location.href='bluetooth.html'">BLUETOOTH</> -->
    </div>
  </div>
</body>
</html>
)rawliteral";

const char PAGE_CONTROLLER[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>Contrôle Robot</title>
  <script src="https://cdn.jsdelivr.net/npm/nipplejs@0.9.0/dist/nipplejs.min.js"></script>
  <style>
    body {
      margin: 0;
      padding: 0;
      font-family: Arial, sans-serif;
      background: linear-gradient(to right, #c1f7d5, #a7d4f5);
    }

    header {
      display: flex;
      justify-content: flex-end;
      align-items: center;
      padding: 10px 20px;
      border-bottom-left-radius: 20px;
      border-bottom-right-radius: 20px;
    }

    .battery {
      margin-right: 10px;
      font-size: 19px;
    }

    .disconnect-btn {
      background: #000;
      color: white;
      border: none;
      padding: 8px 12px;
      border-radius: 10px;
      font-size: 19px;
      cursor: pointer;
    }

    .draw {
      background: #000;
      color: white;
      border: none;
      padding: 8px 12px;
      border-radius: 10px;
      font-size: 19px;
      cursor: pointer;
    }

    .container {
      display: flex;
      justify-content: center;
      align-items: center;
      height: calc(100vh - 60px);
      padding: 10px;
      margin-top: 5px;
    }

    .bloc {
      background: white;
      padding: 20px;
      border-radius: 20px;
      box-shadow: 0 0 15px rgba(0,0,0,0.2);
      display: flex;
      width: 100%;
      max-width: 1200px;
      height: 100%;
      flex-direction: row;
      justify-content: space-around;
      align-items: center;
    }

    .left-controls .directions {
      display: grid;
      grid-template-columns: repeat(3, 60px);
      grid-template-rows: repeat(3, 60px);
      gap: 10px;
    }

    .directions button {
      width: 60px;
      height: 60px;
      border-radius: 50%;
      background-color: #000;
      color: white;
      font-size: 22px;
      border: none;
      cursor: pointer;
    }

    .directions button:empty {
      background: none;
      cursor: default;
    }

    .center-buttons {
      display: grid;
      grid-template-columns: 60px 60px;
      grid-template-rows: 60px 60px;
      gap: 20px;
    }

    .center-buttons button {
      background: #222;
      width: 60px;
      height: 60px;
      border-radius: 50%;
      border: none;
      color: white;
      box-shadow: 2px 2px 5px rgba(0,0,0,0.3);
      cursor: pointer;
    }

    .joystick-wrapper {
      width: 180px;
      height: 180px;
      border-radius: 50%;
      border: 16px solid #000;
      background: white;
      position: relative;
    }

    #joystick-zone {
      width: 100%;
      height: 100%;
      border-radius: 50%;
    }

    .mode-switch {
      display: flex;
      justify-content: center;
      margin-top: 20px;
    }

    .switch {
      position: relative;
      display: inline-block;
      width: 60px;
      height: 30px;
      background: #000;
      border-radius: 30px;
    }

    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    .slider {
      position: absolute;
      top: 3px;
      left: 3px;
      width: 24px;
      height: 24px;
      background: red;
      border-radius: 50%;
      transition: transform 0.3s ease;
    }

    input:checked + .slider {
      transform: translateX(30px);
      background: limegreen;
    }

    @media (orientation: portrait) {
      .container {
        flex-direction: column;
        height: auto;
        padding: 10px;
      }

      .bloc {
        flex-direction: column;
        justify-content: space-between;
        align-items: center;
        width: 90%;
        height: 95vh;
      }

      .left-controls, .center-buttons, .right-joystick, .mode-switch {
        margin: 10px 0;
      }

      .mode-switch {
        flex-direction: row;
      }
    }

    @media (orientation: landscape), (min-width: 1025px) {
      .bloc {
        flex-direction: row;
        justify-content: space-between;
        align-items: center;
      }
      .container {
        flex-direction: row !important;
        justify-content: center;
        align-items: center;
      }

      .bloc {
        flex-direction: row !important;
        align-items: center;
        justify-content: center;
        gap: 20px; /* Réduit l’espace entre les éléments */
        padding: 20px;
      }

      .left-controls .directions {
        margin: 0 70px 0 0;
      }

      .right-joystick {
        margin: 0 0 0 70px;
      }

      .center-buttons {
        margin: 30px 0 10px 0;
      }

      .mode-switch {
        margin-bottom: 10px;
      }

      .left-controls {
        display: flex;
        flex-direction: column;
        align-items: center;
      }

      .middle-zone {
        display: flex;
        flex-direction: column;
        justify-content: space-between;
        align-items: center;
        gap: 30px;
      }

      .right-joystick {
        display: flex;
        flex-direction: column;
        align-items: center;
      }
    }
  </style>
</head>
<body>
  <header>
    <a href=Draw.html><button class="draw">D</button></a>
    <div class="battery" id="batteryText">🔋 --%</div>
    <button class="disconnect-btn">⎋</button>
  </header>

  <div class="container">
    <div class="bloc">
      <div class="left-controls">
        <div class="directions">
          <div></div>
          <button onclick="envoyerCommande('F')">↑</button>
          <div></div>
          <button onclick="envoyerCommande('L')">←</button>
          <button onclick="envoyerCommande('S')">■</button>
          <button onclick="envoyerCommande('R')">→</button>
          <div></div>
          <button onclick="envoyerCommande('B')">↓</button>
          <div></div>
        </div>
      </div>

      <div class="middle-zone">
        <div class="mode-switch">
          <label class="switch">
            <input type="checkbox" id="modeToggle">
            <span class="slider"></span>
          </label>
        </div>
        <div class="center-buttons">
          <button>1</button>
          <button>2</button>
          <button>3</button>
          <button>4</button>
          </div>
      </div>

      <div class="right-joystick">
        <div class="joystick-wrapper">
          <div id="joystick-zone"></div>
        </div>
      </div>
    </div>
  </div>

  <script>
    const toggle = document.getElementById("modeToggle");

    toggle.addEventListener("change", () => {
      const mode = toggle.checked ? "auto" : "manuel";
      fetch(`/mode?type=${mode}`)
        .then(res => res.text())
        .then(msg => console.log("Changement mode :", msg))
        .catch(err => console.error("Erreur de mode :", err));
    });


    document.querySelector(".disconnect-btn").addEventListener("click", () => {
      localStorage.removeItem("mode");
      window.location.href = "index.html";
    });
      function envoyerCommande(cmd) {
  fetch(`/commande?cmd=${cmd}`)
    .then(res => res.text())
    .then(msg => console.log("Réponse ESP32 :", msg))
          .catch(err => console.error("Erreur requête :", err));

    }

    const joystick = nipplejs.create({
      zone: document.getElementById('joystick-zone'),
      mode: 'static',
      position: { left: '50%', top: '50%' },
      color: '#004b4f',
      size: 120
    });

    joystick.on('move', (evt, data) => {
      const { angle, distance } = data;
      if (angle && distance > 10) {
        const deg = angle.degree;
        let direction;

        if (deg > 45 && deg <= 135) direction = "F";        // Haut
        else if (deg > 135 && deg <= 225) direction = "L";  // Gauche
        else if (deg > 225 && deg <= 315) direction = "B";  // Bas
        else direction = "R";                               // Droite

        envoyerCommande(direction);
      }
    });


    joystick.on('end', () => {
      envoyerCommande('S');
    });
    function majBatterie() {
      fetch("/battery")
        .then(res => res.text())
        .then(pourcentage => {
          document.getElementById("batteryText").innerText = `🔋 ${pourcentage}%`;
        })
        .catch(() => {
          document.getElementById("batteryText").innerText = "🔋 --%";
        });
    }

    // Mise à jour toutes les 5 secondes
    setInterval(majBatterie, 5000);
    majBatterie(); // premier appel

  </script>
</body>
</html>
)rawliteral";

const char PAGE_DRAW[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Marcel Controller - Trajet</title>
  <style>
    body {
      margin: 0;
      font-family: Arial, sans-serif;
      background: linear-gradient(to right, #c1f7d5, #a7d4f5);
      display: flex;
      height: 100vh;
    }

    .sidebar {
      width: 380px;
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 20px 10px;
    }

    .logo-box {
      background: linear-gradient(to right, #a7d4f5, #c1f7d5);
      border-radius: 25px;
      padding: 20px;
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 10px;
    }

    .logo-box img {
      width: 140px;
    }

    .logo-box span {
      font-weight: bold;
      font-style: italic;
      font-size: 22px;
    }

    .side-buttons {
      display: flex;
      flex-direction: column;
      align-items: center;
      margin-top: 20px;
      gap: 20px;
    }

    .side-buttons input {
      width: 50px;
      height: 35px;
      border-radius: 25px;
      background: #000;
      color: white;
      border: none;
      text-align: center;
    }

    .side-buttons button {
      font-size: 32px;
      background: none;
      border: none;
      cursor: pointer;

      background: #222;
      width: 40px;
      height: 40px;
      border-radius: 50%;
      border: none;
      color: white;
      box-shadow: 2px 2px 5px rgba(0,0,0,0.3);
      cursor: pointer;
    }

    .main {
      flex: 1;
      position: relative;
      margin: 20px;
      background: white;
      border-radius: 25px;
      overflow: hidden;
      margin-top: 70px;
    }

    .topbar {
      position: absolute;
      top: 15px;
      right: 15px;
      display: flex;
      gap: 20px;
      font-size: 24px;
      z-index: 3;
    }

    .topbar button {
      background: #000;
      color: white;
      border: none;
      padding: 6px 10px;
      border-radius: 8px;
      cursor: pointer;
    }

    canvas {
      width: 100%;
      height: 100%;
      display: block;
    }

    .flag {
      position: absolute;
      width: 30px;
      height: 30px;
      pointer-events: auto;
      z-index: 2;
      user-select: none;
      cursor: move;
    }
    /* Portrait mobile */
    @media screen and (max-width: 768px) and (orientation: portrait) {
      body {
        flex-direction: column;
      }

      .sidebar {
        flex-direction: row;
        flex-wrap: wrap;
        width: 100%;
        padding: 10px;
        justify-content: center;
        gap: 10px;
      }

      .logo-box {
        flex-direction: row;
        gap: 10px;
        padding: 10px;
      }

      .logo-box img {
        width: 80px;
      }

      .logo-box span {
        font-size: 16px;
      }

      .side-buttons {
        flex-direction: row;
        flex-wrap: wrap;
        justify-content: center;
        margin: 0;
        gap: 10px;
      }

      .side-buttons input {
        width: 60px;
        height: 30px;
        font-size: 14px;
      }

      .side-buttons button {
        font-size: 24px;
      }

      .main {
        margin: 10px;
        border-radius: 15px;
      }

      .topbar {
        top: 10px;
        right: 10px;
        font-size: 20px;
        gap: 10px;
      }
    }

    /* Landscape mobile */
    @media screen and (max-width: 1024px) and (orientation: landscape) {
      .sidebar {
        width: 220px;
        padding: 10px 5px;
      }

      .logo-box img {
        width: 100px;
      }

      .logo-box span {
        font-size: 18px;
      }

      .side-buttons input {
        width: 60px;
        height: 30px;
      }

      .side-buttons button {
        font-size: 28px;
      }

      .main {
        margin: 10px;
        border-radius: 20px;
      }

      .topbar {
        font-size: 20px;
        gap: 15px;
      }
    }

  </style>
</head>
<body>
  <div class="sidebar">
    <div class="logo-box">
      <img src="https://img.icons8.com/ios-filled/100/000000/truck.png" alt="Truck">
      <span>MARCEL CONTROLLER</span>
    </div>

    <div class="side-buttons">
      <input id="sideInput" placeholder="__m">
      <button id="penBtn" title="Crayon">&#9998;</button>
      <button id="eraserBtn" title="Gomme">&#128396;</button>
      <button id="sendBtn" title="Envoyer">&#8635;</button>
      <button id="backBtn" title="Retour">&#x21A9;</button>
    </div>
  </div>
  <div class="topbar">
    <div class="battery" id="batteryText">🔋 --%</div>
    <button onclick="disconnect()">⎋</button>
  </div>

  <div class="main">
    <canvas id="drawCanvas"></canvas>
    <img id="flag" class="flag" src="flag.png" style="left:50px; top:50px">

  </div>

  <script>
    const canvas = document.getElementById("drawCanvas");
    const ctx = canvas.getContext("2d");
    const flag = document.getElementById("flag");
    let drawing = false;
    let erasing = false;
    let points = [];

    function resizeCanvas() {
      canvas.width = canvas.offsetWidth;
      canvas.height = canvas.offsetHeight;
    }

    window.addEventListener("resize", resizeCanvas);
    resizeCanvas();

    canvas.addEventListener("mousedown", (e) => {
      if (e.target !== canvas) return;
      drawing = true;
      const flagX = parseInt(flag.style.left) + 15;
      const flagY = parseInt(flag.style.top) + 15;
      ctx.beginPath();
      ctx.moveTo(flagX, flagY);
      points.push([{ x: flagX, y: flagY }]);
      draw(e);
    });

    canvas.addEventListener("mouseup", () => {
      drawing = false;
      ctx.beginPath();
    });

    canvas.addEventListener("mousemove", draw);
    // GESTION TACTILE
    canvas.addEventListener("touchstart", (e) => {
      if (e.target !== canvas) return;
      e.preventDefault();
      drawing = true;
      const touch = e.touches[0];
      const rect = canvas.getBoundingClientRect();
      const x = touch.clientX - rect.left;
      const y = touch.clientY - rect.top;
      const flagX = parseInt(flag.style.left) + 15;
      const flagY = parseInt(flag.style.top) + 15;
      ctx.beginPath();
      ctx.moveTo(flagX, flagY);
      points.push([{ x: flagX, y: flagY }]);
      drawTouch(x, y);
    });

    canvas.addEventListener("touchmove", (e) => {
      if (!drawing) return;
      e.preventDefault();
      const touch = e.touches[0];
      const rect = canvas.getBoundingClientRect();
      const x = touch.clientX - rect.left;
      const y = touch.clientY - rect.top;
      drawTouch(x, y);
    });

    canvas.addEventListener("touchend", () => {
      drawing = false;
      ctx.beginPath();
    });

    function drawTouch(x, y) {
      const p = points[points.length - 1];
      p.push({ x, y });
      ctx.lineJoin = "round";
      ctx.lineWidth = 2;
      ctx.strokeStyle = erasing ? "white" : "black";
      ctx.lineTo(x, y);
      ctx.stroke();
      ctx.beginPath();
      ctx.moveTo(x, y);
    }
    flag.addEventListener("touchstart", (e) => {
      isDragging = true;
      e.preventDefault();
    });

    document.addEventListener("touchmove", (e) => {
      if (!isDragging) return;
      const rect = canvas.getBoundingClientRect();
      const touch = e.touches[0];
      let x = touch.clientX - rect.left - 15;
      let y = touch.clientY - rect.top - 15;
      x = Math.max(0, Math.min(canvas.offsetWidth - 30, x));
      y = Math.max(0, Math.min(canvas.offsetHeight - 30, y));
      flag.style.left = `${x}px`;
      flag.style.top = `${y}px`;
    });

    document.addEventListener("touchend", () => {
      isDragging = false;
    });


    function draw(e) {
      if (!drawing) return;
      const x = e.offsetX;
      const y = e.offsetY;
      const p = points[points.length - 1];
      p.push({ x, y });
      ctx.lineJoin = "round";
      ctx.lineWidth = 2;
      ctx.strokeStyle = erasing ? "white" : "black";
      ctx.lineTo(x, y);
      ctx.stroke();
      ctx.beginPath();
      ctx.moveTo(x, y);
    }

    document.getElementById("penBtn").addEventListener("click", () => {
      erasing = false;
      ctx.beginPath();
    });

    document.getElementById("eraserBtn").addEventListener("click", () => {
      erasing = true;
      ctx.beginPath();
    });

    document.getElementById("sendBtn").addEventListener("click", () => {
      const sideLength = parseFloat(document.getElementById("sideInput").value);
      if (!sideLength || points.length === 0) {
        alert("Veuillez entrer un côté et dessiner un parcours.");
        return;
      }
      const pathData = points.map(path => path.map(p => ({
        x: Math.round(p.x / canvas.width * sideLength),
        y: Math.round(p.y / canvas.height * sideLength)
      })));
      const flagStart = {
        x: Math.round(parseInt(flag.style.left) / canvas.offsetWidth * sideLength),
        y: Math.round(parseInt(flag.style.top) / canvas.offsetHeight * sideLength)
      };
      console.log("Trajet à envoyer:", pathData, "Départ:", flagStart);
    });

    document.getElementById("backBtn").addEventListener("click", () => {
      window.location.href = "controller.html";
    });

    function disconnect() {
      localStorage.clear();
      window.location.href = "index.html";
    }

    let isDragging = false;
    flag.addEventListener("mousedown", (e) => {
      isDragging = true;
      e.preventDefault();
    });

    document.addEventListener("mousemove", (e) => {
      if (!isDragging) return;
      const rect = canvas.getBoundingClientRect();
      let x = e.clientX - rect.left - 15;
      let y = e.clientY - rect.top - 15;
      x = Math.max(0, Math.min(canvas.offsetWidth - 30, x));
      y = Math.max(0, Math.min(canvas.offsetHeight - 30, y));
      flag.style.left = `${x}px`;
      flag.style.top = `${y}px`;
    });

    document.addEventListener("mouseup", () => isDragging = false);
    function majBatterie() {
      fetch("/battery")
        .then(res => res.text())
        .then(pourcentage => {
          document.getElementById("batteryText").innerText = `🔋 ${pourcentage}%`;
        })
        .catch(() => {
          document.getElementById("batteryText").innerText = "🔋 --%";
        });
    }

    // Mise à jour toutes les 5 secondes
    setInterval(majBatterie, 5000);
    majBatterie(); // premier appel

  </script>
</body>
</html>
)rawliteral";

// === Déclaration des pins moteurs (adapter selon ton schéma) ===
// === Moteur avant (U11 - L298N) ===
#define AV_IN1 23
#define AV_IN2 22
#define AV_IN3 21
#define AV_IN4 19

// === Moteur arrière (U12 - L298N) ===
#define AR_IN1 18
#define AR_IN2 5
#define AR_IN3 4
#define AR_IN4 2

#define PIN_BATTERIE 36





float lirePourcentageBatterie() {
  int valeurADC = analogRead(PIN_BATTERIE);
  float tension = (valeurADC / 4095.0) * 3.3 * 2; // pont diviseur
  float pourcentage = (tension - 3.2) / (4.2 - 3.2) * 100.0;
  pourcentage = constrain(pourcentage, 0, 100);
  return pourcentage;
}

// === Fonctions moteurs ===
void avancer() {
  digitalWrite(AV_IN1, HIGH); digitalWrite(AV_IN2, LOW);
  digitalWrite(AV_IN3, HIGH); digitalWrite(AV_IN4, LOW);
  digitalWrite(AR_IN1, HIGH); digitalWrite(AR_IN2, LOW);
  digitalWrite(AR_IN3, HIGH); digitalWrite(AR_IN4, LOW);
}

void reculer() {
  digitalWrite(AV_IN1, LOW); digitalWrite(AV_IN2, HIGH);
  digitalWrite(AV_IN3, LOW); digitalWrite(AV_IN4, HIGH);
  digitalWrite(AR_IN1, LOW); digitalWrite(AR_IN2, HIGH);
  digitalWrite(AR_IN3, LOW); digitalWrite(AR_IN4, HIGH);
}

void gauche() {
  digitalWrite(AV_IN1, LOW); digitalWrite(AV_IN2, HIGH);
  digitalWrite(AV_IN3, HIGH); digitalWrite(AV_IN4, LOW);
  digitalWrite(AR_IN1, LOW); digitalWrite(AR_IN2, HIGH);
  digitalWrite(AR_IN3, HIGH); digitalWrite(AR_IN4, LOW);
}

void droite() {
  digitalWrite(AV_IN1, HIGH); digitalWrite(AV_IN2, LOW);
  digitalWrite(AV_IN3, LOW); digitalWrite(AV_IN4, HIGH);
  digitalWrite(AR_IN1, HIGH); digitalWrite(AR_IN2, LOW);
  digitalWrite(AR_IN3, LOW); digitalWrite(AR_IN4, HIGH);
}

void arreter() {
  digitalWrite(AV_IN1, LOW); digitalWrite(AV_IN2, LOW);
  digitalWrite(AV_IN3, LOW); digitalWrite(AV_IN4, LOW);
  digitalWrite(AR_IN1, LOW); digitalWrite(AR_IN2, LOW);
  digitalWrite(AR_IN3, LOW); digitalWrite(AR_IN4, LOW);
}


// === Suiveur de ligne ===
void executerSuiveurLigne() {
  // À implémenter selon capteurs
}

// === Dessin ===
void recevoirDessin() {
  // Lire les données JSON (ex. : coordonnées normalisées)
  // Les convertir en mouvements moteur séquentiels
  server.send(200, "text/plain", "Trajet reçu");
}

// Définition des modes
enum Mode { MODE_MANUEL, MODE_AUTOMATIQUE, MODE_DESSIN };
Mode modeActuel = MODE_MANUEL;


// === Commandes directionnelles ===
void recevoirCommande() {
  if (modeActuel != MODE_MANUEL) return;
  String cmd = server.arg("cmd");

  if (cmd == "F") avancer();
  else if (cmd == "B") reculer();
  else if (cmd == "L") gauche();
  else if (cmd == "R") droite();
  else arreter();

  server.send(200, "text/plain", "OK");
}

// === Changer de mode ===
void changerMode() {
  String type = server.arg("type");
  if (type == "manuel") modeActuel = MODE_MANUEL;
  else if (type == "auto") modeActuel = MODE_AUTOMATIQUE;
  else if (type == "dessin") modeActuel = MODE_DESSIN;
  arreter();
  server.send(200, "text/plain", "Mode changé");
}

// === SETUP ===
void setup() {
  Serial.begin(115200);

  // Moteurs
// Moteurs avant
pinMode(AV_IN1, OUTPUT); pinMode(AV_IN2, OUTPUT);
pinMode(AV_IN3, OUTPUT); pinMode(AV_IN4, OUTPUT);

// Moteurs arrière
pinMode(AR_IN1, OUTPUT); pinMode(AR_IN2, OUTPUT);
pinMode(AR_IN3, OUTPUT); pinMode(AR_IN4, OUTPUT);

arreter();

  // Connexion Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("Connecté. IP : " + WiFi.localIP().toString());

  // Routes HTML
  server.on("/", []() { server.send(200, "text/html", PAGE_INDEX); });
  server.on("/controller", []() { server.send(200, "text/html", PAGE_CONTROLLER); });
  server.on("/draw", []() { server.send(200, "text/html", PAGE_DRAW); });

  // Routes action
  server.on("/commande", recevoirCommande); // ?cmd=F/B/L/R/S
  server.on("/mode", changerMode);          // ?type=manuel/auto/dessin
  server.on("/draw-data", recevoirDessin);  // pour les coordonnées du tracé

  server.begin();

  server.on("/battery", []() {
  float percent = lirePourcentageBatterie();
  server.send(200, "text/plain", String((int)percent));
});

}

// === LOOP ===
void loop() {
  server.handleClient();
  if (modeActuel == MODE_AUTOMATIQUE) {
    executerSuiveurLigne(); // à compléter
  }
}




