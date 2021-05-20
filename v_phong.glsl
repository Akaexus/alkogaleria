#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lp[2];
//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
//in vec4 color; //kolor zwi¹zany z wierzcho³kiem
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0;

//Zmienne interpolowane
out vec4 ic;
out vec4 l[2];
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;
out vec4 iVert;

void main(void) {
    vec4 color = vec4(0.2, 1, 0.2, 1);
    //vec4 lp[2] = {vec4(17, 1.2, 0, 1), vec4(-17, 1.2, 0, 1)};
    l[0] = normalize(V * lp[0] - V*M*vertex); //wektor do œwiat³a w przestrzeni oka
    l[1] = normalize(V * lp[1] - V*M*vertex); //wektor do œwiat³a w przestrzeni oka
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //wektor do obserwatora w przestrzeni oka
    n = normalize(V * M * normal); //wektor normalny w przestrzeni oka
    iVert = vertex;
    
    
    iTexCoord0 = texCoord0;

    ic = color;
    
    gl_Position=P*V*M*vertex;
}
