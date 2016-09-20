
uniform sampler2D diffuse;
varying vec2 texCoordVar;
uniform sampler2D texture;
uniform float time;

void main() {
	vec2 texCoord = vec2(texCoordVar.x + time, texCoordVar.y);
    gl_FragColor = texture2D(texture, texCoord);
}