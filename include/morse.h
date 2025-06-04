/**
 * @file morse.h
 * @brief Header-Datei für das Morse-Code-Programm
 * 
 * Diese Datei enthält die Deklarationen aller Funktionen und Konstanten,
 * die für die Umwandlung von Text in Morsecode und umgekehrt benötigt werden.
 */

#ifndef MORSE_H
#define MORSE_H

#include <stdbool.h>

/**
 * @brief Wandelt einen Text in Morsecode um
 * 
 * @param text Der zu kodierende Text
 * @param use_slash_wordspacer Flag, ob Slash als Worttrenner verwendet werden soll
 * @return char* Der resultierende Morsecode
 */
char* encode_to_morse(const char* text, bool use_slash_wordspacer);

/**
 * @brief Wandelt Morsecode in Text um
 * 
 * @param morse_code Der zu dekodierende Morsecode
 * @return char* Der resultierende Text
 */
char* decode_from_morse(const char* morse_code);

/**
 * @brief Gibt die Programmierer-Informationen im JSON-Format aus
 */
void print_programmer_info(void);

/**
 * @brief Gibt den Hilfetext aus
 */
void print_help(void);

#endif /* MORSE_H */
