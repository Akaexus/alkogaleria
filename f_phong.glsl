#version 330

uniform sampler2D textureMap0;
uniform vec4 lp[2];
uniform mat4 M;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 ic; 
in vec4 n;
in vec4 l[2];
in vec4 v;
in vec4 iVert;
in vec2 iTexCoord0;

void main(void) {
	// https://wp.faculty.wmi.amu.edu.pl/GRK5E.pdf
	vec4 lightColors[2] = {vec4(0.8f, 0.8f, 0.8f, 1.0f), vec4(0.8f, 0.8f, 0.8f, 1.0f)};
	float pointLightSize = 12.0f;
	pixelColor = vec4(0);
	//Znormalizowane interpolowane wektory
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	vec4 ml[2];
	vec4 mr[2];
	

	//Parametry powierzchni
	// czêœæ lamberta
	vec4 kd = texture(textureMap0, iTexCoord0); // kolor materia³u dla œwiat³a rozproszenia
	// ld - kolor œwiat³a rozporoszonego, domyœlnie bia³y wiec nie ma go we wzorze
	// ka - kolor materia³u dla œwiat³a otoczenia
	// la - kolor œwiat³a otoczenia
	// czêœæ phonga
	vec4 ks = kd; // kolor materia³u dla œwiat³a odbitego
	// ls - kolor œwiat³a odbitego
	// r - wektor odbitego œwiat³a


	float nl[2];
	float rv[2];
	for(int i = 0; i < 2; i++) {
		ml[i] = normalize(l[i]);
		mr[i] = reflect(-ml[i], mn);
		//Obliczenie modelu oœwietlenia
		nl[i] = clamp(dot(mn, ml[i]), 0, 1);
		rv[i] = pow(clamp(dot(mr[i], mv), 0, 1), 300);
		float dist = distance(lp[i], M * iVert);
		float facct = sin(1 - clamp(dist / pointLightSize, 0, 0.95)) + 0.05;
		pixelColor += lightColors[i] * facct * vec4(kd.rgb * nl[i], kd.a) + vec4(ks.rgb*rv[i], 0);
	}
}
