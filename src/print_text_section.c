/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_text_section.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 03:58:43 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/22 03:58:45 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	put_hex_fast(uint64_t num, int bits, char *line, int *linepos)
{
	while ((bits -= 4) >= 0)
		line[(*linepos)++] = "0123456789abcdef"[(num >> bits) & 0xf];
}

void	print_text_section(t_data *d)
{
	size_t	i;
	char	line[80];
	int		linepos;

	linepos = 0;
	i = 0;
	while (i < d->text_section_size)
	{
		put_hex_fast(d->text_section_vaddr + i,
			d->is_64bit ? 64 : 32, line, &linepos);
		line[linepos++] = '\t';
		put_hex_fast(((uint8_t*)d->text_section_addr)[i], 8, line, &linepos);
		if (!*is_big_endian())
			line[linepos++] = ' ';
		while (++i % 16 && i < d->text_section_size)
		{
			put_hex_fast(((uint8_t*)d->text_section_addr)[i],
				8, line, &linepos);
			if (!*is_big_endian() || (*is_big_endian() && i % 4 == 3))
				line[linepos++] = ' ';
		}
		line[linepos++] = '\n';
		write(STDOUT_FILENO, line, linepos);
		linepos = 0;
	}
}
