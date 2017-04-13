/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/08 13:05:02 by qhonore           #+#    #+#             */
/*   Updated: 2017/04/13 22:52:29 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

t_button	g_buttons[] =
{
	{1, button_render_mod, "SEPIA", {WIN_W - 290, 70, 100, 25}, 0, 1, 1},
	{2, button_render_mod, "GREY", {WIN_W - 185, 70, 100, 25}, 0, 1, 1},
	{3, button_render_mod, "FILTER", {WIN_W - 290, 100, 100, 25}, 0, 1, 1},
	{4, button_render_mod, "ADD", {WIN_W - 185, 100, 100, 25}, 0, 1, 1},
	{5, button_render_mod, "NEGATIF", {WIN_W - 290, 130, 100, 25}, 0, 1, 1},
	{6, button_render_mod, "CARTOON", {WIN_W - 185, 130, 100, 25}, 0, 1, 1},
	{7, button_render_mod, "ANTI-ALIASING", \
		{WIN_W - 290, 160, 205, 25}, 0, 1, 1},
	{1, button_scene, "-", {WIN_W - 130, 230, 20, 20}, 0, 0, 1},
	{2, button_scene, "+", {WIN_W - 70, 230, 20, 20}, 0, 0, 1},
	{3, button_scene, "-", {WIN_W - 130, 260, 20, 20}, 0, 0, 1},
	{4, button_scene, "+", {WIN_W - 70, 260, 20, 20}, 0, 0, 1},
	{1, button_objects, "Position:", {WIN_W - 290, 358, 90, 25}, 0, 2, 1},
	{3, button_objects, "Color:", {WIN_W - 290, 388, 90, 25}, 0, 2, 1},
	{5, button_objects, "Radius:", {WIN_W - 290, 418, 90, 25}, 0, 2, 1},
	{6, button_objects, "<", {WIN_W - 185, 450, 20, 20}, 0, 2, 1},
	{7, button_objects, ">", {WIN_W - 135, 450, 20, 20}, 0, 2, 1},
	{1, button_lights, "Position:", {WIN_W - 290, 358, 110, 25}, 0, 3, 0},
	{2, button_lights, "Direction:", {WIN_W - 290, 388, 110, 25}, 0, 3, 0},
	{3, button_objects, "Color:", {WIN_W - 290, 418, 110, 25}, 0, 3, 0},
	{4, button_objects, "Attenuation:", {WIN_W - 290, 448, 110, 25}, 0, 3, 0},
	{5, button_objects, "Angle:", {WIN_W - 290, 478, 110, 25}, 0, 3, 0},
	{1, button_materials, "Ambient:", {WIN_W - 290, 328, 110, 25}, 0, 4, 0},
	{2, button_materials, "Specular:", {WIN_W - 290, 358, 110, 25}, 0, 4, 0},
	{3, button_materials, "Blinn:", {WIN_W - 290, 388, 110, 25}, 0, 4, 0},
	{4, button_materials, "Shininess:", {WIN_W - 290, 418, 110, 25}, 0, 4, 0},
	{5, button_materials, "Reflection:", {WIN_W - 290, 448, 110, 25}, 0, 4, 0},
	{6, button_materials, "Refraction:", {WIN_W - 290, 478, 110, 25}, 0, 4, 0},
	{7, button_materials, "Coef:", {WIN_W - 110, 478, 50, 25}, 0, 4, 0},
	{8, button_materials, "Damier:", {WIN_W - 290, 508, 110, 25}, 0, 4, 0},
	{10, button_materials, "Perlin:", {WIN_W - 290, 538, 110, 25}, 0, 4, 0},
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
