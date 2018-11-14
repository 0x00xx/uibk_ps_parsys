import time
import os
import subprocess
import statistics
import sys

PROBLEM_SIZE = 0
NUMBER_OF_ITERATIONS = 10
COMMAND = "./merge %s"

cycles_list = []
instructions_list = []
cache_misses_list = []
seconds_list = []
time_list = []

def parse_filename(filename):
    filename = filename[: filename.find(b".")]
    name = filename[:filename.rfind(b"_")]
    size = filename[filename.rfind(b"_")+1:]
    return name, size


def strip_string(line):
    line = line.strip()
    return line[:line.find(b" ")]


def parse_content(output):
    cycles = None
    instructions = None
    cache_misses = None
    seconds = None

    for line in output.split(b'\n'):
        if b"cycles" in line and not cycles:
            cycles = strip_string(line)
        elif b"instructions" in line:
            instructions = strip_string(line)
        elif b"cache-misses" in line:
            cache_misses = strip_string(line)
        elif b"seconds time elapsed" in line:
            seconds = strip_string(line)
            break

    return int(cycles.replace(b'.', b"")), int(instructions.replace(b'.', b"")), int(cache_misses.replace(b'.', b"")), float(seconds.replace(b',', b"."))

def perf_bench():
    global cycles_avg, instructions_avg, cache_misses_avg, seconds_avg
    for i in range(0, NUMBER_OF_ITERATIONS):
        proc = subprocess.Popen(["perf stat -e cycles,instructions,cache-references,cache-misses,bus-cycles %s" % (COMMAND % PROBLEM_SIZE)], stderr=subprocess.PIPE, shell=True)		
        (out, err) = proc.communicate()
        cycles, instructions, cache_misses, seconds = parse_content(err)
        cycles_list.append(cycles)
        instructions_list.append(instructions)
        cache_misses_list.append(cache_misses)
        seconds_list.append(seconds)
		

def time_bench():
    global time_list
    for i in range(0, NUMBER_OF_ITERATIONS):
        start_time = time.time()
        os.system(COMMAND % PROBLEM_SIZE)
        end_time = time.time() - start_time
        time_list.append(end_time)
		#print("Round %i took %s seconds" % (i+1, end_time))

def print_results():
    global time_list, cycles_avg, instructions_avg, cache_misses_avg, seconds_avg
    print("\nProblemsize: %s\nIterations: %s" % (PROBLEM_SIZE, NUMBER_OF_ITERATIONS))
    print("Average time: %s" % (statistics.mean(time_list)))
    print("Median time: %s" % (statistics.median(time_list)))
    print("Average Values\n\tCycles: %i \n\tinstr: %i \n\tCache Misses: %i \n\tSeconds: %s" % (statistics.mean(cycles_list), statistics.mean(instructions_list), statistics.mean(cache_misses_list), statistics.mean(seconds_list)))
    print("\nMedian Values\n\tCycles: %i \n\tinstr: %i \n\tCache Misses: %i \n\tSeconds: %s" % (statistics.median(cycles_list), statistics.median(instructions_list), statistics.median(cache_misses_list), statistics.median(seconds_list)))

def main():
    print("Building")
    os.system("g++ Main.cpp MergeSort.h MergeSort.cpp -fopenmp -Wall -O3 -std=c++11 -o merge")
    time_bench()
    perf_bench()
    print_results()
    os.system("rm merge")

def print_help(exe):
    print("Usage:\n\tpython3 %s [problemsize] [version]\n\t\tproblemsize: Size of the array which should be merged\n\t\tversion: Either parallel or sequential" % exe)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print_help(sys.argv[0])
    else:
        PROBLEM_SIZE = int(sys.argv[1])
        if (sys.argv[2] == "parallel"):
            COMMAND = "./merge %s par"
        main()
