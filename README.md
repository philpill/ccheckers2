# ccheckers2

sdl 2
- https://github.com/libsdl-org/SDL/releases
- add path to sdl: c_cpp_properties.json - includePath

```
    "includePath": [
        "${workspaceFolder}/**",
        "C:/dev/SDL2-2.30.9/include/SDL2/*"
    ],
```

https://www.trycatchdebug.net/news/1330312/mingw-compilation-error-undefined-reference-for-winmain-16
https://stackoverflow.com/a/65155457/3983822

add mingw to path
add sdl2 i686-w64-mingw32 contents to path
copy SDL2.dll to output bin file - should be included with MAKE

dependencies - lucasg
https://github.com/lucasg/Dependencies

