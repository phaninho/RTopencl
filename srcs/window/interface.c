/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 13:29:01 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/16 15:49:57 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"
#include "parser/rt_parser.h"

t_vec2i		*last_click(void)
{
	static t_vec2i	pos = (t_vec2i){0, 0};

	return (&pos);
}

static void	draw_filter_n_scene(t_window *win, t_scene *scene)
{
	draw_rect((SDL_Rect){win->width - 290, 30, 280, 2}, DARK_GREY);
	draw_text("FILTERS", CENTER_X, 55, DARK_GREY);
	draw_rect((SDL_Rect){win->width - 290, 190, 280, 2}, DARK_GREY);
	draw_text("SCENE", CENTER_X, 210, DARK_GREY);
	draw_text("MAX REFLECT", win->width - 200, 240, DARK_GREY);
	draw_number(scene->max_reflect, win->width - 90, 240, DARK_GREY);
	draw_text("MAX REFRACT", win->width - 200, 270, DARK_GREY);
	draw_number(scene->max_refract, win->width - 90, 270, DARK_GREY);
}

static void	draw_object(t_window *win, t_scene *scene, t_objects *obj)
{
	draw_rect((SDL_Rect){win->width - 290, 290, 280, 2}, DARK_GREY);
	draw_text("OBJECT", CENTER_X - 10, 310, DARK_GREY);
	draw_number(scene->obj_index, CENTER_X + 45, 310, DARK_GREY);
	draw_text("Type:", CENTER_X - 80, 360, DARK_GREY);
	draw_text(get_tag(obj->type), CENTER_X, 360, DARK_GREY);
	draw_text("Position:", CENTER_X - 95, 390, DARK_GREY);
	draw_number(obj->position.x, CENTER_X - 25, 390, DARK_GREY);
	draw_number(obj->position.y, CENTER_X + 25, 390, DARK_GREY);
	draw_number(obj->position.z, CENTER_X + 75, 390, DARK_GREY);
}

void		draw_ui(void)
{
	t_window	*win;
	t_scene		*scene;
	t_env		*env;

	win = window_get();
	env = env_get();
	scene = &(env->scene);
	draw_rect(win->interface, 0xaaaaaa);
	draw_text(scene->name, CENTER_X, 15, DARK_GREY);
	draw_filter_n_scene(win, scene);
	draw_object(win, scene, &(env->objects[scene->obj_index]));
	draw_buttons();
}
