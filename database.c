#include "eliza.h"

/**
 * A função read_file lê o ficheiro *.dat e inicializa a base de dados.
 * Extrai as 3 mensagens especiais iniciais (greeting, repetition, farewell) e, de seguida,
 * processa os blocos de palavras-chave e respostas, alocando memória dinamicamente
 * para a estrutura BlockDB.
 * Aceita como argumentos:
 * > filename:      nome do ficheiro a ler
 * > greeting:      buffer para guardar a mensagem de acolhimento
 * > repetition:    buffer para guardar a mensagem de repetição
 * > farewell:      buffer para guardar a mensagem de despedida
 * > database       duplo ponteiro para o array dinâmico de BlockDB
 * > nr_blocks      ponteiro para guardar o número total de blocos alocados
 */
void read_file(char* filename, char* greeting, char* repetition,
               char* farewell, BlockDB** database, int* nr_blocks) {
    // abrir o file pointer
    FILE* fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("Erro na abertura do ficheiro\n");
        exit(EXIT_FAILURE);
    }

    int state = 0;          // 0 significa que estamos a ler keywords; 1 significa que estamos a ler respostas
    int block_counter = 0;  // contador de blocos
    int nr_blocks_allocated = 0;
    char buffer[MAX_CHARS];

    // Ler ficheiro linha a linha
    while (fgets(buffer, sizeof(buffer), fptr)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (strlen(buffer) == 0) continue;

        // converte toda a linha lida para maiúsculas
        for (int i = 0; buffer[i] != '\0'; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        // state 0->1: transição de keywords para respostas
        if (strcmp(buffer, "!") == 0) {
            state = 1;
            continue;
        }

        // state 1->0: transição de respostas para keywords
        // fim de bloco, contador aumenta 1
        if (strcmp(buffer, ".") == 0) {
            state = 0;
            block_counter++;
            continue;
        }

        // tratamento das 3 mensagens especiais
        if (block_counter == 0 && state) {
            strcpy(greeting, buffer);
        } else if (block_counter == 1 && state) {
            strcpy(repetition, buffer);
        } else if (block_counter == 2 && state) {
            strcpy(farewell, buffer);
        } else if (block_counter >= 3) {
            // processamento dos blocos de dados normais (depois dos 3 especiais)
            int db_index = block_counter - 3;

            // se for necessário mais espaço no array de blocos, realocar
            if (db_index >= nr_blocks_allocated) {
                nr_blocks_allocated++;

                *database = realloc(*database, nr_blocks_allocated * sizeof(BlockDB));

                memset(&((*database)[db_index]), 0, sizeof(BlockDB));
            }

            // buffer temporário
            BlockDB* bufferDB = &((*database)[db_index]);
            if (!state) {  // armazenar keywords (state == 0)
                // Realocação do array de pointers para as keywords
                if (bufferDB->nr_keywords >= bufferDB->alloc_space_keywords) {
                    bufferDB->alloc_space_keywords += 5;
                    bufferDB->keywords = realloc(bufferDB->keywords, bufferDB->alloc_space_keywords * sizeof(char*));
                }

                bufferDB->keywords[bufferDB->nr_keywords] = malloc(strlen(buffer) + 1);
                strcpy(bufferDB->keywords[bufferDB->nr_keywords], buffer);
                bufferDB->nr_keywords++;

            } else {  // armazenar answers
                if (bufferDB->nr_answers >= bufferDB->alloc_space_answers) {
                    bufferDB->alloc_space_answers += 5;
                    bufferDB->answers = realloc(bufferDB->answers, bufferDB->alloc_space_answers * sizeof(char*));
                }

                bufferDB->answers[bufferDB->nr_answers] = malloc(strlen(buffer) + 1);
                strcpy(bufferDB->answers[bufferDB->nr_answers], buffer);
                bufferDB->nr_answers++;
            }
        }
    }

    *nr_blocks = nr_blocks_allocated;

    // fechar filepointer
    fclose(fptr);
}

