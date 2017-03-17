/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 12:29:57 by stmartin          #+#    #+#             */
/*   Updated: 2017/03/17 12:29:58 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "utils/error.h"
#include "window/window.h"
#include "thread/thread.h"

void	die(const char *error)
{
	fprintf(stderr, "%s\n", error);
	window_end();
	thread_end();
	exit(-1);
}

void	opencl_error(const char *err, const void *p, size_t cb, void *u)
{
	(void)p;
	(void)cb;
	(void)u;
	fprintf(stderr, "%s\n", err);
}
