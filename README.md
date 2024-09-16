# Teamspeak Clicker

Questo plugin di teamspeak è in realtà un simpatico e semplice autoclicker scritto in C++.
Ideato per prendere in giro gli staff di tutti i server pvp di minecraft.
Supporta una modalità di click palese agli anticheat oppure i valori presi dal registratore di click interno.

### Compilazione

I seguenti passaggi sono per compilare il programma manualmente:

1. Scarica i [Microsoft Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) per C++

2. Scarica il toolkit [Qt 5.12.2](https://download.qt.io/archive/qt/5.12/5.12.2/)

3. Dopo aver installato i rispettivi programmi, apri `x64 Native Tools Command Prompt for VS 2022` dal menù start

4. Naviga dentro la cartella del progetto, tramite il prompt precedentemente aperto, ed esegui il file `build.bat`

5. Una volta finita la compilazione, troverai i binaries nella cartella `build`

### Installazione

Se sai come funzionano i plugin di teamspeak, puoi tranquillamente spostare il plugin compilato nella cartella apposita.
Nel caso contrario, segui le successive istruzioni per ottenere il pacchetto `.plugin`, per un installazione semplice.

1. Apri il terminale nella cartella del progetto, ed esegui il file `package.bat`

2. Entra nella cartella `package` con l'explorer e, selezionando tutti i file con la combinazione di tasti `CTRL + A`, usa WinRAR per creare un archivio `.zip`

3. Prendi l'archivio zip generato e rimpiazza l'estensione del file con l'estensione `.ts3_plugin`

4. Per installare il plugin appena generato su Teamspeak 3, doppio click sul file `.ts3_plugin` e seguire il prompt di installazione di Teamspeak 3