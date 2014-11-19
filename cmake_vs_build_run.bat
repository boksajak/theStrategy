mkdir vsbuild
cd vsbuild
del Release\Strategy.exe
cmake -G "Visual Studio 10 Win64" ../source/
C:\Windows\Microsoft.NET\Framework\v4.0.30319\msbuild.exe Strategy.sln /p:Configuration=Release /p:Platform="x64"
cd Release
Strategy.exe
pause
