CFLAGS_DEBUG = -c -g3 -D_GLIBCXX_DEBUG
LFLAGS_DEBUG = 

CFLAGS_RELEASE = -c -O3
LFLAGS_RELEASE = -O3

CFLAGS = $(CFLAGS_RELEASE)
LFLAGS = $(LFLAGS_RELEASE)

EXECS = bin/tslink1 bin/tslink2
OBJS = build/utils.o build/stopwatch.o

build/utils.o : src/utils.cpp
	@mkdir -p $(@D)
	g++ $(CFLAGS) src/utils.cpp -o build/utils.o

build/stopwatch.o : src/stopwatch.cpp
	@mkdir -p $(@D)
	g++ $(CFLAGS) src/stopwatch.cpp -o build/stopwatch.o

build/tslink1.o : src/tslink1.cpp
	@mkdir -p $(@D)
	g++ $(CFLAGS) src/tslink1.cpp -o build/tslink1.o

tslink1 : bin/tslink1

bin/tslink1 : $(OBJS) build/tslink1.o
	@mkdir -p $(@D)
	g++ $(LFLAGS) $(OBJS) build/tslink1.o -o bin/tslink1 -lboost_system -lboost_filesystem

build/tslink2.o : src/tslink2.cpp
	@mkdir -p $(@D)
	g++ $(CFLAGS) src/tslink2.cpp -o build/tslink2.o

tslink2 : bin/tslink2

bin/tslink2 : $(OBJS) build/tslink2.o
	@mkdir -p $(@D)
	g++ $(LFLAGS) $(OBJS) build/tslink2.o -o bin/tslink2 -lboost_system -lboost_filesystem

all : tslink1 tslink2

clean	:
	rm -f $(OBJS) build/tslink1.o build/tslink2.o $(EXECS)