# GameAPIs
 API tools for creating games with PGE

# Info
This stable release is to be used as a library used in conjunction with the dependency tracker.

You may compile this library manually at your sole-discretion.

Documentation is currently unavailable


# Building Help and Static Libraries To Link

When building this library, you need to link the following static libraries:
`freedialog png64 zlib64 freetype64 portaudio64 setupapi winmm user32 gdi32 opengl32 Shlwapi dwmapi stdc++fs comdlg32`

If using the custom build script, add the following libraries to the `LIBRARY_DIRECTORIES` variable:
```
libraries\librapidjson-main
libraries\libjsonloader-main
libraries\libportaudio-main
libraries\libaudio-main
libraries\libfreetype-main
libraries\libpng-main
libraries\libz-main
libraries\libfreedialog-main
```