/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeexpert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/12 13:45:14 by jeexpert          #+#    #+#             */
/*   Updated: 2017/04/12 13:45:16 by jeexpert         ###   ########.fr       */
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
