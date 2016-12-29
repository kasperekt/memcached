# Memcached

## Treść zadania
Serwer memcached. Klient prosi o cos serwer, serwer oddaje z cache jak ma, jesli nie ma zapisuje i oddaje.

## Protokół

1. Klient łączy się z serwerem i wysyła nazwe pliku który chciałby pobrać
2. Serwer odpowiada za pomocą 1B który mowi czy plik znajduje się na serwerze czy nie:
    * MC_ACCEPT (0xFF) jeśli plik znajduje się na serwerze
    * MC_FAILURE (0x00) jeśli plik nie znajduje się na serwerze
3. Jeśli serwer posiada plik, wysyła wtedy rozmiar pliku (`unsigned long`)
4. Wysyła plik a potem zamyka połączenie z socketem

### Gdy serwer nie posiada pliku

3. Jeśli serwer nie posiada pliku, zamyka wtedy połączenie z socketem

## Opis kodu

* main.c - główny plik w którym odbywa się odbieranie połączenia z klientami oraz tworzenie wątków dla każdego klientami
* cache.c - plik który odpowiada za zarządzanie cachem
* client.c - plik odpowiadający za obłsuge połączenia z klientem
* files.c - plik odpowiadający za operacje na plikach, czytanie, usuwanie pliku

## Budowanie
Żeby zbudować:
```
make all
```

Żeby uruchomić:
```
./memcached [ip] [port] [udostępniany folder] [rozmiar cache w bajtach]
```

Przykład (rozmiar cache 200MB):
```
./memcached 127.0.0.1 1234 /var/memcached 20971520
```

Żeby uruchomić serwer na `127.0.0.1:1234` który udostępnia pliki z katalogu `tmp/` o rozmiarze cache `100MB`:
```
make run
```

