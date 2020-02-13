/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 18:10:04 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/12 18:10:04 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

// void	parse_section(void *file, uint32_t nsects)
// {
// 	struct section_64	*section;
// 	void				*ptr;

// 	ptr = file + sizeof(struct mach_header_64);
// 	while (nsects--)
// 	{
// 		section = (struct section_64*)ptr;
// 		ft_printf("cmd size: %d\n", cmd->cmdsize);
// 		ptr = (char*)ptr + cmd->cmdsize;
// 	}
// }

void	parse_symtab_command(struct symtab_command *cmd, void *file)
{
	uint32_t	nsyms;
	struct nlist_64   *symbols;
	char		*strings;

	nsyms = cmd->nsyms;
	symbols = (char*)file + cmd->symoff;
	strings = (char*)file + cmd->stroff;
	while (nsyms--)
	{
		ft_printf("%016llx %s\n", symbols->n_value, strings + symbols->n_un.n_strx);
		symbols++;
	}
}

int parse_commands(void *file, uint32_t ncmds)
{
	struct load_command	*cmd;
	void				*ptr;

	ptr = file + sizeof(struct mach_header_64);
	while (ncmds--)
	{
		cmd = (struct load_command*)ptr;
		// ft_printf("cmd size: %d\n", cmd->cmdsize);
		if (cmd->cmd == LC_SYMTAB)
			parse_symtab_command(cmd, file);
		ptr = (char*)ptr + cmd->cmdsize;
	}
	return (0);
}

int func(t_data *d, char **av)
{
	int						fd;
	struct stat				file_stat;
	struct mach_header_64	*header;

	if (!av[1])
		av[1] = "a.out";
	if ((fd = open(av[1], O_RDONLY)) < 0)
		return (ft_printf("open error: %s\n", av[1]));
	if (fstat(fd, &file_stat) != 0)
		return (ft_printf("fstat error: %s\n", av[1]));
	if (file_stat.st_size == 0)
		return (ft_printf("Invalid file: %s\n", av[1]));
	d->file = mmap(0, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (d->file == MAP_FAILED)
		return (ft_printf("mmap error\n"));
	header = (struct mach_header_64*)d->file;
	if (header->magic != MH_MAGIC_64)
		return (ft_printf("Invalid file: %s\n", av[1]));
	parse_commands(d->file, header->ncmds);
	return (0);
}


int main(int ac, char **av)
{
	(void)ac;
	t_data d = {0};
	if (func(&d, av))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
