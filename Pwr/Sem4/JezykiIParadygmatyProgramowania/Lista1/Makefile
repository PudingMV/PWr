.PHONY: zad1 zad2 zad3 zad4 zad5 zad6 zad7 all

all: zad1 zad2 zad3 zad4 zad5 zad6 zad7
zad1:
	@echo "zad1"
	cd zad1 && rm -f libeuler.so && gcc -c euler.c -o euler.o && \
	ar rcs libeuler.a euler.o && \
	gcc mainEuler.c -L. -leuler -o zad1 && ./zad1
	cd zad1 && gcc -fPIC -c euler.c -o euler.o && \
	gcc -shared -o libeuler.so euler.o && \
	gcc mainEuler.c -L. -leuler -Wl,-rpath=. -o zad1 && \
	./zad1

zad2:
	@echo "zad2"
	cd zad2 && gnatmake -c euler.adb && \
	gnatmake maineuler.adb -o zad2 && ./zad2 && \
	gnatmake -fPIC -c euler.adb && \
	gcc -shared -o libeuler.so euler.o && \
	gnatmake maineuler.adb -o zad2 -L. -leuler -Wl,-rpath=. && \
	./zad2

zad3:
	@echo "zad3"
	cd zad3 && go build -o zad3 maineuler.go && ./zad3

zad4: 
	@echo "zad4"
	cd zad2 && gcc -shared -o libada_euler.so euler.o
	cd zad3/euler_c && go build -buildmode=c-shared -o libgo_euler.so euler_c.go
	cd zad4 && gcc -c wrapper.c -o wrapper.o
	gcc zad4/test.c zad4/wrapper.o -Lzad2 -Lzad3/euler_c -lada_euler -lgo_euler -lgnat -lpthread -lm -o zad4/test_program -Wl,-rpath=zad2:zad3/euler_c -ldl
	zad4/./test_program

zad5:
	@echo "zad5"
	cd zad1 && gcc -fPIC -c euler.c -o euler.o && \
	gcc -shared -o libeuler.so euler.o
	cd zad3/euler_c && go build -buildmode=c-shared -o libgo_euler.so euler_c.go
	cd zad5 && rm -f test_program && \
	gnatmake test_wrapper.adb -o test_program -largs -L../zad1 -leuler -L../zad3/euler_c -lgo_euler -Wl,-rpath=../zad1:../zad3/euler_c && \
	./test_program

zad6:
	@echo "zad6"
	cd zad1 && \
	gcc -c euler.c -o euler.o && \
	gcc -shared -o libeuler.so euler.o
	cd zad2 && \
	gnatmake -c -fPIC euler.adb && \
	gcc -fPIC -shared -o libada_euler.so euler.o -lgnat
	cd zad6 && \
	go build -o zad6 -ldflags "-extldflags=-Wl,-rpath=../zad1:../zad2" . && \
	./zad6

zad7:
	@echo "zad7"
	cd zad7 && LD_LIBRARY_PATH=../zad1:../zad2:../zad3/euler_c python3 main.py


