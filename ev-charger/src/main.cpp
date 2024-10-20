#include <Arduino.h>

#define FIRST_LED 10
#define SECOND_LED 9
#define THIRD_LED 8
#define FOURTH_LED 7

#define START_BUTTON 6
#define STOP_BUTTON 5

#define RED_LED 4
#define GREEN_LED 3
#define BLUE_LED 2

#define FIRST_STAGE 1
#define SECOND_STAGE 2
#define THIRD_STAGE 3
#define LAST_STAGE 4

#define BLINKING_INTERVAL 750
#define EACH_LED_INTERVAL 3000
#define STOP_ANIM_INTERVAL 4000

#define DELAY_BEFORE_CHARGING_ANIM 100
#define DELAY_BEFORE_STOP_ANIM 750

#define DEFAULT_MILLIS 0

bool startBtnState = HIGH;
bool lastStartBtnState = HIGH;
unsigned long lastDebounceTimeStartBtn = DEFAULT_MILLIS;
unsigned long debounceDelayStartBtn = 50; 

bool stopBtnState = HIGH;
bool lastStopBtnState = HIGH;
unsigned long lastDebounceTimeStopBtn = DEFAULT_MILLIS;
unsigned long debounceDelayStopBtn = 1000;

bool isCharging = false;
bool stoppedCharging = false;
uint8_t chargingStage = FIRST_STAGE;

unsigned long blinkingStartMillis = DEFAULT_MILLIS;
unsigned long currentMillis = DEFAULT_MILLIS;
unsigned long prevMillis = DEFAULT_MILLIS;


void setup() {
  /* seteaza toate LED-urile ca OUTPUT si le stinge */
  pinMode(FIRST_LED, OUTPUT);
  digitalWrite(FIRST_LED, LOW);

  pinMode(SECOND_LED, OUTPUT);
  digitalWrite(SECOND_LED, LOW);

  pinMode(THIRD_LED, OUTPUT);
  digitalWrite(THIRD_LED, LOW);

  pinMode(FOURTH_LED, OUTPUT);
  digitalWrite(FOURTH_LED, LOW);

  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);

  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);

  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, LOW);

  /* seteaza butoanele de start si stop ca INPUT si activeaza rezistentele de PULL-UP aferente */
  pinMode(START_BUTTON, INPUT);
  digitalWrite(START_BUTTON, HIGH);

  pinMode(STOP_BUTTON, INPUT);
  digitalWrite(STOP_BUTTON, HIGH);
}

/*
 * Functie ce seteaza culoarea LED-ului RGB
 *
 * @param led - LED-ul care indica disponibilitatea statiei de incarcare
 */
void setRGBLed(uint8_t led) {
  /* aprinde LED-ul corespunzator si le stinge pe restul */
  if (led == RED_LED) {
    /* aprinde LED-ul rosu si le stinge pe cel verde si cel albastru */
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
  } else if (led == GREEN_LED) {
    /* aprinde LED-ul verde si le stinge pe cel rosu si cel albastru */
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
  } else if (led == BLUE_LED) {
    /* aprinde LED-ul albastru si le stinge pe cel rosu si cel verde */
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
  }
}

/*
 * Functie ce aprinde sau stinge simultan cele 4 LED-uri care indica procentul de incarcare
 *
 * @param ledState - noua stare a LED-urilor
 */
void toggleAllLeds(bool ledState) {
  /* seteaza LED-urile in noua stare */
  if (ledState == HIGH) {
    /* aprinde toate LED-urile */
    digitalWrite(FIRST_LED, HIGH);
    digitalWrite(SECOND_LED, HIGH);
    digitalWrite(THIRD_LED, HIGH);
    digitalWrite(FOURTH_LED, HIGH);  
  } else if (ledState == LOW) {
    /* stinge toate LED-urile */
    digitalWrite(FIRST_LED, LOW);
    digitalWrite(SECOND_LED, LOW);
    digitalWrite(THIRD_LED, LOW);
    digitalWrite(FOURTH_LED, LOW);  
  }
}

