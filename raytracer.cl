#define PI 3.14159265359f
#define EPSILON 0.00001f

# define SCENE 0
# define CAMERA (SCENE + 1)

# define OBJECTS (CAMERA + 1)
# define SPHERE (OBJECTS)
# define PLANE (SPHERE + 1)
# define CYLINDER (PLANE + 1)
# define CONE (CYLINDER + 1)
# define TRIANGLE (CONE + 1)
# define PARAL (TRIANGLE + 1)
# define DISK (PARAL + 1)
# define CYLINDERINF (DISK + 1)
# define CONEINF (CYLINDERINF + 1)
# define PARABOLOID (CONEINF + 1)// variable : pos, normal, dist (distance entre les 2 points)
# define ELLIPSOID (PARABOLOID + 1)// variable : pos, dist (distance entre les 2 points), normal, radius
# define TORUS (ELLIPSOID + 1)// variable : pos, normal, Grand radius et petit radius.
# define SOR (TORUS + 1)// pos, normal, taille(dist), coeficient a b c d.
# define END_OBJECTS (SOR)

# define LIGHTS (END_OBJECTS + 1)
# define SPOTLIGHT (LIGHTS)
# define POINTLIGHT (SPOTLIGHT + 1)
# define DIRLIGHT (POINTLIGHT + 1)
# define END_LIGHTS (DIRLIGHT)

# define MATERIAL (END_LIGHTS + 1)
# define RENDER (MATERIAL + 1)

// Pour le parser et le CL du mod de rendu
#define RENDERMODE_SEPIA (RENDER + 1)
#define RENDERMODE_GRIS (RENDERMODE_SEPIA + 1)
#define RENDERMODE_FILTER (RENDERMODE_GRIS + 1)
#define RENDERMODE_ADD (RENDERMODE_FILTER + 1)
#define RENDERMODE_NEGATIF (RENDERMODE_ADD + 1)
#define RENDERMODE_CARTOON (RENDERMODE_NEGATIF + 1)
#define RENDERMODE_ANTI_ALIAS (RENDERMODE_CARTOON + 1)


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
	int			render_debug;
	int			render_mod;
	float4	render_filter;
	int			obj_index;
}				t_scene;

typedef struct	s_cam
{
	float3		position;
	float3		rotation;
}				t_cam;

