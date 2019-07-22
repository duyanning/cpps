@ECHO OFF 
RMDIR /s /q cpp-vc-prebuilt-bin
MD cpp-vc-prebuilt-bin
COPY ..\Release\*.exe cpp-vc-prebuilt-bin
COPY ..\Release\*.dll cpp-vc-prebuilt-bin
COPY ..\minised\*.exe cpp-vc-prebuilt-bin
