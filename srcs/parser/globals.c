/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/23 16:17:44 by qhonore           #+#    #+#             */
/*   Updated: 2017/04/14 12:48:09 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/rt_parser.h"

char		*g_args[] =
{
	"width",
	"height",
	"maxreflect",
	"maxrefract",
	"material",
	"blinn",
	"debug",
	"perlin",
	"fov",
	"znear",
	"zfar",
	"radius",
	"reflection",
	"refraction",
	"attenuation",
	"angle",
	"shininess",
	"radius2",
	"dist",
	"refract_coef",
	"damier",
	"normal",
	"rotation",
	"position",
	"direction",
	"endpos",
	"pos2",
	"color",
	"specular",
	"ambient",
	"filter",
	"load",
	"mod",
	NULL
};

char		*g_tags[] =
{
	"scene",
	"camera",
	"sphere",
	"plane",
	"cylinder",
	"cone",
	"triangle",
	"paral",
	"disk",
	"cylinderinf",
	"coneinf",
	"paraboloid",
	"ellipsoid",
	"torus",
	"sor",
	"spotlight",
	"pointlight",
	"dirlight",
	"material",
	"render",
	NULL
};

char		*g_mods[] =
{
	"sepia",
	"grey",
	"filter",
	"add",
	"negative",
	"cartoon",
	"anti-aliasing",
	NULL
};

char		*get_tag(int i)
{
	return (g_tags[i]);
}

char		*get_arg(int i)
{
	return (g_args[i]);
}

char		*get_mod(int i)
{
	return (g_mods[i]);
}
