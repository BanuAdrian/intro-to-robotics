#include <Arduino.h>

/* viteza de transmisie a datelor pentru comunicarea seriala */
#define BAUD_RATE 28800

/* dificultatile posibile pentru joc */
#define EASY 0
#define MEDIUM 1
#define HARD 2

/* pinii aferenti butoanelor de start/stop si de selectare a dificultatii */
#define START_STOP_BTN 13 /* acestui buton ii corespunde intreruperea de tip pin change PCINT5 */
#define SELECT_DIFF_BTN 9 /* acestui buton ii corespunde intreruperea de tip pin change PCINT1 */

/* pinii aferenti LED-ului RGB */
#define RED_LED 12
#define GREEN_LED 11
#define BLUE_LED 10

/* valoarea implicita pentru timerele folosite la implementarea debouncing-ului si a duratei evenimentelor (e.g. durata unei runde) */
#define DEFAULT_TIME 0

/* limitele numaratorii inverse folosite la inceperea jocului */
#define COUNTER_TOP 3
#define COUNTER_BOTTOM 1

/* culorile pe care le poate avea LED-ul RGB in cadrul jocului */
#define RED_COLOR 1
#define GREEN_COLOR 2
#define WHITE_COLOR 3

/* caracterul ASCII aferent tastei Backspace */
#define BACKSPACE_CHAR '\b'

/* sirul de caractere vid, folosit la resetarea string-ului care retine cuvantul tastat */
#define EMPTY_STRING ""

/* intervalul de timp alocat pentru fiecare comutare a starii LED-ului RGB */
const unsigned int ledTogglingInterval = 600;

/* intervalul de timp alocat in total numaratorii */
const unsigned int countdownInterval = 3000;

/* intervalul de timp alocat fiecarei runde */
const unsigned int roundInterval = 30000;

/* timpul de debounce pentru butonul de start/stop */
const unsigned long debounceDelayStartStopBtn = 200; 

/* momentul de timp in care a avut loc ultima intrerupere generata de butonul de start/stop */
volatile unsigned long lastStartStopInterruptTime = DEFAULT_TIME;

/* timpul de debounce pentru butonul de selectare a dificultatii */
const unsigned long debounceDelaySelectDiffBtn = 200; 

/* momentul de timp in care a avut loc ultima intrerupere generata de butonul de selectare a dificultatii */
volatile unsigned long lastSelectDiffInterruptTime = DEFAULT_TIME;

/* retine daca runda a inceput sau nu */
volatile bool roundStarted = false;

/* retine daca numaratoarea a inceput sau nu */
volatile bool countdownStarted = false;

/* timer folosit pentru numaratoare, memoreaza momentul de timp precedent */
volatile unsigned long lastCountdownTime = DEFAULT_TIME;

/* timer folosit pentru numaratoare, memoreaza momentul de timp in care a inceput numaratoarea */
volatile unsigned long countdownStartTime = DEFAULT_TIME;

/* retine valoarea curenta a numaratorii inverse */
volatile uint8_t reverseCounter = 3;

/* retine daca se poate actualiza valoarea numaratorii, intrucat vrem sa numaram doar la clipirea completa a LED-ului */
volatile bool canUpdateCounter = true;

/* timer folosit pentru runde, memoreaza momentul de timp in care a inceput runda */
unsigned long roundStartTime = DEFAULT_TIME;

/* timer folosit pentru generarea de cuvinte noi, memoreaza momentul de timp (in secunde) in care a aparut noul cuvant */
unsigned long newWordStartSeconds = 0;

