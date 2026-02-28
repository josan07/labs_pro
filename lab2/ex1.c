#include <stdio.h>
#include <stdlib.h>

int main()

{
    int total_s, h, min, s;

    printf("Insira o numero total de segundos: ");
    scanf("%d", &total_s);

    h = total_s / 3600;
    min = (total_s % 3600) / 60;
    s = (total_s % 3600) % 60;

    printf("Conversao: %02d:%02d:%02d", h, min, s);

    return 0;
}
