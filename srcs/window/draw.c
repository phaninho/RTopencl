/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 20:25:43 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/30 16:36:16 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"
#include "libft/libft.h"
#include "utils/error.h"

t_font				*get_font(void)
{
	static t_font	font;

	return (&font);
}

void				init_font(void)
{
	t_font		*font;
	t_window	*win;

	font = get_font();
	win = window_get();
	TTF_Init();
	font->font_size = 18;
	font->font = TTF_OpenFont("/Library/Fonts/Arial Bold.ttf", font->font_size);
}

static SDL_Texture	*surface_to_texture(t_window *win, t_font *font)
{
	SDL_Texture	*text;

	text = SDL_CreateTextureFromSurface(win->renderer, font->surf);
	SDL_FreeSurface(font->surf);
	return (text);
}

void				draw_text(char *str, int x, int y, int c)
{
	t_window	*win;
	t_font		*font;
	SDL_Color	color;

	win = window_get();
	font = get_font();
	color.a = !(c >> 24 & 0xff) ? 0xff : (c >> 24 & 0xff);
	color.r = c >> 16 & 0xff;
	color.g = c >> 8 & 0xff;
	color.b = c & 0xff;
	font->surf = TTF_RenderText_Solid(font->font, str, color);
	font->texture = surface_to_texture(win, font);
	SDL_QueryTexture(font->texture, NULL, NULL, &font->rect.w, &font->rect.h);
	font->rect.x = x - font->rect.w / 2;
	font->rect.y = y - font->rect.h / 2;
	SDL_RenderCopy(win->renderer, font->texture, NULL, &font->rect);
	SDL_DestroyTexture(font->texture);
}

void				draw_number(long nb, int x, int y, int c)
{
	char	*str;

	if (!(str = ft_ltoa(nb)))
		die("draw_number(): malloc failure\n");
	draw_text(str, x, y, c);
	free(str);
}
