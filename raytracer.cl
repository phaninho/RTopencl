# define PI 3.14159265359f
# define EPSILON 0.00001f

# define SCENE 0
# define CAMERA (SCENE + 1)

# define OBJECTS (CAMERA + 1)
# define SPHERE (OBJECTS)
# define PLANE (SPHERE + 1)
# define CYLINDER (PLANE + 1)
# define CONE (CYLINDER + 1)
# define TRIANGLE (CONE + 1)
# define DISK (TRIANGLE + 1)
# define CYLINDERINF (DISK + 1)
# define CONEINF (CYLINDERINF + 1)
# define PARABOLOID (CONEINF + 1)
# define ELLIPSOID (PARABOLOID + 1)
# define TORUS (ELLIPSOID + 1)
# define SOR (TORUS + 1)
# define END_OBJECTS (SOR)

# define LIGHTS (END_OBJECTS + 1)
# define SPOTLIGHT (LIGHTS)
# define POINTLIGHT (SPOTLIGHT + 1)
# define DIRLIGHT (POINTLIGHT + 1)
# define END_LIGHTS (DIRLIGHT)

# define MATERIAL (END_LIGHTS + 1)
# define TEXTURE (MATERIAL + 1)
# define RENDER (TEXTURE + 1)

# define RENDERMODE_SEPIA (RENDER + 1)
# define RENDERMODE_GRIS (RENDERMODE_SEPIA + 1)
# define RENDERMODE_FILTER (RENDERMODE_GRIS + 1)
# define RENDERMODE_ADD (RENDERMODE_FILTER + 1)
# define RENDERMODE_NEGATIF (RENDERMODE_ADD + 1)
# define RENDERMODE_CARTOON (RENDERMODE_NEGATIF + 1)

typedef struct	s_texture
{
	int			width;
	int			height;
	char		*data;
}				t_texture;

typedef struct	s_scene
{
	char		*name;
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
	int			obj_index;
}				t_scene;

typedef struct	s_cam
{
	float3		position;
	float3		rotation;
}				t_cam;

typedef struct	s_objects
{
	float4		color;
	float3		position;
	float3		rotation;
	float3		normal;
	float3		endpos; // position final cylindre et cone
	int			type;
	int			material_id;
	int			texture_id;
	int			in_object;
	float		radius;
	float		radius2; // petit radius pour torus
	float		a; // coefficient pour sor
	float		b; // pour sor
	float		c; // pour sor
	float		d; // pour sor
	float		dist; // distance des point pour parabol, epsilloid
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
	float		damier;
	int			perlin;
	float		refract_coef;
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
	nvec.y = vec.y * half_cos(rx) - vec.z * half_sin(rx);
	nvec.z = vec.y * half_sin(rx) + vec.z * half_cos(rx);
	return (nvec);
}

static float3 rotatey(float3 vec, float degree)
{
	float3 nvec = (float3)(0, 0, 0);
	float ry = degree * PI / 180.0f;
	nvec.x = vec.z * half_sin(ry) + vec.x * half_cos(ry);
	nvec.y = vec.y;
	nvec.z = vec.z * half_cos(ry) - vec.x * half_sin(ry);
	return (nvec);
}

