CFLAGS_DEBUG = -c -g3 -D_GLIBCXX_DEBUG
LFLAGS_DEBUG = 

CFLAGS_RELEASE = -c -O3
LFLAGS_RELEASE = -O3

CFLAGS = $(CFLAGS_RELEASE)
LFLAGS = $(LFLAGS_RELEASE)

EXEC = tslink2
OBJS = utils.o stopwatch.o main.o

tslink2 : $(OBJS)
	g++ $(LFLAGS) $(OBJS) -o $(EXEC)

main.o : main.cpp
	g++ $(CFLAGS) main.cpp

utils.o : utils.cpp
	g++ $(CFLAGS) utils.cpp

stopwatch.o : stopwatch.cpp
	g++ $(CFLAGS) stopwatch.cpp

clean	:
	rm -f $(OBJS) $(EXEC)