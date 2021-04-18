hebcal [![Build Status](https://travis-ci.org/hebcal/hebcal.svg?branch=master)](https://travis-ci.org/hebcal/hebcal)
======

Ein ewiger jüdischer Kalender

von Danny Sadinoff
Teile von Michael J. Radwin

## Beschreibung
Hebcal ist ein Programm, das die Tage des jüdischen Kalenders für ein bestimmtes gregorianisches Jahr ausgibt. Hebcal ist recht flexibel, was die Auswahl der Tage im jüdischen Kalender angeht.
Jedes der folgenden Elemente kann einzeln ein- oder ausgeschaltet werden:

* Das hebräische Datum
* Jüdische Feiertage (einschließlich Jom Ha'atzmaut [Unabhängigkeitstag] und Jom HaShoah usw.)
* Die wöchentliche Sedra
* Der Wochentag
* Die Tage des Omer

## Synopse

```
Verwendung: hebcal [Optionen] [[ Monat [ Tag ]] Jahr ]
       hebcal help
       hebcal info
       hebcal cities
       hebcal warranty
       hebcal copying
```

Hebcal gibt den hebräischen Kalender für jeweils ein Sonnenjahr aus.
Wird ein Argument angegeben, gibt es den Kalender für dieses Jahr aus.
Bei zwei numerischen Argumenten `mm jjjj` gibt es den Kalender für den
Monat `mm` des Jahres `jjjj` aus.

Zum Beispiel gibt `hebcal -ho`
nur die Tage des Omer für das aktuelle Jahr aus.
Hinweis: Verwenden Sie KOMPLETTE Jahre.  Sie sind wahrscheinlich nicht interessiert an
hebcal 93, sondern eher hebcal 1993.

### Optionen
#### Allgemeine Optionen
Option | Beschreibung
--- | ---
  --help | Hilfetext anzeigen
  --version | Versionsnummer anzeigen

#### Eingabeoptionen
Option | Beschreibung
--- | ---
 -H, --hebrew-date | Hebräische Datumsbereiche verwenden - wird nur benötigt z.B. wie `hebcal -H 5373`
 -I, --infile INFILE | Holt hebräische Benutzerereignisse, die nicht Jahrzeiten sind, aus der angegebenen Datei. Das Format ist: `mmm tt string`, wobei `mmm` ein hebräischer Monatsname ist.
 -t, --today | Nur Ausgabe für das heutige Datum
 -T, --today-brief | Gibt die relevanten Informationen von heute aus, kein gregorianisches Datum.
 -Y, --yahrtzeit YAHRTZEIT | Holt Jahrzeitdaten aus der angegebenen Datei. Das Format ist: `mm tt jjjj string`. Die ersten drei Felder geben ein *Gregorianisches* Datum an.

#### Ausgabeoptionen
Option | Beschreibung
--- | ---
   -8 | 8-Bit-Hebräisch verwenden (ISO-8859-8-Logisch).
   -a, --ashkenazi | Aschkenasisches Hebräisch verwenden.
   -d, --add-hebrew-dates | gibt das hebräische Datum für den gesamten Datumsbereich aus.
   -D, --add-hebrew-dates-for-events | gibt das hebräische Datum für Tage mit Ereignissen aus
   -e, --euro-dates | "Europäische" Daten ausgeben - Format TT.MM.JJJJ.
   -E, --24hour | 24-Stunden-Zeiten ausgeben (z. B. 18:37 statt 6:37).
   -F, --daf-yomi | Gibt den Daf Jomi für den gesamten Datumsbereich aus.
   -g, --iso-8601 | ISO-8601-Datumsangaben ausgeben -- JJJJ-MM-TT (dies hat Vorrang vor -y)
   -h, --no-holidays | Standard-Feiertage unterdrücken.
   -i, --israeli | Israelisches Sedra-Schema verwenden.
   --lang LANG | ISO 639-1 LANG-Code zur Sprachwahl verwenden (einen von `ashkenazi`, `ashkenazi_litvish`, `ashkenazi_poylish`, `ashkenazi_standard`, `de`, `es`, `fi`, `fr`, `he`, `hu`, `pl`, `ru`)
   -M, --molad | Druckt den Molad am Schabbat Meworchim.
   -o, --omer | Tage des Omers hinzufügen.
   -O, --sunrise-and-sunset | Sonnenaufgangs- und Sonnenuntergangszeiten zu jedem Tag ausgeben.
   -r, --tabs | Format mit Tabulatoren ausgeben.
   -s, --sedrot | Wöchentliche Sedrot am Samstag hinzufügen.
   -S, --daily-sedra | Sedrot der Woche an allen Kalendertagen ausgeben.
   -w, --weekday | Wochentag hinzufügen.
   -W, --abbreviated | Wöchentliche Ansicht. Omer, Daf Jomi und nicht datumsbezogene Smanim werden einmal pro Woche angezeigt, und zwar an dem Tag, der dem ersten Tag im Bereich entspricht.
   -x, --no-rosh-chodesh | Rosch Chodesch unterdrücken.
   -y, --year-abbrev | Nur die letzten beiden Ziffern des Jahres ausgeben.
   --years N | Erzeugt Ereignisse für `N` Jahre (Voreinstellung `1`)

#### Optionen in Bezug auf die Zeiten zum Kerzenzünden
Option | Beschreibung
--- | ---
   -b, --candle-mins minuten | Legt fest, dass das Kerzenzünden so viele Minuten wie angegeben vor Sonnenuntergang stattfindet
   -c, --candlelighting | Zeiten zum Kerzenzünden ausgeben.
   -C, --city stadt | Breitengrad, Längengrad und Zeitzone entsprechend der angegebenen Stadt einstellen. Diese Option setzt die Option -c voraus.
   -l, --latitude xx,yy | Setzt den Breitengrad für Sonnenberechnungen auf `xx` Grad und `yy` Minuten.  Negative Werte bedeuten Süden.
   -L, --longitude xx,yy | Setzt den Längengrad für Sonnenberechnungen auf `xx` Grad und `yy` Minuten.  *Negative Werte sind OST*. Die Schalter `-l` und `-L` müssen beide verwendet werden, oder aber gar nicht. Diese Schalter haben Vorrang vor dem Schalter `-C` (Lokalisierung auf Stadt).
   -m, --havdalah-mins minuten | Legt fest, dass Hawdalah so viele Minuten wie angegeben nach Sonnenuntergang stattfindet
   -z, --timezone timezone | Verwendet die angegebene Zeitzone und überschreibt den Schalter `-C` (Lokalisierung nach Stadt).
   -Z, --zmanim | **EXPERIMENTELL** Smanim hinzufügen (Alot HaSchachar; Mischejakir; Kriat Schma, Sof Sman; Tfilah, Sof Sman; Chazot haJom; Mincha Gdolah; Mincha Ktanah; Plag HaMincha; Zeit HaKochawim)

## Zeiten des Kerzenzündens

Die Zeiten zum Kerzenzünden von Hebcal sind nur Näherungswerte. Sollten Sie jemals Zweifel an den Zeiten haben, fragen Sie Ihre lokale halachische Autorität. Wenn Sie geographische Koordinaten oberhalb des Polarkreises oder des Antarktischen Kreises eingeben, sind die Zeiten garantiert falsch.

Hebcal enthält eine kleine Datenbank von Städten mit den zugehörigen geografischen Informationen und Zeitzoneninformationen. Die geografischen und zeitlichen Informationen, die für die Berechnung der Sonnenuntergangszeiten erforderlich sind, können auf drei verschiedene Arten zu Hebcal gelangen:

1. Die Vorgabe: Der Systemmanager legt eine Standardstadt fest, wenn das Programm kompiliert wird.
2. Hebcal sucht in der Umgebungsvariablen `HEBCAL_CITY` nach dem Namen einer Stadt in der hebcal-Datenbank, und wenn er eine findet, macht hebcal diese zur neuen Standardstadt.
3. 1 und 2 können durch Kommandozeilenargumente außer Kraft gesetzt werden, einschließlich derer, die in der Umgebungsvariablen `HEBCAL_OPTS` angegeben sind. Der natürlichste Weg, dies zu tun, ist die Verwendung des Befehls `-c stadt`. Dadurch wird hebcal auf die Stadt lokalisiert. Eine Liste der Städte, die hebcal kennt, erhalten Sie durch Eingabe von `hebcal cities` an der Eingabeaufforderung. Wenn die von Ihnen gewünschte Stadt nicht in dieser Liste enthalten ist, können Sie die geografischen Informationen von hebcal direkt mit den Schaltern `-l`, `-L` und `-z` steuern. Beachten Sie, dass das Ändern der geografischen Koordinaten dazu führt, dass die Zeitzone standardmäßig auf "UTC" gesetzt wird.
Um einen Statusbericht über die Anpassungen zu erhalten, geben Sie `hebcal info` an der Eingabeaufforderung ein.

## Umgebung

Hebcal verwendet zwei Umgebungsvariablen:
<dl>
<dt>HEBCAL_CITY
<dd>Hebcal verwendet diesen Wert als Standardstadt für Sonnenuntergangsberechnungen. Eine Liste der verfügbaren Städte erhalten Sie mit von hebcal mit dem Befehl: <code>hebcal cities</code>
<dt>HEBCAL_OPTS
<dd>Der Wert dieser Variablen wird automatisch so verarbeitet, als ob er in der Befehlszeile vor allen anderen tatsächlichen Befehlszeilenargumenten eingegeben wurde.
</dl>

## Autor
Danny Sadinoff

Mit Beiträgen von

* Michael J. Radwin
* Eyal Schachter (JavaScript-Portierung)
* Aaron Peromsik (Daf Yomi, experimentelle Zmanim-Funktion)
* Ben Sandler (Molad und tägliche Sonnenaufgang/Sonnenuntergang-Funktionen)

## Siehe auch

calendar(1), emacs(1), hcal(1), hdate(1), omer(1), remind(1), rise(1)

Die neueste Version des Codes ist unter https://github.com/hebcal/hebcal verfügbar.

Die ursprüngliche Motivation für die Algorithmen in diesem Programm war der _Tur Shulchan Aruch_.

Für Version 3 wurde ein Großteil des Programms unter Verwendung der Kalenderroutinen von Emacs 19 von Edward M. Reingold und Nachum Dershowitz neu geschrieben. Ihr Programm ist extrem übersichtlich und bietet viele lehrreiche Beispiele für schönen Kalendercode in Emacs-LISP.

Für Version 4 wurden die Zeiten des Kerzenanzündens mit Hilfe von Derick Rethans [timelib](https://github.com/derickr/timelib) neu geschrieben.

Eine gut geschriebene Behandlung des jüdischen Kalenders für den Laien findet sich in _Understanding the Jewish Calendar_ von Rabbi Nathan Bushwick. Eine umfassendere Bibliographie zum Thema findet sich dort, wie auch im Eintrag zum Kalender in der _Encyclopedia Judaica_.


## Diagnostik
<dl>
<dt>hebcal help
<dd>Druckt eine kürzere Version dieser Manpage aus, mit Kommentaren zu den einzelnen Optionen.
<dt>hebcal info
<dd>Druckt die Versionsnummer und die Standardwerte des Programms aus.
<dt>hebcal cities
<dt>hebcal cities <d>Druckt eine Liste der Städte aus, die hebcal kennt, geeignet als Argumente für die Option -C stadt.
<dt>hebcal copying
<dd>Druckt die GNU-Lizenz aus, mit Informationen zum Kopieren des Programms. Siehe unten.
<dt>hebcal warranty
<dd>Sagt Ihnen, dass es KEINE GARANTIE für hebcal gibt.
</dl>

## Haftungsausschluss
Dies ist nur ein Programm, das ich während der Sommerschule und bei der Vermeidung meines Abschlussprojekts geschrieben habe. Es sollte nicht mit irgendeiner Art von halachischer Autorität ausgestattet werden.

## Bugs
Hebräische Datumsangaben sind nur vor Sonnenuntergang an diesem säkularen Datum gültig. Eine Option, um dies zu kontrollieren, wird in einer späteren Version hinzugefügt.

Negative Längengrade sind OSTEN von Greenwich.

Einige Kombinationen von Optionen erzeugen seltsame Ergebnisse, z.B.
  `hebcal -dH nisan 5744`
  `hebcal -dH 5744`
Dies kommt ins Spiel, wenn Sie die Umgebungsvariable *HEBCAL_OPTS* verwenden.

Die Sonnenaufgang-/Sonnenuntergang-Routinen sind nicht genau genug. Wenn Sie geographische Koordinaten oberhalb des Polarkreises oder Antarktiskreises eingeben, sind die Zeiten garantiert falsch.

Hebcal übersetzt nur zwischen dem Gregorianischen Kalender und dem jüdischen Kalender. Das bedeutet, dass die Ergebnisse zumindest teilweise unbrauchbar sind, falls der Gregorianische Kalender an einem bestimmten Ort zu einer bestimmten Zeit nicht verwendet wurde, z.B. vor 1752 in Großbritannien und vor ca. 1918 in Russland. Siehe ["Daylight saving time" auf Wikipedia](https://en.wikipedia.org/wiki/Daylight_saving_time) für eine wunderbare Grafik, die zeigt, wann die Umstellung vom Julianischen auf den Gregorianischen Kalender an verschiedenen Orten stattfand.

Hebcal kann leider keine Datumsberechnungen vor dem Jahr 2 n. .d. Z. durchführen.

## Bauen & Installieren

Um hebcal aus dem Quellcode-Repository zu bauen, benötigen Sie ein paar mehr Werkzeuge, als für das Bauen aus Distributionen sonst nötig sind. Insbesondere benötigen Sie

* GNU autoconf Version 2.59
* GNU automake Version 1.9.5 oder höher
* GNU m4 Version 1.4.3 oder höher
* GNU make Version 3.79 oder höher
* Perl v5.0 oder höher
* gperf

Sobald Sie diese haben, können Sie die Build-Umgebung wie folgt vorbereiten:

```
autoreconf --install && ./configure && make
```

Wenn Sie das Programm für Ihre Stadt anpassen möchten, übergeben Sie zusätzliche Optionen an `configure` oben.

Untersuchen Sie `cities.h`.  Wenn Ihre Stadt dort enthalten ist, führen Sie `configure` mit der Option
  `--mit-default-city=STÄDTENAME`-Option wie folgt aus:
   ```
   ./configure --with-default-city=Chicago
   ```
Sie müssen eventuell Leerzeichen angeben:
   ```
   ./configure --with-default-city="Los Angeles"
   ```

Wenn Ihre Stadt NICHT auf der Liste steht, müssen Sie hebcal den Breitengrad, den Längengrad und die Zeitzone übergeben, um es an Ihre Stadt anpassen zu können (siehe Handbuch).

Angenommen, Sie wohnen in Oshkosh, Wisconsin.
Ihr Breitengrad ist 44d1'29", und Ihr Längengrad ist 88d32'33".
Sie befinden sich in der Zeitzone `America/Chicago`.
Wir runden die geographischen Koordinaten auf die nächste Minute.

Um die Kerzenscheinzeiten für das aktuelle Jahr zu erhalten, würden Sie eingeben
  ```
  hebcal -ch -l44,1 -L 88,33 -z Amerika/Chicago
  ```

Wenn Sie das oft machen, kann das sehr anstrengend werden, deshalb gibt es die Umgebungsvariable `HEBCAL_OPTS`, die Sie verwenden können.  Jedes Mal, wenn hebcal gestartet wird, überprüft es diese Variable.  Wenn sie nicht leer ist, werden die Argumente in dieser Variable so gelesen, als ob sie in der Befehlszeile vor den tatsächlich eingegebenen Argumenten eingegeben wurden.

Sie könnten also `HEBCAL_OPTS` wie folgt setzen
   ```
   -l44,1 -L 88,33 -z Amerika/Chicago
   ```
und wenn Sie eingeben
    ```
    hebcal -ch
    ```
wird hebcal denken, Sie hätten getippt
    ```
    hebcal -l44,1 -L 88,33 -z Amerika/Chicago -ch
    ```

HINWEIS: negative Längengrade sind OSTEN von Greenwich.

Für Informationen zum Setzen von Umgebungsvariablen wenden Sie sich an Ihren lokalen Guru.

Sobald die Installation abgeschlossen ist, gibt es drei Möglichkeiten, die Städte zu ändern oder eine Stadt auszuwählen, die nicht in der Liste enthalten ist:

1. Ändern Sie die Umgebungsvariable "CITY".
2. Ändern Sie die Variable "HEBCAL_OPTS", um die Koordinaten der neuen Stadt zu übernehmen.
3. Übergeben Sie ein Argument `-C stadt` an hebcal.

Sie können überprüfen, an welchem Ort hebcal denkt sich zu befinden, indem Sie in die Befehlszeile
    `hebcal info`
eingeben.


## VERBREITUNG
   Urheberrecht (C) 1994-2011 Danny Sadinoff
   Teile davon Copyright (c) 2011 Michael J. Radwin. Alle Rechte vorbehalten.

   Hebcal wird unter der GNU Public License vertrieben.  Das Programm
   und sein Quellcode dürfen frei verteilt werden.  Für Details siehe
   die Datei COPYING in der Distribution.

   Wenn Sie dieses Programm verwenden wollen, schreiben Sie mir bitte eine Nachricht 
   (auf Englisch).
   Ich würde gerne wissen, wer Sie sind, welche Version Sie benutzen und wie
   Sie hebcal verwenden, und alles andere, was Sie mir mitteilen möchten, damit ich das
   damit ich das Programm an die Bedürfnisse der Benutzer anpassen kann.

   Ich verlange KEINE Bezahlung für die Nutzung meines Programms, aber das Schreiben
   dieses Programm zu schreiben, hat Zeit gekostet.  Das "frei" in der GNU Public License
   bezieht sich auf die Verbreitung, nicht unbedingt auf die Bezahlung. Sie können mir gerne
   $10 oder ein Vielfaches von $18 oder einfach eine Postkarte an meine US-Postadresse schicken (mailen Sie mir, um sie zu erhalten).

      Senden Sie eine E-Mail an:
      danny@sadinoff.com

