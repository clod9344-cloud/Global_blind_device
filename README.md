# 👁️ VisionAssist AI

> An AI-powered voice assistant that helps visually impaired people navigate, identify, and interact with their environment — using just a browser and camera.

[![Demo Video](https://img.shields.io/badge/Demo-YouTube-red?style=flat-square&logo=youtube)](https://youtu.be/77OxDeKJ-ZE?si=JkLnDp5twFuI7YhD)
[![Live App](https://img.shields.io/badge/Live-Render-46e3b7?style=flat-square)](https://global-blind-device.onrender.com/)

---

## 🚩 Problem

Over **2.2 billion people** live with visual impairment. Everyday tasks — reading a medicine label, crossing a road, finding a lost item — are daily challenges. Most assistive tools are expensive or limited in real-world intelligence.

## 💡 Solution

VisionAssist AI runs in any browser. Say **"My Eye"** to wake it, then speak any command. The AI sees through your camera, thinks across multiple steps, and speaks the result back — no buttons needed.

---

## ✨ Key Features

- 🤖 **AI Agent** — Give a goal, the agent checks memory → looks at camera → scans the room automatically
- 🧠 **Visual Memory** — Save objects with photos and location; recall them anytime by voice
- 🚦 **Traffic Light Detector** — Real-time color detection with voice alert
- 🍽️ **Food Identifier** — Identifies food items and meal type from camera
- 🪜 **Stair Safety** — Detects step count, direction, and hazards
- 📖 **Page & Medicine Reader** — Auto-reads books and medicine labels aloud
- 👤 **Face Recognition** — Save and recognize known people by name
- 🆘 **Emergency SOS** — Sends GPS location via SMS and email to contacts
- 🗺️ **Navigation** — Outdoor GPS routing + indoor room-to-room guidance

---

## 🛠️ Tech Stack

| | |
|---|---|
| **Backend** | Python, Flask |
| **AI Models** | Groq (Llama 4 Scout), Google Gemini 1.5 Flash |
| **Vision** | OpenCV, YOLOv8, DeepFace |
| **Frontend** | HTML5, CSS3, Vanilla JS, Web Speech API |
| **Maps** | Leaflet.js + OpenStreetMap |
| **Alerts** | Twilio SMS, SMTP Email |

---

## 🚀 Getting Started

```bash
git clone https://github.com/your-username/visionassist-ai.git
cd visionassist-ai
pip install -r requirements.txt
```

Create a `.env` file:

```env
GROQ_API_KEY_1=your_key_here
GEMINI_API_KEY=your_key_here        # optional
```

```bash
python app.py
# Open http://localhost:5000
```

---

## 🎮 Voice Commands

Say **"My Eye"** followed by any command:

| Command | What it does |
|---|---|
| `help me find my medicine` | Agent finds it step by step |
| `detect food` | Identifies what's on your plate |
| `detect traffic` | Checks if it's safe to cross |
| `remember this — keys on table` | Saves object to memory |
| `where are my keys` | Recalls location + visual confirm |
| `start page reader` | Reads books or labels aloud |
| `emergency` | Sends SOS with GPS location |
| `stop all` | Stops everything |

---

## 👤 Built By

**Prince Jha** — Solo Developer

---

## 📄 License

MIT © 2026
