/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 13:29:01 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/10 14:21:23 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

void	draw_rect(SDL_Rect rect, int color)
{
	t_window	*win;
	VEC4		c;

	win = window_get();
	c.w = color >> 24 & 0xff;
	c.x = color >> 16 & 0xff;
	c.y = color >> 8 & 0xff;
	c.z = color & 0xff;
	SDL_SetRenderDrawColor(win->renderer, c.x, c.y, c.z, c.w);
	SDL_RenderFillRect(win->renderer, &(rect));
}

void	draw_ui(void)
{
	t_window	*win;
	t_scene		*scene;

	win = window_get();
	scene = &(env_get()->scene);
	draw_rect(win->interface, 0xffaaaaaa);
	draw_text("File:", win->width - 265, 15, DARK_GREY);
	draw_text(scene->name, win->width - 200, 15, DARK_GREY);
	draw_rect((SDL_Rect){win->width - 290, 30, 280, 2}, 0xff505050);
	draw_text("FILTERS", win->width - win->interface.w / 2, 55, DARK_GREY);
	draw_buttons();
}