/* dictionar de cuvinte care vor fi afisate intr-o ordine aleatorie in cadrul unei runde */
char* words[] = {(char*)"mystery", (char*)"horizon", (char*)"delicate", (char*)"wilderness", (char*)"integrity", 
                 (char*)"synchronize", (char*)"enthusiasm", (char*)"curiosity", (char*)"formidable", (char*)"adventure", 
                 (char*)"tranquility", (char*)"imagination", (char*)"resilience", (char*)"mesmerizing", (char*)"challenge", 
                 (char*)"vibrant", (char*)"remarkable", (char*)"perception", (char*)"elaborate", (char*)"discovery", 
                 (char*)"ephemeral", (char*)"innovation", (char*)"extraordinary", (char*)"phenomenon", (char*)"commitment", 
                 (char*)"sensation", (char*)"courageous", (char*)"picturesque", (char*)"inspiration", (char*)"unpredictable", 
                 (char*)"metamorphosis", (char*)"illumination", (char*)"perspective", (char*)"exhilarating", (char*)"magnificent", 
                 (char*)"harmonious", (char*)"accomplishment", (char*)"spectacular", (char*)"authenticity"};

/* numarul de cuvinte din dictionar */
uint8_t wordsNumber = sizeof(words) / sizeof(words[0]);

/* dificultatea curenta a jocului */
volatile uint8_t gameDifficulty = EASY;

/* secundele alocate fiecarui cuvant in functie de dificultatea aleasa */
uint8_t secondsPerGameDifficulty[3] = {5, 4, 3};

/* retine daca cuvantul tastat este corect sau nu */
bool correctWord = false;

/* retine daca trebuie sa se schimbe cuvantul (i.e. sa se genereze altul) sau nu */
bool changeWord = true;

/* cuvantul curent de tastat */
char* currentWord = nullptr;

/* numarul de cuvinte tastate corect intr-o runda */
uint8_t numCorrectWords = 0;

/* cuvantul tastat de jucator */
String typedWord = EMPTY_STRING;

/* secundele care au trecut de la pornirea programului, aceasta variabila se va incrementa o data la o secunda in cadrul unei intreruperi 
generate de Timer1 */
volatile unsigned long seconds = DEFAULT_TIME;

/*
 * Functie ce seteaza culoarea LED-ului RGB
 *
 * @param ledColor - noua culoare a LED-ului RGB
 */
void setRGBColor(uint8_t ledColor) {
  /* aprinde si/sau stinge LED-urile componente in functie de culoarea dorita */
  if (ledColor == RED_COLOR) {
    /* aprinde LED-ul rosu si le stinge pe cel verde si cel albastru */
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
  } else if (ledColor == GREEN_COLOR) {
    /* aprinde LED-ul verde si le stinge pe cel rosu si cel albastru */
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
  } else if (ledColor == WHITE_COLOR) {
    /* aprinde toate cele 3 LED-uri pentru a forma culoarea alba care indica starea de repaus a jocului */
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
  }
}

/*
 * Functie ce comuta starea LED-ului RGB
 *
 * @param state - noua stare a LED-ului RGB
 */
void toggleRGBLed(bool state) {
  /* opreste sau aprinde LED-ul RGB */
  if (state == LOW) {
    /* opreste LED-ul RGB */
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
  } else if (state == HIGH) {
      /* aprinde LED-ul RGB */
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
  }
}

/*
 * Functie ce se ocupa de oprirea unei runde fie la terminarea timpului alocat, fie la apasarea butonului de stop 
 *
 */
void stopRound() {
  /* reseteaza timerele folosite */
  countdownStartTime = DEFAULT_TIME;
  lastCountdownTime = DEFAULT_TIME;

  /* marcheaza runda ca fiind oprita */
  roundStarted = false;
  
  /* marcheaza numaratoarea ca fiind oprita */
  countdownStarted = false;

  /* reseteaza posibilitatea de a actualiza valoarea numaratorii */
  canUpdateCounter = true;

  /* reseteaza valoarea numaratorii, pornind de la limita superioara */
  reverseCounter = COUNTER_TOP;

  /* afiseaza faptul ca runda s-a incheiat, precum si scorul obtinut de jucator */
  Serial.println("\nRound over!\nCorrect words: " + String(numCorrectWords) + "\n");

  /* reseteaza numarul de cuvinte tastate corect */
  numCorrectWords = 0;
  
  /* seteaza culoarea LED-ului RGB in alb */
  setRGBColor(WHITE_COLOR);

}

