import os
from subprocess import *

os.system("cp backup/small.cpp hello.cpp")
os.system("touch -t 198111140701 hello.cpp")
os.system("cpps hello.cpp > /dev/null");

os.system("rm hello.cpp");
os.system("cp backup/big.cpp hello.cpp")
os.system("touch -t 198111140701 hello.cpp")
os.system("cpps -v hello.cpp > a.txt");


retcode = call("grep hello.cpp a.txt > /dev/null", shell=True)
#retcode = os.system("grep hello.cpp a.txt");

# 1 - not found
if (retcode == 1): 
    print "error found!"
