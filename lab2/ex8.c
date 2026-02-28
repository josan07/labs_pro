
#include <stdio.h>
#include <math.h>

#define PI 3.14159
#define G 9.8



float dist_ideal(float, float);



int main(){

    float d, v, c;

    printf("Digite a distancia: ");
    scanf("%f", &d);
    printf("Digite a velocidade inicial: ");
    scanf("%f", &v);
    printf("Digite o diametro do cesto: ");
    scanf("%f", &c);

    float theta = 0;

    while(theta<90){
        float dist_min = dist_ideal(theta, v) - c/2;
        float dist_max = dist_ideal(theta, v) + c/2;

        if(dist_min <= d && dist_max >= d){
            printf("%.1f\n", theta);
        }

        theta += 1;
    }

    return 0;

}



float dist_ideal(float theta, float v){
    return (pow(v, 2)*sin(2*theta*(PI/180)))/G;
}


