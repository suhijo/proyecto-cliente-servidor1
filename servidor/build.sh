mkdir -p build
g++ -Wall -Werror -std=c++17 servidor.cpp libservidor.cpp -o build/servidor  -lmariadbcpp -pthread
