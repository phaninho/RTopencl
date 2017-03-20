/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 20:25:43 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/17 13:03:27 by qhonore          ###   ########.fr       */
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

void	draw_vec3(VEC3 vec, t_vec2i p, int c, int space)
{
	draw_number(vec.x, p.x, p.y, c);
	draw_number(vec.y, p.x + space, p.y, c);
	draw_number(vec.z, p.x + (space * 2), p.y, c);
}

void	draw_vec4(VEC4 vec, t_vec2i p, int c, int space)
{
	draw_number(vec.x, p.x, p.y, c);
	draw_number(vec.y, p.x + space, p.y, c);
	draw_number(vec.z, p.x + (space * 2), p.y, c);
	draw_number(vec.w, p.x + (space * 3), p.y, c);
}
