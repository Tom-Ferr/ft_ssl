FILES			=	add_padding.c ft_strlen.c divide_into_blocks.c non_linear_processes.c\
					free_star.c buffer_operations.c create_arrays.c ft_math.c md5.c print_hash.c ft_strcmp.c\
					rotations.c sha256.c non_linear_processes_2.c base64_encode.c base64_decode.c ft_rand.c\
					p_box.c des.c bswap.c heap_stack.c hmac.c pbdkf.c pkcss5_padding.c atox.c miller_rabin.c print_base64.c\
					rsa.c prime_gen.c print_rsakey.c parse_pem.c decode_pem.c check_key.c put_int_base.c\
					print_rsakey_values.c ft_puthex.c mgf1.c oaep.c rsa_encryption.c big_int.c free_rsakey.c\
					bigint_string.c bigint_math.c hash_options.c read_stdin.c input_file.c base64_options.c des_options.c\
					hash_to_vec.c genrsa_options.c rsa_options.c rsautl_options.c hex_dump.c sha512.c sha384.c whirlpool.c\
					sha1.c sha224.c sha512-224.c sha512-256.c aes.c aes-128_options.c key_to_der.c format_der.c free_der.c\
					parse_der.c der_to_key.c bigint_miller_rabin.c parse_salted.c mini_atoi.c montgomery.c gen_dsa_params.c gen_dsa_key.c\
					gendsa_options.c ft_split.c format_cmd.c dsa_options.c getpass.c error_message.c getkey_bitsize.c shawe_taylor.c sieve.c gendes_options.c

SRCDIR			= 	../src/

SRCS			= 	$(addprefix $(SRCDIR), $(FILES)) break_rsa.c

OBJS			= 	${SRCS:.c=.o}

HEADS			=	ft_ssl.h permutations.h sboxes.h big_int.h

INC				= 	../include/

DEPS			= 	$(addprefix ${INC}, $(HEADS))

NAME			= 	break_rsa

CXX				=	clang

AR				=	ar -rcs

CXXFLAGS		= 	-g -Wall -Wextra -Werror

INCLUDE 		= 	-I${INC}

SANITIZE 		= 	-fsanitize=address

%.o: %.c		
				${CXX} ${CXXFLAGS} ${INCLUDE} -c $< -o $@

$(NAME):		${OBJS} $(DEPS)
				${CXX} ${CXXFLAGS} ${SANITIZE} ${OBJS} ${INCLUDE} -o ${NAME}

all:			${NAME}

clean:
				rm -f ${OBJS}

fclean:			clean
				rm -f ${NAME}
re:				fclean all

.PHONY:			all clean fclean re