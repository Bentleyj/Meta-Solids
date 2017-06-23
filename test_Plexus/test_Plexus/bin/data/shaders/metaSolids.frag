#version 120

// Big Time Credit ro Kamil Kolaczynski [ https://www.shadertoy.com/view/Md3SDB ]

#define RADIANS(x) ((x) * (PI / 180.0))

#define TETRAHEDRON_DIHEDRAL_ANGLE RADIANS(70.53)
#define HEXAHEDRON_DIHEDRAL_ANGLE RADIANS(90.0)
#define OCTAHEDRON_DIHEDRAL_ANGLE RADIANS(109.47)
#define DODECAHEDRON_DIHEDRAL_ANGLE RADIANS(116.57)
#define ICOSAHEDRON_DIHEDRAL_ANGLE RADIANS(138.19)

#define TETRAHEDRON_SCHLAFLI_SYMBOL vec2(3.0, 3.0)
#define HEXAHEDRON_SCHLAFLI_SYMBOL vec2(4.0, 3.0)
#define OCTAHEDRON_SCHLAFLI_SYMBOL vec2(3.0, 4.0)
#define DODECAHEDRON_SCHLAFLI_SYMBOL vec2(5.0, 3.0)
#define ICOSAHEDRON_SCHLAFLI_SYMBOL vec2(3.0, 5.0)

// Note this noise function explodes after some time

uniform vec2 u_Resolution;
uniform float u_Radius;
uniform float u_CameraDistance;
uniform float u_Noise;
uniform float u_Time;
uniform vec2 u_Mouse;
uniform vec4 u_BallPositions[100];

const float MAX_TRACE_DISTANCE = 10.0;
const float INTERSECTION_PRECISION = 0.01;
const int NUM_OF_TRACE_STEPS = 50;

const float PI = 3.141592;

float cot(float x) {
	return 1.0 / tan(x);
}

vec3 rotx(vec3 p, float a) {
	float s = sin(a), c = cos(a);
	return vec3(p.x, c * p.y - s * p.z, s * p.y + c * p.z);
}
vec3 roty(vec3 p, float a) {
	float s = sin(a), c = cos(a);
	return vec3(c * p.x + s * p.z, p.y, -s * p.x + c * p.z);
}
vec3 rotz(vec3 p, float a) {
	float s = sin(a), c = cos(a);
	return vec3(c * p.x - s * p.y, s * p.x + c * p.y, p.z);
}

float smin(float a, float b, float k)
{
	float res = exp(-k*a) + exp(-k*b);
	return -log(res) / k;
}

float getInradius(vec2 pq, float diha, float radius) {
	float tn = tan(diha * 0.5);
	float a = 2.0 * radius / (tan(PI / pq.y) * tn);
	float r = 0.5 * a * cot(PI / pq.x) * tn;

	return r;
}

float sphere(vec3 p, float radius)
{
	return length(p) - radius;
}

float tetrahedron(vec3 p, float radius) {
	float diha = -RADIANS(180.0 - 70.53); // 180 - "Dihedral angle"
	float tria = -RADIANS(60.0); // triangle angle
	float inra = getInradius(TETRAHEDRON_SCHLAFLI_SYMBOL, TETRAHEDRON_DIHEDRAL_ANGLE, radius);

	float d = p.x - inra;

	p = rotz(p, diha);
	d = max(d, p.x - inra);

	p = rotx(p, tria);
	p = rotz(p, diha);

	d = max(d, p.x - inra);

	p = rotx(p, -tria);
	p = rotz(p, diha);
	d = max(d, p.x - inra);

	return d;
}

float hexahedron(vec3 p, float radius) {
	float inra = getInradius(HEXAHEDRON_SCHLAFLI_SYMBOL, HEXAHEDRON_DIHEDRAL_ANGLE, radius);

	float d = abs(p.x) - inra;

	p = rotz(p, 1.5708); // 90 degrees
	d = max(d, abs(p.x) - inra);

	p = roty(p, 1.5708); // 90 degrees
	d = max(d, abs(p.x) - inra);

	return d;
}

float octahedron(vec3 p, float radius) {
	float d = -1e5;

	float inra = getInradius(OCTAHEDRON_SCHLAFLI_SYMBOL, OCTAHEDRON_DIHEDRAL_ANGLE, radius);

	for (float i = 0.0; i < 4.0; i++) {
		p = rotz(p, 1.231); // 70.53110 degrees
		p = rotx(p, 1.047); // 60 degrees

		d = max(d, max(p.x - inra, -p.x - inra));
	}
	return d;
}

