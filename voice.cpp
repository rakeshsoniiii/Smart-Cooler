<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
    <title>Voice Control - Smart Cooler</title>
    <link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@500&display=swap" rel="stylesheet">
    <link rel="manifest" href="manifest.json" />
    <style>
        :root {
            --primary-neon-color-1: #FF4500;
            --primary-neon-color-2: #8A2BE2;
            --secondary-neon-color-1: #FF00FF;
            --secondary-neon-color-2: #00FFFF;
        }

        body {
            margin: 0;
            font-family: 'Orbitron', sans-serif;
            background-color: var(--primary-neon-color-2);
            color: white;
            text-align: center;
            transition: background-color 1s;
        }

        h1 {
            text-shadow: 0 0 5px var(--secondary-neon-color-1), 0 0 10px var(--secondary-neon-color-2);
            margin-top: 30px;
            font-size: 4rem;
        }

        #start-btn {
            background-color: var(--primary-neon-color-1);
            color: white;
            border: 2px solid white;
            padding: 15px 30px;
            font-size: 18px;
            margin-top: 40px;
            border-radius: 12px;
            box-shadow: 0 0 20px var(--primary-neon-color-1), 0 0 30px var(--secondary-neon-color-1);
            cursor: pointer;
            transition: all 0.3s ease;
        }

        #start-btn:hover {
            box-shadow: 0 0 25px var(--secondary-neon-color-1), 0 0 40px var(--secondary-neon-color-2);
            transform: scale(1.05);
        }

        #start-btn:active {
            transform: scale(0.95);
        }

        #status {
            margin-top: 30px;
            font-size: 2rem;
            color: var(--secondary-neon-color-1);
            min-height: 60px;
        }

        .commands-help {
            margin-top: 40px;
            padding: 20px;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 15px;
            backdrop-filter: blur(10px);
            max-width: 600px;
            margin-left: auto;
            margin-right: auto;
        }

        .commands-help h3 {
            color: var(--secondary-neon-color-1);
            margin-bottom: 15px;
        }

        .commands-help ul {
            text-align: left;
            list-style: none;
            padding: 0;
        }

        .commands-help li {
            margin: 8px 0;
            padding: 5px 10px;
            background: rgba(255, 255, 255, 0.05);
            border-radius: 8px;
        }

        .device-status {
            margin-top: 30px;
            padding: 20px;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 15px;
            backdrop-filter: blur(10px);
            max-width: 500px;
            margin-left: auto;
            margin-right: auto;
        }

        .device-status h3 {
            color: var(--secondary-neon-color-2);
            margin-bottom: 15px;
        }

        .status-item {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin: 10px 0;
            padding: 8px 12px;
            background: rgba(255, 255, 255, 0.05);
            border-radius: 8px;
        }

        .status-indicator {
            padding: 4px 12px;
            border-radius: 15px;
            font-size: 14px;
            font-weight: bold;
        }

        .status-on {
            background-color: #00FF00;
            color: #000;
        }

        .status-off {
            background-color: #FF4444;
            color: #FFF;
        }
    </style>
