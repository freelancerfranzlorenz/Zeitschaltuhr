# Zeitschaltuhr
Zeitschaltuhr mit WiFi

Sie kennen alle WiFi Steckdosen mit einer Zeitschaltfunktion.
Das Problem, dass ich mit allen diesen Geräten hatte war, dass sie für die Zeitschaltfunktion ein aktives WiFi Netzwerk benötigen.

Die hier vorgestellte Zeitschaltuhr behebt dieses Problemchen :-)

# WiFi 
Die WiFi Steckdose benutzt eine aktive WiFi Verbindung zur Einstellung der Uhrzeit und des Wochentags.
Hierzu wird Kontakt zu einem NTP Server aufgenommen und dessen Daten abgefragt.

Die Einstellung Ihres WiFi Servers nehmen Sie in der Datei config.h vor.

- der Name ihres WiFi Netzwerks stellen Sie als String hinter dem #define *WIFI_SSID* ein
- das Passwort ihres WiFi Netzwerks stellen Sie als String hinter dem #define *WIFI_PASSWORD* ein

# Innere Uhr
Die "innere" Uhr der WiFi Steckdose läuft auch weiter, wenn das von Ihnen gewählte WiFi Netzwerk nicht zur Verfügung steht.
Wenn Sie zum Beispiel Ihren WiFi Router Abends abschalten (so wie ich :-) ).

# Inneres WiFi Netzwerk (AccessPoint)
Desweiteren verfügt die Zeitschaltuhr über ihr eigenes WiFi Netzwerk.

Mit Hilfe dieses WiFi Netzwerks (auch AccessPoint genannt) können Sie die Daten und Einstellung der WiFi Steckdose abfragen
und ändern.

Die Einstellung des AccessPoints nehmen Sie in der Datei config.h vor.

- der Name des AccessPoint Netzwerks stellen Sie als String hinter dem #define *WIFI_AP_SSID* ein
- das Passwort des AccessPoint Netzwerks stellen Sie als String hinter dem #define *WIFI_AP_PASSWORD* ein

Sie können ebenfalls noch eine Uhrzeit (nur Stundenangabe) eingeben, zu welchem Zeitpunkt dieses AccessPoint Netzwerks ab- bzw. eingeschaltet werden.
Diese Einstellung ist ebenfalls in der Datei config.h zu finden...

- die Stunde der Abschaltung des AccessPoints stellen Sie als Zahl hinter dem #define *WIFI_AP_SWITCHOFF_HOUR* ein
- die Stunde der Aktivierung des AccessPoints stellen Sie als Zahl hinter dem #define *WIFI_AP_SWITCHON_HOUR* ein




