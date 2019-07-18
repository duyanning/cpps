@echo off
for /d %%i in (*) do (
    cd %%i
    if exist "run-mingw.bat" (
	    echo running %%i
	    call run-mingw.bat | bash -i -c "FileCheck run-mingw.bat"
	    echo done %%i
	)
    cd ..
)
