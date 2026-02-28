#include <stdio.h>


int main()
    int n, j, pow_10;
    float numero, numero_arred;

    printf("Introduza o numero real: ");
    scanf("%f", &numero);

    printf("Introduza o nr de casas decimais: ");
    scanf("%d", &n);

    pow_10 = 1;

    if(1<=n && n<=7){
        for(j=1; j<=n; j++){
            pow_10 *= 10;
        }
    }

    numero_arred = ((int)((numero * pow_10) + .5))/ (float)pow_10;

    printf("%.*f", n, numero_arred);

    return 0;
}

