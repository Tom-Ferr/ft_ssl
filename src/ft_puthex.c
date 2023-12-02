#include <ft_ssl.h>

static	void	write_symbol(int fd, uint64_t z, bool a)
{
	char c;

	c = 'a';
	if (a)
		c = 'A';
	if (z >= 10)
		z = (z % 10) + c;
	else
		z = z + '0';
	write(fd, &z, 1);
}

void		ft_puthex(int fd, uint64_t d, bool a)
{
	long n;
	long m;

	n = d / 16;
	m = d % 16;
	if (n < 16)
	{
		write_symbol(fd, n, a);
		write_symbol(fd, m, a);
	}
	else
	{
		ft_puthex(fd, n, a);
		write_symbol(fd, m, a);
	}
}