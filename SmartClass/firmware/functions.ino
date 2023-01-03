void sensorTick() {
  button.tick();
  boolean changeFlag = false;
  if (button.isClick()) {
    mode++;

#if (CO2_SENSOR == 1)
    if (mode > 3) mode = 0;
#else
    if (mode > 2) mode = 0;
#endif
    changeFlag = true;
  }
  if (button.isHolded()) {
    mode = 0;
    changeFlag = true;
  }

  if (changeFlag) {
    if (mode == 0) {
      lcd.clear();
      loadClock();
      drawClock(hrs, mins, 0, 0, 1);
      if (DISPLAY_TYPE == 1) drawData();
      drawSensors();
    } else {
      lcd.clear();
      loadPlot();
      redrawPlot();
    }
  }
}

void redrawPlot() {
  lcd.clear();
  switch (mode) {
    case 1: drawPlot(0, 3, 15, 4, TEMP_MIN, TEMP_MAX, (int*)tempHour, "t hr");
      break;
   
    case 2: drawPlot(0, 3, 15, 4, HUM_MIN, HUM_MAX, (int*)humHour, "h hr");
      break;
    
   
    case 3: drawPlot(0, 3, 15, 4, CO2_MIN, CO2_MAX, (int*)co2Hour, "c hr");
      break;
   
  }
}

void readSensors() {
 if(myHTU21D.measure()) {
  dispTemp = myHTU21D.getTemperature();
  dispHum = myHTU21D.getHumidity();
 }
#if (CO2_SENSOR == 1)
  ccs.readData();
  dispCO2 = ccs.geteCO2();
if (millis() - myTimer >= 3000000) {
    CO2check();
     
  }
  if (dispCO2 < 800) ;
  else if (dispCO2 < 1200) ;
  else if (dispCO2 >= 1200) ;
#endif

#if (TVOC_SENSOR == 1 && CO2_SENSOR == 1)
  ccs.readData();
  dispTVOC = ccs.getTVOC();
  #endif
  
  #if (LIGHT_SENSOR == 1)
  light.readLightLevel();
  dispLight = light.readLightLevel();
  
}

void drawSensors() {
#if (DISPLAY_TYPE == 1)
  // дисплей 2004
   if(myHTU21D.measure()) {
  lcd.setCursor(1, 2);
  lcd.print(String(dispTemp, 1));
  lcd.write(223);
  lcd.setCursor(6, 2);
  lcd.print(" " + String(dispHum) + "% ");
   }
#if (CO2_SENSOR == 1)
  lcd.print(String(dispCO2) + " ppm");
  if (dispCO2 < 1000) lcd.print(" ");
#endif

  lcd.setCursor(11, 3);
  lcd.print(String(dispTVOC) + " ppb ");
 
  lcd.setCursor(1, 3);
  lcd.print(String(dispLight) + " lx");
#endif
#endif
}

void plotSensorsTick() {
  // 4 минутный таймер
  if (hourPlotTimer.isReady()) {
    for (byte i = 0; i < 14; i++) {
      tempHour[i] = tempHour[i + 1];
      humHour[i] = humHour[i + 1];
     
      co2Hour[i] = co2Hour[i + 1];
    }
    tempHour[14] = dispTemp;
    humHour[14] = dispHum;
    co2Hour[14] = dispCO2;

    
  }

  // 1.5 часовой таймер
  if (dayPlotTimer.isReady()) {
    long averTemp = 0, averHum = 0, averPress = 0, averCO2 = 0;

    for (byte i = 0; i < 15; i++) {
      averTemp += tempHour[i];
      averHum += humHour[i];
     
      averCO2 += co2Hour[i];
    }
    averTemp /= 15;
    averHum /= 15;
    averPress /= 15;
    averCO2 /= 15;

   

  
  }
}


// функция для генерации трех тире
void Lightalarm(){
   for (int i=0; i<3; i++){
    tone(PIN, note, 100);
    delay(200);
    noTone(PIN);
}
}
void alarm(){
   for (int i=0; i<3; i++){
    tone(PIN, note, 100);
    delay(200); 
    noTone(PIN);
  }
  
  for (int i=0; i<3; i++){
    tone(PIN, note, 300);
    delay(400);
    noTone(PIN);
  }
  
   for (int i=0; i<3; i++){
    tone(PIN, note, 100);
    delay(200);
    noTone(PIN);
    
  }
   
}


void lightCheck(){
  
   if (dispLight <= 300) {
    myTimerl = millis();
    checkPeriodl = 60000;
     Lightalarm();
     
   }else{
    checkPeriod = 1000;
   }
   
}


void CO2check(){
 
   if (dispCO2 >= 800) {
    myTimer = millis();
    checkPeriod = 300000;
     alarm();
     
   }else{
    checkPeriod = 1000;
   }
  
}

boolean dotFlag;
void clockTick() {
  dotFlag = !dotFlag;
  if (dotFlag) {          // каждую секунду пересчёт времени
    secs++;
    if (secs > 59) {      // каждую минуту
      secs = 0;
      mins++;
      if (mins <= 59 && mode == 0) drawClock(hrs, mins, 0, 0, 1);
    }
    if (mins > 59) {      // каждый час
      now = rtc.now();
      secs = now.second();
      mins = now.minute();
      hrs = now.hour();
      if (mode == 0) drawClock(hrs, mins, 0, 0, 1);
      if (hrs > 23) {
        hrs = 0;
      }
      if (mode == 0 && DISPLAY_TYPE) drawData();
    }
    if (DISP_MODE == 2 && mode == 0) {
      lcd.setCursor(16, 1);
      if (secs < 10) lcd.print(" ");
      lcd.print(secs);
    }
  }
  if (mode == 0) drawdots(7, 0, dotFlag);

   
   
  }
