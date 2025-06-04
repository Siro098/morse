/**
 * @file CMakeLists.txt
 * @brief CMake-Konfigurationsdatei für das Morse-Code-Projekt
 */

cmake_minimum_required(VERSION 3.10)
project(morse C)

# C-Standard festlegen
set(CMAKE_C_STANDARD 11)

# Include-Verzeichnis hinzufügen
include_directories(include)

# Quellcode-Dateien sammeln
set(SOURCES
    src/main.c
    src/morse.c
    src/morse_tables.c
)

# Ausführbare Datei erstellen
add_executable(morse ${SOURCES})

# Installationsregeln (optional)
install(TARGETS morse DESTINATION bin)
