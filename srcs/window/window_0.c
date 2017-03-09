/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 13:14:15 by mgallo            #+#    #+#             */
/*   Updated: 2017/03/08 16:36:12 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"
#include<stdio.h>

static void	gameloop(t_window *win, t_env *env)
{
	const unsigned int	now = SDL_GetTicks();
	unsigned int		elapse;

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

static void	display_window(t_window *win)
{
	SDL_Texture	*texture;

	texture = SDL_CreateTextureFromSurface(win->renderer, win->surface);
	SDL_RenderCopy(win->renderer, texture, NULL, NULL);
	SDL_DestroyTexture(texture);
	draw_ui();
	SDL_RenderPresent(win->renderer);
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
				mouse_grab(event.motion.x, event.motion.y);
		}
		gameloop(win, env);
		display_window(win);
	}
}
