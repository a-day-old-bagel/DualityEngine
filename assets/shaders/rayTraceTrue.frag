#define time 1.9

uniform samplerCube cubeTexture;
in vec3 eyeDirection;
in vec2 fragCoord;
out vec4 fragColor;

float sphere(vec3 ray, vec3 dir, vec3 center, float radius)
{
	vec3 rc = ray-center;
	float c = dot(rc, rc) - (radius*radius);
	float b = dot(dir, rc);
	float d = b*b - c;
	float t = -b - sqrt(abs(d));
	float st = step(0.0, min(t,d));
	return mix(-1.0, t, st);
}

vec3 envMap(vec3 rd) {
 	return textureCube(iChannel0, rd).xyz;   
}

vec3 background(float t, vec3 rd)
{
	vec3 toLight = normalize(vec3(sin(t), 0.4, cos(t)));
	float sun = max(0.0, dot(rd, toLight));
	float sky = max(0.0, dot(rd, vec3(0.0, 1.0, 0.0)));
	float ground = max(0.0, -dot(rd, vec3(0.0, 1.0, 0.0)));
    float env = envMap(rd).r;
	return 
		(0.1 * env + pow(sun, 256.0) + max(0.0, (0.4 * pow(sun, 2.0)) *
        (-0.2 + env))) * vec3(2.0, 1.6, 1.0);
}

void mainImage()
{
    //location of fragment
	vec2 pixLoc = (-1.0 + ((2.0 * fragCoord.xy) / iResolution.xy)) * 
		vec2(iResolution.x / iResolution.y, 1.0);
    
	vec3 raySrc = vec3(0.0, 0.0, -2.0);			//camera location
	vec3 rayDir = normalize(vec3(pixLoc, 1.0));	//camera direction
	vec3 sphereCenter = vec3(2.0, -1.0, 0.0);	//sphere location
    vec3 sphereColor = vec3(0.6, 1.0, 0.6);		//sphere color
    
	float dist = sphere(raySrc, rayDir, sphereCenter, 1.0);
	vec3 faceNormal = normalize(-(raySrc - sphereCenter + rayDir * dist));
	vec3 backGrndCol = background(time, rayDir);
    //vec3 backGrndCol = envMap(rayDir);
	rayDir = reflect(rayDir, faceNormal);
	vec3 sphereCol = background(time, rayDir) * sphereColor;
    //vec3 sphereCol = envMap(rayDir);
	fragColor = vec4( mix(backGrndCol, sphereCol, step(0.0, dist)), 1.0 );
}














#define time 1.9

float sphere(vec3 ray, vec3 dir, vec3 center, float radius)
{
	vec3 rc = ray-center;
	float c = dot(rc, rc) - (radius*radius);
	float b = dot(dir, rc);
	float d = b*b - c;
	float t = -b - sqrt(abs(d));
	float st = step(0.0, min(t,d));
	return mix(-1.0, t, st);
}

vec3 envMap(vec3 rd) {
 	return textureCube(iChannel0, rd).xyz;   
}

vec3 background(float t, vec3 rd)
{
	vec3 light = normalize(vec3(sin(t), 0.4, cos(t)));
	float sun = max(0.0, dot(rd, light));
	float sky = max(0.0, dot(rd, vec3(0.0, 1.0, 0.0)));
	float ground = max(0.0, -dot(rd, vec3(0.0, 1.0, 0.0)));
	return 
		(pow(sun, 256.0)+0.2*pow(sun, 2.0))*vec3(2.0, 1.6, 1.0) +
		pow(ground, 0.5)*vec3(0.4, 0.3, 0.2)*envMap(rd) +
		pow(sky, 1.0)*vec3(0.5, 0.6, 0.7);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    //location of fragment
	vec2 pixLoc = (-1.0 + ((2.0 * fragCoord.xy) / iResolution.xy)) * 
		vec2(iResolution.x / iResolution.y, 1.0);
    
	vec3 raySrc = vec3(0.0, 0.0, -2.0);			//camera location
	vec3 rayDir = normalize(vec3(pixLoc, 1.0));	//camera direction
	vec3 sphereCenter = vec3(2.0, -1.0, 0.0);	//sphere location
    vec3 sphereColor = vec3(0.6, 1.0, 0.6);		//sphere color
    
	float dist = sphere(raySrc, rayDir, sphereCenter, 1.0);
	vec3 faceNormal = normalize(-(raySrc - sphereCenter + rayDir * dist));
	vec3 backGrndCol = background(iGlobalTime, rayDir);
    //vec3 backGrndCol = envMap(rayDir);
	rayDir = reflect(rayDir, faceNormal);
	vec3 sphereCol = background(iGlobalTime, rayDir) * sphereColor;
    //vec3 sphereCol = envMap(rayDir);
	fragColor = vec4( mix(backGrndCol, sphereCol, step(0.0, dist)), 1.0 );
}