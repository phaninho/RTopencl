/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/05 17:57:54 by mgallo            #+#    #+#             */
/*   Updated: 2016/10/13 22:07:56 by mgallo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC3_H
# define VEC3_H

typedef struct	s_vec3
{
	double	x;
	double	y;
	double	z;
}				t_vec3;

t_vec3			*vec3_new(double x, double y, double z);
void			vec3_free(t_vec3 **v);
t_vec3			*vec3_copy(t_vec3 *v);
double			vec3_lenght(t_vec3 *v);
t_vec3			*vec3_normalize(t_vec3 *v);
t_vec3			*vec3_new_normalize(t_vec3 *v);
double			vec3_dot(t_vec3 *v0, t_vec3 *v1);
t_vec3			*vec3_sub(t_vec3 *v0, t_vec3 *v1);
t_vec3			*vec3_new_sub(t_vec3 *v0, t_vec3 *v1);
t_vec3			*vec3_negate(t_vec3 *v);
t_vec3			*vec3_mul(t_vec3 *v0, t_vec3 *v1);
t_vec3			*vec3_mul_value(t_vec3 *v0, double value);
t_vec3			*vec3_new_mul(t_vec3 *v0, t_vec3 *v1);
t_vec3			*vec3_div(t_vec3 *v0, t_vec3 *v1);
t_vec3			*vec3_new_div(t_vec3 *v0, t_vec3 *v1);
t_vec3			*vec3_rotate_x(t_vec3 *v, float degree);
t_vec3			*vec3_rotate_y(t_vec3 *v, float degree);
t_vec3			*vec3_rotate_z(t_vec3 *v, float degree);
t_vec3			*vec3_rotate_xyz(t_vec3 *v, float dx, float dy, float dz);
t_vec3			*vec3_new_rotate_xyz(t_vec3 *v, float dx, float dy, float dz);
t_vec3			*vec3_reflect(t_vec3 *v, t_vec3 *normal);
t_vec3			*vec3_set(t_vec3 *v, double x, double y, double z);
t_vec3			*vec3_new_cross(t_vec3 *v1, t_vec3 *v2);
t_vec3			*vec3_new_cross3(t_vec3 *v0, t_vec3 *v1, t_vec3 *v2);
t_vec3			*vec3_add(t_vec3 *v0, t_vec3 *v1);
t_vec3 			*vec3_new_add(t_vec3 *v0, t_vec3 *v1);
#endif
