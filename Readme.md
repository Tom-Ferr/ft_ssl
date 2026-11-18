# ft_ssl

A cryptographic toolkit implemented entirely from scratch in C, inspired by OpenSSL and developed as part of the 42 curriculum.

The project provides implementations of cryptographic hash functions, symmetric and asymmetric encryption algorithms, key generation, and encoding utilities without relying on external cryptographic libraries.

## Features

### Standard Commands

| Command | Description |
|---|---|
| `help` | Display available commands |
| `genrsa` | Generate RSA keys |
| `rsa` | Process RSA keys |
| `rsautl` | Perform RSA operations |
| `gendsa` | Generate DSA keys |

### Message Digest Commands

| Command | Algorithm |
|---|---|
| `md5` | MD5 |
| `sha1` | SHA-1 |
| `sha224` | SHA-224 |
| `sha256` | SHA-256 |
| `sha384` | SHA-384 |
| `sha512` | SHA-512 |
| `sha512-224` | SHA-512/224 |
| `sha512-256` | SHA-512/256 |
| `whirlpool` | Whirlpool |

### Cipher Commands

Supported algorithms:

- DES
- Triple DES (3DES)
- AES-128

Supported modes of operation:

- ECB — Electronic Codebook
- CBC — Cipher Block Chaining
- CFB — Cipher Feedback
- OFB — Output Feedback
- CTR — Counter
- PCBC — Propagating Cipher Block Chaining

| DES | 3DES | AES-128 |
|---|---|---|
| `des` | `des3` | `aes-128` |
| `des-ecb` | `des3-ecb` | `aes-128-ecb` |
| `des-cbc` | `des3-cbc` | `aes-128-cbc` |
| `des-cfb` | `des3-cfb` | `aes-128-cfb` |
| `des-ofb` | `des3-ofb` | `aes-128-ofb` |
| `des-ctr` | `des3-ctr` | `aes-128-ctr` |
| `des-pcbc` | `des3-pcbc` | `aes-128-pcbc` |

### Encoding

| Command | Description |
|---|---|
| `base64` | Base64 encoding and decoding |

## Installation

### Requirements

- GCC or Clang
- GNU Make

### Compilation

Clone the repository:

```bash
git clone https://github.com/Tom-Ferr/ft_ssl.git
cd ft_ssl
```

Compile the project:

```bash
make
```

This generates the `ft_ssl` executable.

### Makefile Commands

```bash
make        # Compile the project
make clean  # Remove object files
make fclean # Remove object files and executable
make re     # Rebuild the project
```

## Usage

The general command syntax is:

```bash
./ft_ssl <command> [options] [arguments]
```

### Interactive Mode

Launch the application without arguments to enter interactive mode:

```bash
./ft_ssl
```

Commands can then be executed directly through the interactive prompt.

### Help

Display the available commands:

```bash
./ft_ssl help
```


### Message Digests

Calculate a message digest:

```bash
./ft_ssl sha256 "Hello World"
./ft_ssl md5 "Hello World"
./ft_ssl whirlpool "Hello World"
```

### Symmetric Encryption

Run a cipher using the desired algorithm and mode:

```bash
./ft_ssl aes-128-cbc [options]
./ft_ssl des-cbc [options]
./ft_ssl des3-cbc [options]
```

### Base64

```bash
./ft_ssl base64 [options]
```

### RSA

Generate an RSA key pair:

```bash
./ft_ssl genrsa [options]
```

Process RSA keys:

```bash
./ft_ssl rsa [options]
```

Perform RSA operations:

```bash
./ft_ssl rsautl [options]
```

### DSA

Generate DSA keys:

```bash
./ft_ssl gendsa [options]
```

For command-specific options and arguments, run:

```bash
./ft_ssl <command> -help
```