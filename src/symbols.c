/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 03:49:34 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/22 03:49:36 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

size_t	sizeof_symbol(t_data *d)
{
	return (d->is_64bit ? sizeof(struct nlist_64) :
		sizeof(struct nlist));
}

bool	parse_symbol(t_data *d, int i, struct nlist_64 *sym, char **str)
{
	if (d->is_64bit)
	{
		*sym = d->sym64[i];
		sym->n_un.n_strx = swap32(sym->n_un.n_strx);
		sym->n_value = swap64(sym->n_value);
	}
	else
	{
		sym->n_un.n_strx = swap32(d->sym32[i].n_un.n_strx);
		sym->n_value = swap32(d->sym32[i].n_value);
		sym->n_type = d->sym32[i].n_type;
		sym->n_sect = d->sym32[i].n_sect;
	}
	if (d->is_otool && sym->n_un.n_strx > d->strsize)
	{
		ft_printf("%s:\n", d->filename);
		return (false);
	}
	else if (sym->n_un.n_strx > d->strsize)
		*str = "bad string index";
	else
		*str = d->strtab + sym->n_un.n_strx;
	return (true);
}

void	print_symbols(t_data *d)
{
	int i;

	i = -1;
	while (++i < d->nsyms)
	{
		parse_symbol(d, i, &d->sym, &d->sym_str);
		if (d->sym.n_type & N_STAB)
			continue;
		if (d->sym.n_type &&
				(d->sym.n_type & N_TYPE) == N_UNDF && !d->sym.n_value)
			ft_printf("%*s", d->is_64bit ? 16 : 8, "");
		else
			ft_printf("%0*llx", d->is_64bit ? 16 : 8, d->sym.n_value);
		ft_printf(" %c %s\n", get_type_char(d), d->sym_str);
	}
}

void	swap_symbols(t_data *d, int i, int j)
{
	struct nlist	tmp32;
	struct nlist_64	tmp64;

	if (d->is_64bit)
	{
		tmp64 = d->sym64[i];
		d->sym64[i] = d->sym64[j];
		d->sym64[j] = tmp64;
	}
	else
	{
		tmp32 = d->sym32[i];
		d->sym32[i] = d->sym32[j];
		d->sym32[j] = tmp32;
	}
}

bool	sort_symbols(t_data *d)
{
	int		i;
	int		j;
	int		ret;

	i = -1;
	while (++i < d->nsyms - 1)
	{
		j = -1;
		while (++j < d->nsyms - i - 1)
		{
			if (!parse_symbol(d, j, &d->sym, &d->sym_str))
				return (false);
			if (d->sym.n_type & N_STAB)
				continue ;
			if (!parse_symbol(d, j + 1, &d->sym2, &d->sym_str2))
				return (false);
			if ((ret = ft_strcmp(d->sym_str, d->sym_str2)) > 0 ||
				(ret == 0 && d->sym.n_value > d->sym2.n_value))
				swap_symbols(d, j, j + 1);
		}
	}
	return (true);
}
