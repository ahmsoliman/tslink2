CFLAGS_DEBUG = -c -g3 -D_GLIBCXX_DEBUG
LFLAGS_DEBUG = 

CFLAGS_RELEASE = -c -O3
LFLAGS_RELEASE = -O3

CFLAGS = $(CFLAGS_RELEASE)
LFLAGS = $(LFLAGS_RELEASE)

EXEC = bin/tslink2
OBJS = build/utils.o build/stopwatch.o build/main.o

all : tslink2

tslink2 : bin/tslink2

bin/tslink2 : $(OBJS)
	@mkdir -p $(@D)
	g++ $(LFLAGS) $(OBJS) -o $(EXEC) -lboost_system -lboost_filesystem

build/main.o : src/tslink2.cpp
	@mkdir -p $(@D)
	g++ $(CFLAGS) src/main.cpp -o build/main.o

build/utils.o : src/utils.cpp
	@mkdir -p $(@D)
	g++ $(CFLAGS) src/utils.cpp -o build/utils.o

build/stopwatch.o : src/stopwatch.cpp
	@mkdir -p $(@D)
	g++ $(CFLAGS) src/stopwatch.cpp -o build/stopwatch.o

clean	:
	rm -f $(OBJS) $(EXEC)