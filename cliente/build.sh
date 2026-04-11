mkdir -p build
g++ -Wall -Werror -std=c++17 cliente.cpp libcliente.cpp -o build/cliente -pthread

