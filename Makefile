main: main.o
	g++ 							\
	 -L/Library/Java/JavaVirtualMachines/jdk1.8.0_144.jdk/\
	Contents/Home/jre/lib/server/				\
	main.o -o main -ljvm

main.o: main.cpp
	g++ \
	-I/System/Library/Frameworks/JavaVM.framework/Headers/	\
	 -c main.cpp -o main.o