@echo off
echo Compiling all files...
gcc main.c account.c transactions.c history.c utils.c -Iheaders -o banking.exe
echo Done!
pause