/*
 * Functie ce se ocupa de gestionarea apasarilor butonului de start/stop i.e. implementeaza debouncing, porneste sau opreste runda, inclusiv numaratoarea inversa
 * 
 */
void startStopHandler() {
  /* timer folosit pentru debouncing, memoreaza cate milisecunde au trecut de la pornirea programului */
  unsigned long startStopInterruptTime = millis();

  /* verifica daca butonul a fost mentinut apasat pentru cel putin debounceDelayStartStopBtn milisecunde */
  if (startStopInterruptTime - lastStartStopInterruptTime > debounceDelayStartStopBtn) {
    /* verifica daca runda si numaratoarea nu au inceput, caz in care le porneste, iar in caz contrar opreste runda */
    if (!roundStarted && !countdownStarted) {
      /* marcheaza pornirea numaratorii inverse */
      countdownStarted = true;

      /* stinge LED-ul RGB */
      toggleRGBLed(LOW);
    } else if (roundStarted) {
        /* opreste runda */
        stopRound();
    }
  }

  /* momentul de timp curent devine cel precedent la urmatoarea intrare in functie */
  lastStartStopInterruptTime = startStopInterruptTime;
}

/*
 * Functie ce se ocupa de gestionarea apasarilor butonului de selectare a dificultatii i.e. implementeaza debouncing si schimba dificultatea jocului in functie de cea curenta
 * 
 */
void selectGameDifficultyHandler() {
  /* timer folosit pentru debouncing, memoreaza cate milisecunde au trecut de la pornirea programului */
  unsigned long selectDiffInterruptTime = millis();

  /* verifica daca butonul a fost mentinut apasat pentru cel putin debounceDelaySelectDiffBtn milisecunde, iar runda si numaratoarea nu au inceput */
  if (selectDiffInterruptTime - lastSelectDiffInterruptTime > debounceDelaySelectDiffBtn && !roundStarted && !countdownStarted) {
    /* verifica dificultatea curenta a jocului si selecteaza pe urmatoarea */
    switch (gameDifficulty) {
      /* daca dificultatea curenta e "easy" */
      case EASY:
        /* selecteaza dificultatea "medium" si afiseaza un mesaj corespunzator */
        gameDifficulty = MEDIUM;
        Serial.println("Medium mode on!");
        break;

      /* daca dificultatea curenta e "medium" */
      case MEDIUM:
        /* selecteaza dificultatea "hard" si afiseaza un mesaj corespunzator */
        gameDifficulty = HARD;
        Serial.println("Hard mode on!");
        break;
      
      /* daca dificultatea curenta e "hard" */
      case HARD:
        /* selecteaza dificultatea "easy" si afiseaza un mesaj corespunzator */
        gameDifficulty = EASY;
        Serial.println("Easy mode on!");
        break;
    }
  }

  /* momentul de timp curent devine cel precedent la urmatoarea intrare in functie */
  lastSelectDiffInterruptTime = selectDiffInterruptTime;
}

/*
 * Functie ce se ocupa de golirea buffer-ului alocat comunicarii pe portul serial, deoarece nu vrem ca datele transmise in afara unei runde sa fie folosite la verificarea tastarii corecte a primului cuvant generat la inceperea unei noi runde 
 * 
 */
void clearSerialBuffer(){
  /* verifica daca sunt date in buffer care asteapta sa fie citite */
  while(Serial.available()) {
    /* citeste caracterele din buffer */
    Serial.read();
  }
}

/*
 * Functie ce se ocupa de numaratoarea inversa premergatoare inceperii unei runde
 * 
 */
