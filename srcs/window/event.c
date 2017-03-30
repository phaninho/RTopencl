/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/16 12:51:00 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/30 17:23:24 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"
#include "parser/rt_parser.h"

static int	reset_key(int *key)
{
	if (*key)
		return ((*key)--);
	return (0);
}

static void	update_vec3(t_window *win, VEC3 *vec, float val)
{
	if (reset_key(&win->keys[SDL_SCANCODE_KP_4]))
		vec->x += val;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_6]))
		vec->x -= val;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_8]))
		vec->z -= val;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_5]))
		vec->z += val;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_PLUS]))
		vec->y -= val;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_MINUS]))
		vec->y += val;
}

static void	update_vec4(t_window *win, VEC4 *vec, float val)
{
	if (reset_key(&win->keys[SDL_SCANCODE_KP_4]))
		vec->x = (vec->x - val / 255.0 < 0 ? 0 : vec->x - val / 255.0);
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_6]))
		vec->x = (vec->x + val / 255.0 > 1 ? 1 : vec->x + val / 255.0);
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_8]))
		vec->z = (vec->z + val / 255.0 > 1 ? 1 : vec->z + val / 255.0);
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_5]))
		vec->z = (vec->z - val / 255.0 < 0 ? 0 : vec->z - val / 255.0);
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_PLUS]))
		vec->y = (vec->y + val / 255.0 > 1 ? 1 : vec->y + val / 255.0);
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_MINUS]))
		vec->y = (vec->y - val / 255.0 < 0 ? 0 : vec->y - val / 255.0);
}

static void	update_float(t_window *win, float *nb, float val)
{
	if (reset_key(&win->keys[SDL_SCANCODE_KP_PLUS]))
		*nb += val;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_MINUS]))
		*nb -= val;
}

static void	update_int(t_window *win, int *nb, float val)
{
	if (reset_key(&win->keys[SDL_SCANCODE_KP_PLUS]))
		*nb += val;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_MINUS]))
		*nb -= val;
}

static void	update_objects(t_window *win, t_button *button,\
													t_objects *obj, float val)
{
	if (button->id == 1)
		update_vec3(win, &(obj->position), val);
	else if (button->id == 2)
		update_vec3(win, &(obj->rotation), val);
	else if (button->id == 3)
		update_vec4(win, &(obj->color), val);
	else if (button->id == 4)
		update_vec3(win, &(obj->normal), val);
	else if (button->id == 5)
		update_float(win, &(obj->radius), val);
}

static void	update_lights(t_window *win, t_button *button,\
														t_light *lgt, float val)
{
	if (button->id == 1)
		update_vec3(win, &(lgt->position), val);
	else if (button->id == 2)
		update_vec3(win, &(lgt->direction), val);
	else if (button->id == 3)
		update_vec4(win, &(lgt->color), val);
	else if (button->id == 4)
		update_float(win, &(lgt->attenuation), val);
	else if (button->id == 5)
		update_float(win, &(lgt->angle), val);
}

static void	update_mates(t_window *win, t_button *button,\
													t_material *mat, float val)
{
	if (button->id == 1)
		update_vec4(win, &(mat->ambient_color), val);
	else if (button->id == 2)
		update_vec4(win, &(mat->specular_color), val);
	else if (button->id == 3)
		update_int(win, &(mat->blinn), val);
	else if (button->id == 4)
		update_float(win, &(mat->shininess), val);
	else if (button->id == 5)
		update_float(win, &(mat->reflection), val);
	else if (button->id == 6)
		update_float(win, &(mat->refraction), val);
	else if (button->id == 7)
		update_float(win, &(mat->refract_coef), val);
	else if (button->id == 8)
		update_int(win, &(mat->damier), val);
	else if (button->id == 9)
		update_float(win, &(mat->tile_size), val);
	else if (button->id == 10)
		update_int(win, &(mat->perlin), val);
}

void		update_scene(t_window *win)
{
	t_button	*button;
	t_scene		*scene;
	t_interface	*inter;
	float		val;

	scene = &(env_get()->scene);
	inter = get_interface();
	val = win->keys[SDL_SCANCODE_LSHIFT] ? 10.0f : 1.0f;
	if ((button = get_on_button(TYPE_OBJECT)))
		update_objects(win, button, &(env_get()->objects[inter->index]), val);
	else if ((button = get_on_button(TYPE_LIGHT)))
		update_lights(win, button, &(env_get()->light[inter->index]), val);
	else if ((button = get_on_button(TYPE_MATERIAL)))
		update_mates(win, button, &(env_get()->material[inter->index]), val);
	if (reset_key(&win->keys[SDL_SCANCODE_LEFT]) && inter->index > 0)
		inter->index--;
	else if (reset_key(&win->keys[SDL_SCANCODE_RIGHT])
	&& ((inter->type == TYPE_OBJECT && inter->index + 1 < scene->max_object)
	|| (inter->type == TYPE_LIGHT && inter->index + 1 < scene->max_light) ||
	(inter->type == TYPE_MATERIAL && inter->index + 1 < scene->max_material)))
		inter->index++;
	else if (reset_key(&win->keys[SDL_SCANCODE_UP]) && inter->type < 4)
		update_showed(inter, inter->type + 1);
	else if (reset_key(&win->keys[SDL_SCANCODE_DOWN]) && inter->type > 2)
		update_showed(inter, inter->type - 1);
}
