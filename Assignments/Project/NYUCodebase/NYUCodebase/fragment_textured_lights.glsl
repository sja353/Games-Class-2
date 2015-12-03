
uniform sampler2D diffuse;

uniform vec2 lightPositions[32];
uniform vec3 lightColors[32];

varying vec2 texCoordVar;
varying vec2 varPosition;

float attenuate(float dist, float a, float b){
	return (1.0/(1.0 + (a*dist) + (b * dist * dist)));
}
void main() {
	vec3 brightness = (0.2, 0.5, 0.0);
	for (int i = 0; i < 32; i++){
		brightness+= attenuate(distance(lightPositions[i], varPosition), 5.0, 8.0) * lightColors[i];
	}
	vec4 textureColor = texture2D (diffuse, texCoordVar);
    gl_FragColor.xyz = textureColor.xyz * brightness;
	gl_FragColor.a = textureColor.a;
}