CFLAGS_DEBUG = -c -g3 -D_GLIBCXX_DEBUG
LFLAGS_DEBUG = 

CFLAGS_RELEASE = -c -O3
LFLAGS_RELEASE = -O3

CFLAGS = $(CFLAGS_RELEASE)
LFLAGS = $(LFLAGS_RELEASE)

EXEC = build/tslink2
OBJS = build/utils.o build/stopwatch.o build/main.o

all : tslink2

tslink2 : $(OBJS)
	g++ $(LFLAGS) $(OBJS) -o $(EXEC)

build/main.o : src/main.cpp
	g++ $(CFLAGS) src/main.cpp -o build/main.o

build/utils.o : src/utils.cpp
	g++ $(CFLAGS) src/utils.cpp -o build/utils.o

build/stopwatch.o : src/stopwatch.cpp
	g++ $(CFLAGS) src/stopwatch.cpp -o build/stopwatch.o

clean	:
	rm -f $(OBJS) $(EXEC)