float dodecahedron(vec3 p, float radius) {
	float d = -1e5;

	float inra = getInradius(DODECAHEDRON_SCHLAFLI_SYMBOL, DODECAHEDRON_DIHEDRAL_ANGLE, radius);

	for (float i = 0.0; i <= 4.0; i++) {
		p = roty(p, 0.81); // 46.40958 degrees
		p = rotx(p, 0.759); // 43.48750 degrees
		p = rotz(p, 0.3915); // 22.43130 degrees

		d = max(d, max(p.x - inra, -p.x - inra));
	}

	p = roty(p, 0.577); // 33.05966 degrees
	p = rotx(p, -0.266); // -15.24068 degrees
	p = rotz(p, -0.848); // -48.58682 degrees

	d = max(d, max(p.x - inra, -p.x - inra));

	return d;
}

float icosahedron(vec3 p, float radius) {
	float d = -1e5;

	//center band
	const float n1 = 0.7297; // 41.80873 degrees
	const float n2 = 1.0472; // 60 degrees

	float inra = getInradius(ICOSAHEDRON_SCHLAFLI_SYMBOL, ICOSAHEDRON_DIHEDRAL_ANGLE, radius);

	for (float i = 0.0; i < 5.0; i++) {

		if (mod(i, 2.0) == 0.0) {
			p = rotz(p, n1);
			p = rotx(p, n2);
		}
		else {
			p = rotz(p, n1);
			p = rotx(p, -n2);
		}
		d = max(d, max(p.x - inra, -p.x - inra));
	}

	p = roty(p, 1.048); // 60.04598 degrees
	p = rotz(p, 0.8416); // 48.22013 degrees
	p = rotx(p, 0.7772); // 44.53028 degrees

						 //top caps
	for (float i = 0.0; i < 5.0; i++) {
		p = rotz(p, n1);
		p = rotx(p, n2);

		d = max(d, max(p.x - inra, -p.x - inra));
	}
	return d;
}

vec3 rayPlaneIntersection(vec3 ro, vec3 rd, vec4 plane)
{
	float t = -(dot(ro, plane.xyz) + plane.w) / dot(rd, plane.xyz);
	return ro + t * rd;
}

mat3 calcLookAtMatrix(in vec3 camPos, in vec3 camTar, in float roll)
{
	vec3 ww = normalize(camTar - camPos);
	vec3 uu = normalize(cross(ww, vec3(sin(roll), cos(roll), 0.0)));
	vec3 vv = normalize(cross(uu, ww));
	return mat3(uu, vv, ww);
}

void doCamera(out vec3 camPos, out vec3 camTar, in float time, in vec2 mouse)
{
	float radius = u_CameraDistance;
	float theta = u_Time * 0.5;
	float phi = PI * 0.4;

	float pos_x = radius * cos(theta) * sin(phi);
	float pos_z = radius * sin(theta) * sin(phi);
	float pos_y = radius * cos(phi);

	camPos = vec3(pos_x, pos_y, pos_z);
	camTar = vec3(0.0, 0.0, 0.0);
}

//--------------------------------
// Modelling 
//--------------------------------
vec2 map(vec3 pos) {

	float k = 5.0;
	float b = 0.5;
	float br = 0.6;
	float a = 4.0;
	float rad = 0.7;

	//float t1 = hexahedron(pos + vec3(-4.0, 0.0, 0.0), u_Radius); //+ u_Noise * noise(pos * 1.0 + u_Time*0.75);;
	//t1 = smin(t1, icosahedron(pos + vec3(-2.0, 0.0, 0.0), u_Radius), k);
	float t1 = sphere(pos + vec3(0.0, 0.0, 0.0), u_Noise);

	t1 = smin(t1, sphere(pos + a * vec3(sin(u_Time * b), 0.0, 0.0), rad * 1.2 * abs(sin(u_Time * br))), k);
	t1 = smin(t1, sphere(pos + a * vec3(0.0, sin(u_Time * b), 0.0), rad * 1.2 * abs(sin(u_Time * br))), k);
	t1 = smin(t1, sphere(pos + a * vec3(0.0, 0.0, sin(u_Time * b)), rad * 1.2 * abs(sin(u_Time * br))), k);
	t1 = smin(t1, sphere(pos + a * vec3(-sin(u_Time * b), 0.0, 0.0), rad * 1.2 * abs(sin(u_Time * br))), k);
	t1 = smin(t1, sphere(pos + a * vec3(0.0, -sin(u_Time * b), 0.0), rad * 1.2 * abs(sin(u_Time * br))), k);
	t1 = smin(t1, sphere(pos + a * vec3(0.0, 0.0, -sin(u_Time * b)), rad * 1.2 * abs(sin(u_Time * br))), k);

	b = 1.0;
	br = 3.0;

	//for (int i = 0; i < 40; i++) {
	t1 = smin(t1, sphere(pos + a * vec3( sin(u_Time * b), 0.0, 0.0), rad), k);
	t1 = smin(t1, tetrahedron(pos + a * vec3(0.0, sin(u_Time * b), 0.0), rad), k);
	t1 = smin(t1, hexahedron(pos + a * vec3(0.0, 0.0, sin(u_Time * b)), rad), k);
	t1 = smin(t1, octahedron(pos + a * vec3(-sin(u_Time * b), 0.0, 0.0), rad), k);
	t1 = smin(t1, dodecahedron(pos + a * vec3(0.0, -sin(u_Time * b), 0.0), rad), k);
	t1 = smin(t1, icosahedron(pos + a * vec3(0.0, 0.0, -sin(u_Time * b)), rad), k);

	//}

	return vec2(t1, 1.0);
}

