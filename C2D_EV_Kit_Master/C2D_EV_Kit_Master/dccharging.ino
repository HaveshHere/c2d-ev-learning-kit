void skipDCStep() {
  if (currentStep == 1) ledsRange(9, 1, CRGB::Red);
  else if (currentStep == 2) ledsRange(36, 32, CRGB::Green);
  else if (currentStep == 3) ledsRange(46, 44, CRGB::Green);
  else if (currentStep == 4) ledsRange(43, 42, CRGB::Green);
  else if (currentStep == 5) ledsRange(41, 37, CRGB::Green);
  else if (currentStep == 6) ledsRange(36, 32, CRGB::Green);
}

void displayModule02(int startX, int startY) {
  tft.setTextColor(CLR_TEXT_MUTED); tft.setTextSize(1); tft.setTextDatum(TL_DATUM);
  int gap = 24; 

  tft.drawString(" 1. Grid to DC Charger Processing", startX, startY);
  tft.drawString(" 2. Solar & Battery Storage Systems", startX, startY + gap);
  tft.drawString(" 3. Direct EV Charging Bypass Logic", startX, startY + (gap * 2));
}

void runDCChargingFlow() {
  if (currentStep == 0) {
    ledsAllClear();
    ledsRange(14, 10, CRGB::Red);
    drawStartScreen("DC FAST CHARGING MODE", "Power Flow: Tower -> Substation");
    currentStep = 1;
    tft.fillScreen(CLR_BG);
  }
  
  if (currentStep == 1) {
    drawQuestionLayout(1, "What power supply does TNB provide to charging stations??", "A. Single-phase, 230V", "B. Three-phase, 400V", "C. DC Current, 12V");
  } else if (currentStep == 2) {
    drawQuestionLayout(2, "Where is AC converted to DC in fast charging?", "A. Inside the charging station kiosk", "B. Inside the vehicle", "C. Along the power cables");
  } else if (currentStep == 3) {
    drawQuestionLayout(3, "Solar panels produce:", "A. AC electricity", "B. Mechanical energy", "C. DC electricity");
  } else if (currentStep == 4) {
    drawQuestionLayout(4, "Why do we need a DC-DC Converter?", "A. To convert AC power into DC power", "B. To regulate solar voltage for the battery", "C. To convert electricity into chemical fluid");
  } else if (currentStep == 5) {
    drawQuestionLayout(5, "What does a BESS do at a station?", "A. It reduces the peak power demand", "B. It cools down the EV charging cable", "C. It changes the chemical reaction");
  } else if (currentStep == 6) {
    drawQuestionLayout(6, "Does DC fast charging use the vehicle's OBC?", "A. Yes", "B. No", "C. Sometimes");
  } else if (currentStep == 7) {
    ledsAllClear();
    drawMissionComplete("DC Fast Charge Pro!", "Energy bypassed the OBC successfully!");
  }
}

void evaluateDCQuestion(char choice) {
  bool correct = false; String track = "";
  
  if (currentStep == 1) { if (choice == 'B') correct = true; track = "Power Flow: Substation -> Charging Station"; }
  else if (currentStep == 2) { if (choice == 'A') correct = true; track = "Power Flow: DC Charger -> Battery"; }
  else if (currentStep == 3) { if (choice == 'C') correct = true; track = "Power Flow: Solar Panel -> DC-DC Converter"; }
  else if (currentStep == 4) { if (choice == 'B') correct = true; track = "Power Flow: DC-DC Converter -> BESS"; }
  else if (currentStep == 5) { if (choice == 'A') correct = true; track = "Power Flow: Solar -> BESS -> Charging Station"; }
  else if (currentStep == 6) { if (choice == 'B') correct = true; track = "Power Flow: DC Charger -> Battery"; }

  lastAnswerWasCorrect = correct; currentState = STATE_FEEDBACK;
  
  if (correct) {
    currentStep++;
    beep(1200, 100); delay(120);
    beep(1800, 150);
    tft.fillScreen(CLR_NEON_GREEN); tft.setTextColor(CLR_BG); tft.setTextSize(2); tft.setTextDatum(MC_DATUM);
    tft.drawString("CORRECT!", SCREEN_W/2, 65); tft.setTextSize(1); tft.drawString(track.c_str(), SCREEN_W/2, 145);
    
    if (track == "Power Flow: Substation -> Charging Station") {
      ledsRange(9, 1, CRGB::Red);
    } else if (track == "Power Flow: DC Charger -> Battery") {
      ledsRange(36, 32, CRGB::Green);
    } else if (track == "Power Flow: Solar Panel -> DC-DC Converter") {
      ledsRange(46, 44, CRGB::Green);
    } else if (track == "Power Flow: DC-DC Converter -> BESS") {
      ledsRange(43, 42, CRGB::Green);
    } else if (track == "Power Flow: Solar -> BESS -> Charging Station") {
      ledsRange(41, 37, CRGB::Green);
    } else if (track == "Power Flow: DC Charger -> Battery") {
      ledsRange(36, 32, CRGB::Green);
    }
  } else {
    beep(600, 100); delay(110);
    beep(300, 250);
    tft.fillScreen(CLR_ORANGE_RED); tft.setTextColor(CLR_WHITE); tft.setTextSize(2); tft.setTextDatum(MC_DATUM);
    tft.drawString("TRY AGAIN, EXPLORER!", SCREEN_W/2, 60); tft.setTextSize(1); tft.drawString("Think it through, Explorer!", SCREEN_W/2, 120);
  }
}