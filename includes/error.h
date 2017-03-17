/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvalenti <lvalenti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/06 09:08:14 by mgallo            #+#    #+#             */
/*   Updated: 2016/10/13 13:08:32 by lvalenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define NO_ERROR 0
# define ERROR_WINDOW_SIZE 1
# define ERROR_WINDOW_MLX 2
# define ERROR_WINDOW_CREATE 3
# define ERROR_WINDOW_NOCREATE 4
# define ERROR_IMAGE_MALLOC 5
# define ERROR_IMAGE_ADDRESS 6
# define ERROR_IMAGE_DATA 7
# define ERROR_PARSER_MALLOC 8
# define ERROR_BAD_FILE 9
# define ERROR_RT_MALLOC 10

void	rt_error(int erreur);

#endif
