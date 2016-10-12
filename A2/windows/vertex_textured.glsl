attribute vec4 position;
attribute vec4 color;
attribute vec4 normal;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

varying vec4 varyingColor;
varying vec4 varyingNormal;

void main() {
varyingColor = color;
varyingNormal = normalMatrix * normal;
gl_Position = projectionMatrix * modelViewMatrix * position;
}