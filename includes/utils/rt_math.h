/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_math.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 12:32:26 by stmartin          #+#    #+#             */
/*   Updated: 2017/03/17 12:32:30 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_MATH_H
# define RT_MATH_H

# include <OpenCL/opencl.h>
# define VEC3 cl_float3
# define VEC4 cl_float4
# define PI 3.14159265359f
# define TO_RADIANS(x) (x * PI / 180.0f)

VEC3		rotatex(VEC3 vec, float degree);
VEC3		rotatey(VEC3 vec, float degree);
VEC3		rotatez(VEC3 vec, float degree);
VEC3		rotatexyz(VEC3 vec, VEC3 rot);

#endif
