from subprocess import *

compile_cmd = "g++ -std=c++11 show-args.cpp -o show-args"
retcode = call(compile_cmd, shell=True)

run_cmd = "./show-args a b 'c d'> a.txt"
retcode = call(run_cmd, shell=True)

interpret_cmd = "cpps show-args.cpp a b 'c d' > b.txt"
retcode = call(interpret_cmd, shell=True)

compare_cmd = "diff a.txt b.txt"
retcode = call(compare_cmd, shell=True)

if (retcode != 0):
    print "error found!"


