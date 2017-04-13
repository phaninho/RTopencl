/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 13:29:01 by qhonore           #+#    #+#             */
/*   Updated: 2017/04/13 22:53:04 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"
#include "parser/rt_parser.h"

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

static void	draw_object(t_window *win, t_objects *objects, t_interface *inter)
{
	t_objects	*obj;

	draw_text("OBJECT", CENTER_X - 10, 310, DARK_GREY);
	draw_number(inter->index, CENTER_X + 45, 310, DARK_GREY);
	if (!objects)
		return ;
	obj = &objects[inter->index];
	draw_text("Type:", CENTER_X - 80, 340, DARK_GREY);
	draw_text(get_tag(obj->type), CENTER_X, 340, DARK_GREY);
	draw_vec3(obj->position, (t_vec2i){CENTER_X - 25, 370}, DARK_GREY, 50);
	draw_vec4(to_255(obj->color), (t_vec2i){CENTER_X - 25, 400}, DARK_GREY, 40);
	draw_number(obj->radius, CENTER_X, 430, DARK_GREY);
	draw_text("Material:", CENTER_X - 90, 460, DARK_GREY);
	draw_number(obj->material_id, CENTER_X, 460, DARK_GREY);
}

static void	draw_light(t_window *win, t_light *light, t_interface *inter)
{
	t_light	*lgt;

	draw_text("LIGHT", CENTER_X - 10, 310, DARK_GREY);
	draw_number(inter->index, CENTER_X + 45, 310, DARK_GREY);
	if (!light)
		return ;
	lgt = &light[inter->index];
	draw_text("Type:", CENTER_X - 60, 340, DARK_GREY);
	draw_text(get_tag(lgt->type), CENTER_X + 20, 340, DARK_GREY);
	draw_vec3(lgt->position, (t_vec2i){CENTER_X - 5, 370}, DARK_GREY, 50);
	draw_vec3(lgt->direction, (t_vec2i){CENTER_X - 5, 400}, DARK_GREY, 50);
	draw_vec4(to_255(lgt->color), (t_vec2i){CENTER_X - 5, 430}, DARK_GREY, 40);
	draw_number(lgt->attenuation * 100, CENTER_X + 20, 460, DARK_GREY);
	draw_number(lgt->angle, CENTER_X + 20, 490, DARK_GREY);
}

static void	draw_material(t_window *win, t_material *materials,\
															t_interface *inter)
{
	t_material	*mat;

	draw_text("MATERIAL", CENTER_X - 10, 310, DARK_GREY);
	draw_number(inter->index, CENTER_X + 45, 310, DARK_GREY);
	if (!materials)
		return ;
	mat = &materials[inter->index];
	draw_vec4(to_255(mat->ambient_color), (t_vec2i){CENTER_X - 5, 340},\
																DARK_GREY, 40);
	draw_vec4(to_255(mat->specular_color), (t_vec2i){CENTER_X - 5, 370},\
																DARK_GREY, 40);
	draw_number(mat->blinn, CENTER_X + 20, 400, DARK_GREY);
	draw_number(mat->shininess, CENTER_X + 20, 430, DARK_GREY);
	draw_number(mat->reflection * 100, CENTER_X + 20, 460, DARK_GREY);
	draw_number(mat->refraction * 100, CENTER_X + 20, 490, DARK_GREY);
	draw_number(mat->refract_coef * 100, CENTER_X + 120, 490, DARK_GREY);
	draw_number(mat->damier, CENTER_X + 20, 520, DARK_GREY);
	draw_number(mat->perlin, CENTER_X + 20, 550, DARK_GREY);
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
	if (inter->type == TYPE_OBJECT)
		draw_object(win, env->objects, inter);
	else if (inter->type == TYPE_LIGHT)
		draw_light(win, env->light, inter);
	else if (inter->type == TYPE_MATERIAL)
		draw_material(win, env->material, inter);
	draw_buttons();
}
