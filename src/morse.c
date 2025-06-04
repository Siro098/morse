/**
 * @file morse.c
 * @brief Implementierung der Morse-Code-Funktionen
 * 
 * Diese Datei enthält die Implementierung der Funktionen für die Umwandlung
 * zwischen Text und Morsecode.
 */

#include "../include/morse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Externe Funktionen aus morse_tables.c */
extern const char* get_morse_for_char(char c);
extern char get_char_for_morse(const char* morse_code);

/* Konstanten für die Formatierung */
#define LETTER_SEPARATOR " "
#define WORD_SEPARATOR "   "
#define SLASH_WORD_SEPARATOR " / "
#define UNKNOWN_CHAR "*"
#define MAX_MORSE_CODE_LENGTH 10

/**
 * @brief Wandelt einen Text in Morsecode um
 * 
 * @param text Der zu kodierende Text
 * @param use_slash_wordspacer Flag, ob Slash als Worttrenner verwendet werden soll
 * @return char* Der resultierende Morsecode
 */
char* encode_to_morse(const char* text, bool use_slash_wordspacer) {
    if (text == NULL) {
        return NULL;
    }
    
    /* Abschätzung der maximalen Länge des Morsecodes */
    size_t max_length = strlen(text) * 10;
    char* morse_code = (char*)malloc(max_length);
    if (morse_code == NULL) {
        return NULL;
    }
    
    morse_code[0] = '\0';
    bool first_letter = true;
    bool first_word = true;
    
    for (size_t i = 0; text[i] != '\0'; i++) {
        /* Ignoriere Zeilenumbrüche und Wagenrückläufe */
        if (text[i] == '\n' || text[i] == '\r') {
            continue;
        }
        
        /* Behandle Leerzeichen als Worttrenner */
        if (text[i] == ' ') {
            if (!first_word) {
                if (use_slash_wordspacer) {
                    strcat(morse_code, SLASH_WORD_SEPARATOR);
                } else {
                    strcat(morse_code, WORD_SEPARATOR);
                }
            }
            first_letter = true;
            first_word = false;
            continue;
        }
        
        /* Suche den Morsecode für das aktuelle Zeichen */
        const char* char_morse = get_morse_for_char(text[i]);
        
        /* Füge Buchstabentrenner hinzu, wenn nicht der erste Buchstabe */
        if (!first_letter) {
            strcat(morse_code, LETTER_SEPARATOR);
        } else {
            first_letter = false;
            first_word = false;
        }
        
        /* Füge den Morsecode oder * für unbekannte Zeichen hinzu */
        if (char_morse != NULL) {
            strcat(morse_code, char_morse);
        } else {
            /* Bei UTF-8-Zeichen mit mehreren Bytes, füge für jedes Byte ein * hinzu */
            unsigned char c = (unsigned char)text[i];
            if (c < 128) {
                strcat(morse_code, UNKNOWN_CHAR);
            } else {
                /* Bestimme die Anzahl der Bytes im UTF-8-Zeichen */
                int bytes = 0;
                if ((c & 0xE0) == 0xC0) bytes = 2;
                else if ((c & 0xF0) == 0xE0) bytes = 3;
                else if ((c & 0xF8) == 0xF0) bytes = 4;
                else bytes = 1;
                
                /* Füge für jedes Byte ein * hinzu */
                for (int j = 0; j < bytes; j++) {
                    if (j > 0) {
                        strcat(morse_code, LETTER_SEPARATOR);
                    }
                    strcat(morse_code, UNKNOWN_CHAR);
                    if (j < bytes - 1) {
                        i++;
                    }
                }
            }
        }
    }
    
    return morse_code;
}

/**
 * @brief Wandelt Morsecode in Text um
 * 
 * @param morse_code Der zu dekodierende Morsecode
 * @return char* Der resultierende Text
 */
