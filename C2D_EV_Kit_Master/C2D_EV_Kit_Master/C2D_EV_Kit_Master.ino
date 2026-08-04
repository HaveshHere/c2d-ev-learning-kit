// ============================================================================
//   EV EDUCATIONAL KIT — UNIFIED MASTER ENGINE (PERFECTED TOUCH & LAYOUT)
//   Hardware : TFT LCD 320x240 (TFT_eSPI) + XPT2046 Touch Screen Controller
//   IDE      : Arduino IDE
// ============================================================================

#include <TFT_eSPI.h>
#include <SPI.h>
#include <FastLED.h>

TFT_eSPI tft = TFT_eSPI();

// Universal UI Theme Color Codes
#define CLR_BG          TFT_BLACK
#define CLR_NEON_GREEN  0x07E0  
#define CLR_GREEN_DIM   0x03E0  
#define CLR_ACTIVE_CYAN 0x07FF  
#define CLR_DARK_GRID   0x10A2  
#define CLR_TEXT_MUTED  0xBDF7  
#define CLR_ORANGE_RED  0xD2A0
#define CLR_WHITE       TFT_WHITE
#define BUZZER_PIN      25

#define SCREEN_W  320
#define SCREEN_H  240

#define LED_PIN_SHARED  27
#define NUM_LEDS_TOTAL  50    // adjust to your actual total LED count
#define LED_TYPE        WS2812B
#define COLOR_ORDER     GRB

CRGB leds[NUM_LEDS_TOTAL];

#define NUM_LEDS_REGEN  4
CRGB regenLeds[NUM_LEDS_REGEN];

void ledsAllClear() {
  fill_solid(leds, NUM_LEDS_TOTAL, CRGB::Black);
  fill_solid(regenLeds, NUM_LEDS_REGEN, CRGB::Black);
  FastLED.show();
}

void ledsRange(int from1, int to1, CRGB colour) {
  int step = (to1 >= from1) ? 1 : -1;
  for (int i = from1 - 1; i != to1 - 1 + step; i += step) {
    if (i >= 0 && i < NUM_LEDS_TOTAL) {
      leds[i] = colour;
      FastLED.show();
      delay(200);
    }
  }
}

enum AppState {
  STATE_WELCOME,
  STATE_MOD_SELECT,
  STATE_PREVIEW,
  STATE_QUIZ_PLAY,
  STATE_FEEDBACK,
  STATE_COMPLETE
};
AppState currentState = STATE_WELCOME;

int selectedModule = 1;  
int currentStep    = 0;  
bool lastAnswerWasCorrect = false;

// --- Smooth Blinking Core Engine Variables ---
unsigned long lastBlink    = 0;
bool          blinkState   = true;
const int     BLINK_MS     = 500; 

// --- Calibrated Touch Parameters ---
unsigned long lastTouchTime = 0;
const int     TOUCH_DELAY   = 350; 

// ── Master Prototyping Forward Declarations ──
void drawFirstScreen();
void drawModuleSelectionScreen();
void drawPerfectButton(int x, int y, String num, String title, String sub);
void drawPreQuizBaseFrame(int module, String moduleTitle);
void drawStartScreen(String title, String subtitle);
void drawQuestionLayout(int qNum, String question, String a, String b, String c);
void toggleQuizPrompt(bool showText);
void drawMissionComplete(String rank, String subtext);
void playIntroSong();
void drawFrameDecorations();

// Cross-Tab Function Declarations
void displayModule01(int startX, int startY);
void displayModule02(int startX, int startY);
void displayModule03(int startX, int startY);
void runACChargingFlow();
void runDCChargingFlow();
void runDrivingRegenFlow();
void evaluateACQuestion(char choice);
void evaluateDCQuestion(char choice);
void evaluateRegenQuestion(char choice);
void updateRegenLeds();
void skipACStep();
void skipDCStep();

void beep(int freq, int duration) {
  tone(BUZZER_PIN, freq, duration);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  tft.init();
  tft.setRotation(1); 
  tft.fillScreen(CLR_BG);
  FastLED.addLeds<LED_TYPE, LED_PIN_SHARED, COLOR_ORDER>(leds, NUM_LEDS_TOTAL).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 17, GRB>(regenLeds, NUM_LEDS_REGEN).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(180);
  ledsAllClear();
  drawFirstScreen();
}

