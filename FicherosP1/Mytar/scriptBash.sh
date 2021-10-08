#!/bin/bash

#if [ -x FILE ]	True if FILE exists and is executable.
if [-x mytar]: then echo ""
else echo "No existe programa ejecutable mytar"
fi

#if[ -d FILE ]	True if FILE exists and is a directory.
if [-d tmp]: then 
#rm -r File Remove directories and their contents recursively.
    rm -r tmp
fi

mkdir tmp
cd tmp

touch file1.txt
echo "Hello world|" > file1.txt

touch file2.txt
head -10 /etc/passwd >file2.txt

touch file3.dat
head -c 1024 /dev/urandom >file3.dat

../mytar -c -f filetar.mtar file1.txt file2.txt file3.txt

mkdir out
#cp FileOrigen  directorio
cp filetar.mtar ./tmp/out

cd out
../--/mytar -x -f filetar.mtar

diff file1.txt ../file1.txt && diff file2.txt ../file2.txt && diff file3.dat ../file3.dat


if [$? -eq 0]:then
    cd ../..
    echo "Correcto."
else
    cd ../..
    echo "No es correcto"
fi