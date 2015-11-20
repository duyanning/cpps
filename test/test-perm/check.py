import os

os.system("sudo cp -r perm /")
os.system("cpps /perm/hello.cpp > a.txt")
os.system("sudo rm -r /perm")

os.system("cpps perm/hello.cpp > b.txt")

retcode = os.system("diff a.txt b.txt")

if (retcode != 0):
    print "error found!"
