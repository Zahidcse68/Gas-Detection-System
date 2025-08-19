//gas detection system by zaidu  insta@xaidu_uu
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- OLED setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Pin Setup ---
#define MQ2_AO_PIN A0   // Analog output pin of MQ2
#define BUZZER_PIN 3    // Buzzer

int gasValue = 0;
int threshold = 400; // adjust for your environment

void setup() {
  pinMode(MQ2_AO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // --- OLED init ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;) { } // stop if OLED not found
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  showStartupText();
}

void loop() {
  gasValue = analogRead(MQ2_AO_PIN); // 0 - 1023
  int barLength = map(gasValue, 0, 1023, 0, 100); // scale to OLED bar length

  display.clearDisplay();

  // Title
  display.setTextSize(1);
  display.setCursor(25, 0);
  display.print("Gas Monitor");

  // Gauge Label
  display.setCursor(0, 15);
  display.print("Level:");

  // Gauge Bar
  display.drawRect(30, 15, 100, 10, SSD1306_WHITE); // outline
  display.fillRect(30, 15, barLength, 10, SSD1306_WHITE); // fill

  // Status
  display.setCursor(0, 35);
  if (gasValue > threshold) {
    display.print("Status: DANGER!");
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    display.print("Status: SAFE");
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Gas Value (optional small display)
  display.setCursor(0, 50);
  display.print("Value: ");
  display.print(gasValue);

  display.display();
  delay(300);
}

// --- Function: Startup Scrolling Text ---
void showStartupText() {
  String msg = "Gas Detection System";

  for (int x = SCREEN_WIDTH; x > -msg.length() * 6; x--) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(x, 30);
    display.print(msg);
    display.display();
    delay(50);
  }

  delay(800);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20, 25);
  display.print("System Ready...");
  display.display();
  delay(1000);
  display.clearDisplay();
}