</head>
<body>
    <h1>Voice Control</h1>
    <button id="start-btn">🎤 Start Listening</button>
    <p id="status">Click the button and speak your command</p>

    <div class="device-status">
        <h3>Device Status:</h3>
        <div class="status-item">
            <span>Water Refill:</span>
            <span id="status-water" class="status-indicator status-off">OFF</span>
        </div>
        <div class="status-item">
            <span>Cooling System:</span>
            <span id="status-cooling" class="status-indicator status-off">OFF</span>
        </div>
        <div class="status-item">
            <span>Water Pump:</span>
            <span id="status-pump" class="status-indicator status-off">OFF</span>
        </div>
        <div class="status-item">
            <span>Air Fan:</span>
            <span id="status-fan" class="status-indicator status-off">OFF</span>
        </div>
    </div>

    <div class="commands-help">
        <h3>Available Voice Commands:</h3>
        <ul>
            <li>🔹 "Turn on water refill" / "Water refill on"</li>
            <li>🔹 "Turn off water refill" / "Water refill off"</li>
            <li>🔹 "Turn on cooling" / "Cooling on" / "Peltier on"</li>
            <li>🔹 "Turn off cooling" / "Cooling off" / "Peltier off"</li>
            <li>🔹 "Turn on pump" / "Pump on" / "Honeycomb on"</li>
            <li>🔹 "Turn off pump" / "Pump off" / "Honeycomb off"</li>
            <li>🔹 "Turn on fan" / "Fan on" / "Air motor on"</li>
            <li>🔹 "Turn off fan" / "Fan off" / "Air motor off"</li>
            <li>🔸 "Turn on cooler" / "Start cooler" (All features ON)</li>
            <li>🔸 "Turn off cooler" / "Stop cooler" (All features OFF)</li>
        </ul>
    </div>

    <script>
        const blynkAuth = 'Gorpz7Ujk4dy1pQKsBDQXsb2K9j4T7Wq'; // Your Blynk token

        const commandMap = {
            1: { // V1 - Auto Water Refill
                keywords: ['water', 'refill', 'water refill', 'auto water'],
                name: 'Water Refill',
                statusId: 'status-water'
            },
            2: { // V2 - Peltier Cooling
                keywords: ['cooling', 'peltier', 'cool', 'temperature', 'cold'],
                name: 'Cooling System',
                statusId: 'status-cooling'
            },
            3: { // V3 - Honeycomb Pump
                keywords: ['pump', 'honeycomb', 'water pump', 'circulation'],
                name: 'Water Pump',
                statusId: 'status-pump'
            },
            4: { // V4 - Air Motor/Fan
                keywords: ['fan', 'air', 'motor', 'air motor', 'ventilation', 'blow'],
                name: 'Air Fan',
                statusId: 'status-fan'
            }
        };

        // Track device states locally
        let deviceStates = {
            1: false, // Water Refill
            2: false, // Cooling
            3: false, // Pump
            4: false // Fan
        };

        function updateDeviceStatus(pin, isOn) {
            deviceStates[pin] = isOn;
            const statusElement = document.getElementById(commandMap[pin].statusId);
            if (statusElement) {
                statusElement.textContent = isOn ? 'ON' : 'OFF';
                statusElement.className = `status-indicator ${isOn ? 'status-on' : 'status-off'}`;
            }
        }

        async function sendToBlynk(pin, value) {
            const url = `https://blynk.cloud/external/api/update?token=${blynkAuth}&V${pin}=${value}`;
            
            console.log(`🔄 Sending to Blynk: V${pin}=${value}`);
            
            try {
                const response = await fetch(url);
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
                console.log(`✅ Successfully sent V${pin}=${value} to Blynk`);
                updateDeviceStatus(pin, value === 1);
                return true;
            } catch (error) {
                console.error('❌ Error sending to Blynk:', error);
                updateStatus(`Error: Failed to send command to device`, false);
                return false;
            }
        }

        function updateStatus(message, isSuccess = true) {
            const statusElement = document.getElementById("status");
            statusElement.textContent = message;
            statusElement.style.color = isSuccess ? 'var(--secondary-neon-color-1)' : '#FF4444';
        }

        async function handleCommand(command) {
            const originalCommand = command;
            const lowerCommand = command.toLowerCase().trim();
            
            console.log('Processing command:', originalCommand);
            
            // Check for global cooler commands first
            if (lowerCommand.includes('cooler') || lowerCommand.includes('all')) {
                let action = -1;
                if (lowerCommand.includes('on') || lowerCommand.includes('start')) {
                    action = 1;
                } else if (lowerCommand.includes('off') || lowerCommand.includes('stop')) {
                    action = 0;
                }

                if (action !== -1) {
                    for (let pin in commandMap) {
                        await sendToBlynk(parseInt(pin), action);
                    }
                    updateStatus(`✅ All cooler systems turned ${action === 1 ? 'ON' : 'OFF'}`);
                    return;
                }
            }

            // Check individual feature commands
            for (let pin in commandMap) {
                const feature = commandMap[pin];
                
                const keywordMatch = feature.keywords.some(keyword => 
                    lowerCommand.includes(keyword.toLowerCase())
                );
                
                if (keywordMatch) {
                    let action = -1;
                    
                    if (lowerCommand.includes('off') || lowerCommand.includes('stop') || lowerCommand.includes('turn off') || lowerCommand.includes('switch off') || lowerCommand.includes('deactivate') || lowerCommand.includes('disable')) {
                        action = 0; // OFF = 0
                    } else if (lowerCommand.includes('on') || lowerCommand.includes('start') || lowerCommand.includes('turn on') || lowerCommand.includes('switch on') || lowerCommand.includes('activate') || lowerCommand.includes('enable')) {
                        action = 1; // ON = 1
                    }
                    
                    if (action !== -1) {
                        await sendToBlynk(parseInt(pin), action);
                        const actionText = action === 1 ? 'ON' : 'OFF';
                        updateStatus(`✅ ${feature.name} turned ${actionText}`);
                        return;
                    }
                }
            }
            
            updateStatus(`❌ Command not recognized: "${originalCommand}"`, false);
            console.log('Available commands:', Object.values(commandMap).map(f => f.keywords));
        }

        let isListening = false;

        document.getElementById("start-btn").addEventListener("click", () => {
            if (isListening) {
                return;
            }

            if (!('webkitSpeechRecognition' in window) && !('SpeechRecognition' in window)) {
                updateStatus("❌ Speech recognition not supported in this browser.", false);
                return;
            }

            const SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;
            const recognition = new SpeechRecognition();
            
            recognition.lang = 'en-US';
            recognition.interimResults = false;
            recognition.maxAlternatives = 3;
            recognition.continuous = false;

            isListening = true;
            const startBtn = document.getElementById("start-btn");
            startBtn.textContent = "🔴 Listening...";
            startBtn.style.backgroundColor = '#FF0000';
            
            try {
                recognition.start();
                updateStatus("🎤 Listening... Speak your command now");
                
                setTimeout(() => {
                    if (isListening) {
                        recognition.stop();
                    }
                }, 8000);
                
            } catch (error) {
                console.error('Error starting recognition:', error);
                updateStatus("❌ Error starting voice recognition", false);
                resetButton();
            }

            recognition.onresult = (event) => {
                console.log('Speech recognition results:', event.results);
                
                for (let i = 0; i < event.results[0].length; i++) {
                    const command = event.results[0][i].transcript;
                    const confidence = event.results[0][i].confidence;
                    
                    console.log(`Alternative ${i + 1}: "${command}" (confidence: ${confidence})`);
                    
                    handleCommand(command);
                    break; 
                }
            };

            recognition.onerror = (event) => {
                console.error('Speech recognition error:', event.error);
                let errorMessage = "❌ ";
                
                switch(event.error) {
                    case 'no-speech':
                        errorMessage += "No speech detected. Please try again.";
                        break;
                    case 'audio-capture':
                        errorMessage += "Microphone access denied or not available.";
                        break;
                    case 'not-allowed':
                        errorMessage += "Microphone permission denied. Please allow microphone access.";
                        break;
                    case 'network':
                        errorMessage += "Network error. Check your internet connection.";
                        break;
                    default:
                        errorMessage += `Recognition error: ${event.error}`;
                }
                
                updateStatus(errorMessage, false);
                resetButton();
            };

            recognition.onend = () => {
                console.log('Speech recognition ended');
                resetButton();
            };

            function resetButton() {
                isListening = false;
                startBtn.textContent = "🎤 Start Listening";
                startBtn.style.backgroundColor = 'var(--primary-neon-color-1)';
            }
        });

        const neonThemes = [
            {
                primary1: '#FF4500', primary2: '#8A2BE2',
                secondary1: '#FF00FF', secondary2: '#00FFFF'
            },
            {
                primary1: '#00FFFF', primary2: '#FF00FF',
                secondary1: '#FF4500', secondary2: '#8A2BE2'
            },
            {
                primary1: '#00FF00', primary2: '#0000FF',
                secondary1: '#FF00FF', secondary2: '#FF4500'
            }
        ];

        let currentTheme = 0;
        setInterval(() => {
            currentTheme = (currentTheme + 1) % neonThemes.length;
            const theme = neonThemes[currentTheme];
            document.documentElement.style.setProperty('--primary-neon-color-1', theme.primary1);
            document.documentElement.style.setProperty('--primary-neon-color-2', theme.primary2);
            document.documentElement.style.setProperty('--secondary-neon-color-1', theme.secondary1);
            document.documentElement.style.setProperty('--secondary-neon-color-2', theme.secondary2);
        }, 5000);

        document.addEventListener('DOMContentLoaded', () => {
            console.log('✅ Voice Control System Initialized');
            updateStatus('Ready! Click the button and speak your command');
        });
    </script>
    
    <div style="text-align: center; margin-top: 30px;">
        <button onclick="window.location.href='index.html'" 
                style="
                    font-family: 'Orbitron', sans-serif;
                    font-size: 18px;
                    padding: 12px 24px;
                    background: var(--primary-neon-color-1);
                    color: white;
                    border: 2px solid white;
                    border-radius: 10px;
                    cursor: pointer;
                    box-shadow: 0 0 15px var(--secondary-neon-color-1);
                    transition: all 0.3s ease;
                "
                onmouseover="this.style.boxShadow='0 0 30px var(--secondary-neon-color-2)'"
                onmouseout="this.style.boxShadow='0 0 15px var(--secondary-neon-color-1)'"
        >
            🏠 Back to Home
        </button>
    </div>

    <script>
        if ('serviceWorker' in navigator) {
            navigator.serviceWorker.register('service-worker.js')
                .then(() => console.log('✅ Service Worker registered successfully'))
                .catch(err => console.error('❌ Service Worker registration failed:', err));
        }
    </script>
</body>
</html>
