./build/main.out: ./**/*.cpp
	@g++ ./**/*.cpp -o ./build/main.out

test: ./build/main.out
	@./build/main.out