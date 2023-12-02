#include <ft_ssl.h>

int ft_abs(int n)
{
    if(n < 0)
        return(n * -1);
    return(n);
}

double ft_fabs(double n)
{
    if(n < 0)
        return(n * -1);
    return(n);
}

double ft_floor(double n)
{
    long m;
    double o;

    m = (long)n;
    o = (double)m;
    if(n < 0 && n != o)
        m--;
    return((double)m);
}

double ft_ceil(double n)
{
    long m;
    double o;

    m = (long)n;
    o = (double)m;
    if(n > 0 && n != o)
        m++;
    return((double)m);
}

double ft_sin(double n) {
    double result;
    double term;
    int sign;
    int i;

    result = 0.0;
    term = n;
    sign = 1;
    i = 1;
    while(i <= 10)
    {
        result += sign * term;
        sign = -sign;
        term = term * n * n / (i * (i + 1));
        i += 2;
    }
    return(result);
}

uint64_t ft_pow(uint64_t a, uint64_t n)
{
    uint64_t result;

    if(!n)
        return(0);
    result = a;
    n--;
    while (n)
    {
        result *= a;
        n--;
    }

    return(result);
}

uint64_t ft_mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    uint64_t x;
    uint64_t y;

    x = 0;
    y = a % mod;
    while (b > 0)
    {
        if (b % 2 == 1)
            x = (x + y) % mod;
        y = (y + y) % mod;
        b /= 2;
    }
    return(x % mod);
}

uint64_t ft_pow_mod(uint64_t a, uint64_t n, uint64_t mod)
{
    uint64_t x;
    uint64_t y;

    x = 1;
    y = a;
    while (n > 0)
    {
        if (n % 2 == 1)
            x = ft_mul_mod(x, y, mod);
        y = ft_mul_mod(y, y, mod);
        n /= 2;
    }
    return(x % mod);
}

long inverse_mod_mul(long a, long m_0)
{
    long m;
    long tmp;
    long q;
    long x;
    long y;
    
    
    m = a;
    a = m_0;
    
    x = 0;
    y = 1;

    if (m == 1)
        return(0);

    while (m > 0)
    {
        q = a / m;

        tmp = m;
        m = a % m;
        a = tmp;

        tmp = y;
        y = x - q * y;
        x = tmp;
    }

    if (x < 0)
        x += m_0;

    return(x);
}

uint64_t addition_modulus_uint64max(uint64_t a, uint64_t b)
{
    uint64_t intmax = ~0UL;
    uint64_t diff = intmax - a;

    if(b > diff)
        return(b - diff - 1);
    else
        return(a + b);
}
