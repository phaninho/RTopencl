/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/08 13:05:02 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/09 17:29:42 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

t_button	g_buttons[] =
{
	{1, ONOFF, button_render_mod, "SEPIA", {WIN_W - 280, 100, 100, 50}, 0},
	{2, ONOFF, button_render_mod, "GREY", {WIN_W - 170, 100, 100, 50}, 0},
	{3, ONOFF, button_render_mod, "FILTER", {WIN_W - 280, 160, 100, 50}, 0},
	{4, ONOFF, button_render_mod, "ADD", {WIN_W - 170, 160, 100, 50}, 0},
	{1, SLIDER, slider_render_mod, "r mod", {WIN_W - 280, 220, 210, 8}, 50},
	{2, SLIDER, slider_render_mod, "g mod", {WIN_W - 280, 230, 210, 8}, 50},
	{3, SLIDER, slider_render_mod, "b mod", {WIN_W - 280, 240, 210, 8}, 50}
};

t_button	*get_button(int i)
{
	if (i >= 0 && i < BUTTON_NB)
		return (&(g_buttons[i]));
	return (NULL);
}

t_vec2i		*last_click(void)
{
	static t_vec2i	pos = (t_vec2i){0, 0};

	return (&pos);
}

void		exec_button(int i)
{
	if (i >= 0 && i < BUTTON_NB)
		g_buttons[i].exec(&(g_buttons[i]));
}

int			in_rect(SDL_Rect rec, int x, int y)
{
	if (x > rec.x && x <= rec.x + rec.w && y > rec.y && y <= rec.y + rec.h)
		return (1);
	return (0);
}

int			button_clicked(int x, int y)
{
	int		i;

	i = -1;
	while (++i < BUTTON_NB)
		if (in_rect(g_buttons[i].rect, x, y))
			return (i);
	return (-1);
}
