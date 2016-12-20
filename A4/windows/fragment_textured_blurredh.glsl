varying vec3 varyingNormal;
varying vec3 varyingPosition;
uniform vec3 uColor;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;
uniform sampler2D screenFramebuffer;
uniform sampler2D lookupTable;


varying mat3 varyingTBNMatrix;
varying vec2 varyingTexCoord;
struct Light {
vec3 lightPosition;
vec3 lightColor;
vec3 specularLightColor;
};

uniform Light lights[2];
float attenuate(float dist, float a, float b) {
return 1.0 / (1.0 + a*dist + b*dist*dist);
}
uniform float exposure;
vec4 lookup(vec4 textureColor, sampler2D lookupTable) {
float blueColor = textureColor.b * 63.0;
vec2 quad1;
quad1.y = floor(floor(blueColor) / 8.0);
quad1.x = floor(blueColor) - (quad1.y * 8.0);
vec2 quad2;
quad2.y = floor(ceil(blueColor) / 8.0);
quad2.x = ceil(blueColor) - (quad2.y * 8.0);
vec2 texPos1;
texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);
vec2 texPos2;
texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);
vec4 newColor1 = texture2D(lookupTable, texPos1);
vec4 newColor2 = texture2D(lookupTable, texPos2);
vec4 newColor = mix(newColor1, newColor2, fract(blueColor));
return newColor;
}

void main() {
vec3 diffuseColor = vec3(0.5, 0.5, 0.5);
vec3 specularColor = vec3(0.025, 0.025, 0.025);
vec3 normalColor = vec3(0.025, 0.025, 0.025);

lights[0].lightPosition = vec3(-6.0, 4.0, -10.0);
lights[1].lightPosition = vec3(6.0, 4.0, -10.0);

lights[0].lightColor = vec3(0.0, 0.0, 1.0);
lights[1].lightColor = vec3(0.0, 1.0, 0.0);

lights[0].specularLightColor = vec3(0.0, 1.0, 0.0);
lights[1].specularLightColor = vec3(1.0, 0.0, 0.0);

vec3 textureNormal = normalize((texture2D(normalTexture, varyingTexCoord).xyz * 2.0) -1.0);
textureNormal = normalize(varyingTBNMatrix * textureNormal);

for(int i=0; i< 2; i++) {
vec3 lightDirection = -normalize(varyingPosition-lights[i].lightPosition);
float diffuse = max(0.0, dot(varyingNormal, lightDirection));

float attenuation = attenuate(distance(varyingPosition, lights[i].lightPosition) / 5.0, 2.7, 5.0);
diffuseColor += (lights[i].lightColor * diffuse) * attenuation;


vec3 v = normalize(-varyingPosition);
vec3 h = normalize(v + lightDirection);

float specular = pow(max(0.0, dot(h, varyingNormal)), 64.0);
specularColor += lights[i].specularLightColor * specular * attenuation;
}




vec3 intensity = (texture2D(diffuseTexture, varyingTexCoord).xyz * diffuseColor)
+ (specularColor * texture2D(specularTexture, varyingTexCoord).x) + (normalColor * texture2D(normalTexture, varyingTexCoord).x);

const float blurSize = 0.004;
vec4 sum = vec4(0.0);
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 16.0*blurSize, varyingTexCoord.y)) * 0.000004;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 15.0*blurSize, varyingTexCoord.y)) * 0.000012;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 14.0*blurSize, varyingTexCoord.y)) * 0.00004;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 13.0*blurSize, varyingTexCoord.y)) * 0.00012;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 12.0*blurSize, varyingTexCoord.y)) * 0.000331;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 11.0*blurSize, varyingTexCoord.y)) * 0.000841;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 10.0*blurSize, varyingTexCoord.y)) * 0.001971;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 9.0*blurSize, varyingTexCoord.y)) * 0.004258;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 8.0*blurSize, varyingTexCoord.y)) * 0.008483;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 7.0*blurSize, varyingTexCoord.y)) * 0.015583;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 6.0*blurSize, varyingTexCoord.y)) * 0.026396;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 5.0*blurSize, varyingTexCoord.y)) * 0.04123;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 4.0*blurSize, varyingTexCoord.y)) * 0.059384;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 3.0*blurSize, varyingTexCoord.y)) * 0.07887;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 2.0*blurSize, varyingTexCoord.y)) * 0.096593;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 1.0*blurSize, varyingTexCoord.y)) * 0.109084;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x, varyingTexCoord.y)) * 0.113597;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 16.0*blurSize, varyingTexCoord.y)) * 0.000004;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 15.0*blurSize, varyingTexCoord.y)) * 0.000012;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 14.0*blurSize, varyingTexCoord.y)) * 0.00004;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 13.0*blurSize, varyingTexCoord.y)) * 0.00012;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 12.0*blurSize, varyingTexCoord.y)) * 0.000331;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 11.0*blurSize, varyingTexCoord.y)) * 0.000841;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 10.0*blurSize, varyingTexCoord.y)) * 0.001971;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 9.0*blurSize, varyingTexCoord.y)) * 0.004258;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 8.0*blurSize, varyingTexCoord.y)) * 0.008483;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 7.0*blurSize, varyingTexCoord.y)) * 0.015583;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 6.0*blurSize, varyingTexCoord.y)) * 0.026396;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 5.0*blurSize, varyingTexCoord.y)) * 0.04123;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 4.0*blurSize, varyingTexCoord.y)) * 0.059384;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 3.0*blurSize, varyingTexCoord.y)) * 0.07887;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 2.0*blurSize, varyingTexCoord.y)) * 0.096593;
sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 1.0*blurSize, varyingTexCoord.y)) * 0.109084;

gl_FragColor = vec4(intensity.xyz, 1.0) + sum ;








}