float shadow(in vec3 ro, in vec3 rd)
{
	const float k = 2.0;

	const int maxSteps = 10;
	float t = 0.0;
	float res = 1.0;

	for (int i = 0; i < maxSteps; ++i) {

		float d = map(ro + rd*t).x;

		if (d < INTERSECTION_PRECISION) {

			return 0.0;
		}

		res = min(res, k*d / t);
		t += d;
	}

	return res;
}

vec3 calcNormal(in vec3 pos) {
	vec3 eps = vec3(0.001, 0.0, 0.0);
	vec3 nor = vec3(
		map(pos + eps.xyy).x - map(pos - eps.xyy).x,
		map(pos + eps.yxy).x - map(pos - eps.yxy).x,
		map(pos + eps.yyx).x - map(pos - eps.yyx).x);
	return normalize(nor);
}

void renderColor(vec3 camPos, vec3 viewRay, inout vec3 color, vec3 currPos)
{
	vec3 originalColor = color;
	vec3 lightDir = normalize(vec3(1.0, 0.4, 0.0));
	vec3 normal = calcNormal(currPos);

	float normalDotLight = abs(dot(-viewRay, normal));
	float rim = pow(1.0 - normalDotLight, 3.0);
	color = mix(refract(normal, viewRay, 0.5) * 0.5 + vec3(0.5), color, rim);

	color += rim;


	//color = vec3(1.0, 0.0, 0.0);
}


bool renderRayMarch(vec3 camPos, vec3 viewRay, vec3 viewRayMouse, inout vec3 color)
{
	float t = 0.0;
	float d = 0.0;
	for (int i = 0; i < NUM_OF_TRACE_STEPS; ++i)
	{
		vec3 currPos = camPos + viewRay * t;
		d = map(currPos).x;
		if (d < INTERSECTION_PRECISION)
		{
			break;
		}

		if (d > 10.0) {
			break;
		}

		t += d;
	}

	if (d < INTERSECTION_PRECISION)
	{
		vec3 currPos = camPos + viewRay * t;
		renderColor(camPos, viewRay, color, currPos);
		return true;
	}

	vec3 planePoint = rayPlaneIntersection(camPos, viewRay, vec4(0.0, 0.0, 0.0, 1.0));
	float shadowFloor = shadow(planePoint, vec3(0.0, 1.0, 0.0));
	color = color * mix(0.8, 1.0, shadowFloor);

	return false;
}


void main() {
	vec2 p = (-u_Resolution.xy + 2.0*gl_FragCoord.xy) / u_Resolution.y;
	vec2 m = (-u_Resolution.xy + 2.0*u_Mouse.xy) / u_Resolution.y;

	float dist = distance(p, m);
	//vec2 m = u_Mouse.xy / u_Resolution.xy;

	// camera movement
	vec3 camPos, camTar;
	doCamera(camPos, camTar, 1.0, u_Mouse);

	// camera matrix
	mat3 camMat = calcLookAtMatrix(camPos, camTar, 0.0);  // 0.0 is the camera camPosll
												  // create view ray
	vec3 viewRay = normalize(camMat * vec3(p.xy, 2.0)); // 2.0 is the lens length

	vec3 viewRayMouse = normalize(camMat * vec3(m.xy, 2.0));
												   // calc color
	vec3 col = vec3(0.9);
	renderRayMarch(camPos, viewRay, viewRayMouse, col);

	// vignette, OF COURSE
	float vignette = 1.0 - smoothstep(1.0, 2.5, length(p));
	col.xyz *= mix(0.7, 1.0, vignette);
	gl_FragColor = vec4(col, 1.);
}