void countdownHandler() {
  /* timer folosit pentru numaratoare, memoreaza cate milisecunde au trecut de la pornirea programului */
  unsigned long countdownTime = millis();
  
  /* verifica daca au trecut ledTogglingInterval milisecunde de la ultima comutare a starii LED-ului RGB */
  if ((countdownTime - lastCountdownTime) >= ledTogglingInterval) {
    /* seteaza momentul de timp in care a inceput numaratoarea */
    if (!countdownStartTime) {
      countdownStartTime = millis();
    }

    /* momentul de timp curent devine cel precedent */
    lastCountdownTime = countdownTime;

    /* comuta starea LED-ului RGB */
    toggleRGBLed(!digitalRead(RED_LED));
    
    /* verifica daca se poate actualiza valoarea numaratorii si daca nu s-a ajuns la limita inferioara a acesteia */
    if (canUpdateCounter && reverseCounter >= COUNTER_BOTTOM) {
      /* afiseaza numaratoarea */
      Serial.println(reverseCounter);

      /* decrementeaza valoarea numaratorii */
      reverseCounter--;
    }

    /* comuta posibilitatea de a actualiza numaratoarea, deoarece vrem sa facem acest lucru doar la clipirea completa a LED-ului */
    canUpdateCounter = !canUpdateCounter;
  } 

  /* verifica daca au trecut countdownInterval milisecunde de la inceperea numaratorii */
  if ((countdownTime - countdownStartTime) >= countdownInterval) {
    /* reseteaza timerele folosite */
    countdownStartTime = DEFAULT_TIME;
    lastCountdownTime = DEFAULT_TIME;
    newWordStartSeconds = DEFAULT_TIME;
    
    /* marcheaza momentul de timp in care s-a terminat numaratoarea ca fiind inceputul rundei */
    roundStartTime = millis();

    /* marcheaza numaratoarea ca fiind oprita */
    countdownStarted = false;

    /* marcheaza runda ca fiind pornita */
    roundStarted = true;

    /* reseteaza posibilitatea de a actualiza valoarea numaratorii */
    canUpdateCounter = true;
    
    /* marcheaza necesitatea de a genera un cuvant nou */
    changeWord = true;

    /* reseteaza valoarea numaratorii, pornind de la limita superioara */
    reverseCounter = COUNTER_TOP;

    /* goleste string-ul asociat cuvantului tastat */
    typedWord = EMPTY_STRING;

    /* goleste buffer-ul alocat comunicarii seriale */
    clearSerialBuffer();

    /* seteaza culoarea LED-ului RGB in verde */
    setRGBColor(GREEN_COLOR);
  }
}

/*
 * Functie ce se ocupa de generarea unui nou cuvant de tastat
 * 
 */
char* generateNewWord() {
  /* returneaza un cuvant aleator din dictionarul de cuvinte, folosind un indice aleator cuprins intre 0 si wordsNumber, exclusiv */
  return words[random(wordsNumber)];
}

/*
 * Functie ce se ocupa de desfasurarea unei runde
 * 
 */
void roundHandler() {
  /* timer folosit pentru runde, memoreaza cate milisecunde au trecut de la pornirea programului */
  unsigned long roundTime = millis();

  /* verifica daca timpul alocat rundei nu s-a scurs */
  if ((roundTime - roundStartTime) <= roundInterval) {
    /* verifica daca trebuie generat un nou cuvant de tastat */
    if (changeWord) {
      /* genereaza un nou cuvant de tastat */
      currentWord = generateNewWord();

      /* memoreaza momentul de timp (in secunde) in care a aparut noul cuvant */
      newWordStartSeconds = seconds;

      /* reseteaza necesitatea generarii unui nou cuvant de tastat */
      changeWord = false;

      /* afiseaza noul cuvant de tastat */
      Serial.println("\nNew word: " + String(currentWord));
    }

    /* verifica daca au trecut secundele alocate cuvantului in functie de dificultatea jocului sau daca acesta a fost tastat corect */
    if ((seconds - newWordStartSeconds) >= secondsPerGameDifficulty[gameDifficulty] || correctWord) {
      /* reseteaza faptul ca vechiul cuvant a fost tastat corect */
      correctWord = false;

      /* seteaza necesitatea generarii unui nou cuvant de tastat */
      changeWord = true;

      /* goleste string-ul asociat cuvantului tastat */
      typedWord = EMPTY_STRING;
    } else {
        /* verifica daca sunt date in buffer-ul alocat comunicarii pe portul serial care asteapta sa fie citite */
        if (Serial.available()) {
          /* citeste urmatorul caracter din buffer */
          char readChar = Serial.read();

          /* verifica daca caracterul citit a fost un Backspace */
          if (readChar == BACKSPACE_CHAR) {
            /* sterge ultimul caracter tastat */
            typedWord.remove(typedWord.length() - 1);
          } else {
              /* adauga noul caracter la string-ul asociat cuvantului tastat */
              typedWord += readChar;

              /* verifica daca caracterele tastate nu corespund cuvantului care trebuie scris */
              if (strncmp(typedWord.c_str(), currentWord, typedWord.length()) != 0) {
                /* seteaza culoarea LED-ului RGB in rosu */
                setRGBColor(RED_COLOR);
              } else {
                  /* seteaza culoarea LED-ului RGB in verde */
                  setRGBColor(GREEN_COLOR);

                  /* verifica daca s-a tastat corect tot cuvantul */
                  if (typedWord.length() == String(currentWord).length()) {
                    /* marcheaza faptul ca tastarea cuvantului este corecta */
                    correctWord = true;

                    /* incrementeaza numarul de cuvinte tastate corect */
                    numCorrectWords++;
                  }
              }
          }
        }
    }
  } else {
      /* opreste runda la trecerea intervalului alocat rundei */
      stopRound();
  }
}

