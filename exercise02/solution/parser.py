import csv
import os


def parse_filename(filename):
    filename = filename[: filename.find(".")]
    name = filename[:filename.rfind("_")]
    size = filename[filename.rfind("_")+1:]
    return name, size


def strip_string(line):
    line = line.strip()
    return line[:line.find(" ")]


def parse_content(filepath):
    f = open(filepath, 'r')
    cycles = None
    instructions = None
    cache_misses = None
    seconds = None
    real = None
    user = None

    for line in f:
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


def get_dirs(user, csvfile):
    path = "benchmarks" + os.sep + user
    dirs = os.listdir(path)
    for i in range(0, len(dirs)):
        dir = path + os.sep + dirs[i]
        files = os.listdir(dir)
        for file in files:
            name, size = parse_filename(file)
            cycles, instructions, cache_misses, seconds, real, user = parse_content(dir + os.sep + file)
            csvfile.writerow([str(i), name, size, seconds, real, user, cycles, instructions, cache_misses])


with open('lorenz_benni.csv', 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=';', quotechar='|', quoting=csv.QUOTE_MINIMAL)
    spamwriter.writerow(['Round', 'Executable', 'Size', 'CPU-Time', 'User-Time', 'Realtime', 'Cycles', 'Instructions', 'Cache-misses'])
    get_dirs("lorenz", spamwriter)
