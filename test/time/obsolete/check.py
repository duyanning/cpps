import os
from subprocess import *

os.system("cp backup/hello.cpp .")

os.system("touch -t 198111140701 hello.cpp")
os.system("cpps hello.cpp > /dev/null");

os.system("touch -t 197801031256 hello.cpp")
os.system("cpps -v hello.cpp > a.txt");

#retcode = os.system("grep compiling a.txt");
retcode = call("grep compiling a.txt > /dev/null", shell=True)


# 1 - not found
if (retcode == 1): 
    print "error found!"