static float3 rotatez(float3 vec, float degree)
{
	float3 nvec = (float3)(0, 0, 0);
	float rz = degree * PI / 180.0f;
	nvec.x = vec.x * half_cos(rz) - vec.y * half_sin(rz);
	nvec.y = vec.x * half_sin(rz) + vec.y * half_cos(rz);
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

static float3        float3_refract(const float3 v, const float3 normal, const float ior)
{
    float cosi = clamp(-1.0f, 1.0f, soft_dot(normal, v));
    float etai = 1;
    float etat = ior;
    float3 n = normal;
    if (cosi < 0)
        cosi = -cosi;
    else
    {
        etat = 1;
        etai = ior;
        n = -normal;
    }
    float eta = etai - etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return (k < 0 ? 0 : eta * v + (eta * cosi - half_sqrt(soft_dot(k, k))) * n);
}

static float3 get_normal(t_ray *ray, const t_objects objects)
{
	float3 impact = ray->pos + ray->dir * ray->deph;
	if (objects.type == SPHERE)
	{
		return (soft_normalize(impact - objects.position));
	}
	else if (objects.type == PLANE || objects.type == DISK)
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
	return ((float3)(0.0f, 0.0f, 0.0f));
}

constant static int myPerlin[] = { 151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,151,
   160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,
   37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,
   11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,
   139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,
   46,245,40,244,102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,
   169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
   250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
   189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
   43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
   97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,
   239,107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127,4,
   150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
   };
static float fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}
static float lerp(float t, float a, float b)
{
    return a + t * (b - a);
}
static float grad(int hash, float x, float y, float z)
{
    int h = hash & 15;
    float u = h < 8|| h == 12 || h == 13 ? x : y;
    float v = h < 4 || h == 12 || h == 13 ? y : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

static float noise(float x, float y, float z)
{
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    int Z = (int)floor(z) & 255;
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    float u = fade(x);
    float v = fade(y);
    float w = fade(z);
    int A = myPerlin[X] + Y;
    int AA = myPerlin[A] + Z;
    int AB = myPerlin[A + 1] + Z;
    int B = myPerlin[X + 1] + Y;
    int BA = myPerlin[B] + Z;
    int BB = myPerlin[B + 1] + Z;
    return (lerp(w, lerp(v, lerp(u, grad(myPerlin[AA], x  , y  , z   ),
                                    grad(myPerlin[BA], x - 1, y  , z   )),
                            lerp(u, grad(myPerlin[AB], x  , y - 1, z   ),
                                    grad(myPerlin[BB], x - 1, y - 1, z   ))),
                    lerp(v, lerp(u, grad(myPerlin[AA + 1], x  , y  , z - 1 ),
                                    grad(myPerlin[BA + 1], x - 1, y  , z - 1 )),
                            lerp(u, grad(myPerlin[AB + 1], x  , y - 1, z - 1 ),
                                    grad(myPerlin[BB + 1], x - 1, y - 1, z - 1 )))));
}
static  float limit(float x, float n1, float n2)
{
    if (x < n1)
        return (n1);
    if (x > n2)
        return (n2);
    return (x);
}
static float4   perlin_wood(float3 inter, float frequency)
{
    float pn = 0.0f;
    float4 color1 = (float4)(1.0f, 1.0f, 1.0f, 1.0f);
    int x1 = (inter.x < 0 ? 1 : 0);
    int y1 = (inter.y < 0 ? 1 : 0);
    int z1 = (inter.z < 0 ? 1 : 0);
    inter.x += x1;
    inter.y += y1;
    inter.z += z1;
    float n = noise(frequency * inter.x, frequency * inter.y, frequency * inter.z);
    n *= (n < 0.0f ? -1.0f : 1.0f);
    pn += 10.0f * n;
    int i = (int)pn;
    pn = pn - (float)i;
    if (n < 0.3f)
    {
        color1.x = limit(0.08f * (n - 0.3f) / (0.5f - 0.3f) + 0.47f * (0.5f - n) / (0.5f - 0.3f), 0.0f, 1.0f);
        color1.y = limit(0.47f * (n - 0.3f) / (0.5f - 0.3f) + 0.08f * (0.5f - n) / (0.5f - 0.3f), 0.0f, 1.0f);
        color1.z = limit(0.78f * (n - 0.3f) / (0.5f - 0.3f) + 0.78f * (0.5f - n) / (0.5f - 0.3f), 0.0f, 1.0f);
    }
    else if (n < 0.7f)
    {
        color1.x = limit(0.47f * (n - 0.5f) / (0.7f - 0.5f) + 0.78f * (0.7f - n) / (0.7f - 0.5f), 0.0f, 1.0f);
        color1.y = limit(0.08f * (n - 0.5f) / (0.7f - 0.5f) + 0.47f * (0.7f - n) / (0.7f - 0.5f), 0.0f, 1.0f);
        color1.z = limit(0.78f * (n - 0.5f) / (0.7f - 0.5f) + 0.08f * (0.7f - n) / (0.7f - 0.5f), 0.0f, 1.0f);
    }
    else if (n >= 0.7f)
    {
        color1.x = 0.78f;
        color1.y = 0.47f;
        color1.z = 0.08f;
    }
    return (color1);
}

static float4    light_ambient(float4 color, const t_light light, const t_material material)
{
    float4 color_ambient = color;
    color_ambient *= light.color;
    color_ambient *= material.ambient_color;
    return (color_ambient);
}

static float4 light(t_ray *ray, const t_objects objects, const t_light light, __constant t_material *material, float3 campos)
{
    float3  impact = ray->pos + ray->dir * ray->deph;
    float3  lightDir = light.direction;
    float   distanceToLight;
    float   attenuation = 1.0f;
    float3  normal = get_normal(ray, objects);
    float4  finalColor = objects.color;
		if (material[objects.material_id - 1].perlin)
			finalColor = perlin_wood(impact, 0.1f);
    if (material[objects.material_id - 1].damier)
    {
        int     x1 = (impact.x < 0 ? 1 : 0);
        int     y1 = (impact.y < 0 ? 1 : 0);
        int     z1 = (impact.z < 0 ? 1 : 0);
        int     tmp;
        x1 += (int)(floor(impact.x) / material[objects.material_id - 1].damier) % 2;
        y1 += (int)(floor(impact.y) / material[objects.material_id - 1].damier) % 2;
        z1 += (int)(floor(impact.z) / material[objects.material_id - 1].damier) % 2;
        if (!x1)
        {
            if (((!y1) && (!z1)) || (((y1) && (z1))))
                tmp = 1;
            else
                tmp = 2;
        }
        else
        {
            if ((((!y1) && (!z1))) || (((y1) && (z1))))
                tmp = 2;
            else
                tmp = 1;
        }
        if (tmp == 1)
        {
            finalColor.x = 0.3f;
            finalColor.y = 0.3f;
            finalColor.z = 0.3f;
            finalColor.w = 1.0f;
        }
    }
    if (material[objects.material_id].perlin)
      finalColor = perlin_wood(impact, 0.1f);
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
    float4  ambient = 0.0f;
    if (objects.material_id > 0)
        ambient = light_ambient(finalColor, light, *material);
    //diffuse
    float   diffuse_coeff = max(0.0f, soft_dot(normal, lightDir));
    diffuse_coeff = clamp(diffuse_coeff, 0.0f, 1.0f);
    float4    diffuse = diffuse_coeff * finalColor * light.color;
    //specular
    float   specular_coeff = 0.0f;
    float4  specular = material[objects.material_id - 1].specular_color;
    float3 cameradir = normalize(campos  - impact);
    if (!material[objects.material_id - 1].blinn && soft_dot(lightDir, normal) > 0.0)
    {
            float3 reflectionVector = float3_reflect(-lightDir, normal);
            float specTmp = max(0.0f, soft_dot(reflectionVector, cameradir));
            specular_coeff = pow(specTmp, material[objects.material_id - 1].shininess);
    }
    else if (material[objects.material_id - 1].blinn && soft_dot(lightDir, normal) > 0.0)
    {
            float3 halfwayVector = soft_normalize(lightDir + cameradir);
            float specTmp = max(0.0f, soft_dot(normal, halfwayVector));
            specular_coeff = pow(specTmp, material[objects.material_id - 1].shininess);
    }
    specular = ambient + diffuse + specular_coeff;
    float4 specular1 = specular * ambient;
    float4  linearColor = (specular1 + ambient * attenuation) * (diffuse + specular);
    finalColor = clamp(linearColor, 0.0f, 1.0f);
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

static float intersect(t_ray *ray, const t_objects objects, const float znear, const int enable)
{
	float3 dist;
	float a, b, c;
	float m, n, o, p, q;
	float solve;
	float t0, t1;

	dist = ray->pos - objects.position;
	float3 rdir = soft_normalize(rotatexyz(ray->dir, -objects.rotation));
	m = 1.0f;
	n = 1.0f;
	o = 1.0f;
	p = 1.0f;
	q = 1.0f;

	if (objects.type == SPHERE)
	{
		c = soft_dot(dist, dist) - objects.radius * objects.radius;
		if (enable && c < EPSILON)
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
	else if (objects.type == CYLINDER || objects.type == CYLINDERINF)
	{
		c = dist.x * dist.x + dist.z * dist.z - objects.radius * objects.radius;
 		if (enable && c < EPSILON) // Culling face
 			return (FLT_MAX);
		a = rdir.x * rdir.x + rdir.z * rdir.z;
		b = rdir.x * dist.x + rdir.z * dist.z;
	}
	else if (objects.type == DISK)
	{
		a = soft_dot(-objects.normal, rdir);
		if (enable && a < EPSILON)
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
	else if (objects.type == CONE || objects.type == CONEINF)
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
		float m = soft_dot(rdir, objects.normal) * deph_min(t0, t1) + soft_dot(dist, objects.normal);
		if (fmax(m,0) > EPSILON && fmax(m,0) < 50)
			return (deph_min(t0, t1));
		else
			return (FLT_MAX);
	}
	if (objects.type == CONE)
	{
		float m = soft_dot(rdir, objects.normal) * deph_min(t0, t1) + soft_dot(dist, objects.normal);
		if (fmax(m,0) > EPSILON && fmax(m,0) < 50)
			return (deph_min(t0, t1));
		else
			return (FLT_MAX);
	}
	return (deph_min(t0, t1));
}

static float    shadow(t_ray ray, const t_light light, __constant t_objects *objects, __constant t_scene *scene)
{
    t_ray  ray_light;
    int i = 0;
    ray_light.pos = ray.pos + ray.dir * ray.deph;
    float3  lightDir;
    if (light.type == POINTLIGHT || light.type == SPOTLIGHT)
        ray_light.dir = light.position - ray_light.pos;
    else if (light.type == DIRLIGHT)
    {
        ray_light.dir = light.direction;
        ray_light.deph = scene->zfar;
    }
    ray_light.deph = scene->zfar;
    if (light.type == SPOTLIGHT || light.type == POINTLIGHT)
        ray_light.deph = sqrt(soft_dot(ray_light.dir, ray_light.dir));
    while (i < scene->max_object)
    {
        if (i != ray.object)
        {
            float d  = intersect(&ray_light, objects[i], scene->znear, 1);
            if (d >= EPSILON && d < ray_light.deph)
            {
                ray_light.deph = d;
                ray_light.object = i;
                if (light.type == SPOTLIGHT)
                {
                    lightDir = soft_normalize(light.position - ray_light.pos);
                    float3 coneDirection = soft_normalize(-light.direction);
                    float3 raydirection = lightDir;
                    float lightToSurfaceAngle = soft_dot(coneDirection, raydirection);
                    lightToSurfaceAngle = degrees(acos(lightToSurfaceAngle));
                    if(lightToSurfaceAngle > light.angle / 2)
                        return (1.0f);
                    return(0.5f);
                }
                return (0.5f);
            }
        }
        i++;
    }
    return (1.0f);
}

static float4 reflect_color_in_refract(__constant t_scene *scene, __constant t_light *lights, __constant t_objects *objects, t_ray nray, __constant t_material *materials, float3 campos)
{
	t_ray ray = nray;
	float3	normal;
	t_ray	reflect_ray;
	reflect_ray.object = ray.object;
	float4	color = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
	float4	point_color = color;//objects[ray.object].color;
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
		while (i < scene->max_object)
		{
			float d  = intersect(&reflect_ray, objects[i], EPSILON, 1);
			if (d >= EPSILON && d < reflect_ray.deph && i != ray.object)
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
				float shadow_attenuation = 1.0f;
				i = 0;
				while (i < scene->max_light)
				{
					color += light(&reflect_ray, objects[reflect_ray.object], lights[i], materials, campos);
					shadow_attenuation *= shadow(reflect_ray, lights[i], objects, scene);
					i++;
				}
				color *= (shadow_attenuation);
				//color.w = 1.0f;
			}
			else
				color = noLight(&reflect_ray, objects[reflect_ray.object], materials, scene->max_material);
		}
		else
			return (clamp(color,  0.0f, 1.0f));
		point_color += objects[reflect_ray.object].color * materials[objects[nray.object].material_id - 1].reflection;
		reflect_color = point_color * color;
		ray = reflect_ray;
		j++;
	}
	return (clamp(reflect_color, 0.0f, 1.0f));
}


static float4		refract_color_in_reflect(__constant t_scene *scene, __constant t_objects *objects, t_ray nray, __constant t_material *materials, __constant t_light *lights, float3 campos)
{
	t_ray ray = nray;
	float3	normal;
	t_ray	refract_ray;
	refract_ray.object = ray.object;
	float4	color = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
	float4	point_color = objects[ray.object].color;
	float4	refract_color;
	int j = 0;
	while (j < scene->max_refract)
	{
		if (!materials[objects[ray.object].material_id - 1].refraction)
			return (color * point_color);
		refract_ray.pos = ray.pos + ray.dir * ray.deph;
		normal = soft_normalize(get_normal(&ray, objects[ray.object]));
		refract_ray.deph = scene->zfar;
		refract_ray.dir = soft_normalize(float3_refract(ray.dir, normal, materials[objects[ray.object].material_id - 1].refraction));
		int i = 0;
		while (i < scene->max_object)
		{
			float d  = intersect(&refract_ray, objects[i], EPSILON, 1);
			if (d >= EPSILON && d < refract_ray.deph && i != ray.object)
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
					color += light(&refract_ray, objects[refract_ray.object], lights[i], materials, campos);
					shadow_attenuation *= shadow(refract_ray, lights[i], objects, scene);
					i++;
				}
				color *= (shadow_attenuation);
			}
			else
				color = noLight(&refract_ray, objects[refract_ray.object], materials, scene->max_material);
		}
		else
		{
			return (clamp(color, 0.0f, 1.0f));
		}
		point_color += objects[refract_ray.object].color * materials[objects[nray.object].material_id - 1].refraction;
		refract_color = point_color * color;
		ray = refract_ray;
		j++;
	}
	return (clamp(refract_color, 0.0f, 1.0f));
}