void loop() {
  uint16_t tx = 0, ty = 0;
  bool isTouched = tft.getTouch(&tx, &ty);

  if (isTouched) {
    Serial.print("X=");
    Serial.print(tx);
    Serial.print("  Y=");
    Serial.println(ty);
  }
  
  // --- BLINK ENGINE ACTIVE ACROSS STATES ---
  if (currentState == STATE_WELCOME || currentState == STATE_PREVIEW || currentState == STATE_COMPLETE) {
    if (millis() - lastBlink >= BLINK_MS) {
      blinkState = !blinkState;
      toggleQuizPrompt(blinkState);
      lastBlink = millis();
    }
  }

  if (currentState == STATE_FEEDBACK && lastAnswerWasCorrect) {
  if (millis() - lastBlink >= BLINK_MS) {
    blinkState = !blinkState;

    if (blinkState) {
      tft.setTextColor(CLR_BG);
      tft.setTextSize(1);
      tft.setTextDatum(MC_DATUM);
      tft.drawRoundRect(60, SCREEN_H-30, 200, 22, 3, CLR_BG);
      tft.drawString("TAP ANYWHERE TO CONTINUE", SCREEN_W/2, SCREEN_H-19);
    } else {
      tft.fillRoundRect(58, SCREEN_H-32, 204, 26, 3, CLR_NEON_GREEN);
    }

    lastBlink = millis();
  }
}

if (currentState == STATE_FEEDBACK && !lastAnswerWasCorrect) {
    if (millis() - lastBlink >= BLINK_MS) {
      blinkState = !blinkState;

      if (blinkState) {
        tft.setTextColor(CLR_WHITE);
        tft.setTextSize(1);
        tft.setTextDatum(MC_DATUM);
        tft.drawRoundRect(60, SCREEN_H-30, 200, 22, 3, CLR_WHITE);
        tft.drawString("TAP ANYWHERE TO RETRY", SCREEN_W/2, SCREEN_H-19);
      } else {
        tft.fillRoundRect(58, SCREEN_H-32, 204, 26, 3, CLR_ORANGE_RED);
      }

      lastBlink = millis();
    }
  }

  // --- INTERACTIVE TOUCH ROUTER MATRIX ---
  if (selectedModule == 3 && currentState == STATE_QUIZ_PLAY) updateRegenLeds();
  if (isTouched && (millis() - lastTouchTime > TOUCH_DELAY)) {
    lastTouchTime = millis();
    beep(1000, 80);
    
    if (currentState == STATE_WELCOME) {
      for (int i = 0; i < 2; i++) {
        tft.fillScreen(CLR_NEON_GREEN); delay(45);
        tft.fillScreen(CLR_BG); delay(35);
      }
      currentState = STATE_MOD_SELECT;
      drawModuleSelectionScreen();
    }
    
    else if (currentState == STATE_MOD_SELECT) {
      #define BOX_W   280
      #define BOX_H   48
      #define START_X 20
      
      int touchedMod = 0;
      if (tx >= 19 && tx <= 290) {
        if (ty >= 120 && ty <= 170)       touchedMod = 1;  // M1 row vertical
        else if (ty >= 75 && ty <= 119)   touchedMod = 2;  // M2 row vertical (temp)
        else if (ty >= 30 && ty <= 74)    touchedMod = 3;  // M3 row vertical (temp)
      }
      
      if (touchedMod > 0) {
        selectedModule = touchedMod;
        int targetY = (selectedModule == 1) ? 62 : ((selectedModule == 2) ? 118 : 174);
        tft.drawRoundRect(START_X, targetY, BOX_W, BOX_H, 4, CLR_ACTIVE_CYAN);
        delay(150);
        
        currentState = STATE_PREVIEW;
        currentStep = 0;
        
        // Render Pre-Quiz Preview Content Layout dynamically
        if (selectedModule == 1) {
          drawPreQuizBaseFrame(1, "AC CHARGING MODE");
          displayModule01(28, 92); 
        } else if (selectedModule == 2) {
          drawPreQuizBaseFrame(2, "DC FAST CHARGING");
          displayModule02(28, 92);
        } else if (selectedModule == 3) {
          drawPreQuizBaseFrame(3, "DRIVING & REGENERATIVE BRAKING");
          displayModule03(28, 92);
        }
      }
    }
    
    else if (currentState == STATE_PREVIEW) {

      if (tx >= 0 && tx <= 100 && ty >= 0 && ty <= 10) {
        currentState = STATE_MOD_SELECT;
        drawModuleSelectionScreen();
        return;
      }
      currentState = STATE_QUIZ_PLAY;
      tft.fillScreen(CLR_BG);
      if (selectedModule == 1)      runACChargingFlow();
      else if (selectedModule == 2) runDCChargingFlow();
      else if (selectedModule == 3) runDrivingRegenFlow();
    }
    
    else if (currentState == STATE_QUIZ_PLAY) {
      char selectedChoice = ' ';
      if (ty >= 95 && ty <= 127)     selectedChoice = 'A';
      else if (ty >= 70 && ty <= 90) selectedChoice = 'B';
      else if (ty >= 20 && ty <= 40) selectedChoice = 'C';

      // SKIP button bottom right
      if (tx >= 260 && tx <= 320 && ty >= 0 && ty <= 25) {
        if (selectedModule == 1)      skipACStep();
        else if (selectedModule == 2) skipDCStep();
        currentStep++;
        tft.fillScreen(CLR_BG);
        if (selectedModule == 1)      runACChargingFlow();
        else if (selectedModule == 2) runDCChargingFlow();
        else if (selectedModule == 3) runDrivingRegenFlow();
      }
      
      if (selectedChoice != ' ') {
        if (selectedModule == 1)      evaluateACQuestion(selectedChoice);
        else if (selectedModule == 2) evaluateDCQuestion(selectedChoice);
        else if (selectedModule == 3) evaluateRegenQuestion(selectedChoice);
      }
    }
    
    else if (currentState == STATE_FEEDBACK) {
      currentState = STATE_QUIZ_PLAY;
      tft.fillScreen(CLR_BG);
      if (selectedModule == 1)      runACChargingFlow();
      else if (selectedModule == 2) runDCChargingFlow();
      else if (selectedModule == 3) runDrivingRegenFlow();
    }
  
    else if (currentState == STATE_COMPLETE) {
      currentStep = 0;
      currentState = STATE_MOD_SELECT;
      drawModuleSelectionScreen();
    }
  }
}