typedef struct	s_objects
{
	int			type;
	int			material_id;
	int			in_object;
	float		radius;
	float		radius2; // petit radius pour torus
	float		dist; // distance des point pour parabol, epsilloid
	float3		position;
	float3		rotation;
	float3		normal;
	float3		endpos; // position final cylindre et cone
	float3		pos2;
	float4		color;
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

static float3   soft_cross(float3 va, float3 vb)
{
    float3  v;
    v.x = va.y * vb.z - va.z * vb.y;
    v.y = va.z * vb.x - va.x * vb.z;
    v.z = va.x * vb.y - va.y * vb.x;
    return (v);
}

static float3 rotatex(float3 vec, float degree)
{
	float3 nvec = (float3)(0, 0, 0);
	float rx = degree * PI / 180.0f;
	nvec.x = vec.x;
	nvec.y = vec.y * cos(rx) - vec.z * sin(rx);
	nvec.z = vec.y * sin(rx) + vec.z * cos(rx);
	return (nvec);
}

static float3 rotatey(float3 vec, float degree)
{
	float3 nvec = (float3)(0, 0, 0);
	float ry = degree * PI / 180.0f;
	nvec.x = vec.z * sin(ry) + vec.x * cos(ry);
	nvec.y = vec.y;
	nvec.z = vec.z * cos(ry) - vec.x * sin(ry);
	return (nvec);
}

static float3 rotatez(float3 vec, float degree)
{
	float3 nvec = (float3)(0, 0, 0);
	float rz = degree * PI / 180.0f;
	nvec.x = vec.x * cos(rz) - vec.y * sin(rz);
	nvec.y = vec.x * sin(rz) + vec.y * cos(rz);
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
    return (k < 0 ? 0 : eta * v + (eta * cosi - sqrt(soft_dot(k, k))) * n);
}

static float3 get_normal(t_ray *ray, const t_objects objects)
{
    float3 rdir = soft_normalize(rotatexyz(ray->dir, objects.rotation));
    float3 impact = ray->pos + rdir * ray->deph;
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
		else if (objects.type == TRIANGLE)
        return (soft_normalize(soft_cross(objects.pos2 - objects.position, objects.endpos - objects.position)));
    return ((float3)(0, 0, 0));
}

static float4    light_ambient(float4 color, const t_light light, const t_material material)
{
    float4    color_ambient = color;
    //if (light)
        color_ambient *= light.color;
    //if (material)
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
    }    if (light.type == SPOTLIGHT)
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
        attenuation = light.attenuation;
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

static float    solvequadratic(const float a, const float b, const float c)
{
    float       discriminant;
    float       t;
    float       t0;
    float       t1;
    t = 0;
    if ((discriminant = b * b - a * c) < EPSILON)
        return (FLT_MAX);
    else if (discriminant >= EPSILON)
    {
        discriminant = sqrt(discriminant);
        t0 = ((-b + discriminant) / a);
        t1 = ((-b - discriminant) / a);
        t = deph_min(t0, t1);
        return (t);
    }
    return (FLT_MAX);
}

static float inter_triangle(t_ray *ray, t_objects objects)
{
    float3 V1 = objects.rotation - objects.position;
    float3 V2 = objects.endpos - objects.position;
    float3 p = soft_cross(ray->dir, V2);
    float det = soft_dot(V1, p);
    if (det > -EPSILON && det < EPSILON)
        return (FLT_MAX);
    float3 cam_dir_tri = ray->pos - objects.position;
    float u = soft_dot(cam_dir_tri, p) * (1.0f / det);
    if (u < 0.0f || u > 1.0f)
        return (FLT_MAX);
    float3 q = soft_cross(cam_dir_tri, V1);
    float v = soft_dot(ray->dir, q) * (1 / det);
    if (v < 0.0f || v > 1.0f || v + u > 1.0f)
        return (FLT_MAX);
    float t = soft_dot(V2, q) * (1.0f / det);
    if (t > EPSILON)
        return (t);
    return (FLT_MAX);
}

static float inter_sphere(t_ray *ray, t_objects objects, const float3 rdir)
{
    float3 dist = ray->pos - objects.position;
    float c = soft_dot(dist, dist) - objects.radius * objects.radius;
    if (c < EPSILON)
        return (FLT_MAX);
    float a = soft_dot(rdir, rdir);
    float b = soft_dot(dist, rdir);
    return (solvequadratic(a, b, c));
}

static float inter_plane(t_ray *ray, t_objects objects, const float3 rdir)
{
    float a = soft_dot(-objects.normal, rdir);
    if (a < EPSILON) // Culling face
        return (FLT_MAX);
    float b = soft_dot(-objects.normal, ray->pos);
    float c = soft_dot(-objects.normal, objects.position);
    float solve = -((b - c) / a);
    if (solve < EPSILON)
        return (FLT_MAX);
    return (solve);
}

static float inter_disk(t_ray *ray, t_objects objects, const float3 rdir)
{
    float ng = 1;
    if (soft_dot(soft_normalize(-objects.normal), rdir) < EPSILON)
        ng = -1;
    float a = soft_dot(-objects.normal * ng, rdir);
    if (a < EPSILON) // Culling face
        return (FLT_MAX);
    float b = soft_dot(-objects.normal * ng, ray->pos);
    float c = soft_dot(-objects.normal * ng, objects.position);
    float solve = -((b - c) / a);
    if (solve < EPSILON)
        return (FLT_MAX);
    float3 impact = (ray->pos + rdir * solve);
    float3 v = impact - objects.position;
    if (sqrt(soft_dot(v,v)) <= 50)
            return (solve);
    else
        return (FLT_MAX);
}

static float inter_cylinder(t_ray *ray, t_objects objects, const float3 rdir)
{
    float3 dist = ray->pos - objects.position;
    float c = dist.x * dist.x + dist.z * dist.z - objects.radius * objects.radius;
    if (c < EPSILON) // Culling face
        return (FLT_MAX);
    float a = rdir.x * rdir.x + rdir.z * rdir.z;
    float b = rdir.x * dist.x + rdir.z * dist.z;
    return (solvequadratic(a, b, c));
}

static float inter_cone(t_ray *ray, t_objects objects, const float3 rdir)
{
    float3 dist = ray->pos - objects.position;
    float a = rdir.x * rdir.x + rdir.z * rdir.z - rdir.y * rdir.y;
    float b = rdir.x * dist.x + rdir.z * dist.z - rdir.y * dist.y;
    float c = dist.x * dist.x + dist.z * dist.z - dist.y * dist.y;
    float t = solvequadratic(a, b, c);
    if (objects.type == CONE)
    {
        float m = soft_dot(rdir, objects.normal) * t + soft_dot(dist, objects.normal);
        if (fmax(m,0) > EPSILON && fmax(m,0) < 50)
            return (t);
        else
            return (FLT_MAX);
    }
    return (t);
}

static float intersect(t_ray *ray, const t_objects objects, const float znear, const int enable)
{
    float3 rdir = soft_normalize(rotatexyz(ray->dir, -objects.rotation));
    if (objects.type == SPHERE)
        return (inter_sphere(ray , objects, rdir));
    else if (objects.type == PLANE)
        return (inter_plane(ray , objects, rdir));
    else if (objects.type == CYLINDER || objects.type == CYLINDERINF)
        return (inter_cylinder(ray, objects, rdir));
    else if (objects.type == DISK)
        return (inter_disk(ray, objects, rdir));
    else if (objects.type == CONE || objects.type == CONEINF)
        return (inter_cone(ray, objects, rdir));
    return (FLT_MAX);
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
					float d;
					if (objects && objects[i].type != TRIANGLE)
							d = intersect(&ray_light, objects[i], scene->znear, 1);
					else
							d = inter_triangle(&ray_light, objects[i]);
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
			float d;
            if (objects && objects[i].type != TRIANGLE)
                d = intersect(&reflect_ray, objects[i], EPSILON, 1);
            else
                d = inter_triangle(&reflect_ray, objects[i]);
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
			float d;
            if (objects && objects[i].type != TRIANGLE)
                d = intersect(&refract_ray, objects[i], EPSILON, 1);
            else
                d = inter_triangle(&refract_ray, objects[i]);
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
		point_color += objects[refract_ray.object].color;
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
			float d;
            if (objects && objects[i].type != TRIANGLE)
                d = intersect(&reflect_ray, objects[i], EPSILON, 1);
            else
                d = inter_triangle(&reflect_ray, objects[i]);
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
			color += refract_color_in_reflect(scene, objects, reflect_ray, materials, lights,campos) * materials[objects[reflect_ray.object].material_id - 1].refract_coef;
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
			float d;
            if (objects && objects[i].type != TRIANGLE)
                d = intersect(&refract_ray, objects[i], EPSILON, 1);
            else
                d = inter_triangle(&refract_ray, objects[i]);
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
	int xx = get_global_id(0);
	int yy = get_global_id(1);
  float x = xx;
  float y = yy;
	int index = xx + yy * xmax;
	float4 color = (float4)(0.0f, 0.0f, 0.0f, 1.0f);;
  float4 finalcolor = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
  float4 AAcolor = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
  while (scene->max_material > 0 && y < yy + 1.0f)
    {
        x = xx;
        while (x < xx + 1.0f)
        {
				float shadow_attenuation = 1.0f;
	       t_ray ray;
	       ray.deph = scene->zfar;
	       ray.pos = (float3)(camera->position.x, camera->position.y, camera->position.z);
	       ray.dir = soft_normalize((float3)((x - xmax / 2.0f), (y - ymax / 2.0f), xmax / scene->focale));
	       ray.dir = soft_normalize(rotatexyz(ray.dir, camera->rotation));
	       int i = 0;
	       while (i < scene->max_object)
	       {
					 float d;
					 if (objects && objects[i].type != TRIANGLE)
							d = intersect(&ray, objects[i], scene->znear, 1);
					 else
							d = inter_triangle(&ray, objects[i]);
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
        		color = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
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
          color = (clamp(color, 0.0f, 1.0f));
          AAcolor += color * 0.25f;
          x += 0.5f;
          if (scene->render_mod != RENDERMODE_ANTI_ALIAS)
              x += 1.0f;
        }
        y += 0.5f;
        if (scene->render_mod != RENDERMODE_ANTI_ALIAS)
            y += 1.0f;
    }
    AAcolor.w = 1.0f;
    finalcolor = (clamp(color, 0.0f, 1.0f));
    if (scene->render_mod == RENDERMODE_ANTI_ALIAS)
        finalcolor = clamp(AAcolor, 0.0f, 1.0f);

	pixel[index] = (uchar4)(finalcolor.z * 255.0f, finalcolor.y * 255.0f, finalcolor.x * 255.0f, 255.0f);
}
