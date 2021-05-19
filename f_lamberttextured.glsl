#version 330

uniform sampler2D textureMap0;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 ic; 
in vec4 n;
in vec4 l[2];
in vec4 v;

in vec2 iTexCoord0;

void main(void) {

	pixelColor = vec4(0);
	//Znormalizowane interpolowane wektory
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	vec4 ml[2];
	vec4 mr[2];
	

	//Parametry powierzchni
	// cz�� lamberta
	vec4 kd = texture(textureMap0, iTexCoord0); // kolor materia�u dla �wiat�a rozproszenia
	// ld - kolor �wiat�a rozporoszonego, domy�lnie bia�y wiec nie ma go we wzorze
	// ka - kolor materia�u dla �wiat�a otoczenia
	// la - kolor �wiat�a otoczenia
	// cz�� phonga
	vec4 ks = kd; // kolor materia�u dla �wiat�a odbitego
	// ls - kolor �wiat�a odbitego
	// r - wektor odbitego �wiat�a


	float nl[2];
	float rv[2];
	for(int i = 0; i < 2; i++) {
		ml[i] = normalize(l[i]);
		mr[i] = reflect(-ml[i], mn);
		//Obliczenie modelu o�wietlenia
		nl[i] = clamp(dot(mn, ml[i]), 0, 1);
		rv[i] = pow(clamp(dot(mr[i], mv), 0, 1), 300);
		pixelColor += vec4(kd.rgb * nl[i], kd.a) + vec4(ks.rgb*rv[i], 0);
	}
}
