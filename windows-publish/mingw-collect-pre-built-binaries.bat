@ECHO OFF 
RMDIR /s /q cpp-mingw-prebuilt-bin
MD cpp-mingw-prebuilt-bin
COPY ..\bin\ReleaseStatic\*.exe cpp-mingw-prebuilt-bin
COPY ..\finderror\bin\Release\*.exe cpp-mingw-prebuilt-bin
COPY ..\vc-config-gen\bin\Release\*.exe cpp-mingw-prebuilt-bin
COPY ..\minised\*.exe cpp-mingw-prebuilt-bin
