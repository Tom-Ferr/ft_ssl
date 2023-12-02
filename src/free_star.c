#include <ft_ssl.h>

void	free_star(uint32_t **me)
{
	int	i;

	i = 0;
	while (me[i])
	{
		free(me[i]);
		i++;
	}
	free(me[i]);
	free(me);
}

void	free_star_char(char **me)
{
	int	i;

	i = 0;
	while (me[i])
	{
		free(me[i]);
		i++;
	}
	free(me[i]);
	free(me);
}