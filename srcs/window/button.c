/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/08 13:05:02 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/14 16:59:24 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

t_button	g_buttons[] =
{
	{1, button_render_mod, "SEPIA", {WIN_W - 290, 70, 100, 50}},
	{2, button_render_mod, "GREY", {WIN_W - 185, 70, 100, 50}},
	{3, button_render_mod, "FILTER", {WIN_W - 290, 125, 100, 50}},
	{4, button_render_mod, "ADD", {WIN_W - 185, 125, 100, 50}},
	{5, button_lights, "-", {WIN_W - 130, 230, 20, 20}},
	{6, button_lights, "+", {WIN_W - 70, 230, 20, 20}},
	{7, button_lights, "-", {WIN_W - 130, 260, 20, 20}},
	{8, button_lights, "+", {WIN_W - 70, 260, 20, 20}},
	{9, button_objects, "<", {WIN_W - 190, 320, 20, 20}},
	{10, button_objects, ">", {WIN_W - 130, 320, 20, 20}},
	{11, button_objects, "-", {WIN_W - 190, 380, 20, 20}},
	{12, button_objects, "+", {WIN_W - 90, 380, 20, 20}},
	{13, button_objects, "-", {WIN_W - 190, 400, 20, 20}},
	{14, button_objects, "+", {WIN_W - 90, 400, 20, 20}},
	{15, button_objects, "-", {WIN_W - 190, 420, 20, 20}},
	{16, button_objects, "+", {WIN_W - 90, 420, 20, 20}},
	{0, NULL, NULL, {0, 0, 0, 0}}
};

t_slider	g_sliders[] =
{
	{1, slider_render_mod, {WIN_W - 80, 175, 15, -105}, 100, 1, RED},
	{2, slider_render_mod, {WIN_W - 60, 175, 15, -105}, 100, 1, GREEN},
	{3, slider_render_mod, {WIN_W - 40, 175, 15, -105}, 100, 1, BLUE},
	{0, NULL, {0, 0, 0, 0}, 0, 0, 0}
};

t_button	*get_button(int i)
{
	return (&(g_buttons[i]));
}

t_slider	*get_slider(int i)
{
	return (&(g_sliders[i]));
}

void		exec_button(int i, int type)
{
	if (type == ONOFF)
		g_buttons[i].exec(&(g_buttons[i]));
	else if (type == SLIDER)
		g_sliders[i].exec(&(g_sliders[i]));
}

int			in_rect(SDL_Rect rec, int x, int y)
{
	if (rec.w < 0 && rec.h > 0 &&
	x > rec.x + rec.w && x <= rec.x && y > rec.y && y <= rec.y + rec.h)
		return (1);
	if (rec.w < 0 && rec.h < 0 &&
	x > rec.x + rec.w && x <= rec.x && y > rec.y + rec.h && y <= rec.y)
		return (1);
	if (rec.w > 0 && rec.h < 0 &&
	x > rec.x && x <= rec.x + rec.w && y > rec.y + rec.h && y <= rec.y)
		return (1);
	if (rec.w > 0 && rec.h > 0 &&
	x > rec.x && x <= rec.x + rec.w && y > rec.y && y <= rec.y + rec.h)
		return (1);
	return (0);
}

void		button_clicked(int x, int y)
{
	int		i;

	i = -1;
	while (g_buttons[++i].exec)
		if (in_rect(g_buttons[i].rect, x, y))
			exec_button(i, ONOFF);
	i = -1;
	while (g_sliders[++i].exec)
		if (in_rect(g_sliders[i].rect, x, y))
			exec_button(i, SLIDER);
}
