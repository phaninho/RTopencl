/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_env.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/26 02:29:30 by mgallo            #+#    #+#             */
/*   Updated: 2016/12/19 18:44:05 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_ENV_H
# define RT_ENV_H

# include <OpenCL/opencl.h>
# include "utils/rt_math.h"
# define EPSILON 0.00001f
# define SCENE 0
# define CAMERA 1
# define SPHERE 2
# define PLANE 3
# define CYLINDER 4
# define CONE 5
# define DISK 6
# define TRIANGLE 7
# define SPOTLIGHT 8
# define POINTLIGHT 9
# define DIRLIGHT 10
# define MATERIAL 11
# define TEXTURE 12
# define RENDER 13

// Pour le parser et le CL du mod de rendu
#define RENDERMODE_SEPIA 14
#define RENDERMODE_GRIS 15
#define RENDERMODE_FILTER 16
#define RENDERMODE_ADD 17

typedef struct	s_cam
{
	VEC3	position;
	VEC3	rotation;
}				t_cam;

typedef struct	s_objects
{
	int			type;
	VEC3		position;
	VEC3		rotation;
	VEC3		normal;
	VEC4		color;
	float		radius;
	int			material_id;
	int			texture_id;
	int			in_object;
}				t_objects;

typedef struct	s_light
{
	int			type;
	VEC3		position;
	VEC4		color;
	float		attenuation;
	VEC3		direction;
	float		angle;
}				t_light;

typedef struct	s_material
{
	VEC4		ambient_color;
	VEC4		specular_color;
	int			blinn;
	float		shininess;
	float		reflection;
	float		refraction;
}				t_material;

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
	VEC4		render_filter;
}				t_scene;

typedef struct	s_env
{
	t_scene		scene;
	t_cam		camera;
	t_objects	*objects;
	t_light		*light;
	t_material	*material;
	t_texture	*texture;
}				t_env;

t_env			*env_get(void);
void			env_init(void);
#endif