// ── Layout Asset Implementations ─────────────────────────
void drawFirstScreen() {
  tft.fillScreen(CLR_BG);
  drawFrameDecorations();

  tft.drawRoundRect(SCREEN_W / 2 - 55, 16, 110, 18, 4, CLR_GREEN_DIM);
  tft.setTextColor(CLR_NEON_GREEN); tft.setTextSize(1); tft.setTextDatum(TC_DATUM);
  tft.drawString("WELCOME TO THE", SCREEN_W / 2, 21);

  tft.setTextSize(5); tft.setTextDatum(MC_DATUM);
  tft.drawString("C2D", SCREEN_W / 2, 72);
  tft.setTextSize(2); tft.setTextColor(CLR_GREEN_DIM);
  tft.drawString("EV LEARNING KIT", SCREEN_W / 2, 110);

  tft.fillRect(SCREEN_W/2 - 2, 138, 4, 4, CLR_NEON_GREEN);
  tft.fillRect(SCREEN_W/2 - 16, 138, 4, 4, CLR_GREEN_DIM);
  tft.fillRect(SCREEN_W/2 + 12, 138, 4, 4, CLR_GREEN_DIM);

  tft.setTextColor(CLR_NEON_GREEN); tft.setTextSize(1); tft.setTextDatum(TC_DATUM);
  tft.drawString("EDUCATIONAL QUIZ SYSTEM", SCREEN_W / 2, 152);
  
  blinkState = true;
  toggleQuizPrompt(true);
  playIntroSong();
}

