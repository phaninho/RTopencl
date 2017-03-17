/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/06 09:01:51 by mgallo            #+#    #+#             */
/*   Updated: 2016/10/13 19:42:54 by mgallo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

# include "maths/camera.h"
# include "objects/objects.h"

typedef struct	s_image
{
	void	*addr;
	int		bpp;
	int		width;
	int		endian;
	char	*data;
}				t_image;

typedef struct	s_window
{
	void		*mlx;
	void		*win;
	int			width;
	int			height;
	t_image		*screen;
	int			error;
	t_cam		*camera;
	t_ray		*ray;
	t_objects	**objects;
}				t_window;

t_window		*window_new(void);
void			window_free(t_window **w);
void			window_create(t_window *w, char *title);
t_image			*image_new(t_window *w, int width, int height);
void			image_free(t_window *w, t_image **img);
void			draw_pixel(t_window *w, int x, int y, t_color *color);
void			draw_pixel_gray(t_window *w, int x, int y, double value);
void			raytracer(t_window *win);
#endif
