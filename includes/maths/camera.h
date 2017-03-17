/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/06 05:51:08 by mgallo            #+#    #+#             */
/*   Updated: 2016/10/13 21:50:57 by mgallo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "maths/vec3.h"

typedef struct	s_cam
{
	t_vec3	*pos;
	t_vec3	*rot;
	double	fov;
	double	znear;
	double	zfar;
}				t_cam;

typedef struct	s_ray
{
	t_vec3	*pos;
	t_vec3	*dir;
	double	focale;
	double	deph;
	int		object;
	int		data;
}				t_ray;

t_cam			*camera_new(double fov, double znear, double zfar);
void			camera_free(t_cam **cam);
t_cam			*camera_set(t_cam *cam, double fov, double znear, double zfar);
t_ray			*ray_new(t_vec3 *pos);
void			ray_free(t_ray **ray);
t_vec3			*get_impact(t_ray *ray);
#endif
