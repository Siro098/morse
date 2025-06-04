# Morse-Code-Programm

Dieses Programm ermöglicht die Umwandlung von Text in Morsecode und umgekehrt. Es wurde gemäß den Anforderungen des Programmentwurfs TI24 entwickelt.

## Projektstruktur

```
morse_project/
├── CMakeLists.txt       # CMake-Konfigurationsdatei
├── include/             # Header-Dateien
│   └── morse.h          # Hauptheader mit Funktionsdeklarationen
├── src/                 # Quelldateien
│   ├── main.c           # Hauptprogramm mit Optionsverarbeitung
│   ├── morse.c          # Implementierung der Morse-Funktionen
│   └── morse_tables.c   # Morse-Code-Tabellen
└── build/               # Build-Verzeichnis (wird bei Build erstellt)
```

## Funktionalität

Das Programm unterstützt folgende Funktionen:

- Umwandlung von Text in Morsecode (`-e`, `--encode`)
- Umwandlung von Morsecode in Text (`-d`, `--decode`)
- Ausgabe in eine Datei (`-o`, `--out`)
- Anzeige von Hilfe (`-h`, `--help`)
- Anzeige von Programmierer-Informationen (`--programmer-info`)
- Verwendung von Slash als Worttrenner (`--slash-wordspacer`)

## Unterstützte Zeichen

Das Programm unterstützt die Umwandlung folgender Zeichen:
- Buchstaben A-Z und a-z
- Zahlen 0-9
- Satzzeichen: .,:;?
- Mathematische Symbole: =-+
- Formatierungssymbole: _()/@

## Build-Anleitung

Um das Programm zu kompilieren, benötigen Sie CMake und einen C-Compiler (GCC oder MinGW64):

```bash
mkdir -p build
cd build
cmake ..
make
```

Die ausführbare Datei `morse` wird im Build-Verzeichnis erstellt.

## Verwendung

Beispiele für die Verwendung des Programms:

```bash
# Text in Morsecode umwandeln
./morse -e "Hallo Welt"

# Morsecode in Text umwandeln
./morse -d ".... .- .-.. .-.. ---   .-- . .-.. -"

# Datei in Morsecode umwandeln und in eine andere Datei schreiben
./morse -e input.txt -o output.txt

# Slash als Worttrenner verwenden
./morse -e "Hallo Welt" --slash-wordspacer
```

## Hinweise zur Entwicklung

Für die Entwicklung mit CLion siehe die separate Anleitung `clion_setup_guide.md`.

## Anforderungen

Das Programm erfüllt alle funktionalen und nicht-funktionalen Anforderungen gemäß der Spezifikation:
- Vollständige Unterstützung aller geforderten Zeichensätze
- Korrekte Formatierung mit Buchstaben- und Worttrennung
- Unterstützung für Ein- und Ausgabe über Dateien und Pipes
- Implementierung mit getopt/getopt_long für Kommandozeilenoptionen
- CMake als Build-System
- Doxygen-kompatible Dokumentation
