void skipACStep() {
  if (currentStep == 1) ledsRange(19, 26, CRGB::Red);
  else if (currentStep == 2) ledsRange(27, 29, CRGB::Red);
  else if (currentStep == 3) ledsRange(30, 31, CRGB::Green);
}

void displayModule01(int startX, int startY) {
  tft.setTextColor(CLR_TEXT_MUTED); tft.setTextSize(1); tft.setTextDatum(TL_DATUM);
  int gap = 24; 

  tft.drawString(" 1. Transmission & Substation Infrastructure", startX, startY);
  tft.drawString(" 2. Home AC Charging Grid Delivery", startX, startY + gap);
  tft.drawString(" 3. OBC & EV Battery Chemical Storage", startX, startY + (gap * 2));
}

void runACChargingFlow() {
  if (currentStep == 0) {
    ledsAllClear();
    ledsRange(14, 18, CRGB::Red);
    drawStartScreen("HOME AC CHARGING MODE", "Power Flow: Transmission Tower -> Substation");
    currentStep = 1;
    tft.fillScreen(CLR_BG);
  }
  
  if (currentStep == 1) {
    drawQuestionLayout(1, "What is the main purpose of a substation?", "A. Increase voltage", "B. Reduce voltage for distribution", "C. Store electricity");
  } else if (currentStep == 2) {
    drawQuestionLayout(2, "What is the typical supply for a home EV charger?", "A. AC power", "B. DC power", "C. Battery power");
  } else if (currentStep == 3) {
    drawQuestionLayout(3, "What does an On-Board Charger do?", "A. Store electrical energy", "B. Spins the vehicle wheels ", "C. Converts AC to DC power");
  } else if (currentStep == 4) {
    ledsAllClear();
    drawMissionComplete("AC Charging Expert!", "Energy reached the EV Battery!");
  }
}

void evaluateACQuestion(char choice) {
  bool correct = false; String track = "";
  
  if (currentStep == 1) {
    if (choice == 'B') correct = true;
    track = "Power flow: Substation -> Home";
  } else if (currentStep == 2) {
    if (choice == 'A') correct = true;
    track = "Power flow: AC Charger -> OBC";
  } else if (currentStep == 3) {
    if (choice == 'C') correct = true;
    track = "Power flow: OBC -> Battery";
  }

  lastAnswerWasCorrect = correct; currentState = STATE_FEEDBACK;
  
  if (correct) {
    currentStep++;
    beep(1200, 100); delay(120);
    beep(1800, 150);
    tft.fillScreen(CLR_NEON_GREEN); tft.setTextColor(CLR_BG); tft.setTextSize(2); tft.setTextDatum(MC_DATUM);
    tft.drawString("CORRECT!", SCREEN_W/2, 65); tft.setTextSize(1); tft.drawString(track.c_str(), SCREEN_W/2, 145);

    if (track == "Power flow: Substation -> Home") {
      ledsRange(19, 26, CRGB::Red);
    } else if (track == "Power flow: AC Charger -> OBC") {
      ledsRange(27, 29, CRGB::Red);
    } else if (track == "Power flow: OBC -> Battery") {
      ledsRange(30, 31, CRGB::Green);
    }
  } else {
    beep(600, 100); delay(110);
    beep(300, 250);
    tft.fillScreen(CLR_ORANGE_RED); tft.setTextColor(CLR_WHITE); tft.setTextSize(2); tft.setTextDatum(MC_DATUM);
    tft.drawString("TRY AGAIN, EXPLORER!", SCREEN_W/2, 60); tft.setTextSize(1); tft.drawString("Think it through, Explorer!", SCREEN_W/2, 120);
  }
}