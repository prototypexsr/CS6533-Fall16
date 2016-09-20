
uniform sampler2D diffuse;
varying vec2 texCoordVar;
uniform sampler2D texture;

void main() {
    gl_FragColor = texture2D(texture, texCoordVar);
}