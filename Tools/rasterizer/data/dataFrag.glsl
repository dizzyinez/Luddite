#ifdef GL_ES
precision mediump double;
precision mediump int;
#endif

flat in vec4 vertColor;
flat in vec3 vertNormal;
varying vec3 vertLightDir;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

float map_n(float value)
{
  return map(value, -1.0, 1.0, 0.0, 1.0);
}

void main() {
  // float intensity;
  // vec4 color;
  // intensity = max(0.0, dot(vertLightDir, vertNormal));
  //
  // if (intensity > pow(0.95, fraction)) {
  //   color = vec4(vec3(1.0), 1.0);
  // } else if (intensity > pow(0.5, fraction)) {
  //   color = vec4(vec3(0.6), 1.0);
  // } else if (intensity > pow(0.25, fraction)) {
  //   color = vec4(vec3(0.4), 1.0);
  // } else {
  //   color = vec4(vec3(0.2), 1.0);
  // }
// gl_FragColor = vertColor;
float a = vertColor.r;
// if (vertColor.r > 1)
// {
//   a = 1.0;
// }
// a = 0.3;
gl_FragColor = vec4(map_n(vertNormal.r), map_n(vertNormal.b), map(vertNormal.g, -1.0, 1.0, 1.0, 0.0), a);
}
