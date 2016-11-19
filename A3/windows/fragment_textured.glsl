varying vec3 varyingNormal;
varying vec3 varyingPosition;
uniform vec3 uColor;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;


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
gl_FragColor = vec4(intensity.xyz, 1.0);



}