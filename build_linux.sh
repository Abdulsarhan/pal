cc pal.c application.c glad.c stb_vorbis.c -o application -Wall -Wextra -lGL -lX11 -lXfixes -lXrandr -lpthread -lm -ludev --std=c99 -ggdb &&
    ./application
