
# TypeRacer Game

## Descriere

TypeRacer este un joc online în care jucătorii tastează texte cât de repede pot, concurând individual sau împotriva altor utilizatori.

În acest sens, în cadrul acestui task am creat un joc asemănător cu TypeRacer. Inițial, jocul se află în starea de repaus, acest lucru fiind indicat de culoarea albă a LED-ului RGB. Înainte de a porni o rundă nouă, jucătorul poate să aleagă dificultatea jocului  utilizând butonul dedicat, fiind informat în terminal cu privire la selecția făcută. Ulterior, el trebuie să apese pe butonul de start/stop, care va face ca LED-ul să clipească timp de 3 secunde, în această perioadă fiind afișată și o numărătoare inversă în terminal. La finalul ei, LED-ul devine verde și încep să apară cuvinte de tastat cu o frecvență stabilită de dificultatea aleasă de jucător sau la tastarea corectă și completă a cuvântului curent. În cadrul unei runde, LED-ul va fi verde dacă textul introdus este corect și va deveni roșu în caz de greșeală, care poate fi remediată utilizând tasta Backspace. O rundă se termină după 30 de secunde sau la apăsarea butonului de start/stop. În ambele cazuri, se va afișa în terminal câte cuvinte au fost scrise corect.

## Componente utilizate

- Arduino UNO (ATmega328P microcontroller)
- 1x LED RGB (pentru a semnaliza dacă cuvântul corect e scris greșit sau nu)
- 2x Butoane (pentru start/stop rundă și pentru selectarea dificultății)
- 5x Rezistoare (3x 220 ohm, 2x 1000 ohm)
- Breadboard
- Fire de legătură

## Setup-ul fizic
![robo-2-1](https://github.com/user-attachments/assets/3b5e656d-9b5b-4bca-8302-18e0d8ea7b3e)

![robo-2-2](https://github.com/user-attachments/assets/a30e7182-9f70-4ec3-be4c-470f90450d99)

![robo-2-3](https://github.com/user-attachments/assets/3cd3efbd-b3e8-41d2-af9e-59995b245b31)

![robo-2-4](https://github.com/user-attachments/assets/a9546c19-1e9c-44c8-8fb1-88fd57e6d38c)

## Funcționalitatea montajului fizic

[![Functionalitatea montajului fizic](https://img.youtube.com/vi/gy9aOB4CInc/0.jpg)](https://www.youtube.com/watch?v=gy9aOB4CInc)

## Schema electrică [Wokwi]

![image](https://github.com/user-attachments/assets/d9ae25aa-98e9-4774-99b9-058d17376714)




