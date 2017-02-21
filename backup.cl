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

#ifdef D_FAST_NORMALIZE
# define NORMALIZE(A) fast_normalize(A)
#elif defined D_BUILTIN_NORMALIZE
# define NORMALIZE(A) normalize(A)
#else
# define NORMALIZE(A) soft_normalize(A)
#endif

#ifdef D_NATIVE_SQRT
# define SQRT(A) native_sqrt(A)
#else
# define SQRT(A) sqrt(A)
#endif

#ifdef D_BUILTIN_DOT
# define DOT(A, B) dot(A, B)
#else
# define DOT(A, B) soft_dot(A, B)
#endif

#ifdef D_BUILTIN_LEN
# define LENGTH(A) length(A)
#else
# define LENGTH(A) soft_length(A)
#endif

# define ABS(n)			(n < 0 ? -n : n)

#define UCHAR4_TO_FLOAT4(val) (((float4)(val.x, val.y, val.z, val.w)) / 255.0f)
#define FLOAT4_TO_UCHAR4(val) (((uchar4)(val[0], val[0], val[0], val[0])) * 255.0f)

float3 soft_normalize(float3 vec);
float soft_dot(float3 vec_a, float3 vec_b);
float soft_length(float3 vec);

float3 soft_normalize(float3 vec)
{
	float l = SQRT(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (l == 0.0f)
		return ((float3)(0.0f, 0.0f, 0.0f));
	return (vec * (1 / l));
}

float soft_dot(float3 vec_a, float3 vec_b)
{
	return (vec_a.x * vec_b.x + vec_a.y * vec_b.y + vec_a.z * vec_b.z);
}

float soft_length(float3 vec)
{
	return SQRT(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}


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
}				t_material;

typedef struct	s_ray
{
	float3	pos;
	float3	dir;
	float	focale;
	float	deph;
	int		object;
}				t_ray;

static float4 light(t_ray *ray, __constant t_objects *objects, __constant t_light *light, __constant t_material *material, int i)
{
	//float3 impact = ray->pos + ray->dir * ray->deph;
	if (light[i].type == SPOTLIGHT)
	{

	}
	else if (light[i].type == POINTLIGHT)
	{

	}
	else if (light[i].type == DIRLIGHT)
	{

	}
	return ((float4)(1, 1, 1, 1));
}

static bool intersect(t_ray *ray, __constant t_objects *objects, float znear, int i)
{
	float t0;
	float t1;

	float3 dist = objects[i].position - ray->pos;
	if (objects[i].type == SPHERE)
	{
		float b = DOT(ray->dir, dist);
		float d = b * b - DOT(dist, dist) + objects[i].radius * objects[i].radius;
		if (d < 0.0f)
			return (false);
		d = SQRT(d);
		t0 = b - d;
		t1 = b + d;
		if (t0 > znear && t0 < ray->deph)
			ray->deph = t0;
		if (t1 > znear && t1 < ray->deph)
			ray->deph = t1;
	}
	else if (objects[i].type == PLANE)
	{
		float a = DOT(objects[i].normal, ray->dir);
		if (a < EPSILON)
			return (false);
		t0 = DOT(dist, objects[i].normal) / a;
		if (t0 > znear && t0 < ray->deph)
			ray->deph = t0;
	}
	else if (objects[i].type == CYLINDER)
	{
		float a = ray->dir.x * ray->dir.x + ray->dir.z * ray->dir.z;
		float b = (ray->dir.x * dist.x + ray->dir.z * dist.z) / a;
		float c = b * b - (((dist.x * dist.x + dist.z * dist.z) - objects[i].radius * objects[i].radius) / a);
		if (c < 0)
			return (false);
		c = SQRT(c);
		t0 = b - c;
		t1 = b + c;
		if (t0 > znear && t0 < ray->deph)
			ray->deph = t0;
		if (t1 > znear && t1 < ray->deph)
			ray->deph = t1;
	}
	else if (objects[i].type == CONE)
	{

	}
	return (true);
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
	t_ray ray;
	ray.deph = scene->zfar;
	ray.pos = (float3)(camera->position.x, camera->position.y, camera->position.z);
	ray.dir = NORMALIZE((float3)((x - xmax / 2.0f), (y - ymax / 2.0f), xmax / scene->focale));
	ray.object = -1;

	int i = 0;
	while (i < scene->max_object)
	{
		if (intersect(&ray, objects, scene->znear, i))
			ray.object = i;
		i++;
	}
	if (ray.object >= 0)
	{
		float4 color = (float4)(1, 1, 1, 1);
		i = 0;
		while (i < scene->max_light)
		{
			color = color * light(&ray, objects, lights, materials, i);
			i++;
		}
		pixel[index] = (uchar4)(color.x * 255.0f, color.y * 255.0f, color.z * 255.0f, color.w * 255.0f);
	}
	else
	{
		pixel[index] = (uchar4)(0, 0, 0, 255);
	}
}
