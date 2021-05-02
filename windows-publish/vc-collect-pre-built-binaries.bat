@ECHO OFF 
RMDIR /s /q cpp-vc-prebuilt-bin
MD cpp-vc-prebuilt-bin
COPY ..\Release\*.exe cpp-vc-prebuilt-bin

REM COPY ..\Release\*.dll cpp-vc-prebuilt-bin
COPY f:\libs4vc\boost_1_76_0\lib32-msvc-14.2\boost_chrono-vc*-mt-x*-*.dll cpp-vc-prebuilt-bin
COPY f:\libs4vc\boost_1_76_0\lib32-msvc-14.2\boost_filesystem-vc*-mt-x*-*.dll cpp-vc-prebuilt-bin
COPY f:\libs4vc\boost_1_76_0\lib32-msvc-14.2\boost_program_options-vc*-mt-x*-*.dll cpp-vc-prebuilt-bin
COPY f:\libs4vc\boost_1_76_0\lib32-msvc-14.2\boost_serialization-vc*-mt-x*-*.dll cpp-vc-prebuilt-bin
COPY f:\libs4vc\boost_1_76_0\lib32-msvc-14.2\boost_timer-vc*-mt-x*-*.dll cpp-vc-prebuilt-bin







COPY ..\minised\*.exe cpp-vc-prebuilt-bin
XCOPY /I ..\macro cpp-vc-prebuilt-bin\macro
