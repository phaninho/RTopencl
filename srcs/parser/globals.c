/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/23 16:17:44 by qhonore           #+#    #+#             */
/*   Updated: 2016/12/13 23:20:58 by qhonore          ###   ########.fr       */
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
	"fov",
	"znear",
	"zfar",
	"radius",
	"reflection",
	"refraction",
	"attenuation",
	"angle",
	"shininess",
	"normal",
	"rotation",
	"position",
	"direction",
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
	"disk",
	"triangle",
	"spotlight",
	"pointlight",
	"dirlight",
	"material",
	"texture",
	"render",
	NULL
};

char		*g_mods[] =
{
	"sepia",
	"grey",
	"filter",
	"add",
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
