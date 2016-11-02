varying vec3 varyingNormal;
varying vec3 varyingPosition;
uniform vec3 uColor;

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
vec3 diffuseColor = vec3(0.0, 1.0, 0.0);
vec3 specularColor = vec3(0.3, 0.0, 0.0);

lights[0].lightPosition = vec3(0.0, -1.0, 0.0);
lights[1].lightPosition = vec3(1.0, 1.0, 0.0);

lights[0].lightColor = vec3(0.5, 0.0, 0.0);
lights[1].lightColor = vec3(0.0, 0.0, 0.0);

lights[0].specularLightColor = vec3(1.0, 1.0, 0.0);
lights[1].specularLightColor = vec3(0.0, 1.0, 1.0);



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
vec3 intensity = (uColor * diffuseColor) + specularColor;
gl_FragColor = vec4(intensity.xyz, 1.0);
}