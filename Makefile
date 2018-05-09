system = linux

ifeq (macos, $(system))
main: main.o
	g++ \
	-L/Library/Java/JavaVirtualMachines/jdk1.8.0_144.jdk/\
Contents/Home/jre/lib/server/ \
	main.o -o main -ljvm

main.o: main.cpp
	g++ \
	-I/System/Library/Frameworks/JavaVM.framework/Headers/ \
	-c main.cpp -o main.o
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
