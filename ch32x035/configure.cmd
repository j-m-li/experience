:<<"::CMDGOTO"
@echo off
goto :CMDENTRY
rem https://stackoverflow.com/questions/17510688/single-script-to-run-in-both-windows-batch-and-linux-bash
::CMDGOTO

echo "========== wch32 configure ${SHELL} ================="
DIR=$(dirname "$0")
(mkdir -p ${DIR}/bin;)
cp -f configs/linux/clang*.make bin/
rm -f bin/Makefile
mv  bin/clang.make bin/Makefile
(cd ./bin ; make)
exit $?
:CMDENTRY

echo ============= OS-3o3 configure %COMSPEC% ============
set OLDDIR=%CD%

echo chdir /d %1
if "%CD%" == "%OLDDIR%" (
	echo ;
)
mkdir bin  >nul 2>&1
chdir /d bin
cl %~dp0\tools\build.c /D_CRT_SECURE_NO_WARNINGS=1 /Fe:build.exe >>build.log 2>&1
if %errorlevel% neq 0 (
	clang -D_CRT_SECURE_NO_WARNINGS=1 %~dp0\tools\build.c -o build.exe >>build.log 2>&1
	goto :clang
)
:visualc
echo VISUAL C
goto :NEXT
:clang
if %errorlevel% neq 0 (
	echo cannot fine c compiler.
	exit -1;
)
echo clang

:NEXT
del build.*
chdir /d %OLDDIR%
echo build %1 %2
set HEXFILE=CH32X035G8U6.hex
clang --target=riscv32 -march=rv32imaf -mabi=ilp32f -o t.elf test.S
.\bin\openocd.exe -f bin\wch-riscv.cfg -c init -c halt  -c "program %HEXFILE% verify " -c reset -c wlink_reset_resume -c exit  


exit 0


