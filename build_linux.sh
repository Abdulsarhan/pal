cc application.c glad.c -o application -Wall -Wextra -lX11 -lXfixes -lXrandr -lpthread -lm -ludev --std=c99 -ggdb &&
    ./application
