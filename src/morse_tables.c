/**
 * @file morse_tables.c
 * @brief Implementierung der Morse-Code-Tabellen
 * 
 * Diese Datei enthält die Tabellen für die Umwandlung zwischen Text und Morsecode.
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Struktur für die Zuordnung von Zeichen zu Morsecode
 */
typedef struct {
    char character;
    const char* morse_code;
} MorseMapping;

/**
 * @brief Tabelle für die Zuordnung von Zeichen zu Morsecode
 * 
 * Enthält alle unterstützten Zeichen und ihren entsprechenden Morsecode.
 */
const MorseMapping morse_table[] = {
    /* Buchstaben */
    {'A', ".-"},
    {'B', "-..."},
    {'C', "-.-."},
    {'D', "-.."},
    {'E', "."},
    {'F', "..-."},
    {'G', "--."},
    {'H', "...."},
    {'I', ".."},
    {'J', ".---"},
    {'K', "-.-"},
    {'L', ".-.."},
    {'M', "--"},
    {'N', "-."},
    {'O', "---"},
    {'P', ".--."},
    {'Q', "--.-"},
    {'R', ".-."},
    {'S', "..."},
    {'T', "-"},
    {'U', "..-"},
    {'V', "...-"},
    {'W', ".--"},
    {'X', "-..-"},
    {'Y', "-.--"},
    {'Z', "--.."},
    
    /* Zahlen */
    {'0', "-----"},
    {'1', ".----"},
    {'2', "..---"},
    {'3', "...--"},
    {'4', "....-"},
    {'5', "....."},
    {'6', "-...."},
    {'7', "--..."},
    {'8', "---.."},
    {'9', "----."},
    
    /* Satzzeichen */
    {'.', ".-.-.-"},
    {',', "--..--"},
    {':', "---..."},
    {';', "-.-.-."},
    {'?', "..--.."},
    
    /* Mathematische Symbole */
    {'=', "-...-"},
    {'-', "-....-"},
    {'+', ".-.-."},
    
    /* Formatierungssymbole */
    {'_', "..--.-"},
    {'(', "-.--."},
    {')', "-.--.-"},
    {'/', "-..-."},
    {'@', ".--.-."},
    
    /* Ende der Tabelle */
    {'\0', NULL}
};

/**
 * @brief Gibt den Morsecode für ein Zeichen zurück
 * 
 * @param c Das Zeichen, für das der Morsecode gesucht wird
 * @return const char* Der Morsecode für das Zeichen oder NULL, wenn nicht gefunden
 */
const char* get_morse_for_char(char c) {
    c = toupper(c);
    
    for (int i = 0; morse_table[i].morse_code != NULL; i++) {
        if (morse_table[i].character == c) {
            return morse_table[i].morse_code;
        }
    }
    
    return NULL;
}

/**
 * @brief Gibt das Zeichen für einen Morsecode zurück
 * 
 * @param morse_code Der Morsecode, für den das Zeichen gesucht wird
 * @return char Das Zeichen für den Morsecode oder '\0', wenn nicht gefunden
 */
char get_char_for_morse(const char* morse_code) {
    for (int i = 0; morse_table[i].morse_code != NULL; i++) {
        if (strcmp(morse_table[i].morse_code, morse_code) == 0) {
            return morse_table[i].character;
        }
    }
    
    return '\0';
}
