# Tangram
Project for Advanced Topics in Databases


# includes for vscode
/usr/include/postgresql

# compiling yap2postgres.c
gcc -c -shared -fPIC yap2postgres.c -I/usr/include/postgresql -lpq

ld -shared -o yap2postgres.so yap2postgres.o -lpq


# usage of yap2postgres in prolog
consult(yap2postgres).
db_open('localhost', 'postgis', 'postgres', 'postgres', 'con1').
db_close('con1').

# refs
[postgis functions](https://postgis.net/docs/manual-3.3/reference.html#idm8875)
