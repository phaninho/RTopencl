/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeexpert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/12 13:45:33 by jeexpert          #+#    #+#             */
/*   Updated: 2017/04/12 13:45:36 by jeexpert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "utils/rt_math.h"

VEC3		rotatex(VEC3 vec, float degree)
{
	float rx;
	float tmpy;
	float tmpz;

	rx = TO_RADIANS(degree);
	tmpy = vec.y;
	tmpz = vec.z;
	vec.y = tmpy * cosf(rx) - tmpz * sinf(rx);
	vec.z = tmpy * sinf(rx) + tmpz * cosf(rx);
	return (vec);
}

VEC3		rotatey(VEC3 vec, float degree)
{
	float ry;
	float tmpx;
	float tmpz;

	ry = TO_RADIANS(degree);
	tmpx = vec.x;
	tmpz = vec.z;
	vec.x = tmpz * sin(ry) + tmpx * cos(ry);
	vec.z = tmpz * cos(ry) - tmpx * sin(ry);
	return (vec);
}

VEC3		rotatez(VEC3 vec, float degree)
{
	float rz;
	float tmpx;
	float tmpy;

	rz = TO_RADIANS(degree);
	tmpx = vec.x;
	tmpy = vec.y;
	vec.x = tmpx * cos(rz) - tmpy * sin(rz);
	vec.y = tmpx * sin(rz) + tmpy * cos(rz);
	return (vec);
}

VEC3		rotatexyz(VEC3 vec, VEC3 rot)
{
	vec = rotatez(vec, rot.z);
	vec = rotatex(vec, rot.x);
	vec = rotatey(vec, rot.y);
	return (vec);
}
