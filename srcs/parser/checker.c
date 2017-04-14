/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/26 10:59:57 by qhonore           #+#    #+#             */
/*   Updated: 2017/04/14 12:45:53 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/rt_parser.h"

static void	check_scene(t_env *e, t_arg *arg)
{
	if (arg->aint[WIDTH] > 100 && arg->aint[WIDTH] < 1980)
		e->scene.width = arg->aint[WIDTH];
	if (arg->aint[HEIGHT] > 100 && arg->aint[HEIGHT] < 1080)
		e->scene.height = arg->aint[HEIGHT];
	if (arg->adbl[ZNEAR - DOUBLE] >= 0.0f)
		e->scene.znear = arg->adbl[ZNEAR - DOUBLE];
	if (arg->adbl[ZFAR - DOUBLE] >= e->scene.znear
	&& arg->adbl[ZFAR - DOUBLE] <= 100000.0f)
		e->scene.zfar = arg->adbl[ZFAR - DOUBLE];
	if (arg->adbl[FOV - DOUBLE] >= 30 && arg->adbl[FOV - DOUBLE] <= 180)
		e->scene.fov = arg->adbl[FOV - DOUBLE];
	if (arg->aint[MAXREFLECT] >= 0)
		e->scene.max_reflect = arg->aint[MAXREFLECT];
	if (arg->aint[MAXREFLECT] >= 0)
		e->scene.max_refract = arg->aint[MAXREFRACT];
}

static void	check_object(t_env *e, t_parse *p, t_arg *arg)
{
	t_objects	*o;

	o = &(e->objects[p->object]);
	o->type = p->type;
	o->position = arg->avec[POSITION - VEC];
	o->rotation = arg->avec[ROTATION - VEC];
	o->normal = check_normal(arg->avec[NORMAL - VEC]);
	o->color = check_color(arg->color[COLOR - COLOR]);
	if (arg->adbl[RADIUS - DOUBLE] >= EPSILON)
		o->radius = arg->adbl[RADIUS - DOUBLE];
	if (arg->aint[MATERIALID] >= 1
	&& arg->aint[MATERIALID] <= e->scene.max_material)
		o->material_id = arg->aint[MATERIALID];
	if (p->object + 1 != p->in_object)
		o->in_object = p->in_object;
	o->endpos = arg->avec[ENDPOS - VEC];
	o->pos2 = arg->avec[POS2 - VEC];
	o->radius2 = arg->adbl[RADIUS2 - DOUBLE];
	o->dist = arg->adbl[DIST - DOUBLE];
	p->object++;
}

static void	check_light(t_env *e, t_parse *p, t_arg *arg)
{
	t_light		*l;

	l = &(e->light[p->light]);
	l->type = p->type;
	l->position = arg->avec[POSITION - VEC];
	l->position.x += 0.01f;
	l->position.y += 0.01f;
	l->position.z += 0.01f;
	l->color = check_color(arg->color[COLOR - COLOR]);
	if (arg->adbl[ATTENUATION - DOUBLE] >= 0.1f)
		l->attenuation = arg->adbl[ATTENUATION - DOUBLE];
	l->direction = arg->avec[DIRECTION - VEC];
	if (arg->adbl[ANGLE - DOUBLE] >= .0f && arg->adbl[ANGLE - DOUBLE] <= 360.0f)
		l->angle = arg->adbl[ANGLE - DOUBLE];
	p->light++;
}

static void	check_material(t_env *e, t_parse *p, t_arg *arg)
{
	t_material	*m;

	m = &(e->material[p->mate]);
	m->ambient_color = check_color(arg->color[AMBIENT - COLOR]);
	m->specular_color = check_color(arg->color[SPECULAR - COLOR]);
	if (arg->aint[BLINN] >= 1)
		m->blinn = arg->aint[BLINN];
	if (arg->adbl[SHININESS - DOUBLE] >= 0.0f)
		m->shininess = arg->adbl[SHININESS - DOUBLE];
	if (arg->adbl[REFLECTION - DOUBLE] >= 0.0f
	&& arg->adbl[REFLECTION - DOUBLE] <= 1.0f)
		m->reflection = arg->adbl[REFLECTION - DOUBLE];
	if (arg->adbl[REFRACTION - DOUBLE] >= 0.0f
	&& arg->adbl[REFRACTION - DOUBLE] <= 1.0f)
		m->refraction = arg->adbl[REFRACTION - DOUBLE];
	if (arg->aint[PERLIN] >= 1)
		m->perlin = arg->aint[PERLIN];
	if (arg->adbl[DAMIER - DOUBLE] >= 10.0f)
		m->damier = arg->adbl[DAMIER - DOUBLE];
	if (arg->adbl[REFRACT_COEF - DOUBLE] >= 0.0f
	&& arg->adbl[REFRACT_COEF - DOUBLE] <= 1.0f)
		m->refract_coef = arg->adbl[REFRACT_COEF - DOUBLE];
	p->mate++;
}

void		check_values(t_env *e, t_parse *p, t_arg *arg)
{
	if (p->type == SCENE)
		check_scene(e, arg);
	else if (p->type == RENDER)
		check_render(e, arg);
	else if (p->type == CAMERA)
	{
		e->camera.position = arg->avec[POSITION - VEC];
		e->camera.rotation = arg->avec[ROTATION - VEC];
	}
	else if (p->type >= OBJECTS && p->type <= END_OBJECTS)
		check_object(e, p, arg);
	else if (p->type >= LIGHTS && p->type <= END_LIGHTS)
		check_light(e, p, arg);
	else if (p->type == MATERIAL)
		check_material(e, p, arg);
}
