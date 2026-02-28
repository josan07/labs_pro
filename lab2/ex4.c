#include <stdio.h>
#include <stdlib.h>

int main()
{
    int cent, dec, uni;

    printf("Insira três números: ");
    scanf("%d\n%d\n%d", &cent, &dec, &uni);
    printf("%d", 2 * (100 * cent + 10 * dec + uni));

}
