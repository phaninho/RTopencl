/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface_event.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/16 12:51:00 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/16 16:35:45 by qhonore          ###   ########.fr       */
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

void		update_scene(t_window *win)
{
	t_objects	*obj;
	t_scene		*scene;
	int			speed;

	scene = &(env_get()->scene);
	obj = &(env_get()->objects[scene->obj_index]);
	speed = win->keys[SDL_SCANCODE_LSHIFT] ? 10 : 1;
	if (reset_key(&win->keys[SDL_SCANCODE_KP_4]))
		obj->position.x += speed;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_6]))
		obj->position.x -= speed;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_8]))
		obj->position.z -= speed;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_5]))
		obj->position.z += speed;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_PLUS]))
		obj->position.y -= speed;
	else if (reset_key(&win->keys[SDL_SCANCODE_KP_MINUS]))
		obj->position.y += speed;
	else if (reset_key(&win->keys[SDL_SCANCODE_LEFT]) && scene->obj_index > 0)
		scene->obj_index--;
	else if (reset_key(&win->keys[SDL_SCANCODE_RIGHT])
	&& scene->obj_index + 1 < scene->max_object)
		scene->obj_index++;
}
