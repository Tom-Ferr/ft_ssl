
#include <ft_ssl.h>

uint32_t** shift_array(void)
{
    int r;
    int i;
    int j;
    int k;
    int value;
    uint32_t** s;

    s = (uint32_t**)malloc(5 * sizeof(uint32_t**));
    if (!s)
        return(NULL);
    
    i = 0;
    j = 0;
    r = 0;
    k = 0;
    value = 7;
    while (r < 4)
    {
        s[r] = (uint32_t*)malloc(16 * sizeof(uint32_t*));
        if (!s[r])
        return(NULL);
        if(r == 1)
            value = 5;
        else if(r == 2)
            value = 4;
        else if(r == 3)
            value = 6;
        j = 0;
        while(j < 4)
        {
            i = j;
            k = 0;
            while (k < 16)
            {
                if (!s)
                    return(NULL);
                s[r][i] = value;
                i += 4;
                k++;
            }
            if(r == 0)
                value += 5;
            else if(r == 2)
                value += 7 - ((j%2) * 2);
            else
                value += 4 + j;
            j++;
        }
        r++;
    }
    s[r] = NULL;
    return(s);
}

uint32_t** constant_array(void)
{
    int i;
    int j;
    int c;
    uint32_t** k;

    k = (uint32_t**)malloc(5 * sizeof(uint32_t**));
    if (!k)
        return(NULL);
    j = 0;
    c = 0;
    while(j < 4)
    {
        k[j] = (uint32_t*)malloc(16 * sizeof(uint32_t*));
        if (!k[j])
            return(NULL);
        i = 0;
        while(i < 16)
        {
            k[j][i] = ft_floor(L * ft_fabs(sin(c + 1)));
            i++;
            c++;
        }
        j++;
    }
    k[j] = NULL;
    return(k);
}