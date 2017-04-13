/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeexpert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/12 13:41:30 by jeexpert          #+#    #+#             */
/*   Updated: 2017/04/13 22:36:13 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include <SDL2/SDL_Thread.h>
#include <math.h>
#include "window/window.h"
#include "raytracer/rt_env.h"
#include "utils/rt_math.h"
#include "thread/thread_SDL.h"
#include <stdio.h>

void		updatekeykey(t_env *env, t_window *win, VEC3 dir)
{
	if (win->keys[SDL_SCANCODE_SPACE])
		env->camera.position.y -= win->move_speed;
	if (win->keys[SDL_SCANCODE_LCTRL])
		env->camera.position.y += win->move_speed;
}

void		updatekey(t_env *env, t_window *win, VEC3 dir)
{
	win->move_speed = (win->keys[SDL_SCANCODE_LSHIFT] ? 10 : 1);
	updatekeykey(env, win, dir);
	if (win->keys[SDL_SCANCODE_W])
	{
		dir = forward(&(env->camera.rotation));
		env->camera.position.x += dir.x * win->move_speed;
		env->camera.position.y += dir.y * win->move_speed;
		env->camera.position.z += dir.z * win->move_speed;
	}
	if (win->keys[SDL_SCANCODE_S])
	{
		dir = forward(&(env->camera.rotation));
		env->camera.position.x += dir.x * -win->move_speed;
		env->camera.position.y += dir.y * -win->move_speed;
		env->camera.position.z += dir.z * -win->move_speed;
	}
	if (win->keys[SDL_SCANCODE_D])
	{
		dir = straf(&(env->camera.rotation));
		env->camera.position.x -= dir.x * win->move_speed;
		env->camera.position.z -= dir.z * win->move_speed;
	}
}
