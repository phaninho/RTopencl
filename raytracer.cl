#define PI 3.14159265359f
#define EPSILON 0.00001f
#define SCENE 0
#define CAMERA 1
#define SPHERE 2
#define PLANE 3
#define CYLINDER 4
#define CONE 5
#define DISK 6
#define CYLINDERINF 7
#define CONEINF 8
#define TRIANGLE 9
//#define PARABOLOID 10 // variable : pos, normal, dist (distance entre les 2 points)
//#define ELLIPSOID 11 // variable : pos, dist (distance entre les 2 points), normal, radius
//#define TORUS 12 // variable : pos, normal, Grand radius et petit radius.
//#define SOR 13 //<- ne pas ajourter // pos, normal, taille(dist), coeficient a b c d.
#define SPOTLIGHT 10
#define POINTLIGHT 11
#define DIRLIGHT 12
#define MATERIAL 13
#define TEXTURE 14
#define RENDER 15
// Pour le parser et le CL du mod de rendu
#define RENDERMODE_SEPIA 16
#define RENDERMODE_GRIS 17
#define RENDERMODE_FILTER 18
#define RENDERMODE_ADD 19

typedef struct	s_texture
{
	int			width;
	int			height;
	char		*data;
}				t_texture;

typedef struct	s_scene
{
	int			width;
	int			height;
	float		znear;
	float		zfar;
	float		fov;
	float		focale;
	int			max_reflect;
	int			max_refract;
	int			max_object;
	int			max_material;
	int			max_light;
	int			max_texture;
	int			render_debug;
	int			render_mod;
	float4		render_filter;
}				t_scene;

typedef struct	s_cam
{
	float3		position;
	float3		rotation;
}				t_cam;

typedef struct	s_objects
{
	int			type;
	float3		position;
	float3		rotation;
	float3		normal;
	float4		color;
	float		radius;
	float		radius2; // petit radius pour torus
	//float		a;
	//float		b;
	//float		d;
	float 		dist; // distance des point pour parabol, epsilloid
	int			material_id;
	int			texture_id;
	int			in_object;
}				t_objects;

typedef struct	s_light
{
	int			type;
	float3		position;
	float4		color;
	float		attenuation;
	float3		direction;
	float		angle;
}				t_light;

typedef struct	s_material
{
	float4		ambient_color;
	float4		specular_color;
	int			blinn;
	float		shininess;
	float		reflection;
	float		refraction;
}				t_material;

typedef struct	s_ray
{
	float3	pos;
	float3	dir;
	float	focale;
	float	deph;
	int		object;
}				t_ray;

