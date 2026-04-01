#include "eliza.h"

void help() {
    printf("Uso: ./eliza [OPTIONS]\n\n");
    printf("Opções disponíveis:\n");
    printf("  -h          Mostra esta ajuda para o utilizador e termina\n");
    printf("  -i filename Nome do ficheiro de entrada, em alternativa a stdin\n");
    printf("  -o filename Nome do ficheiro de saída, em alternativa a stdout\n");
    printf("  -l filename Fazer log do input e output para o ficheiro filename\n");
    printf("  -f filename Usar ficheiro filename com base de dados de respostas em vez de \"eliza.dat\"\n");
    printf("  -p          Usar regras de português, em vez de inglês\n");
}

void fprintf_2(FILE* output_file, FILE* log_file, char* msg) {
    fprintf(output_file, "%s\n", msg);
    if (log_file != NULL) {
        fprintf(log_file, "%s\n", msg);
    }
}