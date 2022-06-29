/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebillin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 13:45:23 by jebillin          #+#    #+#             */
/*   Updated: 2022/06/22 11:49:39 by jebillin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"	

/* line_read is given a value to make it run.
 * If the value is zero it is the end of the file.
 * If there is no backup, give it the value of an empty strdup.
 * char_temp is assigned the value of backup, and backup is assigned 
 * the value of strjoin, which is char_temp with buffer attached to the end.
 * char_temp is freed. If a newline is found in buffer the loop is broken.
 * Backup is then returned.*/

static char	*find_newline(int fd, char *buffy, char *backup)
{
	int		line_read;
	char	*char_temp;

	line_read = 1;
	while (line_read != '\0')
	{
		line_read = read(fd, buffy, BUFFER_SIZE);
		if (line_read == -1)
			return (0);
		else if (line_read == 0)
			break ;
		buffy[line_read] = '\0';
		if (!backup)
			backup = ft_strdup("");
		char_temp = backup;
		backup = ft_strjoin(char_temp, buffy);
		free(char_temp);
		char_temp = NULL;
		if (ft_strchr (buffy, '\n'))
			break ;
	}
	return (backup);
}

/* count is set to zero and incremented while the value is not a newline
 * or null terminator. After incrementing through, if the value is a
 * null terminator, return zero.
 * backup is assigned the value of substr, which is created from 
 * line with starting point of count plus one, and the length of line.
 * If there is no value in backup, it is freed. 
 * Otherwise, return backup.*/

static char	*extract(char *line)
{
	size_t	count;
	char	*backup;

	count = 0;
	while (line[count] != '\n' && line[count] != '\0')
		count++;
	if (line[count] == '\0' || line[1] == '\0')
		return (0);
	backup = ft_substr(line, count + 1, ft_strlen(line) - count);
	if (*backup == '\0')
	{
		free(backup);
		backup = NULL;
	}
	line[count + 1] = '\0';
	return (backup);
}

/* Line is the string we want to return, backup holds the 
 * remainder of line only within it's file, so we need to use static char.
 * If there are undefined stdin values in fd or BUFFER_SIZE
 * it will return zero. Buffy is assigned its value; if there
 * is no buffy, return zero. 
 * Line is assigned the value of char *find_newline, which is made up of
 * fd, buffy, and backup. Buffy is freed.
 * If there is no line, return null. */

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffy;
	static char	*backup;

	if ((fd < 0) || (BUFFER_SIZE <= 0))
		return (0);
	buffy = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffy)
		return (0);
	line = find_newline(fd, buffy, backup);
	free(buffy);
	buffy = NULL;
	if (!line)
		return (NULL);
	backup = extract(line);
	return (line);
}
