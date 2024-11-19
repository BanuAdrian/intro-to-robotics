# Quick Time

## Descriere

În cadrul acestui task am lucrat în echipe de câte doi pentru a crea un joc de reflex competitiv în care ambii participanți concurează pentru a obține cel mai mare punctaj, testându-și viteza de reacție. 

În ceea ce privește modul de desfășurare, fiecare jucător dispune de un set propriu de butoane și LED-uri, iar jocul este organizat pe mai multe runde. Pentru a începe, oricare dintre jucători poate apăsa butonul de start, moment în care ecranul LCD care afișa mesajul de bun venit va cere jucătorilor să-și introducă numele de la tastatură, actualizându-se pe măsură ce sunt introduse caractere, utilizatorii având și posibilitatea de a șterge textul introdus în caz de greșeală. Ulterior, buzzer-ul va începe să redea note muzicale, cele două grupuri de LED-uri se vor aprinde, precum și LED-ul RGB al primului jucător, indicând startul jocului. Mai mult, jocul are atașat și un servomotor care se rotește în sens antiorar pe parcursul jocului, indicând progresul. O rotație completă a acestuia marchează sfârșitul jocului.

Obiectivul fiecărui participant este să apese cât mai rapid butonul corespunzător culorii indicate de LED-ul RGB alocat acestuia, primind astfel un punctaj în funcție de viteza de reacție. Atât în cazul unui răspuns corect, cât și a unuia greșit, buzzer-ul va reda o notă muzicală specifică. Scorurile jucătorilor vor fi afișate în timp real pe un ecran LCD și actualizate pe parcursul desfășurării jocului. La final, jucătorul care obține cel mai mare scor va fi desemnat câștigător, buzzer-ul va reda din nou notele muzicale de la începutul jocului, servomotorul va reveni la poziția inițială, iar pe ecranul LCD se va afișa mesajul de bun venit. 


## Componente utilizate

- 6x LED-uri (2 grupuri de câte 3 leduri, în cadrul unui grup trebuie să avem culori diferite)
- 2x LED RGB (1 pentru fiecare jucător)
- 7x butoane (3 pentru fiecare jucător și 1 pentru start)
- 1x LCD
- 1x servomotor
- 2x Breadboard
- Fire de legatură
- 2x Arduino Uno
- 1x Buzzer
- 22x Rezistoare (14x 220 ohm, 1x 330 ohm, 3x 1K, 2x 5K1, 2x 100K)
- 1x Potențiometru (50K)

## Setup-ul fizic

![robo3-1](https://github.com/user-attachments/assets/8aa6e88a-874a-4d70-b960-b22bcc80078b)

![robo3-2](https://github.com/user-attachments/assets/a9d1d4a8-252c-476f-9b0f-328c56bbffce)

![robo3-3](https://github.com/user-attachments/assets/5b65a3e6-4600-4eae-911e-5a1fedd1d93f)

![robo3-4](https://github.com/user-attachments/assets/dcc892c2-9a30-41b2-8646-bfe12b8c7cb1)


## Funcționalitatea montajului fizic

[![Functionalitatea montajului fizic](https://img.youtube.com/vi/DmOSkErLF6A/0.jpg)](https://www.youtube.com/watch?v=DmOSkErLF6A)

## Schema electrică

![schema-tema3](https://github.com/user-attachments/assets/d0cf3bb6-62aa-4117-986f-820afc4d699f)




