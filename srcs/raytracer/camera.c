/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 09:30:05 by mgallo            #+#    #+#             */
/*   Updated: 2017/03/11 12:36:33 by qhonore          ###   ########.fr       */
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

static int	thread_caillou(void *param)
{
	char		*tmp;
	t_window	*win;

	win = (t_window*)param;
	tmp = check_name_screen(win);
	if (!SDL_SaveBMP(win->surface, tmp))
		free(tmp);
	printf("PRINTSCREEN\n");
	return (0);
}

static VEC3	forward(const VEC3 *rot)
{
	VEC3 vec;
	float y[2];
	float len;

	vec = (VEC3){{0, 0, 1}};
	return (rotatexyz(vec, *rot));
}

static VEC3	straf(const VEC3 *rot)
{
	VEC3 vec;
	float y[2];
	float len;

	vec = (VEC3){{0, 0, 0}};
	y[0] = cosf(TO_RADIANS(rot->y));
	y[1] = sinf(TO_RADIANS(rot->y));
	vec.x = -y[0];
	vec.z = y[1];
	len = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec.x /= len;
	vec.y /= len;
	vec.z /= len;
	return (vec);
}

static void	updatekey(t_env *env, t_window *win, VEC3 dir)
{
	win->move_speed = (win->keys[SDL_SCANCODE_LSHIFT] ? 10 : 1);
	if (win->keys[SDL_SCANCODE_SPACE])
		env->camera.position.y -= win->move_speed;
	if (win->keys[SDL_SCANCODE_LCTRL])
		env->camera.position.y += win->move_speed;
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

static void	updatekey2(t_env *env, t_window *win, VEC3 dir, t_threadSDL *tsdl)
{
	SDL_Thread	*thread;

	if (win->keys[SDL_SCANCODE_A])
	{
		dir = straf(&(env->camera.rotation));
		env->camera.position.x += dir.x * win->move_speed;
		env->camera.position.z += dir.z * win->move_speed;
	}
	if (win->keys[SDL_SCANCODE_P])
	{
		thread = SDL_CreateThread(thread_caillou,"test", win);
		win->keys[SDL_SCANCODE_P] = 0;
//		SDL_KillThread(thread);
	}
}

void		update(void)
{
	t_env		*env;
	VEC3		dir;
	t_window	*win;
	t_threadSDL	*tsdl;

	env = env_get();
	win = window_get();
	if (mouse_isgrab())
	{
		if (win->keys[SDL_SCANCODE_ESCAPE])
			mouse_ungrab();
		env->camera.rotation.y += win->xrel * win->rot_speed;
		env->camera.rotation.x += win->yrel * -win->rot_speed;
		if (env->camera.rotation.x > 90)
			env->camera.rotation.x = 90;
		if (env->camera.rotation.x < -90)
			env->camera.rotation.x = -90;
	}
	updatekey(env, win, dir);
	updatekey2(env, win, dir, tsdl);
}
