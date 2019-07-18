@echo off
for /d %%i in (*) do (
    cd %%i
    if exist "run-vc.bat" (
	    echo running %%i
	    call run-vc.bat | bash -i -c "FileCheck run-vc.bat"
	    echo done %%i
	)
    cd ..
)
