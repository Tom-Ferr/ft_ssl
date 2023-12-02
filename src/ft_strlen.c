#include <ft_ssl.h>

size_t	ft_strlen(const char *str)
{
	size_t i;

	i = 0;
	if(str == NULL)
		return(i);
	while (*str++)
		i++;
	return (i);
}

size_t	ft_veclen(const uint8_t *vec)
{
	size_t i;

	i = 0;
	if(vec == NULL)
		return(i);
	while (*vec++)
		i++;
	return (i);
}