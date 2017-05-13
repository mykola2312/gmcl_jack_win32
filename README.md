# gmcl_jack_win32

A GMod Lua Module with a custom lua interface, which independent of gmod lua interface (aka ILuaInterface/ILuaBase),
With the exception is Lua C API in lua_shared.dll

When module loads, LoadLibraryA in hl2.exe calls DllMain, where starting loading lua c api interface
to array lua_api. If you need a custom DllMain, write changes in state.cpp

This repository goes together with example of usage in main.cpp