void drawFrameDecorations() {
  int padding = 8;
  int length = 15;
  tft.drawFastHLine(padding, padding, length, CLR_GREEN_DIM);
  tft.drawFastVLine(padding, padding, length, CLR_GREEN_DIM);
  tft.drawFastHLine(SCREEN_W - padding - length, padding, length, CLR_GREEN_DIM);
  tft.drawFastVLine(SCREEN_W - padding, padding, length, CLR_GREEN_DIM);
  tft.drawFastHLine(padding, SCREEN_H - padding, length, CLR_GREEN_DIM);
  tft.drawFastVLine(padding, SCREEN_H - padding - length, length, CLR_GREEN_DIM);
  tft.drawFastHLine(SCREEN_W - padding - length, SCREEN_H - padding, length, CLR_GREEN_DIM);
  tft.drawFastVLine(SCREEN_W - padding, SCREEN_H - padding - length, length, CLR_GREEN_DIM);

  tft.drawFastHLine(padding, SCREEN_H / 2, 4, CLR_DARK_GRID);
  tft.drawFastHLine(SCREEN_W - padding - 4, SCREEN_H / 2, 4, CLR_DARK_GRID);
}

void drawModuleSelectionScreen() {
  tft.fillScreen(CLR_BG);
  tft.setTextColor(CLR_ACTIVE_CYAN); tft.setTextSize(2); tft.setTextDatum(TC_DATUM);
  tft.drawString("Hello, Explorer!", SCREEN_W / 2, 8);
  tft.setTextColor(CLR_TEXT_MUTED); tft.setTextSize(1);
  tft.drawString("What do you want to learn today?", SCREEN_W / 2, 30);
  tft.drawFastHLine(30, 48, SCREEN_W - 60, CLR_GREEN_DIM);

  drawPerfectButton(20, 62, "01", "HOME AC CHARGING", "SLOW GRID TO HOME CHARGING");
  drawPerfectButton(20, 118, "02", "DC FAST CHARGING", "RAPID PUBLIC STATION CHARGING");
  drawPerfectButton(20, 174, "03", "DRIVING & REGENERATIVE BRAKING", "FROM MOTION TO REGENERATION");
}

void drawPerfectButton(int x, int y, String num, String title, String sub) {
  tft.fillRoundRect(x, y, 280, 48, 4, CLR_DARK_GRID);
  tft.drawRoundRect(x, y, 280, 48, 4, CLR_GREEN_DIM);
  tft.setTextColor(CLR_ACTIVE_CYAN); tft.setTextSize(1); tft.setTextDatum(ML_DATUM);
  tft.drawString("MOD " + num + " >", x + 12, y + 24);
  tft.setTextColor(CLR_NEON_GREEN); tft.setTextDatum(TL_DATUM);
  tft.drawString(title.c_str(), x + 65, y + 10);
  tft.setTextColor(CLR_TEXT_MUTED);
  tft.drawString(sub.c_str(), x + 65, y + 26);
}

void drawPreQuizBaseFrame(int module, String moduleTitle) {

  tft.fillScreen(CLR_BG);
  tft.drawFastHLine(0, 42, SCREEN_W, CLR_GREEN_DIM); 
  
  tft.setTextColor(CLR_ACTIVE_CYAN); tft.setTextSize(2); tft.setTextDatum(TC_DATUM);
  tft.drawString("Are You Ready, Explorer?", SCREEN_W / 2, 12);

  tft.drawRect(15, 54, SCREEN_W - 30, 134, CLR_GREEN_DIM);
  
  tft.fillRect(15, 54, 8, 2, CLR_NEON_GREEN); tft.fillRect(15, 54, 2, 8, CLR_NEON_GREEN);
  tft.fillRect(SCREEN_W-23, 54, 8, 2, CLR_NEON_GREEN); tft.fillRect(SCREEN_W-17, 54, 2, 8, CLR_NEON_GREEN);
  tft.fillRect(15, 186, 8, 2, CLR_NEON_GREEN); tft.fillRect(15, 180, 2, 8, CLR_NEON_GREEN);
  tft.fillRect(SCREEN_W-23, 186, 8, 2, CLR_NEON_GREEN); tft.fillRect(SCREEN_W-17, 180, 2, 8, CLR_NEON_GREEN);

  tft.setTextColor(CLR_NEON_GREEN); tft.setTextSize(1); tft.setTextDatum(TL_DATUM);
  tft.drawString(moduleTitle, 28, 66);
  tft.drawFastHLine(15, 79, SCREEN_W - 31, CLR_GREEN_DIM);

  tft.setTextColor(CLR_TEXT_MUTED); tft.setTextSize(1); tft.setTextDatum(ML_DATUM);
  tft.drawString("< BACK", 10, SCREEN_H - 12);
  
  blinkState = true;
  toggleQuizPrompt(true);
}

