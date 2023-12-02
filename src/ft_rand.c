#include <ft_ssl.h>

int ft_rand(void)
{
	static int					first;
	static unsigned long int	next;

	if (!first++)
		next = (unsigned long int)&next;
	next = next * 1103515245 + 12345;
	return ((unsigned int)(next / 65536) % 0x7fffffff);
}

uint64_t ft_urand(void)
{
	int fd;
	uint64_t buff;

	fd = open("/dev/urandom", O_RDONLY);
	buff = 0;
	read(fd, &buff, 8);

	close(fd);

	return(buff);
}

