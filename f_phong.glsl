#version 330

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;
uniform vec4 lp[2];
uniform mat4 M;
uniform float alcoholLevel;
uniform float alcoholicAngle;
uniform float alcoholRandomShift;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 ic; 
in vec4 n;
in vec4 l[2];
in vec4 v;
in vec4 iVert;
in vec2 iTexCoord0;


// https://stackoverflow.com/questions/8507885/shift-hue-of-an-rgb-color
vec3 hueShift(vec3 col, float shift){
    vec3 m = vec3(cos(shift), -sin(shift) * .57735, 0);
    m = vec3(m.xy, -m.y) + (1. - m.x) * .33333;
    return mat3(m, m.zxy, m.yzx) * col;
}

void main(void) {
	// https://wp.faculty.wmi.amu.edu.pl/GRK5E.pdf
	vec4 lightColors[2] = {vec4(0.8f, 0.8f, 0.8f, 1.0f), vec4(0.8f, 0.8f, 0.8f, 1.0f)};
	float pointLightSize = 20.0f;
	pixelColor = vec4(0);
	//Znormalizowane interpolowane wektory
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	vec4 ml[2];
	vec4 mr[2];
	
	float alocoholHueShift = sin(alcoholicAngle + alcoholRandomShift) * clamp(alcoholLevel / 10.0f, 0.0f, 1.0f) * 2 * 3.141526;

	//Parametry powierzchni
	// czêœæ lamberta
	
	vec4 kd = texture(textureMap0, iTexCoord0); // kolor materia³u dla œwiat³a rozproszenia
	kd = vec4(hueShift(kd.rgb, alocoholHueShift), kd.a);
	// ld - kolor œwiat³a rozporoszonego, domyœlnie bia³y wiec nie ma go we wzorze
	// ka - kolor materia³u dla œwiat³a otoczenia
	// la - kolor œwiat³a otoczenia
	// czêœæ phonga
	vec4 ks = texture(textureMap1, iTexCoord0); // kolor materia³u dla œwiat³a odbitego ze specular map
	// ls - kolor œwiat³a odbitego
	// r - wektor odbitego œwiat³a


	float nl[2];
	float rv[2];
	for(int i = 0; i < 2; i++) {
		ml[i] = normalize(l[i]);
		mr[i] = reflect(-ml[i], mn);
		//Obliczenie modelu oœwietlenia
		nl[i] = clamp(dot(mn, ml[i]), 0, 1);
		rv[i] = pow(clamp(dot(mr[i], mv), 0, 1), 25);
		float dist = distance(lp[i], M * iVert);
		float facct = sin(1 - clamp(dist / pointLightSize, 0, 0.95)) + 0.05;
		pixelColor += lightColors[i] * facct * vec4(kd.rgb * nl[i], kd.a) + facct * vec4(ks.rgb*rv[i], 0);
	}
}
