#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159
#define GRAV 9.8



typedef struct{
    float min;
    float max;
}twoFloats;



//prototypes
float dist_ideal(float theta, float v);
float maximof(float, float);
float minimof(float, float);
twoFloats theta_min_max(float primeiro_ang, float ultimo_ang, float d, float v, float c);


int main(){

    float d, v, c;

    printf("Digite a distancia: ");
    scanf("%f", &d);
    printf("Digite a velocidade inicial: ");
    scanf("%f", &v);
    printf("Digite o diametro do cesto: ");
    scanf("%f", &c);


    twoFloats t1 = theta_min_max(0,45,d,v,c);
    twoFloats t2 = theta_min_max(45.01,90,d,v,c);

    if(t1.min == -5 || t2.min == -5){
        printf("A bola nunca entra no cesto");
    }else{
    float angulo_minimo = minimof(t1.min, t2.min);
    float angulo_maximo = maximof(t1.max, t2.max);

    printf("Angulo minimo = %.1f \nAngulo maximo = %.1f", angulo_minimo, angulo_maximo);}

    return 0;
}


twoFloats theta_min_max(float primeiro_ang, float ultimo_ang, float d, float v, float c){
    
    twoFloats angulo;

    float theta = primeiro_ang;

    angulo.min = -5;
    angulo.max = -5;


    int dentro_cesto = 0;

    while(theta < ultimo_ang){

        float dist_min = dist_ideal(theta, v) - (c/2);
        float dist_max = dist_ideal(theta, v) + (c/2);
        
        if(!dentro_cesto && dist_min <= d && dist_max >= d){

            angulo.min = theta;
            dentro_cesto = 1;

        }else if(dentro_cesto && dist_max < d){
            angulo.max = theta - .01;
            break;
        }

        theta += .01;
    }
    return angulo;
}


float dist_ideal(float theta, float v){
    return (pow(v, 2)*sin(2*theta*(PI/180)))/GRAV;
}


float maximof(float x, float y){
    if(x>=y) return x;
    else return y;
}


float minimof(float x, float y){
    if(x>=y) return y;
    else return x;
}

