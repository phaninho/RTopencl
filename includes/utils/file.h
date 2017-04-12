/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeexpert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/12 13:35:36 by jeexpert          #+#    #+#             */
/*   Updated: 2017/04/12 13:35:41 by jeexpert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_H
# define FILE_H

#ifndef BUFF_SIZE
	# define BUFF_SIZE (1 << 14)
#endif

void		file2str(const char *file, char **str, size_t *size);

#endif
