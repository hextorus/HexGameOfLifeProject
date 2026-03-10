#x86_64-w64-mingw32-gcc hashtableRecode.c fonctions.c raylibFunctions.c code2PatternDetectOnGridModifyToVisual.c -o TIPEvisu.exe -lraylib -mwindows -lwinmm -lm \
 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference
echo compilation terminée
#./TIPEvisu.exe

gcc hashtableRecode.c fonctions.c raylibFunctions.c code2PatternDetectOnGridModifyToVisual.c -o TIPEvisu.out \
 -lraylib -lGL -lX11 -lm -lpthread -lrt -ldl \
 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference \
 -I/usr/local/include\
 -L/usr/local/lib
echo compilation terminée
./TIPEvisu.out
