# Carent
**Symulator oprogramowania CRM/ERM dla firmy prowadzącej wypożyczalnie pojazdów.**

## Założenia projektowe

- **Przeglądanie i filtrowanie floty:** Wyświetlanie listy pojazdów w przejrzystej tabeli z podziałem na kategorie, ich aktualny status oraz dodatkowe informacje.
- **Obsługa wypożyczeń i zwrotów:** Możliwość wyboru dostępnego pojazdu z listy, określenia czasu najmu oraz zatwierdzanie wypożyczenia lub zwrotu pojazdu jedyn przyciskiem, generator umowy oraz protokołu zdawczo-odbiorczego PDF.
- **Automatyczny kalkulator kosztów:** Wyliczanie końcowej ceny wypożyczenia w zależności od kategorii pojazdu i jego specyficznych parametrów.
- **Zarządzanie bazą pojazdów:** Formularz pozwalający na dodanie nowego pojazdu o określonych parametrach do systemu oraz usunięcie wybranego pojazdu z floty.
- **Zarządzanie bazą klientów:** Lista aktualnych i byłych klientów, ich historia wynajmu oraz dokumenty (prawo jazdy). 
- **System powiadomień:** Centrum powiadamiania z informacjami (koniec wynajmu, zdarzenie losowe na drodze, zbliżający się przegląd, kończące się AC/OC).
- **Śledzenie pojazdów:** Mapa z podglądem geolokacji pojazdów i śledzeniem ich statusu.
- **Śledzenie konserwacji:** Śledzenie konserwacji pojazdów, podgląd ich stanu technicznego oraz wysyłanie do warsztatu. 
- **Symulator zdarzeń losowych:** Możliwość reagowania na sytuacje losowe (wypadki, kolizje, usterki), możliwość wysłania pomocy technicznej, lawety.

## Podział interfejsu

- **Dashboard:** Porównanie wypożyczone/wolne pojazdy, Lista wypozyczonych pojazdów (4 z najdłuższym czasem najmu), Pole statusu pojazdów (4 z czymś wymagającym uwagi np. konieczność serwisu), Call-To-Action (Przycisk wypożycz otwierające formularz).
- **Powiadomienia:** Centrum powiadomień aktualnych i przychodzących wydarzeń.
- **Flota:** Lista dostępnych pojazdów oraz ich stanu. Możliwość dodawania i usuwania pojazdów.
- **Klienci:** Lista obecnych i byłbych klientów, dokumenty, obecne i przeszłe wynajmy.
- **Finanse:** Podsumowanie aktualnych finansów firmy i status aktualnych wynajmów.
- **Śledź:** Otwiera mape z podglądem Poznania i śledzenia pojazdów.
- **Warsztat:** Podgląd stanu technicznego pojazdów, podgląd statusu naprawy, możliwość wysłania pojazdu do naprawy, wezwanie pomocy technicznej.
- **Ustawienia:** Ustawienia konfiguracyjne.

## Stos technologiczny
- **Środowisko programistyczne:** Docker Ubuntu 24.02
- **Język programowania:** C++
- **Obsługa GUI:** Qt6
- **Przechowywanie i obsługa danych:** SQLite, Qt SQL
- **Mapa i śledzenie:** QWebEngineView, Leaflet.js, OpenStreetMap
  
## Autorzy
Daniel Borowski, Adam Jączak \
Programowanie Obiektowe \
Informatyka stac. sem III  
Politechnika Poznańska 