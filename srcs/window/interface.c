/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 13:29:01 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/29 20:12:54 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"
#include "parser/rt_parser.h"

t_interface	*get_interface(void)
{
	return (&(env_get()->interface));
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

static void	draw_object(t_window *win, t_scene *scene, t_objects *obj,\
															t_interface *inter)
{
	draw_text("OBJECT", CENTER_X - 10, 310, DARK_GREY);
	draw_number(inter->index, CENTER_X + 45, 310, DARK_GREY);
	draw_text("Type:", CENTER_X - 80, 340, DARK_GREY);
	draw_text(get_tag(obj->type), CENTER_X, 340, DARK_GREY);
	draw_vec3(obj->position, (t_vec2i){CENTER_X - 25, 370}, DARK_GREY, 50);
	draw_vec3(obj->rotation, (t_vec2i){CENTER_X - 25, 400}, DARK_GREY, 50);
	draw_vec4(to_255(obj->color), (t_vec2i){CENTER_X - 25, 430}, DARK_GREY, 40);
	draw_vec3(obj->normal, (t_vec2i){CENTER_X - 25, 460}, DARK_GREY, 50);
	draw_number(obj->radius, CENTER_X, 490, DARK_GREY);
	draw_text("Material:", CENTER_X - 90, 520, DARK_GREY);
	draw_number(obj->material_id, CENTER_X, 520, DARK_GREY);
}

void		draw_ui(void)
{
	t_window	*win;
	t_scene		*scene;
	t_env		*env;
	t_interface	*inter;

	win = window_get();
	env = env_get();
	scene = &(env->scene);
	inter = get_interface();
	draw_rect(win->interface, 0xaaaaaa);
	draw_text(scene->name, CENTER_X, 15, DARK_GREY);
	draw_filter_n_scene(win, scene);
	draw_rect((SDL_Rect){win->width - 290, 290, 280, 2}, DARK_GREY);
	if (inter->type == 1)
		draw_object(win, scene, &(env->objects[inter->index]), inter);
	draw_buttons();
}
