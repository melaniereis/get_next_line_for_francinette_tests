/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meferraz <meferraz@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:28:27 by meferraz          #+#    #+#             */
/*   Updated: 2024/11/07 13:28:45 by meferraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * This file implements the get_next_line function,
 * which reads a line from a file descriptor until a newline or EOF.
 */

#include "./get_next_line.h"

// Function prototypes
static char	*read_and_store(int fd, char *buff);
static char	*ft_create_buffer(char *buff, char *buffer);
static char	*extract_line(char *buff);
static char	*update_buffer(char *buff);

/*
 * get_next_line - Reads a line from the file descriptor `fd`.
 *
 * Return: A pointer to the line read (including newline), or NULL on failure.
 */
char *get_next_line(int fd)
{
    static char *buffer; // Static buffer for leftover data
    char *line;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL); // Invalid input
    buffer = read_and_store(fd, buffer); // Read data into buffer
    if (!buffer)
        return (NULL); // Reading failed
    line = extract_line(buffer); // Extract line from buffer
    buffer = update_buffer(buffer); // Update buffer for next call
    return (line);
}

/*
 * read_and_store - Reads data from the file descriptor into `buff`.
 *
 * Return: The updated buffer containing new data.
 */
static char *read_and_store(int fd, char *buff)
{
    char	*buffer;
    ssize_t	bytes_read;

    if (!buff)
    {
        buff = malloc(1); // Allocate initial buffer if NULL
        if (!buff)
            return (NULL);
        buff[0] = '\0'; // Initialize as empty string
    }
    buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1)); // Allocate space for reading
    if (!buffer)
        return (free(buff), NULL); // Free existing buffer on error
    bytes_read = 1; // Initialize bytes_read for loop condition
    while (bytes_read > 0 && !ft_strchr(buff, '\n'))
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE); // Read data into buffer
        if (bytes_read <= 0)
            break; // Break on EOF or error
        buffer[bytes_read] = '\0'; // Null-terminate string
        buff = ft_create_buffer(buff, buffer); // Combine buffers
        if (!buff) // Check for allocation failure
        	return (free(buffer), NULL);
    }
    return (free(buffer), buff); // Free temporary buffer after use
}

/*
 * ft_create_buffer - Concatenates two buffers into a new one.
 *
 * Return: A new combined buffer or NULL on failure.
 */
static char	*ft_create_buffer(char *buff, char *buffer)
{
	char	*tempbuff;

	tempbuff = ft_strjoin(buff, buffer); // Join buffers together
	if (!tempbuff)
	{
		free(buff); // Free existing buffer on failure
		return (NULL);
	}
	return (tempbuff);
}

/*
 * extract_line - Extracts a line from the given buffer.
 *
 * Return: A pointer to the extracted line or NULL on failure.
 */
static char *extract_line(char *buff)
{
    size_t	line_size;
    size_t	i;
    char	*line;

    if (!buff || !*buff)
        return (NULL); // Return NULL if buffer is empty
    line_size = 0;
    while (buff[line_size] && buff[line_size] != '\n')
        line_size++; // Calculate length of line
    line = malloc(sizeof(char) * (line_size + 2)); // Allocate space for line + \n + \0
    if (!line)
        return (NULL);
    i = -1;
    while (++i < line_size)
        line[i] = buff[i]; // Copy characters from buff to line
    if (buff[i] == '\n')
        line[i++] = '\n'; // Include newline character if present
    line[i] = '\0'; // Null-terminate the string
    return (line);
}

/*
 * update_buffer - Updates the existing buffer by removing the extracted line.
 *
 * Return: A new buffer with remaining data or NULL on failure.
 */
static char *update_buffer(char *buff)
{
    size_t	i;
    size_t	j;
    char	*buffer;

    i = 0;
    while (buff[i] && buff[i] != '\n')
        i++; // Find position of newline character
    if (!buff[i]) // No newline found; free and return NULL
        return (free(buff), NULL);
    buffer = malloc(sizeof(char) * (ft_strlen(buff) - i + 1)); // Allocate space for remaining characters after newline
    if (!buffer)
        return (free(buff), NULL);
    i++; // Move past newline character
    j = 0;
    while (buff[i]) 
        buffer[j++] = buff[i++]; // Copy remaining characters
    buffer[j] = '\0'; // Null-terminate new buffer
    free(buff); 
    return (buffer);
}