static float4 reflect_color(__constant t_scene *scene, __constant t_light *lights, __constant t_objects *objects, t_ray nray, __constant t_material *materials, float3 campos)
{
	t_ray ray = nray;
	float3	normal;
	t_ray	reflect_ray;
	reflect_ray.object = ray.object;
	float4	color = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
	float4	point_color = color;//objects[ray.object].color;
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
		while (i < scene->max_object)
		{
			float d  = intersect(&reflect_ray, objects[i], EPSILON, 1);
			if (d >= EPSILON && d < reflect_ray.deph && i != ray.object)
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
				float shadow_attenuation = 1.0f;
				i = 0;
				while (i < scene->max_light)
				{
					color += light(&reflect_ray, objects[reflect_ray.object], lights[i], materials, campos);
					shadow_attenuation *= shadow(reflect_ray, lights[i], objects, scene);
					i++;
				}
				color *= (shadow_attenuation);
			}
			else
				color = noLight(&reflect_ray, objects[reflect_ray.object], materials, scene->max_material);
		}
		else
			return (clamp(color,  0.0f, 1.0f));
		point_color += objects[reflect_ray.object].color * materials[objects[nray.object].material_id - 1].reflection;
		if (scene->max_refract > 0 && materials[objects[reflect_ray.object].material_id - 1].refraction > 0.0)
			color += refract_color_in_reflect(scene, objects, reflect_ray, materials, lights,campos);
		reflect_color = point_color * color;
		ray = reflect_ray;
		j++;
	}
	return (clamp(reflect_color, 0.0f, 1.0f));
}

