#define PI 3.14159265359f
#define EPSILON 0.00001f
#define SCENE 0
#define CAMERA 1
#define SPHERE 2
#define PLANE 3
#define CYLINDER 4
#define CONE 5
#define TRIANGLE 6
#define SPOTLIGHT 7
#define POINTLIGHT 8
#define DIRLIGHT 9
#define MATERIAL 10
#define TEXTURE 11
#define RENDER 12
// Pour le parser et le CL du mod de rendu
#define RENDERMODE_SEPIA 13
#define RENDERMODE_GRIS 14
#define RENDERMODE_FILTER 15
#define RENDERMODE_ADD 16

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
	else if (objects.type == CYLINDER)
	{
		float3 nor = impact - objects.position;
		nor.y = 0;
		return (soft_normalize(nor));
	}
	else if (objects.type == CONE)
	{
		float3 nor = impact - objects.position;
		nor.y = -0.01f * nor.y;
		return (soft_normalize(nor));
	}
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
		specular = specular_coeff * light.color;
*/

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
	float4	linearColor = (/*specular */+ ambient + attenuation) * (diffuse /*+ specular*/);
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

static float intersect(t_ray *ray, const t_objects objects, const float znear, const int enable)
{
	float3 dist;
	float a, b, c;
	float solve;
	float t0, t1;

	dist = ray->pos - objects.position;
	float3 rdir = soft_normalize(rotatexyz(ray->dir, objects.rotation));
	//float3 rdir = ray->dir;
	if (objects.type == SPHERE)
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
	else if (objects.type == CYLINDER)
	{
		c = dist.x * dist.x + dist.z * dist.z - objects.radius * objects.radius;
 		if (enable && c < EPSILON) // Culling face
 			return (FLT_MAX);
		a = rdir.x * rdir.x + rdir.z * rdir.z;
		b = rdir.x * dist.x + rdir.z * dist.z;
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
	return (deph_min(t0, t1));
}

static float	shadow(t_ray ray, const t_light light, __constant t_objects *objects, __constant t_scene *scene)
{
	t_ray  ray_light;
	int i = -1;

	ray_light.object = -1;
	ray_light.pos = ray.pos + ray.dir * ray.deph;
	if (light.type == POINTLIGHT)
		ray_light.dir = soft_normalize(light.position - ray_light.pos);
	else if (light.type == SPOTLIGHT)
		ray_light.dir = soft_normalize(light.direction);
	else if (light.type == DIRLIGHT)
		ray_light.dir = soft_normalize(light.direction);
	ray_light.deph = sqrt(soft_dot(ray_light.dir, ray_light.dir));//scene->zfar;
	while (++i < scene->max_object)
	{
		if (i != ray_light.object)
		{
			float d  = intersect(&ray_light, objects[i], EPSILON, 1);
			if (d >= EPSILON && d < ray_light.deph)
			{
				ray_light.deph = d;
				ray_light.object = i;
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