/*
 * Functie ce asociaza intreruperilor declansate de butoanele de start/stop si de selectare a dificultatii cate un handler
 * 
 */
ISR(PCINT0_vect){
  /* verifica daca intreruperea a fost declansata de butonul de start/stop */
  if ((PINB & (1 << PB5)) == 0){
      /* apeleaza handler-ul asociat acestui buton */
      startStopHandler();
  }

  /* verifica daca intreruperea a fost declansata de butonul de selectare a dificultatii */
  if ((PINB & (1 << PB1)) == 0){
      /* apeleaza handler-ul asociat acestui buton */
      selectGameDifficultyHandler();
  }
}

/*
 * Functie ce incrementeaza secundele trecute de la pornirea programului la declansarea unei intreruperi de tip Timer/Counter1 Compare Match A
 * 
 */
ISR(TIMER1_COMPA_vect) {
  seconds++;
}

void setup() {
  /* seteaza LED-urile componente LED-ului RGB ca output */
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  /* seteaza culoarea LED-ului RGB in alb pentru a indica starea de repaus */
  setRGBColor(WHITE_COLOR);

  /* seteaza cele doua butoane folosite ca input si activeaza rezistentele de pull-up aferente */
  pinMode(START_STOP_BTN, INPUT_PULLUP);
  pinMode(SELECT_DIFF_BTN, INPUT_PULLUP);

  /* incepe comunicarea seriala cu o viteza de transmisie a datelor data de BAUD_RATE */
  Serial.begin(BAUD_RATE);

  /* activeaza vectorul de intreruperi pentru intreruperile de tip PCINT0 */
  PCICR |= (1 << PCIE0);

  /* activeaza intreruperile de tip pin change pe pin-urile PCINT1 si PCINT5 */
  PCMSK0 |= ((1 << PCINT1) | (1 << PCINT5));

  /* reseteaza TCCR1A */
  TCCR1A = 0;

  /* seteaza Timer1 in mod CTC si Prescaler la 256 */
  TCCR1B = (1 << WGM12) | (1 << CS12);

  /* activeaza interuperea la atingerea valorii OCR1A de catre contorul asociat lui Timer1 */
  TIMSK1 |= (1 << OCIE1A);

  /* seteaza valoarea asociata unei frecvente de 1 Hz pentru Timer1 */
  OCR1A = 62500;

  /* activeaza intreruperile la nivel global */
  sei();
}

void loop() {
  /* verifica daca numaratoarea sau runda sunt incepute */
  if (countdownStarted) {
    /* gestioneaza numaratoarea inversa */
    countdownHandler();
  } else if (roundStarted) {
      /* gestioneaza runda */
      roundHandler();
  }
}
