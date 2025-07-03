# 🕒 ESP32 OLED Clock with AWS API Gateway ⏰

This project displays the **real-time clock on an SSD1306 OLED display** using an **ESP32**.  
Time is fetched from a custom-built **AWS Lambda function via API Gateway** — no external RTC module required!

## 🔧 Features

✨ Uses ESP32 + SSD1306 (I2C)  
🌐 Fetches time from AWS Lambda (custom timezone logic)  
🕰️ Displays time in **12-hour format with AM/PM**  
📡 Wi-Fi-based time update  
📱 Fully customizable

## 🔗 Project Structure

```
esp32-oled-clock-aws/
├── src/
│   └── main.cpp       ← Main firmware
├── platformio.ini     ← Build config for PlatformIO
└── README.md          ← You're reading it!
```

## 🔌 Hardware Required

| Component       | Example                          |
|----------------|----------------------------------|
| ✅ ESP32 board  | DOIT ESP32 DEVKIT V1, etc.       |
| ✅ OLED Display | SSD1306 I2C 128×64 (0x3C address) |
| ✅ Jumper Wires | Male-to-female                   |

## ⚙️ Setup Instructions

### 🖥️ 1. AWS Lambda

```python
import json
from datetime import datetime, timedelta

def lambda_handler(event, context):
    ist_time = datetime.utcnow() + timedelta(hours=5, minutes=30)
    formatted_time = ist_time.strftime('%I:%M:%S %p %d-%m-%Y')
    return {
        'statusCode': 200,
        'headers': { 'Content-Type': 'application/json' },
        'body': json.dumps({
            'time': formatted_time,
            'timezone': 'Asia/Kolkata'
        })
    }
```

### 🌐 2. API Gateway

- Create a **GET endpoint** at `/time`
- Link it to the Lambda above
- Enable **CORS** and deploy

Copy the full URL:  
`https://your-api-id.execute-api.region.amazonaws.com/time`

### 🤖 3. ESP32 Setup

- Install [PlatformIO](https://platformio.org/)
- Clone this repo
- Update these in `main.cpp`:
  - `ssid`, `password`
  - `apiURL` (your API Gateway endpoint)

Then upload to ESP32 using:
```bash
platformio run --target upload
```

## 💬 Output Example

**Serial Monitor**:
```
{"time":"02:45:59 PM 03-07-2025","timezone":"Asia/Kolkata"}
```

**OLED Display**:
```
  02:45:59 PM
  03-07-2025
```

## 🚀 Bonus Ideas

- Add support for displaying date + weekday
- Add `retry` if API fails
- Show Wi-Fi signal strength on OLED
- Switch between 12/24h format via button
- Store last time in NVS or RTC memory for offline mode

## 🛠 Built With

- ESP32 (PlatformIO / Arduino framework)
- SSD1306 OLED (Adafruit libraries)
- AWS Lambda + API Gateway
- Python (serverless code)

## 📜 License

MIT