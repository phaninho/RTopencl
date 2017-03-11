/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 20:25:43 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/11 16:52:12 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "raytracer/rt_env.h"
#include "window/window.h"
#include "window/interface.h"

void	draw_rect(SDL_Rect rect, int color)
{
	t_window	*win;
	VEC4		c;

	win = window_get();
	c.w = (color >> 24 & 0xff);
	if (!c.w)
		c.w = 0xff;
	c.x = color >> 16 & 0xff;
	c.y = color >> 8 & 0xff;
	c.z = color & 0xff;
	SDL_SetRenderDrawColor(win->renderer, c.x, c.y, c.z, c.w);
	SDL_RenderFillRect(win->renderer, &(rect));
}
