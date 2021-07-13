CFLAGS_DEBUG = -c -g3 -D_GLIBCXX_DEBUG
CFLAGS_RELEASE = -c -O3

CFLAGS = $(CFLAGS_RELEASE)
EXEC = tslink2
OBJS = utils.o main.o 

tslink2 : main.o utils.o
	g++ -O3 main.o utils.o -o $(EXEC)

main.o : main.cpp
	g++ $(CFLAGS) main.cpp

utils.o : utils.cpp
	g++ $(CFLAGS) utils.cpp

clean	:
	rm -f $(OBJS) $(EXEC)