/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_exec2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/09 11:19:57 by qhonore           #+#    #+#             */
/*   Updated: 2017/04/01 19:00:27 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

void		button_materials(void *param)
{
	t_button	*button;

	button = (t_button*)param;
	update_onoff(button);
}
