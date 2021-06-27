@echo off
REM		Build Script

REM Set Compiler Settings Here

cls

set CPP=c++
set GPP=g++
set OUTPUT=archiver.exe
set DEBUGMODE=1

set LINK_ONLY=0
set VERBOSE=0

set ASYNC_BUILD=1

set COMPILER_FLAGS=-std=c++20
set ADDITIONAL_LIBRARIES=-static-libstdc++ -lpthread -static -lportaudio -lsetupapi -lwinmm -luser32 -lgdi32 -lopengl32 -lShlwapi -ldwmapi -lstdc++fs -lpng -lz
set ADDITIONAL_LIBDIRS=-Llibrary
set ADDITIONAL_INCLUDEDIRS=-Ilibrary -Isrc\completed -Isrc -Iautoarchive

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

del /S /Q "autoarchive\*.o" >nul 2>nul

echo Building API Files...
for %%F in (autoarchive\*.cpp) do (
	if not exist autoarchive\%%~nF.o (
		echo Building %%~nF.o
		if %VERBOSE% GTR 0 (
			echo %CPP% %ADDITIONAL_INCLUDEDIRS% %COMPILER_FLAGS% %DEBUG_INFO% -c %%F -o autoarchive\%%~nF.o
		)
		start /B %WAIT% "%%~nF.o" %CPP% %ADDITIONAL_INCLUDEDIRS% %COMPILER_FLAGS% %DEBUG_INFO% -c %%F -o autoarchive\%%~nF.o
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
for /f "delims=" %%A in ('dir /b /a-d "src\completed\*.o" ') do set "files=!files! src\completed\%%A"
for /f "delims=" %%A in ('dir /b /a-d "autoarchive\*.o" ') do set "files=!files! autoarchive\%%A"
for /f "delims=" %%A in ('dir /b /a-d ".objs64\*.o" ') do set "files=!files! .objs64\%%A"

:link
echo Linking Executable...

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