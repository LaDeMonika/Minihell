/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 18:30:47 by lilin             #+#    #+#             */
/*   Updated: 2024/05/18 15:03:21 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*join_strings(char *storage, char *buffer)
{
	char	*joined;
	int		i;

	joined = malloc(sizeof(char) * (ft_strlen(storage) + ft_strlen(buffer)
				+ 1));
	if (!joined)
		return (free_and_reset_ptr((void **)&storage));
	i = 0;
	while (storage && storage[i])
	{
		joined[i] = storage[i];
		i++;
	}
	free_and_reset_ptr((void **)&storage);
	while (buffer && *buffer)
	{
		joined[i] = *(buffer++);
		i++;
	}
	joined[i] = '\0';
	return (joined);
}

static char	*extract_line(char *storage, int newline_index)
{
	char	*line;
	int		i;

	line = malloc(sizeof(char) * (newline_index + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (i < newline_index)
	{
		line[i] = storage[i];
		i++;
	}
	line[newline_index] = '\n';
	line[newline_index + 1] = '\0';
	return (line);
}

char	*update_storage(char *storage, int newline_index, char **line)
{
	char	*updated;
	int		i;

	if (ft_strlen(storage) - newline_index <= 1)
		return (free_and_reset_ptr((void **)&storage));
	updated = malloc(sizeof(char) * (ft_strlen(storage) - newline_index));
	if (!updated)
		return (free_and_reset_ptr((void **)&storage),
			free_and_reset_ptr((void **)line));
	i = 0;
	while (storage[i + newline_index + 1])
	{
		updated[i] = storage[i + newline_index + 1];
		i++;
	}
	updated[i] = '\0';
	free_and_reset_ptr((void **)&storage);
	return (updated);
}

char	*read_and_store(char **storage, int fd)
{
	char	*buffer;
	int		bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (free_and_reset_ptr((void **)storage));
	while (!ft_strchr(*storage, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			*storage = join_strings(*storage, buffer);
			if (!(*storage))
				return (free_and_reset_ptr((void **)&buffer));
		}
		else if (bytes_read < 0)
			return (free_and_reset_ptr((void **)storage),
				free_and_reset_ptr((void **)&buffer));
		else
			break ;
	}
	free_and_reset_ptr((void **)&buffer);
	return (ft_strchr(*storage, '\n'));
}

char	*get_next_line(int fd)
{
	char		*newline_found;
	static char	*storage;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	newline_found = read_and_store(&storage, fd);
	if (newline_found)
	{
		line = extract_line(storage, newline_found - storage);
		if (!line)
			return (free_and_reset_ptr((void **)&storage));
		storage = update_storage(storage, newline_found - storage, &line);
	}
	else if (storage && *storage)
	{
		line = storage;
		storage = NULL;
	}
	else
		line = NULL;
	return (line);
}

/*
#include <stdio.h>
int	main(void)
{
	int		fd;
	char	*line;

	fd = open("test3.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

 */