void drawStartScreen(String title, String subtitle) {
  tft.fillScreen(CLR_BG);
  tft.drawRect(10, 10, SCREEN_W - 20, SCREEN_H - 20, CLR_GREEN_DIM);
  tft.fillRect(10, 10, 8, 2, CLR_ACTIVE_CYAN); tft.fillRect(10, 10, 2, 8, CLR_ACTIVE_CYAN);
  tft.fillRect(SCREEN_W-18, 10, 8, 2, CLR_ACTIVE_CYAN); tft.fillRect(SCREEN_W-12, 10, 2, 8, CLR_ACTIVE_CYAN);

  tft.setTextColor(CLR_ACTIVE_CYAN); tft.setTextSize(2); tft.setTextDatum(MC_DATUM);
  tft.drawString(title, SCREEN_W / 2, (SCREEN_H / 2) - 20);
  tft.setTextColor(CLR_TEXT_MUTED); tft.setTextSize(1.5);
  tft.drawString(subtitle, SCREEN_W / 2, (SCREEN_H / 2) + 20);
  delay(2500); 
}

void drawQuestionLayout(int qNum, String question, String a, String b, String c) {
  tft.fillScreen(CLR_BG);
  tft.fillRect(0, 0, SCREEN_W, 35, CLR_DARK_GRID);
  tft.drawFastHLine(0, 35, SCREEN_W, CLR_GREEN_DIM);
  tft.setTextColor(CLR_ACTIVE_CYAN); tft.setTextSize(2); tft.setTextDatum(ML_DATUM);
  
  tft.drawString(" Module " + String(selectedModule) + ": Question " + String(qNum), 12, 17);
  
  tft.setTextSize(1); tft.setTextDatum(TL_DATUM);

  tft.setTextColor(CLR_WHITE); tft.setTextDatum(TL_DATUM);
  tft.drawString(question, 15, 50); 
  
  tft.fillRoundRect(15, 95, SCREEN_W - 30, 32, 4, CLR_DARK_GRID);
  tft.drawRoundRect(15, 95, SCREEN_W - 30, 32, 4, CLR_GREEN_DIM);
  tft.setTextColor(CLR_NEON_GREEN); tft.drawString(a, 25, 105);
  
  tft.fillRoundRect(15, 135, SCREEN_W - 30, 32, 4, CLR_DARK_GRID);
  tft.drawRoundRect(15, 135, SCREEN_W - 30, 32, 4, CLR_GREEN_DIM);
  tft.drawString(b, 25, 145);
  
  tft.fillRoundRect(15, 175, SCREEN_W - 30, 32, 4, CLR_DARK_GRID);
  tft.drawRoundRect(15, 175, SCREEN_W - 30, 32, 4, CLR_GREEN_DIM);
  tft.drawString(c, 25, 185);

  tft.setTextColor(CLR_TEXT_MUTED); tft.setTextSize(1); tft.setTextDatum(MR_DATUM);
  tft.drawString("SKIP >>", SCREEN_W - 15, SCREEN_H - 12);
}

void toggleQuizPrompt(bool showText) {
  int rectY = SCREEN_H - 30;
  int rectH = 22;

  int x = 60;
  int w = SCREEN_W - 120;

  tft.setTextSize(1);
  tft.setTextDatum(MC_DATUM);

  if (showText) {
    if (currentState == STATE_COMPLETE) {
      tft.drawRoundRect(x, rectY, w, rectH, 3, CLR_ACTIVE_CYAN);
      tft.drawRoundRect(x - 1, rectY - 1, w + 2, rectH + 2, 3, CLR_ACTIVE_CYAN);
      tft.setTextColor(CLR_ACTIVE_CYAN);
    } else {
      tft.drawRoundRect(x, rectY, w, rectH, 3, CLR_NEON_GREEN);
      tft.drawRoundRect(x - 1, rectY - 1, w + 2, rectH + 2, 3, CLR_GREEN_DIM);
      tft.setTextColor(CLR_NEON_GREEN);
    }

    if (currentState == STATE_WELCOME)
      tft.drawString("TAP ANYWHERE TO CONTINUE", SCREEN_W / 2, rectY + 11);
    else if (currentState == STATE_COMPLETE)
      tft.drawString("TAP TO TRY ANOTHER MODULE!", SCREEN_W / 2, rectY + 11);
    else
      tft.drawString("TAP ANYWHERE TO START QUIZ", SCREEN_W / 2, rectY + 11);
  }
  else {
    // CLEAR entire blink area (box + text)
    tft.fillRoundRect(x - 2, rectY - 2, w + 4, rectH + 4, 3, CLR_BG);
  }
}

