/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/28 16:14:13 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/29 15:28:15 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/rt_parser.h"

void	init_scene(t_env *e)
{
	e->scene.width = 640;
	e->scene.height = 480;
	e->scene.znear = 0.0f;
	e->scene.zfar = 1000.0f;
	e->scene.fov = 70.0f;
	e->scene.max_reflect = 5;
	e->scene.max_refract = 5;
	e->scene.max_object = 0;
	e->scene.max_material = 0;
	e->scene.max_light = 0;
	e->scene.max_texture = 0;
	e->scene.render_debug = 1;
	e->scene.render_mod = 0;
	e->scene.render_filter = (VEC4){{1.0f, 1.0f, 1.0f, 1.0f}};
}

void	init_camera(t_env *e)
{
	e->camera.position = (VEC3){{0.0f, 0.0f, 0.0f}};
	e->camera.rotation = (VEC3){{0.0f, 0.0f, 0.0f}};
}

void	init_objects(t_env *e, int max)
{
	while (--max >= 0)
	{
		e->objects[max].type = SPHERE;
		e->objects[max].position = (VEC3){{0.0f, 0.0f, 0.0f}};
		e->objects[max].rotation = (VEC3){{0.0f, 0.0f, 0.0f}};
		e->objects[max].normal = (VEC3){{0.0f, -1.0f, 0.0f}};
		e->objects[max].color = (VEC4){{255.0f, 255.0f, 255.0f, 255.0f}};
		e->objects[max].radius = 1.0f;
		e->objects[max].endpos = (VEC3){{1.0f, 1.0f, 1.0f}};
		e->objects[max].radius2 = 0.5f;
		e->objects[max].a = 0.0f;
		e->objects[max].b = 0.0f;
		e->objects[max].c = 0.0f;
		e->objects[max].d = 0.0f;
		e->objects[max].dist = 0.0f;
		e->objects[max].material_id = 0;
		e->objects[max].texture_id = 0;
		e->objects[max].in_object = 0;
	}
}

void	init_lights(t_env *e, int max)
{
	while (--max >= 0)
	{
		e->light[max].type = POINTLIGHT;
		e->light[max].position = (VEC3){{0.0f, 0.0f, 0.0f}};
		e->light[max].color = (VEC4){{255.0f, 255.0f, 255.0f, 255.0f}};
		e->light[max].attenuation = 0.0f;
		e->light[max].direction = (VEC3){{0.0f, 0.0f, 0.0f}};
		e->light[max].angle = 0.0f;
	}
}

void	init_materials(t_env *e, int max)
{
	while (--max >= 0)
	{
		e->material[max].ambient_color = (VEC4){{255, 255, 255, 255}};
		e->material[max].specular_color = (VEC4){{255, 255, 255, 255}};
		e->material[max].blinn = 0;
		e->material[max].shininess = 64.0f;
		e->material[max].reflection = 0.0f;
		e->material[max].refraction = 0.0f;
		e->material[max].damier = 0;
		e->material[max].tile_size = 10.0f;
		e->material[max].perlin = 0;
		e->material[max].refract_coef = 0.0f;
	}
}
