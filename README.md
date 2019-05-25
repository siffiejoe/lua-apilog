#          apilog -- Logging Lua API calls and stack effects         #

##                           Introduction                           ##

When developing C bindings for Lua -- especially if some non-trivial
Lua stack interaction is required -- it is nice to see the effects
that Lua API calls have on the stack to ensure that the binding code
does what it's supposed to do. This C header file allows you to log
API calls and the Lua stack during development without much effort.


##                          Getting Started                         ##

`#include` the header file `apilog.h` and define a static variable
`apilog_func` of type `char const*` in every function where you want
to trace Lua stack effects. On C99 compilers (and later) the line
`static char const* apilog_func = __func__;` is recommended. In case
you want to log *all* Lua API calls in a C file, `#define apilog_func
__func__` should do it. All logged API calls and stack effects are
written to `stderr`.

Typical output might look like this:

```
luaL_newlib in luaopen_fx_core@fx.c:661:  [ s s t ]
lua_newtable in luaopen_fx_core@fx.c:662:  [ s s t t ]
lua_pushcclosure in luaopen_fx_core@fx.c:663:  [ s s t f ]
lua_setfield in luaopen_fx_core@fx.c:664:  [ s s t ]
lua_pushlightuserdata in luaopen_fx_core@fx.c:665:  [ s s t l ]
lua_setfield in luaopen_fx_core@fx.c:666:  [ s s t ]
lua_newtable in luaopen_fx_core@fx.c:667:  [ s s t t ]
lua_pushcfunction in luaopen_fx_core@fx.c:668:  [ s s t t f ]
lua_setfield in luaopen_fx_core@fx.c:669:  [ s s t t ]
lua_setmetatable in luaopen_fx_core@fx.c:670:  [ s s t ]
lua_settop in compose@fx.c:401:  [ s f ]
lua_pushnil in compose@fx.c:404:  [ s f n ]
lua_load in check_callable@fx.c:153:  [ s f n f ]
lua_call in check_callable@fx.c:155:  [ s f n f ]
lua_replace in check_callable@fx.c:156:  [ f f n ]
lua_pushvalue in compose@fx.c:410:  [ f f n f ]
lua_pushvalue in compose@fx.c:410:  [ f f n f f ]
lua_pushinteger in compose@fx.c:420:  [ f f n f f i ]
lua_replace in compose@fx.c:421:  [ f f i f f ]
lua_pushcclosure in compose@fx.c:423:  [ f f f ]
lua_settop in composedk@fx.c:353:  [ i n ]
lua_pushvalue in composedk@fx.c:356:  [ i n f ]
lua_pushvalue in composedk@fx.c:356:  [ i n f f ]
lua_pushvalue in composedk@fx.c:359:  [ i n f f i ]
lua_pushinteger in composedk@fx.c:363:  [ i n f f i i ]
lua_replace in composedk@fx.c:364:  [ i n f f i ]
lua_pushinteger in composedk@fx.c:365:  [ i n f f i i ]
lua_replace in composedk@fx.c:366:  [ i i f f i ]
lua_pushinteger in composedk@fx.c:373:  [ i i f i i ]
lua_replace in composedk@fx.c:374:  [ i i f i ]
lua_pushinteger in composedk@fx.c:373:  [ i i i i ]
lua_replace in composedk@fx.c:374:  [ i i i ]
```

The current stack contents are indicated in the square brackets at the
end. The letters have the following meaning:

*   `"n"`: nil
*   `"b"`: boolean
*   `"l"`: lightuserdata
*   `"i"`: integer (Lua 5.3+)
*   `"d"`: number (think `double`)
*   `"s"`: string
*   `"t"`: table
*   `"f"`: function
*   `"u"`: userdata
*   `"c"`: coroutine


##                              Contact                             ##

Philipp Janda, siffiejoe(a)gmx.net

Comments and feedback are always welcome.


##                              License                             ##

**apilog** is *copyrighted free software* distributed under the MIT
license (the same license as Lua 5.1). The full license text follows:

    apilog (c) 2019 Philipp Janda

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

