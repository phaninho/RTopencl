/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_math.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeexpert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/12 13:35:53 by jeexpert          #+#    #+#             */
/*   Updated: 2017/04/12 13:35:55 by jeexpert         ###   ########.fr       */
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
