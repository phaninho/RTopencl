/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/16 12:51:00 by qhonore           #+#    #+#             */
/*   Updated: 2017/04/10 12:42:47 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"
#include "parser/rt_parser.h"

static void		update_objects(t_window *win, t_button *button,\
													t_objects *obj, float val)
{
	if (button->id == 1)
		update_vec3(win, &(obj->position), val);
	else if (button->id == 2)
		update_vec3(win, &(obj->rotation), val);
	else if (button->id == 3)
		update_vec4(win, &(obj->color), val);
	else if (button->id == 5)
		check_float(update_float(win, &(obj->radius), val), 10000.0f, EPSILON);
}

static void		update_lights(t_window *win, t_button *button,\
														t_light *lgt, float val)
{
	if (button->id == 1)
		update_vec3(win, &(lgt->position), val);
	else if (button->id == 2)
		check_vec3(update_vec3(win, &(lgt->direction), 1),\
						(VEC3){1.0f, 1.0f, 1.0f}, (VEC3){-1.0f, -1.0f, -1.0f});
	else if (button->id == 3)
		update_vec4(win, &(lgt->color), val);
	else if (button->id == 4)
		check_float(update_float(win, &(lgt->attenuation), val / 100), 1.0f,\
																		0.0f);
	else if (button->id == 5)
		check_float(update_float(win, &(lgt->angle), val), 360.0f, 0.0f);
}

static void		update_mates(t_window *win, t_button *button,\
													t_material *mat, float val)
{
	if (button->id == 1)
		update_vec4(win, &(mat->ambient_color), val);
	else if (button->id == 2)
		update_vec4(win, &(mat->specular_color), val);
	else if (button->id == 3)
		check_int(update_int(win, &(mat->blinn), 1), 1, 0);
	else if (button->id == 4)
		check_float(update_float(win, &(mat->shininess), val), 1000.0f, 0.0f);
	else if (button->id == 5)
		check_float(update_float(win, &(mat->reflection), val / 100), 0.5f,\
																		0.0f);
	else if (button->id == 6)
		check_float(update_float(win, &(mat->refraction), val / 100), 1.0f,\
																		0.0f);
	else if (button->id == 7)
		check_float(update_float(win, &(mat->refract_coef), val / 100), 1.0f,\
																		0.0f);
	else if (button->id == 8)
		check_float(update_float(win, &(mat->damier), val * 10), 1000.0f, 0.0f);
	else if (button->id == 10)
		check_int(update_int(win, &(mat->perlin), 1), 1, 0);
}

void			update_scene(t_window *win)
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
