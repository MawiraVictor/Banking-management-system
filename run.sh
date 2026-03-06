#!/bin/bash
echo "Compiling all files..."
gcc -Iheaders main.c account.c transactions.c history.c utils.c -o banking
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo ""
    ./banking
else
    echo "Compilation failed!"
fi