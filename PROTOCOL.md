# Protokół

1. Klient łączy się z serwerem i wysyła nazwe pliku który chciałby pobrać
2. Serwer odpowiada za pomocą 1B który mowi czy plik znajduje się na serwerze czy nie:
    * MC_ACCEPT (0xFF) jeśli plik znajduje się na serwerze
    * MC_FAILURE (0x00) jeśli plik nie znajduje się na serwerze
3. Jeśli serwer posiada plik, wysyła wtedy rozmiar pliku (`unsigned long`)
4. Wysyła plik a potem zamyka połączenie z socketem

### Gdy serwer nie posiada pliku

3. Jeśli serwer nie posiada pliku, zamyka wtedy połączenie z socketem