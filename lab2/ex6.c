#include <stdio.h>
#include <stdlib.h>

int main()
{
    int nr_mes;

    printf("digite o numero do mes: ");
    scanf("%d", &nr_mes);

    if (nr_mes>=1 && nr_mes<=12){
    switch(nr_mes){
    case 2:
        printf("28 dias");
        break;
    case 4: case 6: case 9: case 11:
        printf("30 dias");
        break;
    default:
        printf("31 dias");
        break;
    }}else{printf("Numero invalido");}

    return 0;
}
