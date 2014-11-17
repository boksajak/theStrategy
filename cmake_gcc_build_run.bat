mkdir build
cd build
del Strategy.exe
cmake -D CMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ../source/
mingw32-make 2>make.log
Strategy.exe
pause