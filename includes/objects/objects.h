/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/06 14:28:10 by mgallo            #+#    #+#             */
/*   Updated: 2016/10/13 22:31:46 by mgallo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

# include "maths/vec3.h"
# include "maths/camera.h"
# include "graphics/color.h"

# define OBJECTS_SPHERE 0
# define OBJECTS_PLANE 1
# define OBJECTS_CYLINDER 2
# define OBJECTS_CONE 3
# define OBJECTS_TRIANGLES 4

typedef struct	s_triangle
{
	t_vec3	*v0;
	t_vec3	*v1;
	t_vec3	*v2;
}				t_triangle;

typedef struct	s_objects
{
	int			type;
	t_vec3		*pos;
	t_vec3		*rot;
	t_color		*color;
	double		radius;
	int			material;
	t_vec3		*normal;
	t_triangle	**data;
}				t_objects;

t_objects		*objects_new(int type);
void			objects_free(t_objects **obj);
void			sphere_intersect(t_objects *obj, t_ray *ray, double znear);
t_vec3			*sphere_normal(t_objects *obj, t_vec3 *impact);
void			plane_intersect(t_objects *obj, t_ray *ray, double znear);
t_vec3			*plane_normal(t_objects *obj);
void			cylinder_intersect(t_objects *obj, t_ray *ray, double znear);
t_vec3			*cylinder_normal(t_objects *obj, t_vec3 *impact);
t_triangle		*triangle_new(t_vec3 *v0, t_vec3 *v1, t_vec3 *v2);
void			triangle_free(t_triangle **t);
void			objects_add_triangle(t_objects *obj, t_triangle *t);
void			triangle_intersect(t_objects *obj, t_ray *ray, double znear);
t_vec3			*triangle_normal(t_objects *obj, t_ray *ray);

#endif
