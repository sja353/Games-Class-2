uniform sampler2D diffuse;

varying vec4 vertexColor;
varying vec2 varTexCoord;
void main() {
// When untextured, causes colors to be muted. Possible solution: For untextured, simply pass in white square texture?
  // vec4 textureColor = texture2D(diffuse, varTexCoord);
  // gl_FragColor = textureColor*vertexColor;
  // gl_FragColor.a = textureColor.a;
	gl_FragColor = vertexColor;
}