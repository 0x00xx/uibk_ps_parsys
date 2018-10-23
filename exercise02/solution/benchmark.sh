#!/bin/bash
# Produces one output file per parameter per executable.

# config
# compiled with:
# mmul1 = g++ mmul.cpp -o mmul1 -DNESTED_VECTOR -Wall -O3 -std=c++11
# mmul2 = g++ mmul.cpp -o mmul2 -DCONTIGUOUS_WITH_MULTIPLICATION -Wall -O3 -std=c++11
# mmul3 = g++ mmul.cpp -o mmul3 -DCONTIGUOUS_WITH_INDIRECTION -Wall -O3 -std=c++11
# mmul_utm = g++ mmul_utm.cpp -o mmul_utm -DNESTED_VECTOR -Wall -O3 -std=c++11
# mmul_utm_omp = g++ mmul_utm_omp.cpp -o mmul_utm_omp -Wall -O3 -std=c++11 -fopenmp

readonly executables=(mmul1 mmul2 mmul3 mmul_utm mmul_utm_omp) # exes
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

# Various basic CPU statistics
bench_cpu(){
    echo "measuring CPU counters..."
    echo "bench_cpu" >> "$outfile"
    command perf stat -e cycles,instructions,cache-references,cache-misses,bus-cycles -o "$outfile" "./$e" "$p"
    echo -e "\n" >> "$outfile"
}

# execution
mkdir benchmarks
mkdir benchmarks/${USER}

for r in {1..7}; do
	mkdir benchmarks/${USER}/$r
	for e in "${executables[@]}"; do
		echo -e "\nbenchmarking $e:"

		for p in "${params[@]}"; do
		echo "param: $p"
		    # set and clear output file
		    outfile="benchmarks/${USER}/$r/${e}_${p}.bench.txt"
		    > "$outfile"

		    # benchmarks
		bench_cpu
		bench_time
		if [ $r == 1 ]
		then
			bench_mem
			bench_cache
		fi
		done
	done
done

rm *grind*

