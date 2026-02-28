#include <stdio.h>
#include <stdlib.h>

int main()
{
    int total_s_1, total_s_2, h_1, min_1, s_1, h_2, min_2, s_2, dif_s;

    printf("Insira o primeiro tempo: ");
    scanf("%d:%d:%d", &h_1, &min_1, &s_1);
    total_s_1 = h_1 * 3600 + min_1 * 60 + s_1;


    printf("Insira o segundo tempo: ");
    scanf("%d:%d:%d", &h_2, &min_2, &s_2);
    total_s_2 = h_2 * 3600 + min_2 * 60 + s_2;


    dif_s = total_s_1 - total_s_2;
    printf("%d segundos \n", dif_s);
    conversao(dif_s);

    return 0;
}

void conversao(int total_s){
    int h, min, s;
    h = total_s / 3600;
    min = (total_s % 3600) / 60;
    s = (total_s % 3600) % 60;

    printf("%02d:%02d:%02d", h, min, s);
}
