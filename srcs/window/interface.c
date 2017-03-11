/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 13:29:01 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/11 18:03:19 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

t_vec2i		*last_click(void)
{
	static t_vec2i	pos = (t_vec2i){0, 0};

	return (&pos);
}

static void	draw_filter_n_scene(t_window *win, t_scene *scene)
{
	draw_rect((SDL_Rect){win->width - 290, 30, 280, 2}, DARK_GREY);
	draw_text("FILTERS", win->width - win->interface.w / 2, 55, DARK_GREY);
	draw_rect((SDL_Rect){win->width - 290, 190, 280, 2}, DARK_GREY);
	draw_text("SCENE", win->width - win->interface.w / 2, 210, DARK_GREY);
	draw_text("MAX REFLECT", win->width - 200, 240, DARK_GREY);
	draw_number(scene->max_reflect, win->width - 90, 240, DARK_GREY);
	draw_text("MAX REFRACT", win->width - 200, 270, DARK_GREY);
	draw_number(scene->max_refract, win->width - 90, 270, DARK_GREY);
}

static void	draw_object(t_window *win, t_scene *scene)
{
	draw_rect((SDL_Rect){win->width - 290, 290, 280, 2}, DARK_GREY);
}

void		draw_ui(void)
{
	t_window	*win;
	t_scene		*scene;

	win = window_get();
	scene = &(env_get()->scene);
	draw_rect(win->interface, 0xaaaaaa);
	draw_text("File:", win->width - 265, 15, DARK_GREY);
	draw_text(scene->name, win->width - 190, 15, DARK_GREY);
	draw_filter_n_scene(win, scene);
	draw_object(win, scene);
	draw_buttons();
}
