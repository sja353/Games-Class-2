//PROBLEMS WITH LIGHTING SOLVED
uniform sampler2D diffuse;

uniform vec2 lightPositions[32];
uniform vec3 lightColors[32];
uniform vec2 lightAB[32];
uniform vec3 brightness;

varying vec2 texCoordVar;
varying vec2 varPosition;

float attenuate(float dist, float a, float b){
	return (1.0/(1.0 + (a*dist) + (b * dist * dist)));
}
void main() {
	vec3 bright =brightness;
	for (int i = 0; i < 32; i++){
		bright += attenuate(distance(lightPositions[i], varPosition), lightAB[i].x, lightAB[i].y) * lightColors[i];
	}
	vec4 textureColor = texture2D (diffuse, texCoordVar);
    gl_FragColor.xyz = textureColor.xyz * bright;
	gl_FragColor.a = textureColor.a;
}