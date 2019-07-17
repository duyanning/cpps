@echo off
for /d %%i in (*) do (
    cd %%i
    if exist "test.bat" (
	    echo running %%i
	    call test.bat
	    echo done %%i
	)
    cd ..
)
