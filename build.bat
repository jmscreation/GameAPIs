@echo off
REM		Build Script

REM Set Compiler Settings Here

cls

set CPP=c++
set GPP=g++
set WINRES=windres
set ARCH=64
set OUTPUT=program.exe
set DEBUGMODE=1

set LINK_ONLY=0
set VERBOSE=0

set ASYNC_BUILD=1

set ADDITIONAL_LIBRARIES=-static-libstdc++ -lpthread -static -lsetupapi -lwinmm -luser32 -lgdi32 -lopengl32 -lpng -lz -lShlwapi -ldwmapi -lstdc++fs
set ADDITIONAL_LIBDIRS=-Llibrary
set ADDITIONAL_INCLUDEDIRS=-Ilibrary

del %OUTPUT% 2>nul

setlocal enabledelayedexpansion


if %LINK_ONLY% GTR 0 (
	goto linker
)

if %DEBUGMODE% GTR 0 (
	set DEBUG_INFO=-ggdb -g
) else (
	set DEBUG_INFO=-s
)

if %ASYNC_BUILD% GTR 0 (
	set WAIT=
) else (
	set WAIT=/WAIT
)

del /S /Q ".objs64\*" 2>nul

if not exist .objs64 (
	echo Creating Object Directory Structure...
	mkdir .objs64
)

echo Building API Files...
for %%F in (*.cpp) do (
	if not exist .objs64\%%~nF.o (
		echo Building %%~nF.o
		start /B %WAIT% "%%~nF.o" %CPP% %ADDITIONAL_INCLUDEDIRS% -std=c++20 %DEBUG_INFO% -c %%F -o .objs64\%%~nF.o

		if %VERBOSE% GTR 0 (
			echo %CPP% %ADDITIONAL_INCLUDEDIRS% -std=c++20 %DEBUG_INFO% -c %%F -o .objs64\%%~nF.o
		)
	)
)

REM Wait for building process to finish
:loop
for /f %%G in ('tasklist ^| find /c "%CPP%"') do ( set count=%%G )
if %count%==0 (
	goto linker
) else (
	timeout /t 2 /nobreak>nul
	goto loop
)

:linker

set "files="
for /f "delims=" %%A in ('dir /b /a-d ".objs64\%*" ') do set "files=!files! .objs64\%%A"

echo Linking Executable...
if %ARCH%==64 (
	goto link
)
if %ARCH%==32 (
	goto link
)
echo ARCH Must be 32 or 64! Make sure ARCH matches the compiler's architecture!
goto finish
:link

if %DEBUGMODE% GTR 0 (
	set MWINDOWS=
) else (
	set MWINDOWS=-mwindows
)

if %VERBOSE% GTR 0 (
	echo %GPP% %ADDITIONAL_LIBDIRS% -o %OUTPUT% %files% %ADDITIONAL_LIBRARIES% %MWINDOWS%
)

%GPP% %ADDITIONAL_LIBDIRS% -o %OUTPUT% %files% %ADDITIONAL_LIBRARIES% %MWINDOWS%

:finish
if exist .\%OUTPUT% (
	echo Build Success!
) else (
	echo Build Failed!
)