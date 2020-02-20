/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endianness.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 17:43:47 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/14 17:43:47 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

bool		*is_big_endian(void)
{
	static bool	is_big_endian = false;

	return (&is_big_endian);
}

uint16_t	swap16(uint16_t n)
{
	if (*is_big_endian() == false)
		return (n);
	return (((n & 0xff) << 8) |
			((n & 0xff00) >> 8));
}

uint32_t	swap32(uint32_t n)
{
	if (*is_big_endian() == false)
		return (n);
	return (((n & 0xff) << 24) |
			((n & 0xff00) << 8) |
			((n & 0xff0000) >> 8) |
			((n & 0xff000000) >> 24));
}

uint64_t	swap64(uint64_t n)
{
	if (*is_big_endian() == false)
		return (n);
	return (((n & 0xff) << 56) |
			((n & 0xff00) << 40) |
			((n & 0xff0000) << 24) |
			((n & 0xff000000) << 8) |
			((n & 0xff00000000) >> 8) |
			((n & 0xff0000000000) >> 24) |
			((n & 0xff000000000000) >> 40) |
			((n & 0xff00000000000000) >> 56));
}
