CC=g++
build:
	${CC} src/*.cpp -o main
build-debug:
	${CC} -DDEBUG src/*.cpp -o main
run: build
	./main
install: build
	sudo cp ./main /usr/local/bin/load-env
