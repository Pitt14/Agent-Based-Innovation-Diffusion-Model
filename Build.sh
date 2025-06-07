if [ ! -d ./build ]; then
  mkdir -p ./build;
fi

g++ -fdiagnostics-color=always -g Main.cpp -o ./build/InnovationDiffusion -O3 -std=c++20 -Wall -fopenmp -lpthread -floop-parallelize-all -I./Libraries/indicators/include