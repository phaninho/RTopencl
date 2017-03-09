/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/08 13:11:09 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/09 17:25:44 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_H
# define INTERFACE_H

# define RGBA (SDL_Color)
# define DARK_GREY (SDL_Color){100, 100, 100, 255}
# define WHITE (SDL_Color){240, 240, 240, 255}
# define SLIDER_RED 0xff, 0x0, 0x0, 0xff
# define SLIDER_GREEN 0x0, 0xff, 0x0, 0xff
# define SLIDER_BLUE 0x0, 0x0, 0xff, 0xff

# define BUTTON_NB 7
# define ONOFF 0
# define SLIDER 1

typedef struct s_font	t_font;
typedef struct s_button	t_button;
typedef struct s_slider	t_slider;
typedef struct s_vec2i	t_vec2i;
typedef void(*t_func)(t_button*);

struct		s_vec2i
{
	int		x;
	int		y;
};

struct		s_font
{
	uint8_t		font_size;
	TTF_Font	*font;
	SDL_Surface	*surf;
	SDL_Texture	*texture;
	SDL_Rect	rect;
};

struct		s_button
{
	int			id;
	int			type;
	t_func		exec;
	char		*name;
	SDL_Rect	rect;
	int			val;
};

t_font		*get_font(void);
void		init_font(void);
void		draw_ui(void);
void		draw_text(char *str, int x, int y, SDL_Color color);
void		draw_rect(int x, int y, int w, int h);

t_button	*get_button(int i);
t_vec2i		*last_click(void);
void		exec_button(int i);
int			in_rect(SDL_Rect rec, int x, int y);
int			button_clicked(int x, int y);
void		draw_buttons(void);
void		slider_move(t_button *button);

void		button_render_mod(t_button *button);
void		slider_render_mod(t_button *button);

#endif
