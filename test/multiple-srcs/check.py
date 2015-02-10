from subprocess import *

compile_cmd = "g++ -std=c++11 hello.cpp foo.cpp bar.cpp -o hello"
retcode = call(compile_cmd, shell=True)

run_cmd = "./hello > a.txt"
retcode = call(run_cmd, shell=True)

interpret_cmd = "cpps hello.cpp > b.txt"
retcode = call(interpret_cmd, shell=True)

compare_cmd = "diff a.txt b.txt"
retcode = call(compare_cmd, shell=True)

if (retcode != 0):
    print "error found!"
