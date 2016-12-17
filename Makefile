meet: main.cpp moores.cpp brzozowski.cpp
	@g++ -std=c++11 main.cpp moores.cpp brzozowski.cpp -o main

clean:
	@rm main
