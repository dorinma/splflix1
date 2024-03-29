all: splflix1

splflix1: bin/Action.o bin/Main.o bin/Session.o bin/User.o bin/Watchable.o
	@echo 'linking'
	g++ -o bin/splflix1 bin/Action.o bin/Main.o bin/Session.o bin/User.o bin/Watchable.o

bin/Action.o: src/Action.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

bin/Main.o: src/Main.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

bin/Session.o: src/Session.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp

bin/User.o: src/User.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/User.o src/User.cpp

bin/Watchable.o: src/Watchable.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Watchable.o src/Watchable.cpp

clean:
	@echo 'cleaning'
	rm -f bin/*
