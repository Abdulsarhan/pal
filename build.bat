@echo off
setlocal

if "%1"=="--release" (
  set CFLAGS= -Wall -Wextra -O3 -march=nocona -std=c89
) else (
  set CFLAGS= -Wall -Wextra -ggdb -std=c89
)

clang %CFLAGS% app.c glad.c -o app.exe -I thirdparty -lkernel32 -luser32 -ladvapi32 -lgdi32 -lshell32 -lcomdlg32

endlocal
