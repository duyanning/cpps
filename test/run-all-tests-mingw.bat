@echo off
for /d %%i in (*) do (
    cd %%i
    if exist "test-mingw.bat" (
	    echo running %%i
	    call test.bat
	    echo done %%i
	)
    cd ..
)
