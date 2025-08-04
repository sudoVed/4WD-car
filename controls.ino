#include <WiFi.h>
#include <WebServer.h>
#include <math.h>

WebServer server(80);

#define ENA 14       // PWM pin for speed control
#define IN1 26       // Motor direction pin 1
#define IN2 27       // Motor direction pin 2
#define ENB 25       // PWM pin for speed control
#define IN3 32       // Motor direction pin 1
#define IN4 33       // Motor direction pin 2

int inputA = 0;
int inputB = 0;

// Your AP credentials
const char* ssid = "SSID";
const char* password = "password";

// HTML page stored in flash
const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">

    <title>ESP32 Car Controller</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Courier New', monospace;
            background: linear-gradient(135deg, #0a0a0a 0%, #1a1a2e 50%, #16213e 100%);
            color: #00ff99;
            height: 100vh;
            overflow: hidden;
            position: relative;
        }

        /* Cyberpunk grid background */
        body::before {
            content: '';
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background-image:
                linear-gradient(rgba(0, 255, 65, 0.03) 1px, transparent 1px),
                linear-gradient(90deg, rgba(0, 255, 65, 0.03) 1px, transparent 1px);
            background-size: 50px 50px;
            z-index: -1;
        }

        .container {
            display: flex;
            flex-direction: column;
            height: 100vh;
            padding: 20px;
            justify-content: space-between;
        }

        .title {
            text-align: center;
            font-size: 2.5em;
            text-shadow: 0 0 20px #00ff99;
            margin-bottom: 20px;
            letter-spacing: 3px;
            animation: glow 2s ease-in-out infinite alternate;
        }

        @keyframes glow {
            from {
                text-shadow: 0 0 20px #00ff99, 0 0 30px #00ff99;
            }

            to {
                text-shadow: 0 0 30px #00ff99, 0 0 40px #00ff99;
            }
        }

        .controls {
            display: flex;
            justify-content: center;
            gap: 60px;
            flex-grow: 1;
            align-items: center;
            max-height: 60vh;
        }

        .slider-container {
            display: flex;
            flex-direction: column;
            align-items: center;
            position: relative;
        }

        .slider-label {
            font-size: 1.2em;
            margin-bottom: 20px;
            text-transform: uppercase;
            letter-spacing: 2px;
            text-shadow: 0 0 10px #00ff99;
        }

        .value-display {
            font-size: 1.5em;
            font-weight: bold;
            margin-bottom: 20px;
            color: #00ccff;
            text-shadow: 0 0 15px #00ccff;
            min-height: 30px;
        }

        /* Vertical slider for base speed */
        .vertical-slider {
            writing-mode: vertical-lr;
            /* IE */
            -webkit-appearance: none;

            /* WebKit */
            width: 40px;
            height: 300px;
            background: linear-gradient(to top,
                    #00ff99 0%,
                    #00ff99 35%,
                    #333 50%,
                    #00ff99 65%,
                    #00ff99 100%);
            outline: none;
            border: 2px solid #00ff99;
            border-radius: 20px;
            box-shadow:
                0 0 20px rgba(0, 255, 65, 0.5),
                inset 0 0 20px rgba(0, 0, 0, 0.5);
        }

        /* Horizontal slider for direction */
        .horizontal-slider {
            -webkit-appearance: none;
            width: 300px;
            height: 40px;
            background: linear-gradient(to right,
                    #00ff99 0%,
                    #00ff99 35%,
                    #333 50%,
                    #00ff99 65%,
                    #00ff99 100%);
            outline: none;
            border: 2px solid #00ff99;
            border-radius: 20px;
            box-shadow:
                0 0 20px rgba(0, 255, 65, 0.5),
                inset 0 0 20px rgba(0, 0, 0, 0.5);
        }

        /* Slider thumb styling */
        .horizontal-slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 30px;
            height: 50px;
            background: linear-gradient(45deg, #00ff99, #00ccff);
            cursor: pointer;
            border-radius: 10px;
            box-shadow: 0 0 15px rgba(0, 255, 65, 0.8);
            border: 2px solid #fff;
        }

        .horizontal-slider::-moz-range-thumb {
            width: 30px;
            height: 50px;
            background: linear-gradient(45deg, #00ff99, #00ccff);
            cursor: pointer;
            border-radius: 10px;
            box-shadow: 0 0 15px rgba(0, 255, 65, 0.8);
            border: 2px solid #fff;
        }

        .vertical-slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 50px;
            height: 30px;
            background: linear-gradient(45deg, #00ff99, #00ccff);
            cursor: pointer;
            border-radius: 10px;
            box-shadow: 0 0 15px rgba(0, 255, 65, 0.8);
            border: 2px solid #fff;
        }

        .reset-button {
            background: linear-gradient(45deg, #ff0040, #ff4080);
            border: 3px solid #ff0040;
            color: white;
            font-size: 1.8em;
            font-weight: bold;
            padding: 20px 40px;
            border-radius: 15px;
            cursor: pointer;
            text-transform: uppercase;
            letter-spacing: 3px;
            box-shadow:
                0 0 30px rgba(255, 0, 64, 0.6),
                inset 0 0 20px rgba(255, 255, 255, 0.1);
            transition: all 0.3s ease;
            margin: 20px auto;
            display: block;
            font-family: 'Courier New', monospace;
        }

        .reset-button:hover {
            background: linear-gradient(45deg, #ff4080, #ff0040);
            box-shadow:
                0 0 40px rgba(255, 0, 64, 0.8),
                inset 0 0 30px rgba(255, 255, 255, 0.2);
            transform: translateY(-2px);
        }

        .reset-button:active {
            transform: translateY(0px);
            box-shadow:
                0 0 20px rgba(255, 0, 64, 0.6),
                inset 0 0 15px rgba(0, 0, 0, 0.3);
        }

        /* Mobile responsiveness */
        @media (max-width: 768px) {
            .controls {
                flex-direction: column;
                gap: 30px;
            }

            .title {
                font-size: 2em;
            }

            .horizontal-slider {
                width: 250px;
            }

            .vertical-slider {
                height: 250px;
            }
        }

        /* Status indicators */
        .status-indicator {
            position: absolute;
            top: 10px;
            right: 10px;
            width: 15px;
            height: 15px;
            border-radius: 50%;
            background: #00ff99;
            box-shadow: 0 0 15px #00ff99;
            animation: pulse 1.5s infinite;
        }

        @keyframes pulse {
            0% {
                opacity: 1;
            }

            50% {
                opacity: 0.5;
            }

            100% {
                opacity: 1;
            }
        }
    </style>
</head>

<body>
    <div class="status-indicator"></div>

    <div class="container">


        <div class="controls">
            <!-- Base Speed Control (Vertical) -->
            <div class="slider-container">
                <div class="slider-label">Base Speed</div>
                <input type="range" id="baseSpeed" class="vertical-slider" min="-100" max="100" value="0"
                    orient="vertical">
            </div>

            <!-- Direction Control (Horizontal) -->
            <div class="slider-container">
                <div class="slider-label">Direction</div>
                <input type="range" id="direction" class="horizontal-slider" min="-100" max="100" value="0">
            </div>
        </div>
        <div class="value-display">
            <button class="reset-button" id="resetButton">RESET ALL</button>
        </div>

        <script>
            const baseSpeedSlider = document.getElementById('baseSpeed');
            const directionSlider = document.getElementById('direction');
            const resetButton = document.getElementById('resetButton');

            let baseSpeedValue = 0;
            let directionValue = 0;
            let directionTimeout;


            // Base speed slider - maintains position
            baseSpeedSlider.addEventListener('input', function () {
                baseSpeedValue = parseInt(this.value);
                sendControlData();
            });

            // Direction slider - returns to 0 after release
            directionSlider.addEventListener('input', function () {
                directionValue = parseInt(this.value);
                sendControlData();

                // Clear existing timeout
                clearTimeout(directionTimeout);
            });

            // Return direction to 0 when user stops interacting
            directionSlider.addEventListener('mouseup', returnDirectionToZero);
            directionSlider.addEventListener('touchend', returnDirectionToZero);
            directionSlider.addEventListener('mouseleave', returnDirectionToZero);

            function returnDirectionToZero() {
                directionTimeout = setTimeout(() => {
                    directionValue = 0;
                    directionSlider.value = 0;
                    sendControlData();
                }, 50); // Small delay to feel natural
            }

            // Reset button
            resetButton.addEventListener('click', function () {
                baseSpeedValue = 0;
                directionValue = 0;
                baseSpeedSlider.value = 0;
                directionSlider.value = 0;
                sendControlData();

                // Visual feedback
                this.style.transform = 'scale(0.95)';
                setTimeout(() => {
                    this.style.transform = 'scale(1)';
                }, 150);
            });





            // Function to send control data to ESP32
            function sendControlData() {
                const xhttp = new XMLHttpRequest();
                xhttp.open("GET", `/slider?val1=${baseSpeedValue}&val2=${directionValue}`, true);
                xhttp.send();
            }



            // Prevent context menu on long press (mobile)
            document.addEventListener('contextmenu', function (e) {
                e.preventDefault();
            });
        </script>
</body>

</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  // Create WiFi AP
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);  // usually 192.168.4.1

  // Route for root / page
  server.on("/", []() {
    server.send_P(200, "text/html", MAIN_page);
  });

  server.on("/slider", []() {
  inputA = server.arg("val1").toInt();
  inputB = server.arg("val2").toInt();
  server.send(200, "text/plain", "OK");
  });

  server.begin();
  Serial.println("HTTP server started");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  ledcAttach(ENA, 20000, 8);
  ledcAttach(ENB, 20000, 8);


}

int clamp(int value, int min2 = 100, int max2 = 255) {
  return max(min2, min(max2, value));
}

int clampn(int value, int min2 = -255, int max2 = -100) {
  return max(min2, min(max2, value));
}


void loop() {
  server.handleClient();
  int baseSpeedValue = inputA;
  int directionValue = inputB;
  int outputA = 0;
  int outputB = 0;
  int outputAB = 0;

                // Calculate outputs based on base speed and direction
  if (-baseSpeedValue > 0) {
    outputA = 100 + (-baseSpeedValue * 1.55);
    outputB = 100 + (-baseSpeedValue * 1.55);
    outputAB = outputA;
  } 
  else if (-baseSpeedValue < 0) {
    outputA = (-baseSpeedValue * 1.55) - 100;
    outputB = (-baseSpeedValue * 1.55) - 100;
    outputAB = outputB;
  }
  else {
    outputA = 0;
    outputB = 0;
    outputAB = 0;
  }


  if (directionValue < 0 && outputAB > 0) {
    outputA -= abs(directionValue) * 5.1;            //Forward left turn
  }
  else if (directionValue > 0 && outputAB > 0) {
    outputB -= abs(directionValue) * 5.1;            //Forward right turn
  }
  if (directionValue < 0 && outputAB < 0) {
    outputA += abs(directionValue) * 5.1;            //Backward left turn
  }
  else if (directionValue > 0 && outputAB < 0) {
    outputB += abs(directionValue) * 5.1;            //Backward right turn
  }


  outputA = ceil(outputA);
  outputB = ceil(outputB);

  if (outputA < 0) {
    outputA = clampn(outputA);
  } 
  else {
    outputA = clamp(outputA);
  }
  if (outputB < 0) {
    outputB = clampn(outputB);
  } 
  else {
    outputB = clamp(outputB);
  }

  if (abs(outputA) == 100) {
    outputA = 0;
  }
  if (abs(outputB) == 100) {
    outputB = 0;
  }

  if (outputA == -254) {
    outputA = -255;
  }
  if (outputB == -254) {
    outputB = -255;
  }

  if (outputA > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    ledcWrite(ENA, abs(outputA));
  }
  else if (outputA < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    ledcWrite(ENA, abs(outputA));
  }
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    ledcWrite(ENA, 0);
  }

  if (outputB > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    ledcWrite(ENB, abs(outputB));
  }
  else if (outputB < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    ledcWrite(ENB, abs(outputB));
  }
  else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    ledcWrite(ENB, 0);
  }

  Serial.println(String(outputA) + ", " + String(outputB));

}
