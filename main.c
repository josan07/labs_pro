#include "eliza.h"

int main(int argc, char* argv[]) {
    char greeting[MAX_CHARS];
    char repetition[MAX_CHARS];

    char farewell[MAX_CHARS];
    BlockDB* database = NULL;
    int nr_blocks = 0;

    // FLAGS
    int opt;
    char language = 'E';  // 0 para inglês; 1 para português
    char* filename = "eliza.dat";

    char* f_inputName = NULL;
    FILE* f_input = stdin;

    char* f_outputName = NULL;
    FILE* f_output = stdout;

    char* f_logName = NULL;
    FILE* f_log = NULL;

    int flag_i = 0;
    int flag_o = 0;
    int flag_l = 0;

    while ((opt = getopt(argc, argv, "hi:o:l:f:p")) != -1) {
        switch (opt) {
            case 'h':
                help();
                exit(EXIT_SUCCESS);
            case 'i':
                f_inputName = optarg;
                flag_i = 1;
                break;
            case 'o':
                f_outputName = optarg;
                flag_o = 1;
                break;
            case 'l':
                f_logName = optarg;
                flag_l = 1;
                break;
            case 'f':
                filename = optarg;
                break;
            case 'p':
                language = 'P';
                break;
            case '?':
                printf("Opção inválida.\n");
                exit(EXIT_FAILURE);
        }
    }

    if (flag_i) {
        f_input = fopen(f_inputName, "r");
        if (f_input == NULL) {
            printf("Erro ao abrir %s\n", f_inputName);
            exit(EXIT_FAILURE);
        }
    }
    if (flag_o) {
        f_output = fopen(f_outputName, "w");
        if (f_output == NULL) {
            printf("Erro ao abrir %s\n", f_outputName);
            exit(EXIT_FAILURE);
        }
    }
    if (flag_l) {
        f_log = fopen(f_logName, "w");
        if (f_log == NULL) {
            printf("Erro ao abrir %s\n", f_logName);
            exit(EXIT_FAILURE);
        }
    }

    // chamar a função read_file, que regista a base de dados após ler o ficheiro
    read_file(filename, greeting, repetition, farewell, &database, &nr_blocks);

    char former_line[MAX_CHARS] = "";
    // registo da ultima entrada, util pra chamar o repetition
    char line[MAX_CHARS];

    // antes de tudo, usar greeting para saudar o utilizador
    fprintf_2(f_output, f_log, greeting);

    // ciclo que recebe como inputs stdin e os regista de cada vez como line, terminando implicitamente no EOF
    while (fgets(line, sizeof(line), f_input)) {
        // cortar a linha no enter
        line[strcspn(line, "\r\n")] = '\0';

        // regista os inputs no log
        if (f_log != NULL) {
            fprintf(f_log, "%s\n", line);
        }
        // formatar a linha
        format_input(line);

        // ignorar se o user deu enter sem escrever nada
        if (strlen(line) == 0) continue;

        // ignorar linhas apenas com espaços (input tipo '   ')
        int spaces_only = 1;
        for (int i = 0; line[i] != 0; i++) {
            if (!isspace(line[i])) {
                spaces_only = 0;
                break;
            }
        }

        if (spaces_only) continue;

        // mostrar mensagem de despedida se o user digitar especificamente 'BYE'
        if (((strcmp("BYE", line) == 0) && language == 'E') ||
            ((strcmp("ADEUS", line) == 0) && language == 'P')) {
            fprintf_2(f_output, f_log, farewell);
            break;
        }

        // comparar linha atual com linha imediatamente anterior: se forem iguais mostrar mensagem de repetição
        if (strcmp(former_line, line) == 0) {
            fprintf_2(f_output, f_log, repetition);
            continue;
        }

        // atualizar valor da ultima linha, para poder comparar a proxima linha com esta
        strcpy(former_line, line);

        // invocar função search_database para dar print à resposta (Stdout)
        search_database(line, database, nr_blocks, f_output, f_log, language);
    }

    if (f_input != stdin) {
        fclose(f_input);
    }
    if (f_output != stdout) {
        fclose(f_output);
    }
    if (f_log != NULL) {
        fclose(f_log);
    }

    for (int i = 0; i < nr_blocks; i++) {
        for (int j = 0; j < database[i].nr_keywords; j++) {
            free(database[i].keywords[j]);
        }
        free(database[i].keywords);

        for (int j = 0; j < database[i].nr_answers; j++) {
            free(database[i].answers[j]);
        }
        free(database[i].answers);
    }
    free(database);


    return EXIT_SUCCESS;
}