char* decode_from_morse(const char* morse_code) {
    if (morse_code == NULL) {
        return NULL;
    }
    
    /* Abschätzung der maximalen Länge des Textes */
    size_t max_length = strlen(morse_code);
    char* text = (char*)malloc(max_length);
    if (text == NULL) {
        return NULL;
    }
    
    text[0] = '\0';
    size_t text_index = 0;
    
    /* Temporärer Puffer für einen einzelnen Morsecode */
    char morse_buffer[MAX_MORSE_CODE_LENGTH + 1];
    size_t morse_index = 0;
    
    /* Zustand für die Erkennung von Worttrennern */
    int space_count = 0;
    bool in_slash_wordspacer = false;
    
    for (size_t i = 0; morse_code[i] != '\0'; i++) {
        if (morse_code[i] == ' ') {
            /* Wenn ein Morsecode im Puffer ist, wandle ihn um */
            if (morse_index > 0) {
                morse_buffer[morse_index] = '\0';
                char c = get_char_for_morse(morse_buffer);
                if (c != '\0') {
                    text[text_index++] = c;
                }
                morse_index = 0;
            }
            
            /* Zähle aufeinanderfolgende Leerzeichen */
            space_count++;
            
            /* Prüfe auf Slash-Worttrenner */
            if (space_count == 1 && morse_code[i+1] == '/' && morse_code[i+2] == ' ') {
                in_slash_wordspacer = true;
                i += 2; /* Überspringe den Slash und das folgende Leerzeichen */
                space_count = 0;
                text[text_index++] = ' ';
            }
            /* Prüfe auf normalen Worttrenner (3 Leerzeichen) */
            else if (space_count == 3) {
                text[text_index++] = ' ';
                space_count = 0;
            }
        } else {
            /* Zurücksetzen des Leerzeichenzählers */
            space_count = 0;
            in_slash_wordspacer = false;
            
            /* Füge das Zeichen zum Morsecode-Puffer hinzu */
            if (morse_index < MAX_MORSE_CODE_LENGTH) {
                morse_buffer[morse_index++] = morse_code[i];
            }
        }
    }
    
    /* Verarbeite den letzten Morsecode im Puffer */
    if (morse_index > 0) {
        morse_buffer[morse_index] = '\0';
        char c = get_char_for_morse(morse_buffer);
        if (c != '\0') {
            text[text_index++] = c;
        }
    }
    
    text[text_index] = '\0';
    return text;
}

/**
 * @brief Gibt die Programmierer-Informationen im JSON-Format aus
 */
void print_programmer_info(void) {
    printf("{\n");
    printf("    \"firstname\": \"VORNAME\",\n");
    printf("    \"surname\": \"NACHNAME\",\n");
    printf("    \"branch_of_study\": \"TIT\",\n");
    printf("    \"contact\": \"\"\n");
    printf("}\n");
}

/**
 * @brief Gibt den Hilfetext aus
 */
void print_help(void) {
    printf("Verwendung: morse [OPTIONEN] [DATEI...]\n");
    printf("Wandelt Text in Morsecode um oder dekodiert Morsecode zurück in Text.\n\n");
    printf("Optionen:\n");
    printf("  -h, --help                Zeigt diese Hilfe an\n");
    printf("  -e, --encode              Kodiert Text in Morsecode (Standard)\n");
    printf("  -d, --decode              Dekodiert Morsecode in Text\n");
    printf("  -o, --out DATEI           Schreibt die Ausgabe in die angegebene Datei\n");
    printf("  --programmer-info         Zeigt Informationen über den Programmierer an\n");
    printf("  --slash-wordspacer        Verwendet '/' als Worttrenner (nur mit -e)\n\n");
    printf("Wenn keine Datei angegeben ist, wird von der Standardeingabe gelesen.\n");
    printf("Beispiele:\n");
    printf("  morse -e \"Hallo Welt\"     Kodiert den Text \"Hallo Welt\" in Morsecode\n");
    printf("  morse -d \".... .- .-.. .-.. ---\"  Dekodiert den Morsecode in Text\n");
    printf("  morse -e datei.txt -o datei.morse  Kodiert den Inhalt von datei.txt und\n");
    printf("                                     speichert ihn in datei.morse\n");
}
