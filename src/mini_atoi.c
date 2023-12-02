#include <ft_ssl.h>

uint32_t	mini_atoi(char argv[])
{
	int	pow;
	int	i;
	int	ret;

	pow = 1;
	ret = 0;
	i = ft_strlen(argv);
	while (--i >= 0)
	{
		if (argv[i] < '0' || argv[i] > '9')
			return (0);
		ret = ret + (argv[i] - '0') * pow;
		pow *= 10;
	}
	return (ret);
}