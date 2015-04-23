precision highp float;
uniform float time;
uniform vec2 resolution;
varying vec3 fPosition;
varying vec3 fNormal;

vec3 rim(vec3 color, float start, float end, float coef)
{
  vec3 normal = normalize(fNormal);
  vec3 eye = normalize(-fPosition.xyz);
  float rim = smoothstep(start, end, 1.0 - dot(normal, eye));
  return clamp(rim, 0.0, 1.0) * coef * color;
}

void main()
{
  gl_FragColor = vec4(rim(vec3(0.299, 0.587, 0.114), 0.0, 3.0, 5.0),1.0);
}
