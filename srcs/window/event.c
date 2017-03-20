/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface_event.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/16 12:51:00 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/20 17:49:34 by qhonore          ###   ########.fr       */
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

static void	update_vec3(t_window *win, VEC3 *vec, int val)
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

static void	update_float(t_window *win, float *nb, int val)
{
	if (reset_key(&win->keys[SDL_SCANCODE_KP_PLUS]))
		*nb += val;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_MINUS]))
		*nb -= val;
}

// static void	update_int(t_window *win, int *nb, int val)
// {
// 	if (reset_key(&win->keys[SDL_SCANCODE_KP_PLUS]))
// 		*nb += val;
// 	else if (reset_key(&win->keys[SDL_SCANCODE_KP_MINUS]))
// 		*nb -= val;
// }

static void	update_objects(t_window *win, t_button *button,\
														t_objects *obj, int val)
{
	if (button->id == 9)
		update_vec3(win, &(obj->position), val);
	else if (button->id == 10)
		update_vec3(win, &(obj->rotation), val);
	else if (button->id == 11)
		update_vec4(win, &(obj->color), val);
	else if (button->id == 12)
		update_vec3(win, &(obj->normal), val);
	else if (button->id == 13)
		update_float(win, &(obj->radius), val);
}

void		update_scene(t_window *win)
{
	t_objects	*obj;
	t_button	*button;
	t_scene		*scene;
	int			val;

	scene = &(env_get()->scene);
	obj = &(env_get()->objects[scene->obj_index]);
	val = win->keys[SDL_SCANCODE_LSHIFT] ? 10 : 1;
	if ((button = get_on_button(2)))
		update_objects(win, button, obj, val);
	if (reset_key(&win->keys[SDL_SCANCODE_LEFT]) && scene->obj_index > 0)
		scene->obj_index--;
	else if (reset_key(&win->keys[SDL_SCANCODE_RIGHT])
	&& scene->obj_index + 1 < scene->max_object)
		scene->obj_index++;
}
