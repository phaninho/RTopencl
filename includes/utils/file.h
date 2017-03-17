/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 12:32:15 by stmartin          #+#    #+#             */
/*   Updated: 2017/03/17 12:32:16 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_H
# define FILE_H

#ifndef BUFF_SIZE
	# define BUFF_SIZE (1 << 14)
#endif

void		file2str(const char *file, char **str, size_t *size);

#endif
