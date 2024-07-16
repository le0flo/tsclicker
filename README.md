# 😎 TS clicker

Questo plugin di teamspeak è in realtà un simpatico e semplice autoclicker scritto in c.
Ideato per prendere in giro gli staff di tutti i server pvp di minecraft.
Supporta una modalità di click palese agli anticheat oppure i valori presi da un registratore di click.

### ATTENZIONE: questo plugin funziona soltanto su windows.

# Compilazione

Per compilare il plugin, rispetta i seguenti passaggi:

1. scarica [msys2](https://www.msys2.org/#installation), un ambiente di compilazione per windows, il predefinito del progetto
2. apri la shell **MSYS2 MINGW64** per poi aggiornare i pacchetti attuali con il comando `pacman -Syu`
3. successivamente, dopo aver riavviato come da prompt la shell, scarica i pacchetti con il comando `pacman -S mingw-w64-x86_64-clang mingw-w64-x86_64-clang-tools-extra make`
4. infine, per aprire il progetto nella shell basta sapere che il disco c si raggiunge con il comando `cd /c`, perciò una volta entrato nel disco c tramite la shell di msys2, naviga alla cartella del progetto e digita il comando `make`
5. il file `tsclicker.dll` si troverà nella cartella `build`

# Installazione

Una volta che sei in possesso del file `tsclicker.dll`, spostalo nella cartella `%appdata%\TS3Client\plugins\`.

Se hai seguito alla lettera tutti gli step sei pronto per burlare il team di staffer nel tuo server preferito.
