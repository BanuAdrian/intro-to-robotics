# EV Charger

## Descriere

În cadrul acestui task am utilizat mai multe LED-uri și butoane și le-am coordonat pentru a simula o stație de încărcare a unui vehicul electric.

Inițial, starea stației este liberă, adică LED-urile care indică gradul de încărcare al bateriei sunt stinse, iar LED-ul pentru disponibilitate este verde. La apăsarea butonului de start procesul de încărcare începe prin modificarea LED-ului din urmă în roșu și clipirea timp de 3 secunde a LED-ului care indică primul interval de încărcare al bateriei. La încărcarea cu un procent de 25%, LED-ul curent rămâne pornit și se face trecerea către următorul LED, care va începe să clipească timp de 3 secunde. Acest proces este reluat până fie se ajunge la 100%, fie butonul de stop este ținut apăsat timp de minimum 1 secundă. În ambele cazuri, încărcarea se oprește, acest lucru fiind indicat prin clipirea simultană a tuturor LED-urilor de 3 ori, urmată de stingerea acestora și modificarea LED-ului pentru disponibilitate în verde.

De menționat este faptul că la apăsarea butonului de start în timpul încărcării sau a butonului de stop cât timp stația este liberă nu se întamplă nimic. Mai mult, pentru prinderea apăsărilor butoanelor am implementat cod care să facă debouncing. 

## Componente utilizate

- 4x LED-uri albastre (pentru a simula procentul de încărcare)
- 1x LED RGB (pentru starea de liber sau ocupat)
- 2x Butoane (pentru start încărcare și stop încărcare)
- 9x Rezistoare (7x 220ohm, 2x 1K)
- Breadboard
- Linii de legătură

## Setup-ul fizic
![robo-1](https://github.com/user-attachments/assets/02bfb440-f7bf-406f-bc58-8fa8ac8f3d8c)

![robo-2](https://github.com/user-attachments/assets/9c3c189a-9a34-4487-8dcf-13d8c7b28109)

![robo-3](https://github.com/user-attachments/assets/68544f02-8446-4a92-9b72-d5c793708e0a)

![robo-4](https://github.com/user-attachments/assets/a8e289b4-2dd3-44e6-b4c7-502b68d93688)


## Funcționalitatea montajului fizic

[![Functionalitatea montajului fizic](https://img.youtube.com/vi/I25eiOSB5YE/0.jpg)](https://www.youtube.com/watch?v=I25eiOSB5YE)

## Schema electrică

![image](https://github.com/user-attachments/assets/967b6c8d-2c47-4a19-9940-226a5d42bbd3)

