#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GRAV 9.8
#define PI 3.141592

int main()
{
    float deg_theta, v_0, altura, rad_theta, d, h;

    printf("Insira o ângulo theta em graus: ");
    scanf("%f", &deg_theta);

    printf("Insira a velocidade inicial: ");
    scanf("%f", &v_0);

    printf("Insira a altura do teto: ");
    scanf("%f", &altura);

    rad_theta = deg_theta * (PI / 180);

    d = ( pow(v_0, 2) * sin(2*rad_theta) ) / GRAV;
    h = ( pow(v_0, 2) * pow( sin(rad_theta), 2) ) / (2*GRAV);

    if(h>=altura){
            printf("O projétil bate no teto");
    }else{
    printf("Deve estar a %.3f metros do cesto", d);
    }
    return 0;
}