void drawMissionComplete(String rank, String subtext) {
  currentState = STATE_COMPLETE;
  
  tft.fillScreen(CLR_BG);
  
  tft.setTextColor(CLR_NEON_GREEN); tft.setTextSize(2); tft.setTextDatum(MC_DATUM);
  tft.drawString("MISSION COMPLETE!", SCREEN_W / 2, 65); 
  
  tft.setTextColor(CLR_WHITE); tft.setTextSize(2);
  tft.drawString(rank, SCREEN_W / 2, 125); 
  
  tft.setTextColor(CLR_TEXT_MUTED); tft.setTextSize(1);
  tft.drawString(subtext, SCREEN_W / 2, 175); 
  tft.setTextColor(CLR_ACTIVE_CYAN); tft.setTextSize(1); tft.setTextDatum(MC_DATUM);
  tft.drawRoundRect(60, SCREEN_H-30, 200, 22, 3, CLR_ACTIVE_CYAN);
  tft.drawString("TAP TO TRY ANOTHER MODULE!", SCREEN_W/2, SCREEN_H-19);
  beep(1000, 80); delay(90);
  beep(1200, 80); delay(90);
  beep(1500, 80); delay(90);
  beep(1800, 80); delay(90);
  beep(2000, 150); delay(170);
  beep(2000, 150);
  
}

void playIntroSong() {

  int melody[] = {
    659, 494, 523, 587, 523, 494,
    440, 440, 523, 659, 587, 523,
    494, 523, 587, 659, 523, 440, 440,
    587, 698, 880, 784, 698,
    659, 523, 659, 587, 523,
    494, 494, 523, 587, 659,
    523, 440, 440, 0
  };

  int duration[] = {
    400, 200, 200, 400, 200, 200,
    400, 200, 200, 400, 200, 200,
    300, 200, 400, 400, 400, 400, 400,
    300, 200, 400, 300, 100,
    400, 200, 400, 300, 100,
    200, 200, 200, 400, 400,
    400, 400, 400, 400
  };

  const float speedFactor = 0.65;   // 🔥 faster than before (0.80 was slow)
  const int noteGap = 1;            // 🔥 almost zero gap (was 3)

  int notes = sizeof(melody) / sizeof(melody[0]);

  uint16_t tx, ty;

  for (int i = 0; i < notes; i++) {

    // 🚀 instant interrupt check
    if (tft.getTouch(&tx, &ty)) {
      noTone(BUZZER_PIN);
      return;
    }

    int noteTime = (int)(duration[i] * speedFactor);
    unsigned long startTime = millis();

  // LED on every odd note, off every even note
  if (i % 2 == 0) {
    fill_solid(leds, NUM_LEDS_TOTAL, CRGB::Green);
    fill_solid(regenLeds, NUM_LEDS_REGEN, CRGB::Green);
  } else {
    fill_solid(leds, NUM_LEDS_TOTAL, CRGB::Black);
    fill_solid(regenLeds, NUM_LEDS_REGEN, CRGB::Black);
  }
  FastLED.show();

  // play note
  if (melody[i] == 0) {
      noTone(BUZZER_PIN);
    } else {
      tone(BUZZER_PIN, melody[i]);
    }

    // ⏱️ non-blocking wait
    while (millis() - startTime < noteTime) {

      if (tft.getTouch(&tx, &ty)) {
        noTone(BUZZER_PIN);
        return;
      }

      if (millis() - lastBlink >= BLINK_MS) {
        blinkState = !blinkState;
        toggleQuizPrompt(blinkState);
        lastBlink = millis();
      }

      delay(1); // tiny yield only (prevents watchdog issues)
    }

    // stop note ONLY when needed
    noTone(BUZZER_PIN);
    fill_solid(leds, NUM_LEDS_TOTAL, CRGB::Black);
    fill_solid(regenLeds, NUM_LEDS_REGEN, CRGB::Black);
    FastLED.show();
    delay(noteGap);
  }

  noTone(BUZZER_PIN);
}