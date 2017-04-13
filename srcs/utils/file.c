/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 16:35:43 by qhonore           #+#    #+#             */
/*   Updated: 2017/04/13 22:40:15 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/error.h"
#include "libft/libft.h"

size_t		get_file(const char *name, char **str)
{
	FILE	*file;
	size_t	len;

	if (!(file = fopen(name, "r")))
		die("open failed");
	fseek(file, 0, SEEK_END);
	len = ftell(file);
	fseek(file, 0, SEEK_SET);
	if (!(*str = (char*)malloc(sizeof(char) * (len + 1))))
		die("malloc failure");
	fread(*str, len, 1, file);
	(*str)[len] = '\0';
	fclose(file);
	return (len);
}

int			file_exist(const char *name)
{
	FILE	*file;

	if ((file = fopen(name, "r")))
	{
		fclose(file);
		return (1);
	}
	return (0);
}

int			screen_exist(int nb)
{
	return (1);
}
