/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/14 11:57:47 by helmanso          #+#    #+#             */
/*   Updated: 2021/07/28 17:15:13 by helmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pestilence.h"

extern void    decrypt(void *payload, int size, uint32_t key) __attribute__((optimize("-O1")));

void    decrypt(void *payload, int size, uint32_t key)
{
    uint32_t             i;

    i = 0;
    while (i < size)
    {
        ((unsigned char *)payload)[i] ^= key;
        i++;
    } // supposed to be a dummy decryption
}


void    encrypt(void *payload, int size, t_seed *seed)
{
    uint32_t             i;

    uint32_t        random;

   
    i = 0;
    ps_random(seed);
    while (i < size)
    {
        ((unsigned char *)payload)[i] ^= seed->w;
        i++;
    }
}

void    get_seed(void *payload, t_seed *seed)
{
    unsigned char *code = (unsigned char *)payload;

    ft_memcpy(seed, code, 16);
}


void    ps_random(t_seed *seed) 
{
    uint32_t t = seed->x;
    t ^= t << 11U;
    t ^= t >> 8U;
    seed->x = seed->y; seed->y = seed->z; seed->z = seed->w;
    seed->w ^= seed->w >> 19U;
    seed->w ^= t;
}