void search_database(char* line, BlockDB normal[MAX_BLOCKS], int nr_blocks, FILE* f_output, FILE* f_log, char language) {
    char* keyword_to_check;
    int keyword_found = 0;
    int num_keyword_blocks = nr_blocks - 1;

    char* pt_find[] = {"EU", "TU", "COMIGO", "CONTIGO", "TEU", "SEU", "TEUS", "SEUS", "TUA", "SUA", "TUAS", "SUAS", "MEU", "MEUS", "MINHA", "MINHAS", "MIM", "TI"};
    char* pt_replace[] = {"TU", "EU", "CONTIGO", "COMIGO", "MEU", "MEU", "MEUS", "MEUS", "MINHA", "MINHA", "MINHAS", "MINHAS", "TEU", "TEUS", "TUA", "TUAS", "TI", "MIM"};

    char* en_find[] = {"ARE", "AM", "WERE", "WAS", "YOU", "I", "YOUR", "MY", "IVE", "YOUVE", "IM", "ME", "US", "WE"};
    char* en_replace[] = {"AM", "ARE", "WAS", "WERE", "I", "YOU", "MY", "YOUR", "YOUVE", "IVE", "YOURE", "YOU", "YOU", "YOU"};

    for (int i = 0; i < num_keyword_blocks; i++) {  // iterar  nos blocos
        int k = normal[i].nr_keywords;

        // k para iterar ao longo das keywords de  cada bloco
        for (int j = 0; j < k; j++) {
            keyword_to_check = normal[i].keywords[j];
            // checkar a keyword índice j
            char* kw_pos = clean_boundaries(line, keyword_to_check);

            if (kw_pos != NULL) {
                char* answerBuffer = normal[i].answers[normal[i].rr_counter];
                if (strstr(answerBuffer, "*")) {
                    char finalAnswer[MAX_CHARS] = "";

                    char* ptr_asterisk = strchr(answerBuffer, '*');
                    int length_before_asterisk = ptr_asterisk - answerBuffer;
                    strncat(finalAnswer, answerBuffer, length_before_asterisk);

                    char* raw_input = kw_pos + strlen(keyword_to_check);
                    if (*raw_input == ' ') {
                        strcat(finalAnswer, " ");
                        raw_input++;
                    }

                    char conjugated_input[MAX_CHARS] = "";
                    if (language == 'P') {
                        conjugate(raw_input, pt_replace, pt_find, 18, conjugated_input, 'P');
                    } else if (language == 'E') {
                        conjugate(raw_input, en_replace, en_find, 14, conjugated_input, 'E');
                    }
                    strcat(finalAnswer, conjugated_input);

                    char* answer_after_asterisk = answerBuffer + length_before_asterisk + 1;
                    strcat(finalAnswer, answer_after_asterisk);

                    fprintf_2(f_output, f_log, finalAnswer);

                } else {  // se não for detectado asterisco na resposta do ficheiro *.dat
                    fprintf_2(f_output, f_log, normal[i].answers[normal[i].rr_counter]);
                }
                normal[i].rr_counter = (normal[i].rr_counter + 1) % normal[i].nr_answers;
                keyword_found = 1;
                return;
            }
        }
    }
    if (!keyword_found) {  // nao encontrou keyword!
        int nkf_pos = nr_blocks - 1;
        fprintf_2(f_output, f_log, normal[nkf_pos].answers[normal[nkf_pos].rr_counter]);
        normal[nkf_pos].rr_counter = (normal[nkf_pos].rr_counter + 1) % normal[nkf_pos].nr_answers;
        return;
    }
}

void conjugate(char* sentence, char** list_replace, char** list_find, int list_size, char* conjugated_sentence, char language) {
    char copy[MAX_CHARS];
    strcpy(copy, sentence);

    conjugated_sentence[0] = '\0';

    char* token = strtok(copy, " ");
    char* next_token = strtok(NULL, " ");

    while (token != NULL) {
        int nr_word = -1;

        for (int i = 0; i < list_size; i++) {
            if (strcmp(list_find[i], token) == 0) {
                nr_word = i;
                break;
            }
        }

        if (nr_word == -1) {
            strcat(conjugated_sentence, token);
        } else {
            if ((language == 'E') && (next_token == NULL) && (strcmp(token, "YOU") == 0)) {
                strcat(conjugated_sentence, "ME");
            } else {
                strcat(conjugated_sentence, list_replace[nr_word]);
            }
        }

        if (next_token != NULL) {
            strcat(conjugated_sentence, " ");
        }

        token = next_token;

        if (token != NULL) {
            next_token = strtok(NULL, " ");
        }
    }
}
