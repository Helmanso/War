/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_nothing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 18:08:08 by helmanso          #+#    #+#             */
/*   Updated: 2021/07/28 11:23:46 by helmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pestilence.h"


void    do_nothing(int fd)
{
    
    unsigned char inst[] = {'\x90', '\x90', '\x90', '\x90'};

 //   write_wrapper(fd, &inst, sizeof(inst));
    return ;
}