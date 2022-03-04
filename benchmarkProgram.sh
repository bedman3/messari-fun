#!/bin/bash

if [ ! -f ./output.log ]; then
    if [ ! -f ./stdoutinator_amd64_linux.bin ]; then
        echo "no stdoutinator, please download!"
        exit 1
    fi
    ./stdoutinator_amd64_linux.bin > output.log
fi

if [ ! -f /usr/bin/g++ ]; then
    echo "please download g++!"
    exit 1
fi

if [ ! -f /usr/bin/clang++ ]; then
    echo "please download clang++!"
    exit 1
fi

runBenchmark() {
    for i in {1..3}
    do
        # sec=$(/usr/bin/time -f "%e" cat output.log | ./messari > release_$1.log )
        # echo "The program ran for $sec second"
        /usr/bin/time -f "%e" cat output.log | ./messari > release_$1.log
    done
}

echo "GCC Compile Benchmark"
cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ .
make
runBenchmark gcc


echo "Clang Compile Benchmark"
cmake -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ .
make
runBenchmark clang





