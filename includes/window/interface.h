/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/08 13:11:09 by qhonore           #+#    #+#             */
/*   Updated: 2017/04/01 19:06:28 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_H
# define INTERFACE_H

# include "raytracer/rt_env.h"

# define DARK_GREY 0x646464
# define WHITE 0xf0f0f0
# define RED 0xff0000
# define GREEN 0x00ff00
# define BLUE 0x0000ff
# define DEFCOLOR 0xeeeeee

# define ONOFF 0
# define SLIDER 1

# define CENTER_X (win->width - win->interface.w / 2)

# define TYPE_FILTER 1
# define TYPE_OBJECT 2
# define TYPE_LIGHT 3
# define TYPE_MATERIAL 4

typedef struct s_font	t_font;
typedef struct s_button	t_button;
typedef struct s_slider	t_slider;
typedef struct s_vec2i	t_vec2i;
typedef void(*t_func)(void*);

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
	t_func		exec;
	char		*name;
	SDL_Rect	rect;
	int			on;
	int			type;
	int			show;
};

struct		s_slider
{
	int			id;
	t_func		exec;
	SDL_Rect	rect;
	int			val;
	int			vert;
	int			color;
	int			show;
};

t_interface	*get_interface(void);
t_font		*get_font(void);
void		init_font(void);
void		draw_ui(void);
void		draw_text(char *str, int x, int y, int c);
void		draw_number(long nb, int x, int y, int c);
void		draw_vec3(VEC3 vec, t_vec2i p, int c, int space);
void		draw_vec4(VEC4 vec, t_vec2i p, int c, int space);
void		draw_rect(SDL_Rect rect, int color);

t_button	*get_button(int i);
t_slider	*get_slider(int i);
t_vec2i		*last_click(void);
VEC4		to_255(VEC4 color);
void		exec_button(int i, int type);
int			in_rect(SDL_Rect rec, int x, int y);
void		button_clicked(int x, int y);
void		draw_buttons(void);
void		slider_move(t_slider *button);
void		update_onoff(t_button *button);
void		update_showed(t_interface *inter, int type);
t_button	*get_on_button(int type);

int			reset_key(int *key);
void		check_int(int *nb, int max, int min);
void		check_float(float *nb, float max, float min);
void		check_vec3(VEC3 *vec, VEC3 max, VEC3 min);
int			*update_int(t_window *win, int *nb, float val);
float		*update_float(t_window *win, float *nb, float val);
VEC4		*update_vec4(t_window *win, VEC4 *vec, float val);
VEC3		*update_vec3(t_window *win, VEC3 *vec, float val);

void		button_render_mod(void *button);
void		slider_render_mod(void *button);
void		button_scene(void *param);
void		button_objects(void *param);
void		button_lights(void *param);
void		button_materials(void *param);

void		update_scene(t_window *win);

#endif
