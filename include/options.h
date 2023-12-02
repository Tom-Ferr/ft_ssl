#ifndef OPTIONS_H
# define OPTIONS_H

typedef struct s_hash_options
{
    char** file;
    char* string;
    uint64_t options;
} t_hash_options;

typedef struct s_base64_options
{
    char* in_file;
    char* out_file;
    uint64_t options;
} t_base64_options;

typedef struct s_des_options
{
    char* in_file;
    char* out_file;
    char* password;
    uint8_t* key;
    uint8_t* salt;
    uint8_t* iv;
    uint64_t options;
    size_t key_size;
} t_des_options;

typedef struct s_genrsa_options
{
    char* in_file;
    char* out_file;
    size_t bits;
} t_genrsa_options;

typedef struct s_rsa_options
{
    char* in_file;
    char* out_file;
    char* password_in;
    char* password_out;
    uint64_t options;
} t_rsa_options;

typedef struct s_rsautl_options
{
    char* in_file;
    char* out_file;
    char* key;
    char* password_in;
    uint64_t options;
} t_rsautl_options;

#endif