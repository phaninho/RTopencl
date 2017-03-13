/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 13:14:15 by mgallo            #+#    #+#             */
/*   Updated: 2016/12/04 12:55:36 by mgallo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "raytracer/rt_env.h"
#include <stdio.h>

t_window	*window_get(void)
{
	static t_window window;

	return (&window);
}

int			window_init(void)
{
	t_window	*win;

	env_init();
	win = window_get();
	if (win->width <= 0 || win->height <= 0)
		return (0);
	win->win = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, win->width, win->height, 0);
	win->surface = SDL_GetWindowSurface(win->win);
	win->pixels = (unsigned char *)win->surface->pixels;
	win->close_request = 0;
	win->render = NULL;
	win->update = NULL;
	win->time_tick = SDL_GetTicks();
	win->time_delta = 0.0f;
	win->move_speed = 1.0f;
	win->rot_speed = 0.75f;
	win->timer = 0;
	win->tick = 0;
	win->frame = 0;
	return (1);
}

static void	gameloop(t_window *win, t_env *env)
{
	unsigned int now;
	unsigned int elapse;

	now = SDL_GetTicks();
	elapse = now - win->time_tick;
	win->time_delta += (float)(elapse);
	win->time_tick = now;
	win->timer += elapse;
	if (win->time_delta >= TICK_PER_MS)
	{
		win->time_delta -= TICK_PER_MS;
		if (win->update != NULL)
			(*win->update)();
		win->tick++;
	}
	else if (win->render != NULL)
		(*win->render)();
	win->frame++;
	if (win->timer >= 1000)
	{
		if (env->scene.render_debug)
			printf("FPS: %d, UPS: %d\n", win->frame, win->tick);
		win->tick = 0;
		win->frame = 0;
		win->timer -= 1000;
	}
}

void		window_loop(void)
{
	t_window	*win;
	t_env		*env;
	SDL_Event	event;

	win = window_get();
	env = env_get();
	while (!win->close_request)
	{
		mouse_motion(0, 0);
		while (SDL_PollEvent(&event))
		{
			if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				win->close_request = 1;
			if (event.type == SDL_KEYDOWN)
				win->keys[event.key.keysym.scancode] = 1;
			if (event.type == SDL_KEYUP)
				win->keys[event.key.keysym.scancode] = 0;
			if (event.type == SDL_MOUSEMOTION)
				mouse_motion(event.motion.xrel, event.motion.yrel);
			if (event.type == SDL_MOUSEBUTTONDOWN)
				mouse_grab();
		}
		gameloop(win, env);
		SDL_UpdateWindowSurface(win->win);
	}
}

void		window_end(void)
{
	t_window	*win;

	win = window_get();
	if (win->win != NULL)
		SDL_DestroyWindow(win->win);
	SDL_Quit();
}
