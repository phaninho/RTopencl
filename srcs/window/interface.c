/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 13:29:01 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/09 13:24:45 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

void				draw_ui(void)
{
	t_window	*win;
	t_scene		*scene;

	win = window_get();
	scene = &(env_get()->scene);
	SDL_SetRenderDrawColor(win->renderer, 0xaa, 0xaa, 0xaa, 0xff);
	SDL_RenderFillRect(win->renderer, &(win->interface));
	draw_text("File:", win->width - 265, 40, DARK_GREY);
	draw_text(scene->name, win->width - 200, 40, DARK_GREY);
	draw_buttons();
}
