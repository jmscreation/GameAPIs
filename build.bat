@echo off
REM		Build Script

REM Set Compiler Settings Here

cls

set CPP=c++
set GPP=g++
set OUTPUT=program.exe
set DEBUGMODE=1

set BUILD_COMPELTED=0
set PACK_ARCHIVE=1
set REBUILD_RESOURCES=1
set LINK_ONLY=0
set VERBOSE=0

set ASYNC_BUILD=1

set COMPILER_FLAGS=-std=c++20
set ADDITIONAL_LIBRARIES=-static-libstdc++ -lpthread -static -lportaudio -lsetupapi -lwinmm -luser32 -lgdi32 -lopengl32 -lShlwapi -ldwmapi -lstdc++fs -lpng -lz
set ADDITIONAL_LIBDIRS=-Llibrary
set ADDITIONAL_INCLUDEDIRS=-Ilibrary -Isrc\completed -Isrc -Iresources

del %OUTPUT% 2>nul

setlocal enabledelayedexpansion


if %LINK_ONLY% GTR 0 (
	goto linker
)

if %PACK_ARCHIVE% GTR 0 (
	echo Generating Precompiled Archive Data Buffer...
	cd resources
	start /B /WAIT "Archive" bin2cpp archive.dat archive_data test_archive_data
	cd ..\
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

del /S /Q ".objs64\*.o" >nul 2>nul

if not exist .objs64 (
	echo Creating Object Directory Structure...
	mkdir .objs64
)

if %BUILD_COMPELTED% GTR 0 (
	del /S /Q "src\completed\*.o" >nul 2>nul
)

if %REBUILD_RESOURCES% GTR 0 (
	del /S /Q "resources\*.o" >nul 2>nul
)

echo Building Resource Files...
for %%F in (resources\*.cpp) do (
	if not exist resources\%%~nF.o (
		echo Building %%~nF.o
		if %VERBOSE% GTR 0 (
			echo %CPP% %ADDITIONAL_INCLUDEDIRS% %COMPILER_FLAGS% %DEBUG_INFO% -c %%F -o resources\%%~nF.o
		)
		start /B %WAIT% "%%~nF.o" %CPP% %ADDITIONAL_INCLUDEDIRS% %COMPILER_FLAGS% %DEBUG_INFO% -c %%F -o resources\%%~nF.o
	)
)

echo Re-Building Completed API Files...
for %%F in (src\completed\*.cpp) do (
	if not exist src\completed\%%~nF.o (
		echo Building %%~nF.o
		if %VERBOSE% GTR 0 (
			echo %CPP% %ADDITIONAL_INCLUDEDIRS% %COMPILER_FLAGS% %DEBUG_INFO% -c %%F -o src\completed\%%~nF.o
		)
		start /B %WAIT% "%%~nF.o" %CPP% %ADDITIONAL_INCLUDEDIRS% %COMPILER_FLAGS% %DEBUG_INFO% -c %%F -o src\completed\%%~nF.o
	)
)

echo Building API Files...
for %%F in (src\*.cpp) do (
	if not exist .objs64\%%~nF.o (
		echo Building %%~nF.o
		if %VERBOSE% GTR 0 (
			echo %CPP% %ADDITIONAL_INCLUDEDIRS% %COMPILER_FLAGS% %DEBUG_INFO% -c %%F -o .objs64\%%~nF.o
		)
		start /B %WAIT% "%%~nF.o" %CPP% %ADDITIONAL_INCLUDEDIRS% %COMPILER_FLAGS% %DEBUG_INFO% -c %%F -o .objs64\%%~nF.o
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
for /f "delims=" %%A in ('dir /b /a-d "resources\*.o" ') do set "files=!files! resources\%%A"
for /f "delims=" %%A in ('dir /b /a-d ".objs64\%*" ') do set "files=!files! .objs64\%%A"

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