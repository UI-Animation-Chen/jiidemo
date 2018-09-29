system = linux

ifeq (macos, $(system))

java_home = /Library/Java/JavaVirtualMachines/jdk1.8.0_144.jdk/Contents/Home

main: main.o
	g++ -L$(java_home)/jre/lib/server main.o -o main -ljvm

main.o: main.cpp
	g++ -I$(java_home)/include -I$(java_home)/include/darwin -c main.cpp -o main.o
endif

ifeq (linux, $(system))
main: main.o
	g++ \
	-L/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server \
	main.o -pthread -o main -ljvm
main.o: main.cpp
	g++ \
	-I/usr/lib/jvm/java-8-openjdk-amd64/include \
	-I/usr/lib/jvm/java-8-openjdk-amd64/include/linux \
	-c -pthread main.cpp -o main.o
endif
