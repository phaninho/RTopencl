/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/08 13:05:02 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/30 13:36:14 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

t_button	g_buttons[] =
{
	{1, button_render_mod, "SEPIA", {WIN_W - 290, 70, 100, 35}, 0, 1, 1},
	{2, button_render_mod, "GREY", {WIN_W - 185, 70, 100, 35}, 0, 1, 1},
	{3, button_render_mod, "FILTER", {WIN_W - 290, 110, 100, 35}, 0, 1, 1},
	{4, button_render_mod, "ADD", {WIN_W - 185, 110, 100, 35}, 0, 1, 1},
	{5, button_render_mod, "NEGATIF", {WIN_W - 290, 150, 100, 35}, 0, 1, 1},
	{6, button_render_mod, "CARTOON", {WIN_W - 185, 150, 100, 35}, 0, 1, 1},
	{1, button_scene, "-", {WIN_W - 130, 230, 20, 20}, 0, 0, 1},
	{2, button_scene, "+", {WIN_W - 70, 230, 20, 20}, 0, 0, 1},
	{3, button_scene, "-", {WIN_W - 130, 260, 20, 20}, 0, 0, 1},
	{4, button_scene, "+", {WIN_W - 70, 260, 20, 20}, 0, 0, 1},
	{1, button_objects, "Position:", {WIN_W - 290, 358, 90, 25}, 0, 2, 1},
	{2, button_objects, "Rotation:", {WIN_W - 290, 388, 90, 25}, 0, 2, 1},
	{3, button_objects, "Color:", {WIN_W - 290, 418, 90, 25}, 0, 2, 1},
	{4, button_objects, "Normal:", {WIN_W - 290, 448, 90, 25}, 0, 2, 1},
	{5, button_objects, "Radius:", {WIN_W - 290, 478, 90, 25}, 0, 2, 1},
	{6, button_objects, "<", {WIN_W - 185, 510, 20, 20}, 0, 2, 1},
	{7, button_objects, ">", {WIN_W - 135, 510, 20, 20}, 0, 2, 1},
	{1, button_lights, "Position:", {WIN_W - 290, 358, 110, 25}, 0, 3, 0},
	{2, button_lights, "Direction:", {WIN_W - 290, 388, 110, 25}, 0, 3, 0},
	{3, button_objects, "Color:", {WIN_W - 290, 418, 110, 25}, 0, 3, 0},
	{4, button_objects, "Attenuation:", {WIN_W - 290, 448, 110, 25}, 0, 3, 0},
	{5, button_objects, "Angle:", {WIN_W - 290, 478, 110, 25}, 0, 3, 0},
	{0, NULL, NULL, {0, 0, 0, 0}, 0, 0, 0}
};

t_slider	g_sliders[] =
{
	{1, slider_render_mod, {WIN_W - 80, 185, 15, -115}, 100, 1, RED, 1},
	{2, slider_render_mod, {WIN_W - 60, 185, 15, -115}, 100, 1, GREEN, 1},
	{3, slider_render_mod, {WIN_W - 40, 185, 15, -115}, 100, 1, BLUE, 1},
	{0, NULL, {0, 0, 0, 0}, 0, 0, 0, 0}
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
		if (g_buttons[i].show && in_rect(g_buttons[i].rect, x, y))
			exec_button(i, ONOFF);
	i = -1;
	while (g_sliders[++i].exec)
		if (g_sliders[i].show && in_rect(g_sliders[i].rect, x, y))
			exec_button(i, SLIDER);
}