/*
 * Functie ce se ocupa de procesul de incarcare a bateriei
 *
 * @param currentLed - LED-ul care indica gradul de incarcare al bateriei
 */
void chargingHandler(uint8_t currentLed) {
  /* memoreaza milisecundele care au trecut de la pornirea programului */
  currentMillis = millis();
  
  /* verifica daca au trecut BLINKING_INTERVAL milisecunde de la ultima comutare a starii LED-ului */
  if ((currentMillis - prevMillis) >= BLINKING_INTERVAL) {
    /* seteaza momentul de timp in care a avut loc prima comutare a starii LED-ului */
    if (!blinkingStartMillis) 
      blinkingStartMillis = millis();

    /* memoreaza momentul de timp curent */
    prevMillis = currentMillis;

    /* comuta starea LED-ului */
    digitalWrite(currentLed, !digitalRead(currentLed));
  } 

  /* verifica daca au trecut EACH_LED_INTERVAL milisecunde de la prima comutare a LED-ului */
  if ((millis() - blinkingStartMillis) >= EACH_LED_INTERVAL) {
    /* reseteaza timerele folosite */
    blinkingStartMillis = DEFAULT_MILLIS;
    prevMillis = DEFAULT_MILLIS;

    /* verifica daca s-a ajuns la finalul incarcarii */
    if (chargingStage == LAST_STAGE) {
      /* stinge toate LED-urile */
      toggleAllLeds(LOW);
      
      /* aplica o intarziere inainte de a incepe animatia de finalizare a incarcarii */
      delay(DELAY_BEFORE_STOP_ANIM);

      /* seteaza faptul ca incarcarea s-a oprit */
      stoppedCharging = true;
    } else {
      /* mentine LED-ul curent aprins */
      digitalWrite(currentLed, HIGH);

      /* trece la urmatoarea etapa a incarcarii */
      chargingStage++;
    }
  }
}

/*
 * Functie ce se ocupa de finalizarea incarcarii bateriei
 *
 */
void stoppedChargingHandler() {
  /* memoreaza milisecundele care au trecut de la pornirea programului */
  currentMillis = millis();

  /* verifica daca au trecut BLINKING_INTERVAL milisecunde de la ultima comutare a starii celor 4 LED-uri */
  if ((currentMillis - prevMillis) >= BLINKING_INTERVAL) {
    /* seteaza momentul de timp in care a avut loc prima comutare a starii LED-urilor */
    if (!blinkingStartMillis)
      blinkingStartMillis = millis();

    /* memoreaza momentul de timp curent */
    prevMillis = currentMillis;

    /* comuta simultan cele 4 LED-uri care indica procentul de incarcare */
    toggleAllLeds(!digitalRead(FIRST_LED));
  }

  /* verifica daca au trecut STOP_ANIM_INTERVAL milisecunde de la prima comutare a celor 4 LED-uri */
  if ((millis() - blinkingStartMillis) >= STOP_ANIM_INTERVAL) {
    /* seteaza LED-ul pentru disponibilitate in verde */
    setRGBLed(GREEN_LED);

    /* stinge cele 4 LED-uri care indica procentul de incarcare */
    toggleAllLeds(LOW);

    /* reseteaza timerele folosite */
    blinkingStartMillis = DEFAULT_MILLIS;
    prevMillis = DEFAULT_MILLIS;

    /* reseteaza statia la starea libera */
    stoppedCharging = false;
    isCharging = false;
    chargingStage = FIRST_STAGE;
  }
}

/*
 * Functie ce se ocupa de prinderea apasarilor butonului de start
 *
 */
