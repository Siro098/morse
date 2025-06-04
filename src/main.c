/**
 * @file main.c
 * @brief Hauptprogramm für das Morse-Code-Tool
 * 
 * Diese Datei enthält die main-Funktion und die Verarbeitung der Kommandozeilenoptionen
 * für das Morse-Code-Tool.
 */

#include "../include/morse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/**
 * @brief Maximale Größe für Eingabe-/Ausgabepuffer
 */
#define MAX_BUFFER_SIZE 10240

/**
 * @brief Struktur für die Programmoptionen
 */
typedef struct {
    bool encode;
    bool decode;
    bool help;
    bool programmer_info;
    bool slash_wordspacer;
    char* output_file;
    char** input_files;
    int input_file_count;
} ProgramOptions;

/**
 * @brief Verarbeitet die Kommandozeilenoptionen
 * 
 * @param argc Anzahl der Kommandozeilenargumente
 * @param argv Array der Kommandozeilenargumente
 * @param options Zeiger auf die Struktur für die Programmoptionen
 * @return int 0 bei Erfolg, -1 bei Fehler
 */
int parse_options(int argc, char* argv[], ProgramOptions* options) {
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"encode", no_argument, 0, 'e'},
        {"decode", no_argument, 0, 'd'},
        {"out", required_argument, 0, 'o'},
        {"programmer-info", no_argument, 0, 'p'},
        {"slash-wordspacer", no_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;

    /* Standardwerte setzen */
    options->encode = true;  /* Encode ist Standard */
    options->decode = false;
    options->help = false;
    options->programmer_info = false;
    options->slash_wordspacer = false;
    options->output_file = NULL;
    options->input_files = NULL;
    options->input_file_count = 0;

    while ((c = getopt_long(argc, argv, "hedo:", long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                options->help = true;
                break;
            case 'e':
                options->encode = true;
                break;
            case 'd':
                options->encode = false;
                options->decode = true;
                break;
            case 'o':
                options->output_file = optarg;
                break;
            case 'p':
                options->programmer_info = true;
                break;
            case 's':
                options->slash_wordspacer = true;
                break;
            case '?':
                /* getopt_long gibt bereits eine Fehlermeldung aus */
                return -1;
            default:
                return -1;
        }
    }

    /* Prüfe auf widersprüchliche Optionen */
    if (options->encode && options->decode) {
        fprintf(stderr, "Fehler: Die Optionen -e/--encode und -d/--decode können nicht gleichzeitig verwendet werden.\n");
        return -1;
    }

    /* Prüfe, ob --slash-wordspacer nur mit -e/--encode verwendet wird */
    if (options->slash_wordspacer && options->decode) {
        fprintf(stderr, "Fehler: Die Option --slash-wordspacer kann nur mit -e/--encode verwendet werden.\n");
        return -1;
    }

    /* Sammle die verbleibenden Argumente als Eingabedateien */
    if (optind < argc) {
        options->input_file_count = argc - optind;
        options->input_files = &argv[optind];
    }

    return 0;
}

/**
 * @brief Liest den Inhalt einer Datei
 * 
 * @param filename Pfad zur Datei
 * @return char* Inhalt der Datei oder NULL bei Fehler
 */
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Fehler: Datei '%s' konnte nicht geöffnet werden.\n", filename);
        return NULL;
    }

    /* Bestimme die Größe der Datei */
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    /* Speicher für den Inhalt allozieren */
    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Fehler: Nicht genügend Speicher verfügbar.\n");
        fclose(file);
        return NULL;
    }

    /* Datei lesen */
    size_t read_size = fread(buffer, 1, file_size, file);
    buffer[read_size] = '\0';

    fclose(file);
    return buffer;
}

/**
 * @brief Liest den Inhalt von stdin
 * 
 * @return char* Inhalt von stdin oder NULL bei Fehler
 */
char* read_stdin() {
    char buffer[MAX_BUFFER_SIZE];
    size_t total_size = 0;
    size_t buffer_size = MAX_BUFFER_SIZE;
    char* content = (char*)malloc(buffer_size);
    
    if (content == NULL) {
        fprintf(stderr, "Fehler: Nicht genügend Speicher verfügbar.\n");
        return NULL;
    }
    
    content[0] = '\0';
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t read_size = strlen(buffer);
        
        /* Vergrößere den Puffer, wenn nötig */
        if (total_size + read_size + 1 > buffer_size) {
            buffer_size *= 2;
            char* new_content = (char*)realloc(content, buffer_size);
            
            if (new_content == NULL) {
                fprintf(stderr, "Fehler: Nicht genügend Speicher verfügbar.\n");
                free(content);
                return NULL;
            }
            
            content = new_content;
        }
        
        /* Füge den gelesenen Text hinzu */
        strcat(content, buffer);
        total_size += read_size;
    }
    
    return content;
}

/**
 * @brief Schreibt Text in eine Datei oder auf stdout
 * 
 * @param text Der zu schreibende Text
 * @param filename Pfad zur Datei oder NULL für stdout
 * @return int 0 bei Erfolg, -1 bei Fehler
 */
int write_output(const char* text, const char* filename) {
    if (filename == NULL) {
        /* Ausgabe auf stdout */
        printf("%s\n", text);
        return 0;
    } else {
        /* Ausgabe in Datei */
        FILE* file = fopen(filename, "w");
        if (file == NULL) {
            fprintf(stderr, "Fehler: Datei '%s' konnte nicht zum Schreiben geöffnet werden.\n", filename);
            return -1;
        }
        
        fprintf(file, "%s\n", text);
        fclose(file);
        return 0;
    }
}

/**
 * @brief Hauptfunktion des Programms
 * 
 * @param argc Anzahl der Kommandozeilenargumente
 * @param argv Array der Kommandozeilenargumente
 * @return int Exitcode des Programms
 */
int main(int argc, char* argv[]) {
    ProgramOptions options;
    
    /* Verarbeite die Kommandozeilenoptionen */
    if (parse_options(argc, argv, &options) != 0) {
        return EXIT_FAILURE;
    }
    
    /* Zeige Hilfe an */
    if (options.help) {
        print_help();
        return EXIT_SUCCESS;
    }
    
    /* Zeige Programmierer-Informationen an */
    if (options.programmer_info) {
        print_programmer_info();
        return EXIT_SUCCESS;
    }
    
    /* Verarbeite die Eingabe */
    char* input = NULL;
    
    if (options.input_file_count > 0) {
        /* Lese die erste Eingabedatei */
        input = read_file(options.input_files[0]);
        if (input == NULL) {
            return EXIT_FAILURE;
        }
    } else {
        /* Lese von stdin */
        input = read_stdin();
        if (input == NULL) {
            return EXIT_FAILURE;
        }
    }
    
    /* Verarbeite den Text */
    char* output = NULL;
    
    if (options.decode) {
        output = decode_from_morse(input);
    } else {
        output = encode_to_morse(input, options.slash_wordspacer);
    }
    
    free(input);
    
    if (output == NULL) {
        fprintf(stderr, "Fehler: Verarbeitung fehlgeschlagen.\n");
        return EXIT_FAILURE;
    }
    
    /* Schreibe die Ausgabe */
    int result = write_output(output, options.output_file);
    
    free(output);
    
    return (result == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
