#include <ft_ssl.h>

int	ft_strcmp(char *str1, char *str2)
{
	if (!str1 || !str2)
		return (-1);
	while (*str1 && *str2)
	{
		if (*str1++ != *str2++)
			return (*(--str1) - *(--str2));
	}
	return (*str1 - *str2);
}

int	ft_strlcmp(char *str1, char *str2, size_t len)
{
	for (size_t i = 0; i < len; i++)
		if(str1[i] != str2[i])
			return(str1[i] - str2[i]);
	return(0);
	
}