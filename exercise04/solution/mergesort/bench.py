import time
import os
import subprocess
import statistics

PROBLEM_SIZE = 1024
NUMBER_OF_ITERATIONS = 10
COMMAND = "./merge %s"
# COMMAND = "./merge %s par"

cycles_list = []
instructions_list = []
cache_misses_list = []
seconds_list = []
real_list = []
user_list = []
time_list = []

def parse_filename(filename):
    filename = filename[: filename.find(".")]
    name = filename[:filename.rfind("_")]
    size = filename[filename.rfind("_")+1:]
    return name, size


def strip_string(line):
    line = line.strip()
    return line[:line.find(" ")]


def parse_content(output):
    cycles = None
    instructions = None
    cache_misses = None
    seconds = None
    real = None
    user = None

    for line in output:
        if "cycles" in line and not cycles:
            cycles = strip_string(line)
        elif "instructions" in line:
            instructions = strip_string(line)
        elif "cache-misses" in line:
            cache_misses = strip_string(line)
        elif "seconds time elapsed" in line:
            seconds = strip_string(line)
        elif "real" in line:
            real = line.split(" ")[1].strip()
        elif "user" in line:
            user = line.split(" ")[1].strip()
            break

    return cycles, instructions, cache_misses, seconds, real, user

def perf_bench():
	global cycles_avg, instructions_avg, cache_misses_avg, seconds_avg, real_avg, user_avg
	for i in range(0, NUMBER_OF_ITERATIONS):
		proc = subprocess.Popen(["perf stat -e cycles,instructions,cache-references,cache-misses,bus-cycles \"%s\"" % (COMMAND % PROBLEM_SIZE)], stdout=subprocess.PIPE, shell=True)		
		(out, err) = proc.communicate()
		cycles, instructions, cache_misses, seconds, real, user = parse_content(out)
		cycles_list.append(cycles)
		instructions_list.append(instructions)
		cache_misses_list.append(cache_misses)
		seconds_list.append(seconds)
		real_list.append(real)
		user_list.append(user)
	print("Averagevalues\ncycles: %i, instr: %i, CM: %i, Sec: %i, Real %i, Usr: %i" % (statistics.mean(cycles_list), statistics.mean(instructions_list), statistics.mean(cache_misses_list), statistics.mean(seconds_list), statistics.mean(real_list), statistics.mean(user_list)))
	print("Medianvalues\ncycles: %i, instr: %i, CM: %i, Sec: %i, Real %i, Usr: %i" % (statistics.median(cycles_list), statistics.median(instructions_list), statistics.median(cache_misses_list), statistics.median(seconds_list), statistics.median(real_list), statistics.median(user_list)))
		

def time_bench():
	global time_list
	for i in range(0, NUMBER_OF_ITERATIONS):
		start_time = time.time()
		os.system(COMMAND % PROBLEM_SIZE)
		end_time = time.time() - start_time
		time_list.append(end_time)
		#print("Round %i took %s seconds" % (i+1, end_time))
	print("\nAverage time: %s" % (statistics.mean(time_list)))
	print("Median time: %s" % (statistics.median(time_list)))

def main():
	print("Building")
	os.system("g++ Main.cpp MergeSort.h MergeSort.cpp -fopenmp -Wall -O3 -std=c++11 -o merge")
	time_bench()
	perf_bench()
	os.system("rm merge")

if __name__ == "__main__":
	main()
