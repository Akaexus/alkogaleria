#version 330

uniform sampler2D textureMap0;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 ic; 
in vec4 n;
in vec4 l[2];
in vec4 v;

in vec2 iTexCoord0;

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 ml[2] = {normalize(l[0]), normalize(l[1])};
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	//Wektor odbity
	vec4 mr[2] = {reflect(-ml[0], mn), reflect(-ml[0], mn)};

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

	//Obliczenie modelu oœwietlenia
	float nl[2] = {clamp(dot(mn, ml[0]), 0, 1), clamp(dot(mn, ml[1]), 0, 1)};
	float rv[2] = {pow(clamp(dot(mr[0], mv), 0, 1), 300), pow(clamp(dot(mr[1], mv), 0, 1), 300)};
	pixelColor= vec4(kd.rgb * nl[0], kd.a) + vec4(ks.rgb*rv[0], 0);
	pixelColor+= vec4(kd.rgb * nl[1], kd.a) + vec4(ks.rgb*rv[1], 0);
}
