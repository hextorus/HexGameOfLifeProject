clear
#pour detecter les fuites de mémoire
#gcc hashtableRecode.c fonctions.c code2PatternDetectOnGrid.c -o TIPE.exe -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -fsanitize=address

#pour avoir le même aléatoire que la version visuelle
x86_64-w64-mingw32-gcc hashtableRecode.c fonctions.c code2PatternDetectOnGrid.c -o TIPE.exe -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference
echo compilation terminée
./TIPE.exe

#  -fsanitize=address