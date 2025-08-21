:<<"::CMDGOTO"
@echo off
goto :CMDENTRY
rem https://stackoverflow.com/questions/17510688/single-script-to-run-in-both-windows-batch-and-linux-bash
::CMDGOTO

echo "========== logical16x16 build ${SHELL} ================="
DIR=$(dirname "$0")
(mkdir -p bin;)
(echo build .. $2)
(cd ${DIR}/bin;iverilog -Winfloop -g2012 -gspecify -Ttyp -I../src -o tb.vvp ../src/top_tb.v; vvp -N tb.vvp -lx2)
exit $?
:CMDENTRY

echo ============= logical16x16 build %COMSPEC% ============
set OLDDIR=%CD%
mkdir bin  >nul 2>&1
cd bin
echo build %1 %2
cl %~dp0\src\rikiki.c /D_CRT_SECURE_NO_WARNINGS=1 /Fe:rkc.exe >>build.log 2>&1
clang -D_CRT_SECURE_NO_WARNINGS=1 %~dp0\src\rikiki.c -o rkc.exe >>build.log 2>&1
chdir /d %OLDDIR%
exit 0

