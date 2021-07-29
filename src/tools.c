/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 18:54:23 by helmanso          #+#    #+#             */
/*   Updated: 2021/06/06 10:46:36 by helmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pestilence.h"

int		ft_strcmp(const char *s1, const char *s2)
{
	int				i;
	unsigned char	*s1_1;
	unsigned char	*s2_1;

	s1_1 = (unsigned char *)s1;
	s2_1 = (unsigned char *)s2;
	i = 0;
	while (s1_1[i] != '\0' || s2_1[i] != '\0')
	{
		if (s1_1[i] != s2_1[i])
		{
			return (s1_1[i] - s2_1[i]);
		}
		i++;
	}
	return (0);
}

char	*ft_strcpy(char *dst, const char *src)
{
	int i;

	i = 0;
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}


char	*ft_strcat(char *dest, const char *src)
{
	int i;
	int j;

	i = 0;
	j = 0;
	if (dest == src)
		return (NULL);
	while (dest[i] != '\0')
		i++;
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}


int		ft_strlen(const char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

void	ft_putstr(char *str)
{
	while (*str != '\0')
		ft_putchar(*str++);
}
void	ft_putchar(char c)
{
	write_wrapper(1, &c, 1);
}


void    ft_putnbr(int n)
{
        long long i;

        i = (long long)n;
        if (i < 0)
        {
                ft_putchar('-');
                i *= -1;
        }
        if (i <= 9)
                ft_putchar(i + '0');
        else
        {
                ft_putnbr(i / 10);
                ft_putnbr(i % 10);
        }
}


int             ft_memcmp(const void *s1, const void *s2, size_t n)
{
        size_t                  i;
        unsigned char   *s1_1;
        unsigned char   *s2_1;

        i = 0;
        s1_1 = (unsigned char *)s1;
        s2_1 = (unsigned char *)s2;
        while (i < n)
        {
                if (s1_1[i] != s2_1[i])
                        return (s1_1[i] - s2_1[i]);
                i++;
        }
        return (0);
}

void	*ft_memcpy(void *dest, const void *src, size_t count)
{
	char	*src1;
	char	*dest1;
	size_t	i;

	src1 = (char *)src;
	dest1 = (char *)dest;
	i = 0;
	while (i < count && src1 != dest1)
	{
		dest1[i] = src1[i];
		i++;
	}
	return (dest1);
}