static float4		refract_color(__constant t_scene *scene, __constant t_objects *objects, t_ray nray, __constant t_material *materials, __constant t_light *lights, float3 campos)
{
	t_ray ray = nray;
	float3	normal;
	t_ray	refract_ray;
	refract_ray.object = ray.object;
	float4	color = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
	float4	point_color = objects[ray.object].color;
	float4	refract_color;
	int j = 0;
	while (j < scene->max_refract)
	{
		if (!materials[objects[ray.object].material_id - 1].refraction)
			return (color * point_color);
		refract_ray.pos = ray.pos + ray.dir * ray.deph;
		normal = soft_normalize(get_normal(&ray, objects[ray.object]));
		refract_ray.deph = scene->zfar;
		refract_ray.dir = soft_normalize(float3_refract(ray.dir, normal, materials[objects[ray.object].material_id - 1].refraction));
		int i = 0;
		while (i < scene->max_object)
		{
			float d  = intersect(&refract_ray, objects[i], EPSILON, 1);
			if (d >= EPSILON && d < refract_ray.deph && i != ray.object)
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
					color += light(&refract_ray, objects[refract_ray.object], lights[i], materials, campos);
					shadow_attenuation *= shadow(refract_ray, lights[i], objects, scene);
					i++;
				}
				color *= (shadow_attenuation);
			}
			else
				color = noLight(&refract_ray, objects[refract_ray.object], materials, scene->max_material);
		}
		else
		{
			return (clamp(color, 0.0f, 1.0f));
		}
		point_color += objects[refract_ray.object].color * materials[objects[nray.object].material_id - 1].refraction;
		if (scene->max_reflect > 0 && materials[objects[refract_ray.object].material_id - 1].reflection > 0.0)
			color += reflect_color_in_refract(scene, lights, objects, refract_ray, materials, campos);
		refract_color = point_color * color;
		ray = refract_ray;
		j++;
	}
	return (clamp(refract_color, 0.0f, 1.0f));
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
	float4 color = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
	float shadow_attenuation = 1.0f;
	t_ray ray;
	ray.deph = scene->zfar;
	ray.pos = (float3)(camera->position.x, camera->position.y, camera->position.z);
	ray.dir = soft_normalize((float3)((x - xmax / 2.0f), (y - ymax / 2.0f), xmax / scene->focale));
	ray.dir = soft_normalize(rotatexyz(ray.dir, camera->rotation));
	int i = 0;
	while (i < scene->max_object)
	{
		float d  = intersect(&ray, objects[i], scene->znear, 1);
		if (d >= EPSILON && d < ray.deph)
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
			color = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
			i = 0;
			while (i < scene->max_light)
			{
				color += light(&ray, objects[ray.object], lights[i], materials, camera->position);
				shadow_attenuation *= shadow(ray, lights[i], objects, scene);
				i++;
			}
			color *= (shadow_attenuation);
			if (scene->max_refract > 0 && materials[objects[ray.object].material_id - 1].refraction > 0.0)
				color += (refract_color(scene, objects, ray, materials, lights, camera->position) * materials[objects[ray.object].material_id - 1].refract_coef);
			if (scene->max_reflect > 0 && materials[objects[ray.object].material_id - 1].reflection > 0.0)
				color += reflect_color(scene, lights, objects, ray, materials, camera->position);
		}
		else
			color = noLight(&ray, objects[ray.object], materials, scene->max_material);
	}
	else
		color = (float4)(0, 0, 0, 1.0f);
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
	else if (scene->render_mod == RENDERMODE_NEGATIF)
	{
		color.r = 1 - color.r;
		color.g = 1 - color.g;
		color.b = 1 - color.b;
	}
	else if (scene->render_mod == RENDERMODE_CARTOON)
	{
		if (color.x < 0.2f)
			color.x = 0.0f;
		if (color.y < 0.2f)
			color.y = 0.0f;
		if (color.z < 0.2f)
			color.z = 0.0f;
		if (color.x >= 0.2f && color.x < 0.4f)
			color.x = 0.2f;
		if (color.y >= 0.2f && color.y < 0.4f)
			color.y = 0.2f;
		if (color.z >= 0.2f && color.z < 0.4f)
			color.z = 0.2f;
		if (color.x >= 0.4f && color.x < 0.5f)
			color.x = 0.4f;
		if (color.y >= 0.4f && color.y < 0.5f)
			color.y = 0.4f;
		if (color.z >= 0.4f && color.z < 0.5f)
			color.z = 0.4f;
		if (color.x >= 0.5f)
			color.x = 1.0f;
		if (color.y >= 0.5f)
			color.y = 1.0f;
		if (color.z >= 0.5f)
			color.z = 1.0f;
	}
  color = clamp(color, 0.0f, 1.0f);
	pixel[index] = (uchar4)(color.z * 255.0f, color.y * 255.0f, color.x * 255.0f, 255.0f);
}