static float soft_length(float3 vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

static float3 soft_normalize(float3 vec)
{
	float l = soft_length(vec);
	if (l == 0.0f)
		return (vec);
	return (vec / l);
}

static float soft_dot(float3 vec_a, float3 vec_b)
{
	return (vec_a.x * vec_b.x + vec_a.y * vec_b.y + vec_a.z * vec_b.z);
}

static float3 rotatex(float3 vec, float degree)
{
	float3 nvec = (float3)(0, 0, 0);
	float rx = degree * PI / 180.0f;
	nvec.x = vec.x;
	nvec.y = vec.y * cosf(rx) - vec.z * sinf(rx);
	nvec.z = vec.y * sinf(rx) + vec.z * cosf(rx);
	return (nvec);
}

static float3 rotatey(float3 vec, float degree)
{
	float3 nvec = (float3)(0, 0, 0);
	float ry = degree * PI / 180.0f;
	nvec.x = vec.z * sinf(ry) + vec.x * cosf(ry);
	nvec.y = vec.y;
	nvec.z = vec.z * cosf(ry) - vec.x * sinf(ry);
	return (nvec);
}

static float3 rotatez(float3 vec, float degree)
{
	float3 nvec = (float3)(0, 0, 0);
	float rz = degree * PI / 180.0f;
	nvec.x = vec.x * cosf(rz) - vec.y * sinf(rz);
	nvec.y = vec.x * sinf(rz) + vec.y * cosf(rz);
	nvec.z = vec.z;
	return (nvec);
}

static float3 rotatexyz(float3 vec, float3 d)
{
	float3 nvec = rotatez(vec, d.z);
	nvec = rotatex(nvec, d.x);
	nvec = rotatey(nvec, d.y);
	return (nvec);
}

static float deph_min(float t0, float t1)
{
	if (t0 > 0)
	{
		if (t1 > 0)
		{
			if (t1 < t0)
				return (t1);
		}
		return (t0);
	}
	if (t1 > 0)
	{
		return (t1);
	}
	return (FLT_MAX);
}

static float3		float3_reflect(const float3 v, const float3 normal)
{
	return (v - (normal * 2.0f * soft_dot(v, normal)));
}

static float3		float3_refract(const float3 v, const float3 normal, const float eta)
{
	float air = 1.33f;
	float n = eta / air;
	float cosI = -(soft_dot(normal, v));
	float sinT2 = eta * eta * (1.0f - cosI * cosI);
	if (sinT2 > 1.0)
		return ((float3)(0, 0, 0));
	float cosT = sqrt(1.0 - sinT2);
	return (v * n + normal * (n * cosI - cosT));
	/*float n = -(soft_dot(normal, v));
	float k = 1.f - eta * eta * (1.f - n * n);
	if (k < 0.0f)
		return ((float3)(0, 0, 0));
	return (eta * v + (eta * n - sqrtf(k)) * normal);*/

}

static float3 get_normal(t_ray *ray, const t_objects objects)
{
	float3 impact = ray->pos + ray->dir * ray->deph;

	if (objects.type == SPHERE)
	{
		return (soft_normalize(impact - objects.position));
	}
	else if (objects.type == PLANE)
	{
		return (soft_normalize(objects.normal));
	}
	else if (objects.type == DISK)
	{
		return (soft_normalize(objects.normal));
	}
	else if (objects.type == CYLINDER || objects.type == CYLINDERINF)
	{
		float3 nor = impact - objects.position;
		nor.y = 0;
		return (soft_normalize(nor));
	}
	else if (objects.type == CONE || objects.type == CONEINF)
	{
		float3 nor = impact - objects.position;
		nor.y = -0.01f * nor.y;
		return (soft_normalize(nor));
	}
	else if (objects.type == TORUS)
	{
		float k = soft_dot((impact - objects.position), objects.normal);
		float3 a = impact - objects.position * k;
		float m = sqrt(pow(objects.radius2, 2) - k * k );
		return (soft_normalize(impact - a - (objects.position - a) *\
		 m / (objects.radius + m)));
	}
	else if (objects.type == PARABOLOID)
		return (soft_normalize(impact - objects.position - objects.normal * \
			(soft_dot((impact-objects.position), objects.normal) + objects.dist)));
	else if (objects.type == ELLIPSOID)
	{
		float A1  = 2 * objects.depth * soft_dot(ray->dir, objects.normal);
   		float A2  = pow(objects.radius, 2) + 2 * objects.dist * soft_dot(dist, objects.normal) - objects.dist;
   		a = 4 * pow(objects.radius, 2) * soft_dot(ray->dir, ray->dir) - A1 * A1;
   		b = 2 * (4 * pow(objects.radius, 2) * soft_dot(ray->dir, dist) - A1 * A2);
   		float3 Cmid = objects.position + objects.normal * objects.dist / 2;
  		float3 R = impact - Cmid;
  		return (soft_normalize( R - objects.normal * (1 - (b * b) / (a * a))* soft_dot(R, objects.normal));
   	}
	// else if (objects.type == SOR)
	// {
	// 	float m = soft_dot(ray->dir, objects.normal) * ray->deph +\
	// 	 soft_dot((impact - objects.position),objects.normal);
	// 	float dval = 3 * objects.a * pow(m,2) + 2 * objects.b * m + objects.c;
	// 	float3 r = impact - objects.pos - objects.normal * m;
	// 	return (soft_normalize( r - V * sqrt(soft_dot(r,r))/dval ));
	// }
	return ((float3)(0, 0, 0));
}

static float2 getTextureUV(t_ray *ray, const t_objects objects)
{
	float2 uv = (float2)(1, 1);
	float3 impact = ray->pos + ray->dir * ray->deph;
	float3 normal = get_normal(ray, objects);
	if (objects.type == SPHERE)
	{
		uv.x = 0.5f + (atan2(normal.z, normal.x)) / (2.0f * PI);
		uv.y = 0.5f - (asin(normal.y)) / PI;
	}
	else if (objects.type == PLANE)
	{
		float3 uAxis = (float3)(normal.y, normal.z, -normal.x);
		float3 vAxis = cross(uAxis, normal);
		uv.x = soft_dot(impact, uAxis);
		uv.y = soft_dot(impact, vAxis);
		uv.x = uv.x - floor(uv.x);
		uv.y = uv.y - floor(uv.y);
	}
	else if (objects.type == CYLINDER)
	{
		/*uv.x = 0.5f + (atan2(normal.z, normal.x)) / (2.0f * PI);
		uv.y = impact.y - objects.position.y;
		uv.x = uv.x * sqrt(objects.radius);
		uv.y = uv.y / objects.radius;
		uv.x = uv.x - floor(uv.x);
		uv.y = uv.y - floor(uv.y);*/
		uv.x = 0.5f + (atan2(normal.z, normal.x)) / (2.0f * PI);
		uv.y = 0.5f - (asin(normal.y)) / PI;
	}
	else if (objects.type == CONE)
	{
		uv.x = 0.5f + (atan2(normal.z, normal.x)) / (2.0f * PI);
		uv.y = 0.5f - (asin(normal.y)) / PI;
	}
	return (uv);
}

static float4	light_ambient(const t_objects obj, const t_light light, const t_material material)
{
	float4	color_ambient = obj.color;
	//if (light)
		color_ambient *= light.color;
	//if (material)
		color_ambient *= material.ambient_color;
	return (color_ambient);
}

static float4 light(t_ray *ray, const t_objects objects, const t_light light, __constant t_material *material)
{
	float3	impact = ray->pos + ray->dir * ray->deph;
	float3	lightDir;
	float	distanceToLight;
	float	attenuation = 1.0f;
	float3	impactDir = soft_normalize(ray->pos - impact);
	float3	normal = get_normal(ray, objects);
	float4	finalColor = objects.color;


	if (light.type == SPOTLIGHT)
	{
		lightDir = soft_normalize(light.position - impact);
		float3 coneDirection = soft_normalize(-light.direction);
		float3 raydirection = lightDir;
		float lightToSurfaceAngle = soft_dot(coneDirection, raydirection);
		lightToSurfaceAngle = degrees(acos(lightToSurfaceAngle));
		if(lightToSurfaceAngle > light.angle / 2)
    		attenuation = light.attenuation;
	}
	else if (light.type == POINTLIGHT)
	{
		lightDir = soft_normalize(light.position - impact);
		//attenuation
		distanceToLight = soft_length(light.position - impact);
		attenuation = 1.0f / (1.0f + light.attenuation * pow(distanceToLight, 2.0f));
	}
	else if (light.type == DIRLIGHT)
	{
		lightDir = soft_normalize(light.direction);
		attenuation = 1.0f;
	}
	//ambient
	float4	ambient = 0.0f;
	if (objects.material_id > 0)
		ambient = light_ambient(objects, light, material[objects.material_id - 1]);
	//diffuse
	float 	diffuse_coeff = max(0.0f, soft_dot(normal, lightDir));
	diffuse_coeff = clamp(diffuse_coeff, 0.0f, 1.0f);
	float4	diffuse = diffuse_coeff * objects.color * light.color;
	//specular
	float	specular_coeff = 0.0f;
	if (diffuse_coeff > 0.0f)
	{
		if (objects.material_id > 0)
			specular_coeff = pow(max(0.0f, soft_dot(impactDir, float3_reflect(-lightDir, normal))), material[objects.material_id - 1].shininess);
	//	else
	//		specular_coeff = pow(max(0.0f, soft_dot(impactDir, float3_reflect(-lightDir, normal))), 64.0f);
		specular_coeff = clamp(specular_coeff, 0.0f, 1.0f);
	}
	/*float4	specular = (float4)(0, 0, 0, 0);
	if (objects.material_id > 0)
		specular = specular_coeff * material[objects.material_id - 1].specular_color * light.color;
	//else
	//	specular = light.color;
	else
		specular = specular_coeff * light.color;*/
	/*float	specular_coeff = 0.0f;
	if (diffuse_coeff > 0.0f)
	{
		if (material->blinn)
		{
			float3 light_calc = lightDir + ray->dir;
			soft_normalize(light_calc);
			specular_coeff = dot(normal, light_calc);
		}
		else
			specular_coeff = dot(ray->dir, float3_reflect(-lightDir, normal));
	}
	float4	specular;
	if (objects.material_id > 0)
	{
		specular_coeff = clamp(pow(max(0.0f, specular_coeff), material[objects.material_id - 1].shininess), 0.0f, 1.0f);
		specular = specular_coeff * material[objects.material_id - 1].specular_color * light.color;
	}
	else
	{
		specular_coeff = clamp(pow(max(0.0f, specular_coeff), 64.0f), 0.0f, 1.0f);
		specular = specular_coeff * light.color;
	}*/

	//
	//linear color (color before gamma correction)
	//
	float4	linearColor = (/*specular*/ + ambient + attenuation) * (diffuse  /*+specular*/);
	//final color (after gamma correction)
	//float4	gamma = 2.2f;
	//finalColor = pow(linearColor, gamma);
	finalColor = clamp(linearColor, 0.0f, 1.0f);
	finalColor.w = 1.0f;
	return (finalColor);
}

static float4 noLight(t_ray *ray, const t_objects objects, __constant t_material *material, const int max_material)
{
	float3 normal = get_normal(ray, objects);
	float4 objColor = objects.color;
	float coef = clamp(soft_dot(normal, -ray->dir), 0.0f, 1.0f);
	if (objects.material_id > 0)
	{
		objColor *= material[objects.material_id - 1].ambient_color;
	}
	objColor *= (float4)(coef, coef, coef, 1.0f);
	return (objColor);
}

float		solvequadratic(float a, float b, float c)
{
	float		discriminant;
	float		t;
	float		t0;
	float		t1;

	t = 0;
	if ((discriminant = b * b - 4 * a * c) < 0)
		return (0);
	else if (discriminant == 0)
		t = -0.5 * b / a;
	else if (discriminant >= 0)
	{
		discriminant = sqrt(discriminant);
		t0 = ((-b + discriminant) / (2 * a));
		t1 = ((-b - discriminant) / (2 * a));
		t = (t0 < t1) ? t0 : t1;
	}
	return (t);
}

float   findclosest(float *roots)
{
	int 	i = 0;
	float 	t;

	t = roots[i];
	while (roots[i])
	{
		if (t > roots[i])
			t = roots[i];
		++i;
	}
	return (t);
}

float 	solvecubic(float a, float b, float c, float d)
{
	float roots[3];
    int i = 0;

    if (a == 0)
        return (solvequadratic(b, c, d));
    b /= a;
    c /= a;
    d /= a;

    float S = b / 3.0;
    float D = c / 3.0 - S * S;
    float E = S * S * S + (d - S * c) / 2.0;
    float Froot = sqrt(E * E + D * D * D);

    float F = -Froot - E;

    if (F == 0) 
        F = Froot - E;
    while (i < 3) 
    {
        const float G = pow(F, ((1 / 3) * i));
        roots[i] = pow(F, ((1 / 3) * i)) - D / G - S;
        ++i;
    }
    return (findclosest(roots));
}

float 	solvequartic(float a, float b, float c, float d, float e)
{
    if (a == 0)
        return (solvecubic(b, c, d, e));
    b /= a;
    c /= a;
    d /= a;
    e /= a;
    float roots[4];
    float b2 = b * b;
    float b3 = b * b2;
    float b4 = b2 * b2;
    float alpha = (-3.0/8.0) * b2 + c;
    float beta  = b3 / 8.0 - b * c/ 2.0 + d;
    float gamma = (-3.0 / 256.0) * b4 + b2 * c/16.0 - b * d / 4.0 + e;
    float alpha2 = alpha * alpha;
    float t = -b / 4.0;
    if (beta == 0)
    {
        float rad = sqrt(alpha2 - 4.0 * gamma);
        float r1 = sqrt((-alpha + rad) / 2.0);
        float r2 = sqrt((-alpha - rad) / 2.0);
        roots[0] = t + r1;
        roots[1] = t - r1;
        roots[2] = t + r2;
        roots[3] = t - r2;
    }
    else
    {
        float alpha3 = alpha * alpha2;
        float P = - (alpha2 / 12.0 + gamma);
        float Q = - alpha3 / 108.0 + alpha * gamma / 3.0 - beta * beta / 8.0;
        float R = -Q / 2.0 + sqrt(Q * Q / 4.0 + P * P * P/ 27.0);
        float U = cbrt(R);
        float y = (-5.0 / 6.0) * alpha + U;
        (U == 0 ? y -= cbrt(Q) : P/ (3.0 * U));
        float W = sqrt(alpha + 2.0 * y);
        float r1 = sqrt(-(3.0 * alpha + 2.0 * y + 2.0 * beta / W));
        float r2 = sqrt(-(3.0 * alpha + 2.0 * y - 2.0 * beta / W));
        roots[0] = t + ( W - r1) / 2.0;
        roots[1] = t + ( W + r1) / 2.0;
        roots[2] = t + (-W - r2) / 2.0;
        roots[3] = t + (-W + r2) / 2.0;
    }
    return (findclosest(roots));
}

static float intersect(t_ray *ray, const t_objects objects, const float znear, const int enable)
{
	float3 dist;
	float a, b, c, d, e;
	float m, n, o, p, q;
	float solve;
	float t0, t1, t;

	dist = ray->pos - objects.position;
	float3 rdir = soft_normalize(rotatexyz(ray->dir, objects.rotation));
	//float3 rdir = ray->dir;

	m = soft_dot(rdir, rdir);
	n = soft_dot(rdir, dist);
	o = soft_dot(dist, dist);
	p = soft_dot(rdir, objects.normal);
	q = soft_dot(dist, objects.normal);

	// if (objects.type == SOR)
	// {
	// 	m = soft_dot(rdir, rdir);
	// 	n = soft_dot(rdir, dist);
	// 	o = soft_dot(dist, dist);
	// 	p = soft_dot(rdir, objects.normal);
	// 	q = soft_dot(dist, objects.normal);

	// 	a = m - p*p;
 // 		b = 2 * (n - p * q);
 //  		c = o - q * q;
 //  		float max = soft_dot(rdir, objects.normal) * solvequadratic(a, b, c) + soft_dot(dist, objects.normal);
 //  		if (fmax(max,0) > 0 && fmax(max,0) < 50)
	// 		return (solvequadratic(a, b, c));
	// 	else
	// 		return (FLT_MAX);
	// }
	if (objects.type == ELLIPSOID)
	{
		float A1  = 2 * objects.depth * p;
   		float A2  = pow(objects.radius, 2) + 2 * objects.dist * q - objects.dist;
   		a   = 4 * pow(objects.radius, 2) * m - A1 * A1;
   		b = 2 * (4 * pow(objects.radius, 2) * n - A1 * A2);
   		c   = 4 * pow(objects.radius, 2) * o - A2 * A2;
   		return (solvequadratic(a, b, c));
	}
	else if (objects.type == PARABOLOID)
	{
		a = m - p * p;
 		b = 2 * (n - p * (q + 2 * objects.dist));
		c = o - q * (q + 4 * objects.dist);
		float max = soft_dot(rdir, objects.normal) * solvequadratic(a, b, c) + soft_dot(dist, objects.normal);
  		if (fmax(max,0) > 0 && fmax(max,0) < 50)
			return (solvequadratic(a, b, c));
		else
			return (FLT_MAX);
	}
	else if (objects.type == TORUS)
	{
		a = m * m;
		b = 4 * m * n;

		c = 4 * m * m + 2 * m * o - \
		2 * (pow(objects.radius,2) + pow(objects.radius2, 2)) * m + \
		 4 * pow(objects.radius, 2) * pow(p, 2);

		d = 4 * n * o - \
		4 * (pow(objects.radius, 2) + pow(objects.radius2, 2)) * n + \
		 8 * pow(objects.radius, 2) * p * q;

		e = pow(o,2) - \
		2 * (pow(objects.radius, 2) + pow(objects.radius2, 2)) * o + \
		4 * pow(objects.radius, 2) * pow(q, 2) + \
		 pow((pow(objects.radius, 2) + pow(objects.radius2, 2)),2);
		return (solvequartic(a, b, c, d, e));
	}
	else if (objects.type == SPHERE)
	{
		c = soft_dot(dist, dist) - objects.radius * objects.radius;
		if (enable && c < EPSILON) // Culling face
			return (FLT_MAX);
		a = soft_dot(rdir, rdir);
		b = soft_dot(dist, rdir);
	}
	else if (objects.type == PLANE)
	{
		a = soft_dot(-objects.normal, rdir);
 		if (enable && a < EPSILON) // Culling face
 			return (FLT_MAX);
 		b = soft_dot(-objects.normal, ray->pos);
 		c = soft_dot(-objects.normal, objects.position);
 		solve = -((b - c) / a);
		if (solve < EPSILON)
			return (FLT_MAX);
		return (solve);
	}
	else if (objects.type == DISK)
	{
		a = soft_dot(-objects.normal, rdir);
		if (enable && a < EPSILON) // Culling face
			return (FLT_MAX);
		b = soft_dot(-objects.normal, ray->pos);
		c = soft_dot(-objects.normal, objects.position);
		solve = -((b - c) / a);
		if (solve < EPSILON)
			return (FLT_MAX);
		float3 impact = (ray->pos + rdir * solve);
		float3 v = impact - objects.position;
		if (sqrt(soft_dot(v,v)) <= 50)
				return (solve);
		else
			return (FLT_MAX);
	}
	else if (objects.type == CYLINDER)
	{
		c = dist.x * dist.x + dist.z * dist.z - objects.radius * objects.radius;
 		if (enable && c < EPSILON) // Culling face
 			return (FLT_MAX);
		a = rdir.x * rdir.x + rdir.z * rdir.z;
		b = rdir.x * dist.x + rdir.z * dist.z;
	}
	else if (objects.type == CYLINDERINF)
	{
		c = dist.x * dist.x + dist.z * dist.z - objects.radius * objects.radius;
 		if (enable && c < EPSILON) // Culling face
 			return (FLT_MAX);
		a = rdir.x * rdir.x + rdir.z * rdir.z;
		b = rdir.x * dist.x + rdir.z * dist.z;
	}
	else if (objects.type == CONEINF)
	{
		a = rdir.x * rdir.x + rdir.z * rdir.z - rdir.y * rdir.y;
		b = rdir.x * dist.x + rdir.z * dist.z - rdir.y * dist.y;
		c = dist.x * dist.x + dist.z * dist.z - dist.y * dist.y;
	}
	else if (objects.type == CONE)
	{
		a = rdir.x * rdir.x + rdir.z * rdir.z - rdir.y * rdir.y;
		b = rdir.x * dist.x + rdir.z * dist.z - rdir.y * dist.y;
		c = dist.x * dist.x + dist.z * dist.z - dist.y * dist.y;
	}
	else
		return (FLT_MAX);
	solve = b * b - a * c;
	if (solve < EPSILON)
		return (FLT_MAX);
	t0 = (-b - sqrt(solve)) / a;
	t1 = (-b + sqrt(solve)) / a;
	if (objects.type == CYLINDER)
	{
		float max = soft_dot(rdir, objects.normal) * deph_min(t0, t1) + soft_dot(dist, objects.normal);
		if (fmax(max,0) > 0 && fmax(max,0) < 50)
			return (deph_min(t0, t1));
		else
			return (FLT_MAX);
	}
	if (objects.type == CONE)
	{
		float max = soft_dot(rdir, objects.normal) * deph_min(t0, t1) + soft_dot(dist, objects.normal);
		if (fmax(max,0) > 0 && fmax(max,0) < 50)
			return (deph_min(t0, t1));
		else
			return (FLT_MAX);
	}
	return (deph_min(t0, t1));
}

static float	shadow(t_ray ray, const t_light light, __constant t_objects *objects, __constant t_scene *scene)
{
	t_ray  ray_light;
	int i = -1;
	int	tmp = scene->zfar;

	ray_light.object = -1;
	ray_light.pos = ray.pos + ray.dir * ray.deph;
	if (light.type == POINTLIGHT)
		ray_light.dir = light.position - ray_light.pos;
	else if (light.type == SPOTLIGHT)
		ray_light.dir = soft_normalize(-light.direction);
	else if (light.type == DIRLIGHT)
		ray_light.dir = soft_normalize(light.direction);
	ray_light.deph = sqrt(soft_dot(ray_light.dir, ray_light.dir));//scene->zfar;
	while (++i < scene->max_object)
	{
		if (i != ray_light.object)
		{
			float d  = intersect(&ray_light, objects[i], EPSILON, 1);
			//if (tmp > d)

			tmp = d;
			if (d >= EPSILON && d < ray_light.deph)
			{
				ray_light.deph = d;
				ray_light.object = i;
				//if (i == scene->max_object)
				return (0.5f);
			}
		}
	}
	return (1.0f);
}

float4		refract_color(__constant t_scene *scene, __constant t_objects *objects, t_ray nray, __constant t_material *materials, __constant t_light *lights);

static float4 reflect_color(__constant t_scene *scene, __constant t_light *lights, __constant t_objects *objects, t_ray nray, __constant t_material *materials)
{
	t_ray ray = nray;
	float3	normal;
	t_ray	reflect_ray;
	reflect_ray.object = ray.object;
	float4	color = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
	float4	point_color = objects[ray.object].color;
	float4	reflect_color;
	int		j = 0;
	while (j < scene->max_reflect)
	{
		if (!materials[objects[ray.object].material_id - 1].reflection)
			return (color * point_color);
		int i = 0;
		reflect_ray.pos = ray.pos + ray.dir * ray.deph;
		normal = soft_normalize(get_normal(&ray, objects[ray.object]));
		reflect_ray.deph = scene->zfar;
		reflect_ray.dir = soft_normalize(float3_reflect(ray.dir, normal));
		//reflect_ray.dir = soft_normalize(rotatexyz(reflect_ray.dir, objects[ray.object].rotation));
		while (i < scene->max_object)
		{
			float d  = intersect(&reflect_ray, objects[i], EPSILON, 1);
			if (d >= EPSILON && d < reflect_ray.deph)
			{
				reflect_ray.deph = d;
				reflect_ray.object = i;
			}
			i++;
		}
		if (reflect_ray.deph < scene->zfar)
		{
			if (scene->max_light > 0)
			{
				float shadow_attenuation;
				i = 0;
				while (i < scene->max_light)
				{
					color += light(&reflect_ray, objects[reflect_ray.object], lights[i], materials);
					shadow_attenuation = shadow(reflect_ray, lights[i], objects, scene);
					i++;
				}
				color *= (shadow_attenuation);
				color.w = 1.0f;
			}
			else
				color = noLight(&reflect_ray, objects[reflect_ray.object], materials, scene->max_material);
		}
		else
		{
			return (clamp(color,  0.0f, 1.0f)  /*(float)(1.0f, 0.0f, 0.0f, 1.0f)*/);
		}
		//le return en dessous donne un effet ressemblant a l'effet cartoon
		//return (point_color);
		point_color += objects[reflect_ray.object].color * materials[objects[ray.object].material_id - 1].reflection;
		reflect_color = point_color * color;
		ray = reflect_ray;
		j++;
	}
	return (clamp(reflect_color, 0.0f, 1.0f));
}


float4		refract_color(__constant t_scene *scene, __constant t_objects *objects, t_ray nray, __constant t_material *materials, __constant t_light *lights)
{
	t_ray ray = nray;
	float3	normal;
	t_ray	refract_ray;
	//refract_ray.object = ray.object;
	float4	color = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
	int j = 0;
	float4	point_color = objects[ray.object].color;
	float4	refract_color;
	while (j < scene->max_refract)
	{
		if (!materials[objects[ray.object].material_id - 1].refraction)
			return (color * point_color);
		refract_ray.pos = ray.pos + ray.dir * ray.deph;
		normal = soft_normalize(get_normal(&ray, objects[ray.object]));
		refract_ray.deph = scene->zfar;
		refract_ray.dir = soft_normalize(float3_refract(ray.dir, normal, materials[objects[ray.object].material_id - 1].refraction));
		//refract_ray.dir = soft_normalize(rotatexyz(refract_ray.dir, objects[ray.object].rotation));
		int i = 0;
		while (i < scene->max_object)
		{
			float d  = intersect(&refract_ray, objects[i], EPSILON, 1);
			if (d >= EPSILON && d < refract_ray.deph)
			{
				refract_ray.deph = d;
				refract_ray.object = i;
			}
			i++;
		}
		if (refract_ray.deph < scene->zfar)
		{
			if (scene->max_light > 0)
			{
				float shadow_attenuation = 1.0f;
				i = 0;
				while (i < scene->max_light)
				{
					color += light(&refract_ray, objects[refract_ray.object], lights[i], materials);
					shadow_attenuation = shadow(refract_ray, lights[i], objects, scene);
					i++;
				}
				color *= (shadow_attenuation);
				color.w = 1.0f;
			}
			else
				color = noLight(&refract_ray, objects[refract_ray.object], materials, scene->max_material);
		}
		else
		{//printf("laaaaaaaaaa\n");
			//color = (float4)(0.0f, 1.0f, 0.0f, 1.0f);
			return (color);
		}
		point_color *= objects[refract_ray.object].color * materials[objects[ray.object].material_id - 1].refraction;
		refract_color = point_color * color;
		ray = refract_ray;
		j++;
	}
	return (refract_color);
}

__kernel void raytracer(__global uchar4* pixel,
	__constant t_scene *scene,
	__constant t_cam *camera,
	__constant t_objects *objects,
	__constant t_light *lights,
	__constant t_material *materials)
{
	int xmax = get_global_size(0);
	int ymax = get_global_size(1);
	int x = get_global_id(0);
	int y = get_global_id(1);
	int index = x + y * xmax;
	float4 color;
	float shadow_attenuation;
	t_ray ray;
	float4 cc = (float4)(0, 0, 0, 1);
	ray.deph = scene->zfar;
	ray.pos = (float3)(camera->position.x, camera->position.y, camera->position.z);
	ray.dir = soft_normalize((float3)((x - xmax / 2.0f), (y - ymax / 2.0f), xmax / scene->focale));
	ray.dir = soft_normalize(rotatexyz(ray.dir, camera->rotation));
	ray.object = -1;
	int i = 0;
	while (i < scene->max_object)
	{
		float d  = intersect(&ray, objects[i], scene->znear, 1);
		if (d >= scene->znear && d < ray.deph)
		{
			ray.deph = d;
			ray.object = i;
		}
		i++;
	}
	if (ray.object >= 0 && ray.deph < scene->zfar)
	{
		if (scene->max_light > 0)
		{
			color = (float4)(0, 0, 0, 1);
			i = 0;
			while (i < scene->max_light)
			{
				color += light(&ray, objects[ray.object], lights[i], materials);
				shadow_attenuation = shadow(ray, lights[i], objects, scene);
				i++;
			}
			color *= (shadow_attenuation);
			color = clamp(color, 0.0f, 1.0f);
			color.w = 1.0f;
			if (scene->max_refract > 0 && materials[objects[ray.object].material_id - 1].refraction > 0.0)
				color += refract_color(scene, objects, ray, materials, lights);
			if (scene->max_reflect > 0 && materials[objects[ray.object].material_id - 1].reflection > 0.0)
				color += reflect_color(scene, lights, objects, ray, materials);
		}
		else
			color = noLight(&ray, objects[ray.object], materials, scene->max_material);
	}
	else
	{//printf("laaaaaaaaaa\n");
		color = (float4)(0, 0, 0, 1.0f);
	}
	if (scene->render_mod == RENDERMODE_GRIS)
 	{
 		float gray = (color.x + color.y + color.z) / 3.0f;
 		color.x = gray;
 		color.y = gray;
 		color.z = gray;
 	}
 	else if (scene->render_mod == RENDERMODE_SEPIA)
 	{
		float gray = (color.x + color.y + color.z) / 3.0f;
 		color.x = gray + 0.4f;
 		color.y = gray + 0.2f;
 		color.z = gray - 0.1f;
 	}
	else if (scene->render_mod == RENDERMODE_FILTER)
	{
		color *= scene->render_filter;
 	}
 	else if (scene->render_mod == RENDERMODE_ADD)
 	{
 		color = (color + scene->render_filter) / 2.0f;
 	}
  	color = clamp(color, 0.0f, 1.0f);
 	pixel[index] = (uchar4)(color.z * 255.0f, color.y * 255.0f, color.x * 255.0f, 255.0f);
}
