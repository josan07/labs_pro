#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RADIO 3E9
#define MICROWAVE 3E12
#define INFRARED 4.3E14
#define VISIBLE 7.5E14
#define ULTRAVIOLET 3E17
#define XRAYS 3E19

int main()
{
    float frequencia;
    float base, exp;

    printf("Insira a frequencia (notacao cientifica): ");
    scanf("%fE%f", &base, &exp);
    frequencia = base * pow(10, exp);

    if(frequencia < RADIO){printf("Radio waves");}
    else if(frequencia < MICROWAVE){printf("Microwaves");}
    else if(frequencia < INFRARED){printf("Infrared light");}
    else if(frequencia < VISIBLE){printf("Visible light");}
    else if(frequencia < ULTRAVIOLET){printf("Ultraviolet");}
    else if(frequencia < XRAYS){printf("X-rays");}
    else{printf("Gamma rays");}}
