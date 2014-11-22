mkdir vsbuild
cd vsbuild
del Release\Strategy.exe
cmake -G "Visual Studio 10 Win64" ../source/
pause
