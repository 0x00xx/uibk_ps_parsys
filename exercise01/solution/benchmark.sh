#!/bin/bash
# Produces one output file per parameter per executable.

# config
# compiled with g++ mmul.cpp -o mmul -std=c++11 -Wall
readonly executables=(mmul mmul_seq1 mmul_seq2) # exes
readonly params=(128 256 512 1024 2048) # args


# benchmarks
# elapsed and cpu time
bench_time() {
    echo "measuring execution time..."
    echo "bench_time" >> "$outfile"
    command time -ao "$outfile" -f "real %e\nuser %U" "./$e" "$p"
    echo -e "\n" >> "$outfile"
}

bench_mem(){
    [[ "$p" -eq 2048 || "$p" -eq 1024 ]] && return
    echo "checking memory errors..."
    echo "bench_memory" >> "$outfile"
    command valgrind --leak-check=yes "./$e" "$p" 2>&1 >/dev/null | grep -e 'ERROR SUMMARY' >> "$outfile"
    echo -e "\n" >> "$outfile"
}

bench_cache(){
    [[ "$p" -eq 2048 || "$p" -eq 1024 ]] && return
    echo "checking cache hits/misses..."
    echo "bench_cache" >> "$outfile"
    command valgrind --tool=cachegrind "./$e" "$p" 2>&1 >/dev/null | sed -n -e '/refs/,$p' >> "$outfile"
    command sed -i '$ d' "$outfile"
    echo -e "\n" >> "$outfile"
}

# execution
mkdir benchmarks
mkdir benchmarks/${USER}

for e in "${executables[@]}"; do
    echo -e "\nbenchmarking $e:"

    for p in "${params[@]}"; do
	echo "param: $p"
        # set and clear output file
        outfile="benchmarks/${USER}/${e}_${p}.bench.txt"
        > "$outfile"

        # benchmarks
        bench_time
        bench_mem
        bench_cache
    done
done
rm *grind*

