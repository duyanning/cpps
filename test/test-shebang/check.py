from subprocess import *

shell_cmd = "chmod +x test-shebang.cpp"
retcode = call(shell_cmd, shell=True)

interpret_cmd = "./test-shebang.cpp > b.txt"
retcode = call(interpret_cmd, shell=True)

compare_cmd = "diff a.txt b.txt"
retcode = call(compare_cmd, shell=True)

if (retcode != 0):
    print "error found!"