void startBtnHandler() {
  /* citeste valoarea butonului de start intr-o variabila locala */
  bool startBtnReading = digitalRead(START_BUTTON);

  /* verifica daca starea butonului s-a schimbat */
  if (startBtnReading != lastStartBtnState)
    /* memoreaza momentul de timp in care butonul si-a schimbat starea */
    lastDebounceTimeStartBtn = millis();

  /* verifica daca noua stare a butonului a fost mentinuta pentru cel putin debounceDelayStartBtn milisecunde */
  if ((millis() - lastDebounceTimeStartBtn) > debounceDelayStartBtn) {
      /* reverifica daca starea butonului s-a schimbat */
      if (startBtnReading != startBtnState) {
        /* salveaza noua stare a butonului */
        startBtnState = startBtnReading;

        /* verifica daca butonul a fost apasat si statia de incarcare este in starea libera */
        if (startBtnState == LOW && !isCharging) {
          /* seteaza faptul ca statia incarca */
          isCharging = true;

          /* seteaza LED-ul pentru disponibilitate in rosu */
          setRGBLed(RED_LED);

          /* aplica o intarziere inainte de a incepe animatia procesului de incarcare */
          delay(DELAY_BEFORE_CHARGING_ANIM);
        }
      }
  }
  /* salveaza starea curenta a butonului ca ultima stare */
  lastStartBtnState = startBtnReading;
}

/*
 * Functie ce se ocupa de prinderea apasarilor butonului de stop
 *
 */
void stopBtnHandler() {
  /* citeste valoarea butonului de stop intr-o variabila locala */
  bool stopBtnReading = digitalRead(STOP_BUTTON);

  /* verifica daca starea butonului s-a schimbat */
  if (stopBtnReading != lastStopBtnState)
      /* memoreaza momentul de timp in care butonul si-a schimbat starea */
      lastDebounceTimeStopBtn = millis();

  /* verifica daca noua stare a butonului a fost mentinuta pentru cel putin debounceDelayStopBtn milisecunde */
  if ((millis() - lastDebounceTimeStopBtn) > debounceDelayStopBtn) {
    /* reverifica daca starea butonului s-a schimbat */
    if (stopBtnReading != stopBtnState) {
      /* salveaza noua stare a butonului */
      stopBtnState = stopBtnReading;

      /* verifica daca butonul a fost apasat si statia de incarcare nu este in starea libera, dar nici in animatia de finalizare */
      if (stopBtnState == LOW && isCharging && !stoppedCharging) {
        /* seteaza faptul ca statia a fost oprita  */
        stoppedCharging = true;
        isCharging = false;

        /* reseteaza timerele folosite la animatia de finalizare */
        blinkingStartMillis = DEFAULT_MILLIS;
        prevMillis = DEFAULT_MILLIS;

        /* stinge cele 4 LED-uri care indica procentul de incarcare */
        toggleAllLeds(LOW);

        /* aplica o intarziere inainte de a incepe animatia de finalizare a procesului de incarcare */
        delay(DELAY_BEFORE_STOP_ANIM);
      }
    }
  }
  /* salveaza starea curenta a butonului ca ultima stare */
  lastStopBtnState = stopBtnReading;
}

void loop() {
    /* prinde apasarile butoanelor de start si stop */
    startBtnHandler();
    stopBtnHandler();

    /* verifica daca statia incarca si nu este in animatia de finalizare */
    if (isCharging && !stoppedCharging) {
      /* verifica gradul de incarcare al bateriei */
      if (chargingStage == FIRST_STAGE) {
        /* bateria se afla intre 0-25% */
        chargingHandler(FIRST_LED);
      } else if (chargingStage == SECOND_STAGE) {
        /* bateria se afla intre 25-50% */
        chargingHandler(SECOND_LED);
      } else if (chargingStage == THIRD_STAGE) {
        /* bateria se afla intre 50-75% */
        chargingHandler(THIRD_LED);
      } else if (chargingStage == LAST_STAGE) {
        /* bateria se afla intre 75-100% */
        chargingHandler(FOURTH_LED);
      }
    }
    
    /* verifica daca incarcarea s-a finalizat */
    if (stoppedCharging)
      stoppedChargingHandler();
}
