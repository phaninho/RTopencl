/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 13:14:15 by mgallo            #+#    #+#             */
/*   Updated: 2017/03/02 20:21:46 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "raytracer/rt_env.h"

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
	win->surface = SDL_CreateRGBSurface(0, win->width, win->height,\
							32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	win->renderer = SDL_CreateRenderer(win->win, -1, SDL_RENDERER_ACCELERATED);
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

void		window_end(void)
{
	t_window	*win;

	win = window_get();
	if (win->win != NULL)
		SDL_DestroyWindow(win->win);
	SDL_Quit();
}
