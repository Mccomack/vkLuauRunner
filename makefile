SOURCES := $(shell find ./src -name '*.cpp')

./build/main.out: $(SOURCES)
	@g++ -std=c++23 $(SOURCES) -o ./build/main.out

test: ./build/main.out
	@./build/main.out

clean:
	@rm -f ./build/main.out