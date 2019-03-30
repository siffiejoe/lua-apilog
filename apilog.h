#ifndef APILOG_H_
#define APILOG_H_

#include <stddef.h>
#include <stdarg.h>
#include <lua.h>
#include <lauxlib.h>


#ifndef APILOG_API
#if defined( __GNUC__ ) || __has_attribute( __unused__ )
#define APILOG_API __attribute__((__unused__)) static
#else
#define APILOG_API static
#endif
#endif


#ifndef APILOG_PRINT
#define APILOG_PRINT
#include <stdio.h>

APILOG_API void apilog_print( lua_State* L,
                              char const* func,
                              char const* filename,
                              int lineno,
                              char const* api ) {
    if( func ) {
        int top = lua_gettop( L );
        int i = 0;
        fprintf( stderr, "%s in %s@%s:%d:  [", api, func, filename, lineno );
        for( i = 1; i <= top; ++i ) {
            switch( lua_type( L, i ) ) {
                case LUA_TNONE: /* fall through */
                case LUA_TNIL: fputs( " n", stderr ); break;
                case LUA_TBOOLEAN: fputs( " b", stderr ); break;
                case LUA_TLIGHTUSERDATA: fputs( " l", stderr ); break;
                case LUA_TNUMBER:
#if LUA_VERSION_NUM >= 503
                    if( lua_isinteger( L, i ) )
                        fputs( " i", stderr );
                    else
#endif
                        fputs( " d", stderr );
                    break;
                case LUA_TSTRING: fputs( " s", stderr ); break;
                case LUA_TTABLE: fputs( " t", stderr ); break;
                case LUA_TFUNCTION: fputs( " f", stderr ); break;
                case LUA_TUSERDATA: fputs( " u", stderr ); break;
                case LUA_TTHREAD: fputs( " c", stderr ); break;
                default:
                  fputs( " ?", stderr ); break;
            }
        }
        fputs( " ]\n", stderr );
    }
}
#endif /* APILOG_PRINT */


#define apilog_func NULL


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API void apilog_arith( char const* func,
                              char const* filename,
                              int lineno,
                              lua_State* L,
                              int op ) {
    lua_arith( L, op );
    apilog_print( L, func, filename, lineno, "lua_arith" );
}
#undef lua_arith
#define lua_arith( L, op ) \
    apilog_arith( apilog_func, __FILE__, __LINE__, (L), (op) )
#endif


APILOG_API void apilog_call( char const* func,
                             char const* filename,
                             int lineno,
                             lua_State* L,
                             int nargs,
                             int nresults ) {
    lua_call( L, nargs, nresults );
    apilog_print( L, func, filename, lineno, "lua_call" );
}
#undef lua_call
#define lua_call( L, nargs, nresults ) \
    apilog_call( apilog_func, __FILE__, __LINE__, (L), (nargs), (nresults) )


APILOG_API void apilog_concat( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               int n ) {
    lua_concat( L, n );
    apilog_print( L, func, filename, lineno, "lua_concat" );
}
#undef lua_concat
#define lua_concat( L, n ) \
    apilog_concat( apilog_func, __FILE__, __LINE__, (L), (n) )


#if LUA_VERSION_NUM == 501
APILOG_API int apilog_cpcall( char const* func,
                              char const* filename,
                              int lineno,
                              lua_State* L,
                              lua_CFunction f,
                              void* ud ) {
    int result = lua_cpcall( L, f, ud );
    apilog_print( L, func, filename, lineno, "lua_cpcall" );
    return result;
}
#undef lua_cpcall
#define lua_cpcall( L, f, ud ) \
    apilog_cpcall( apilog_func, __FILE__, __LINE__, (L), (f), (ud) )
#endif


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API void apilog_copy( char const* func,
                             char const* filename,
                             int lineno,
                             lua_State* L,
                             int fromidx,
                             int toidx ) {
    lua_copy( L, fromidx, toidx );
    apilog_print( L, func, filename, lineno, "lua_copy" );
}
#undef lua_copy
#define lua_copy( L, fromidx, toidx ) \
    apilog_copy( apilog_func, __FILE__, __LINE__, (L), (fromidx), (toidx) )
#endif


APILOG_API void apilog_createtable( char const* func,
                                    char const* filename,
                                    int lineno,
                                    lua_State* L,
                                    int narr,
                                    int nrec ) {
    lua_createtable( L, narr, nrec );
    apilog_print( L, func, filename, lineno, "lua_createtable" );
}
#undef lua_createtable
#define lua_createtable( L, narr, nrec ) \
    apilog_createtable( apilog_func, __FILE__, __LINE__, (L), (narr), (nrec) )


#if LUA_VERSION_NUM == 501
APILOG_API void apilog_getfenv( char const* func,
                                char const* filename,
                                int lineno,
                                lua_State* L,
                                int index ) {
    lua_getfenv( L, index );
    apilog_print( L, func, filename, lineno, "lua_getfenv" );
}
#undef lua_getfenv
#define lua_getfenv( L, index ) \
    apilog_getfenv( apilog_func, __FILE__, __LINE__, (L), (index) )
#endif


#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
APILOG_API int apilog_getfield( char const* func,
                                char const* filename,
                                int lineno,
                                lua_State* L,
                                int index,
                                char const* field ) {
    int result = lua_getfield( L, index, field );
    apilog_print( L, func, filename, lineno, "lua_getfield" );
    return result;
}
#else
APILOG_API void apilog_getfield( char const* func,
                                 char const* filename,
                                 int lineno,
                                 lua_State* L,
                                 int index,
                                 char const* field ) {
    lua_getfield( L, index, field );
    apilog_print( L, func, filename, lineno, "lua_getfield" );
}
#endif
#undef lua_getfield
#define lua_getfield( L, index, field ) \
    apilog_getfield( apilog_func, __FILE__, __LINE__, (L), (index), (field) )


#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
APILOG_API int apilog_getglobal( char const* func,
                                 char const* filename,
                                 int lineno,
                                 lua_State* L,
                                 char const* field ) {
    int result = lua_getglobal( L, field );
    apilog_print( L, func, filename, lineno, "lua_getglobal" );
    return result;
}
#else
APILOG_API void apilog_getglobal( char const* func,
                                  char const* filename,
                                  int lineno,
                                  lua_State* L,
                                  char const* field ) {
    lua_getglobal( L, field );
    apilog_print( L, func, filename, lineno, "lua_getglobal" );
}
#endif
#undef lua_getglobal
#define lua_getglobal( L, field ) \
    apilog_getglobal( apilog_func, __FILE__, __LINE__, (L), (field) )


#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
APILOG_API int apilog_geti( char const* func,
                            char const* filename,
                            int lineno,
                            lua_State* L,
                            int index,
                            lua_Integer i ) {
    int result = lua_geti( L, index, i );
    apilog_print( L, func, filename, lineno, "lua_geti" );
    return result;
}
#undef lua_geti
#define lua_geti( L, index, field ) \
    apilog_geti( apilog_func, __FILE__, __LINE__, (L), (index), (field) )
#endif


APILOG_API int apilog_getmetatable( char const* func,
                                    char const* filename,
                                    int lineno,
                                    lua_State* L,
                                    int index ) {
    int result = lua_getmetatable( L, index );
    apilog_print( L, func, filename, lineno, "lua_getmetatable" );
    return result;
}
#undef lua_getmetatable
#define lua_getmetatable( L, index ) \
    apilog_getmetatable( apilog_func, __FILE__, __LINE__, (L), (index) )


#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
APILOG_API int apilog_gettable( char const* func,
                                char const* filename,
                                int lineno,
                                lua_State* L,
                                int index ) {
    int result = lua_gettable( L, index );
    apilog_print( L, func, filename, lineno, "lua_gettable" );
    return result;
}
#else
APILOG_API void apilog_gettable( char const* func,
                                 char const* filename,
                                 int lineno,
                                 lua_State* L,
                                 int index ) {
    lua_gettable( L, index );
    apilog_print( L, func, filename, lineno, "lua_gettable" );
}
#endif
#undef lua_gettable
#define lua_gettable( L, index ) \
    apilog_gettable( apilog_func, __FILE__, __LINE__, (L), (index) )


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
APILOG_API int apilog_getuservalue( char const* func,
                                    char const* filename,
                                    int lineno,
                                    lua_State* L,
                                    int index ) {
    int result = lua_getuservalue( L, index );
    apilog_print( L, func, filename, lineno, "lua_getuservalue" );
    return result;
}
#else
APILOG_API void apilog_getuservalue( char const* func,
                                     char const* filename,
                                     int lineno,
                                     lua_State* L,
                                     int index ) {
    lua_getuservalue( L, index );
    apilog_print( L, func, filename, lineno, "lua_getuservalue" );
}
#endif
#undef lua_getuservalue
#define lua_getuservalue( L, index ) \
    apilog_getuservalue( apilog_func, __FILE__, __LINE__, (L), (index) )
#endif


APILOG_API void apilog_insert( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               int index ) {
    lua_insert( L, index );
    apilog_print( L, func, filename, lineno, "lua_insert" );
}
#undef lua_insert
#define lua_insert( L, index ) \
    apilog_insert( apilog_func, __FILE__, __LINE__, (L), (index) )


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API void apilog_len( char const* func,
                            char const* filename,
                            int lineno,
                            lua_State* L,
                            int index ) {
    lua_len( L, index );
    apilog_print( L, func, filename, lineno, "lua_len" );
}
#undef lua_len
#define lua_len( L, index ) \
    apilog_len( apilog_func, __FILE__, __LINE__, (L), (index) )
#endif


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API int apilog_load( char const* func,
                            char const* filename,
                            int lineno,
                            lua_State* L,
                            lua_Reader reader,
                            void* data,
                            char const* chunkname,
                            char const* mode ) {
    int result = lua_load( L, reader, data, chunkname, mode );
    apilog_print( L, func, filename, lineno, "lua_load" );
    return result;
}
#undef lua_load
#define lua_load( L, reader, data, chunkname, mode ) \
    apilog_load( apilog_func, __FILE__, __LINE__, (L), (reader), (data), (chunkname), (mode) )
#else
APILOG_API int apilog_load( char const* func,
                            char const* filename,
                            int lineno,
                            lua_State* L,
                            lua_Reader reader,
                            void* data,
                            char const* chunkname ) {
    int result = lua_load( L, reader, data, chunkname );
    apilog_print( L, func, filename, lineno, "lua_load" );
    return result;
}
#undef lua_load
#define lua_load( L, reader, data, chunkname ) \
    apilog_load( apilog_func, __FILE__, __LINE__, (L), (reader), (data), (chunkname) )
#endif


APILOG_API void apilog_newtable( char const* func,
                                 char const* filename,
                                 int lineno,
                                 lua_State* L ) {
    lua_newtable( L );
    apilog_print( L, func, filename, lineno, "lua_newtable" );
}
#undef lua_newtable
#define lua_newtable( L ) \
    apilog_newtable( apilog_func, __FILE__, __LINE__, (L) )


APILOG_API void apilog_newthread( char const* func,
                                  char const* filename,
                                  int lineno,
                                  lua_State* L ) {
    lua_newthread( L );
    apilog_print( L, func, filename, lineno, "lua_newthread" );
}
#undef lua_newthread
#define lua_newthread( L ) \
    apilog_newthread( apilog_func, __FILE__, __LINE__, (L) )


APILOG_API void* apilog_newuserdata( char const* func,
                                     char const* filename,
                                     int lineno,
                                     lua_State* L,
                                     size_t size ) {
    void* result = lua_newuserdata( L, size );
    apilog_print( L, func, filename, lineno, "lua_newuserdata" );
    return result;
}
#undef lua_newuserdata
#define lua_newuserdata( L, size ) \
    apilog_newuserdata( apilog_func, __FILE__, __LINE__, (L), (size) )


APILOG_API int apilog_next( char const* func,
                            char const* filename,
                            int lineno,
                            lua_State* L,
                            int index ) {
    int result = lua_next( L, index );
    apilog_print( L, func, filename, lineno, "lua_next" );
    return result;
}
#undef lua_next
#define lua_next( L, index ) \
    apilog_next( apilog_func, __FILE__, __LINE__, (L), (index) )


APILOG_API int apilog_pcall( char const* func,
                             char const* filename,
                             int lineno,
                             lua_State* L,
                             int nargs,
                             int nresults,
                             int msgh ) {
    int result = lua_pcall( L, nargs, nresults, msgh );
    apilog_print( L, func, filename, lineno, "lua_pcall" );
    return result;
}
#undef lua_pcall
#define lua_pcall( L, nargs, nresults, msgh ) \
    apilog_pcall( apilog_func, __FILE__, __LINE__, (L), (nargs), (nresults), (msgh) )


APILOG_API void apilog_pop( char const* func,
                            char const* filename,
                            int lineno,
                            lua_State* L,
                            int n ) {
    lua_pop( L, n );
    apilog_print( L, func, filename, lineno, "lua_pop" );
}
#undef lua_pop
#define lua_pop( L, n ) \
    apilog_pop( apilog_func, __FILE__, __LINE__, (L), (n) )


APILOG_API void apilog_pushboolean( char const* func,
                                    char const* filename,
                                    int lineno,
                                    lua_State* L,
                                    int b ) {
    lua_pushboolean( L, b );
    apilog_print( L, func, filename, lineno, "lua_pushboolean" );
}
#undef lua_pushboolean
#define lua_pushboolean( L, b ) \
    apilog_pushboolean( apilog_func, __FILE__, __LINE__, (L), (b) )


APILOG_API void apilog_pushcclosure( char const* func,
                                     char const* filename,
                                     int lineno,
                                     lua_State* L,
                                     lua_CFunction fn,
                                     int n ) {
    lua_pushcclosure( L, fn, n );
    apilog_print( L, func, filename, lineno, "lua_pushcclosure" );
}
#undef lua_pushcclosure
#define lua_pushcclosure( L, fn, n ) \
    apilog_pushcclosure( apilog_func, __FILE__, __LINE__, (L), (fn), (n) )


APILOG_API void apilog_pushcfunction( char const* func,
                                      char const* filename,
                                      int lineno,
                                      lua_State* L,
                                      lua_CFunction fn ) {
    lua_pushcfunction( L, fn );
    apilog_print( L, func, filename, lineno, "lua_pushcfunction" );
}
#undef lua_pushcfunction
#define lua_pushcfunction( L, fn ) \
    apilog_pushcfunction( apilog_func, __FILE__, __LINE__, (L), (fn) )


#if defined( __STDC_VERSION__ ) && __STDC_VERSION__+0 >= 199901L
APILOG_API char const* apilog_pushfstring( char const* func,
                                           char const* filename,
                                           int lineno,
                                           lua_State* L,
                                           char const* fmt,
                                           ... ) {
    char const* result = NULL;
    va_list argp;
    va_start( argp, fmt );
    result = (lua_pushvfstring)( L, fmt, argp );
    va_end( argp );
    apilog_print( L, func, filename, lineno, "lua_pushfstring" );
    return result;
}
#undef lua_pushfstring
#define lua_pushfstring( ... ) \
    apilog_pushfstring( apilog_func, __FILE__, __LINE__, __VA_ARGS__ )
#endif


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API void apilog_pushglobaltable( char const* func,
                                        char const* filename,
                                        int lineno,
                                        lua_State* L ) {
    lua_pushglobaltable( L );
    apilog_print( L, func, filename, lineno, "lua_pushglobaltable" );
}
#undef lua_pushglobaltable
#define lua_pushglobaltable( L ) \
    apilog_pushglobaltable( apilog_func, __FILE__, __LINE__, (L) )
#endif


APILOG_API void apilog_pushinteger( char const* func,
                                    char const* filename,
                                    int lineno,
                                    lua_State* L,
                                    lua_Integer n ) {
    lua_pushinteger( L, n );
    apilog_print( L, func, filename, lineno, "lua_pushinteger" );
}
#undef lua_pushinteger
#define lua_pushinteger( L, n ) \
    apilog_pushinteger( apilog_func, __FILE__, __LINE__, (L), (n) )


APILOG_API void apilog_pushlightuserdata( char const* func,
                                          char const* filename,
                                          int lineno,
                                          lua_State* L,
                                          void* p ) {
    lua_pushlightuserdata( L, p );
    apilog_print( L, func, filename, lineno, "lua_pushlightuserdata" );
}
#undef lua_pushlightuserdata
#define lua_pushlightuserdata( L, p ) \
    apilog_pushlightuserdata( apilog_func, __FILE__, __LINE__, (L), (p) )


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API char const* apilog_pushlstring( char const* func,
                                           char const* filename,
                                           int lineno,
                                           char const* api,
                                           lua_State* L,
                                           char const* s,
                                           size_t len ) {
    char const* result = lua_pushlstring( L, s, len );
    apilog_print( L, func, filename, lineno, api );
    return result;
}
#else
APILOG_API void apilog_pushlstring( char const* func,
                                    char const* filename,
                                    int lineno,
                                    char const* api,
                                    lua_State* L,
                                    char const* s,
                                    size_t len ) {
    lua_pushlstring( L, s, len );
    apilog_print( L, func, filename, lineno, api );
}
#endif
#undef lua_pushliteral
#undef lua_pushlstring
#define lua_pushliteral( L, s ) \
    apilog_pushlstring( apilog_func, __FILE__, __LINE__, "lua_pushliteral", (L), s "",  sizeof( s )-1 )
#define lua_pushlstring( L, s, n ) \
    apilog_pushlstring( apilog_func, __FILE__, __LINE__, "lua_pushlstring", (L), (s),  (n) )


APILOG_API void apilog_pushnil( char const* func,
                                char const* filename,
                                int lineno,
                                lua_State* L ) {
    lua_pushnil( L );
    apilog_print( L, func, filename, lineno, "lua_pushnil" );
}
#undef lua_pushnil
#define lua_pushnil( L ) \
    apilog_pushnil( apilog_func, __FILE__, __LINE__, (L) )


APILOG_API void apilog_pushnumber( char const* func,
                                   char const* filename,
                                   int lineno,
                                   lua_State* L,
                                   lua_Number n ) {
    lua_pushnumber( L, n );
    apilog_print( L, func, filename, lineno, "lua_pushnumber" );
}
#undef lua_pushnumber
#define lua_pushnumber( L, n ) \
    apilog_pushnumber( apilog_func, __FILE__, __LINE__, (L), (n) )


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API char const* apilog_pushstring( char const* func,
                                          char const* filename,
                                          int lineno,
                                          lua_State* L,
                                          char const* s ) {
    char const* result = lua_pushstring( L, s );
    apilog_print( L, func, filename, lineno, "lua_pushstring" );
    return result;
}
#else
APILOG_API void apilog_pushstring( char const* func,
                                   char const* filename,
                                   int lineno,
                                   lua_State* L,
                                   char const* s ) {
    lua_pushstring( L, s );
    apilog_print( L, func, filename, lineno, "lua_pushstring" );
}
#endif
#undef lua_pushstring
#define lua_pushstring( L, s ) \
    apilog_pushstring( apilog_func, __FILE__, __LINE__, (L), (s) )


APILOG_API int apilog_pushthread( char const* func,
                                  char const* filename,
                                  int lineno,
                                  lua_State* L ) {
    int result = lua_pushthread( L );
    apilog_print( L, func, filename, lineno, "lua_pushthread" );
    return result;
}
#undef lua_pushthread
#define lua_pushthread( L ) \
    apilog_pushthread( apilog_func, __FILE__, __LINE__, (L) )


#if LUA_VERSION_NUM == 502
APILOG_API void apilog_pushunsigned( char const* func,
                                     char const* filename,
                                     int lineno,
                                     lua_State* L,
                                     lua_Unsigned u ) {
    lua_pushunsigned( L, u );
    apilog_print( L, func, filename, lineno, "lua_pushunsigned" );
}
#undef lua_pushunsigned
#define lua_pushunsigned( L, u ) \
    apilog_pushunsigned( apilog_func, __FILE__, __LINE__, (L), (u) )
#endif


APILOG_API void apilog_pushvalue( char const* func,
                                  char const* filename,
                                  int lineno,
                                  lua_State* L,
                                  int value ) {
    lua_pushvalue( L, value );
    apilog_print( L, func, filename, lineno, "lua_pushvalue" );
}
#undef lua_pushvalue
#define lua_pushvalue( L, value ) \
    apilog_pushvalue( apilog_func, __FILE__, __LINE__, (L), (value) )


APILOG_API char const* apilog_pushvfstring( char const* func,
                                            char const* filename,
                                            int lineno,
                                            lua_State* L,
                                            char const* fmt,
                                            va_list ap ) {
    char const* result = lua_pushvfstring( L, fmt, ap );
    apilog_print( L, func, filename, lineno, "lua_pushvfstring" );
    return result;
}
#undef lua_pushvfstring
#define lua_pushvfstring( L, fmt, ap ) \
    apilog_pushvfstring( apilog_func, __FILE__, __LINE__, (L), (fmt), (ap) )


#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
APILOG_API int apilog_rawget( char const* func,
                              char const* filename,
                              int lineno,
                              lua_State* L,
                              int index ) {
    int result = lua_rawget( L, index );
    apilog_print( L, func, filename, lineno, "lua_rawget" );
    return result;
}
#else
APILOG_API void apilog_rawget( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               int index ) {
    lua_rawget( L, index );
    apilog_print( L, func, filename, lineno, "lua_rawget" );
}
#endif
#undef lua_rawget
#define lua_rawget( L, index ) \
    apilog_rawget( apilog_func, __FILE__, __LINE__, (L), (index) )


#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
APILOG_API int apilog_rawgeti( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               int index,
                               lua_Integer n ) {
    int result = lua_rawgeti( L, index, n );
    apilog_print( L, func, filename, lineno, "lua_rawgeti" );
    return result;
}
#else
APILOG_API void apilog_rawgeti( char const* func,
                                char const* filename,
                                int lineno,
                                lua_State* L,
                                int index,
                                int n ) {
    lua_rawgeti( L, index, n );
    apilog_print( L, func, filename, lineno, "lua_rawgeti" );
}
#endif
#undef lua_rawgeti
#define lua_rawgeti( L, index, n ) \
    apilog_rawgeti( apilog_func, __FILE__, __LINE__, (L), (index), (n) )


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
APILOG_API int apilog_rawgetp( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               int index,
                               void const* p ) {
    int result = lua_rawgetp( L, index, p );
    apilog_print( L, func, filename, lineno, "lua_rawgetp" );
    return result;
}
#else
APILOG_API void apilog_rawgetp( char const* func,
                                char const* filename,
                                int lineno,
                                lua_State* L,
                                int index,
                                void const* p ) {
    lua_rawgetp( L, index, p );
    apilog_print( L, func, filename, lineno, "lua_rawgetp" );
}
#endif
#undef lua_rawgetp
#define lua_rawgetp( L, index, p ) \
    apilog_rawgetp( apilog_func, __FILE__, __LINE__, (L), (index), (p) )
#endif


APILOG_API void apilog_rawset( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               int index ) {
    lua_rawset( L, index );
    apilog_print( L, func, filename, lineno, "lua_rawset" );
}
#undef lua_rawset
#define lua_rawset( L, index ) \
    apilog_rawset( apilog_func, __FILE__, __LINE__, (L), (index) )


APILOG_API void apilog_rawseti( char const* func,
                                char const* filename,
                                int lineno,
                                lua_State* L,
                                int index,
#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
                                lua_Integer n
#else
                                int n
#endif
                              ) {
    lua_rawseti( L, index, n );
    apilog_print( L, func, filename, lineno, "lua_rawseti" );
}
#undef lua_rawseti
#define lua_rawseti( L, index, n ) \
    apilog_rawseti( apilog_func, __FILE__, __LINE__, (L), (index), (n) )


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API void apilog_rawsetp( char const* func,
                                char const* filename,
                                int lineno,
                                lua_State* L,
                                int index,
                                void const* p ) {
    lua_rawsetp( L, index, p );
    apilog_print( L, func, filename, lineno, "lua_rawsetp" );
}
#undef lua_rawsetp
#define lua_rawsetp( L, index, p ) \
    apilog_rawsetp( apilog_func, __FILE__, __LINE__, (L), (index), (p) )
#endif


APILOG_API void apilog_remove( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               int index ) {
    lua_remove( L, index );
    apilog_print( L, func, filename, lineno, "lua_remove" );
}
#undef lua_remove
#define lua_remove( L, index ) \
    apilog_remove( apilog_func, __FILE__, __LINE__, (L), (index) )


APILOG_API void apilog_replace( char const* func,
                                char const* filename,
                                int lineno,
                                lua_State* L,
                                int index ) {
    lua_replace( L, index );
    apilog_print( L, func, filename, lineno, "lua_replace" );
}
#undef lua_replace
#define lua_replace( L, index ) \
    apilog_replace( apilog_func, __FILE__, __LINE__, (L), (index) )


#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
APILOG_API void apilog_rotate( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               int idx,
                               int n ) {
    lua_rotate( L, idx, n );
    apilog_print( L, func, filename, lineno, "lua_rotate" );
}
#undef lua_rotate
#define lua_rotate( L, idx, n ) \
    apilog_rotate( apilog_func, __FILE__, __LINE__, (L), (idx), (n) )
#endif


#if LUA_VERSION_NUM == 501
APILOG_API int apilog_setfenv( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               int index ) {
    int result = lua_setfenv( L, index );
    apilog_print( L, func, filename, lineno, "lua_setfenv" );
    return result;
}
#undef lua_setfenv
#define lua_setfenv( L, index ) \
    apilog_setfenv( apilog_func, __FILE__, __LINE__, (L), (index) )
#endif


APILOG_API void apilog_setfield( char const* func,
                                 char const* filename,
                                 int lineno,
                                 lua_State* L,
                                 int index,
                                 char const* k ) {
    lua_setfield( L, index, k );
    apilog_print( L, func, filename, lineno, "lua_setfield" );
}
#undef lua_setfield
#define lua_setfield( L, index, k ) \
    apilog_setfield( apilog_func, __FILE__, __LINE__, (L), (index), (k) )


APILOG_API void apilog_setglobal( char const* func,
                                  char const* filename,
                                  int lineno,
                                  lua_State* L,
                                  char const* name ) {
    lua_setglobal( L, name );
    apilog_print( L, func, filename, lineno, "lua_setglobal" );
}
#undef lua_setglobal
#define lua_setglobal( L, name ) \
    apilog_setglobal( apilog_func, __FILE__, __LINE__, (L), (name) )


#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
APILOG_API void apilog_seti( char const* func,
                             char const* filename,
                             int lineno,
                             lua_State* L,
                             int index,
                             lua_Integer n ) {
    lua_seti( L, index, n );
    apilog_print( L, func, filename, lineno, "lua_seti" );
}
#undef lua_seti
#define lua_seti( L, index, n ) \
    apilog_seti( apilog_func, __FILE__, __LINE__, (L), (index), (n) )
#endif


APILOG_API void apilog_setmetatable( char const* func,
                                     char const* filename,
                                     int lineno,
                                     lua_State* L,
                                     int index ) {
    lua_setmetatable( L, index );
    apilog_print( L, func, filename, lineno, "lua_setmetatable" );
}
#undef lua_setmetatable
#define lua_setmetatable( L, index ) \
    apilog_setmetatable( apilog_func, __FILE__, __LINE__, (L), (index) )


APILOG_API void apilog_settable( char const* func,
                                 char const* filename,
                                 int lineno,
                                 lua_State* L,
                                 int index ) {
    lua_settable( L, index );
    apilog_print( L, func, filename, lineno, "lua_settable" );
}
#undef lua_settable
#define lua_settable( L, index ) \
    apilog_settable( apilog_func, __FILE__, __LINE__, (L), (index) )


APILOG_API void apilog_settop( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               int index ) {
    lua_settop( L, index );
    apilog_print( L, func, filename, lineno, "lua_settop" );
}
#undef lua_settop
#define lua_settop( L, index ) \
    apilog_settop( apilog_func, __FILE__, __LINE__, (L), (index) )


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API void apilog_setuservalue( char const* func,
                                     char const* filename,
                                     int lineno,
                                     lua_State* L,
                                     int index ) {
    lua_setuservalue( L, index );
    apilog_print( L, func, filename, lineno, "lua_setuservalue" );
}
#undef lua_setuservalue
#define lua_setuservalue( L, index ) \
    apilog_setuservalue( apilog_func, __FILE__, __LINE__, (L), (index) )
#endif


#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
APILOG_API size_t apilog_stringtonumber( char const* func,
                                         char const* filename,
                                         int lineno,
                                         lua_State* L,
                                         char const* s ) {
    size_t result = lua_stringtonumber( L, s );
    apilog_print( L, func, filename, lineno, "lua_stringtonumber" );
    return result;
}
#undef lua_stringtonumber
#define lua_stringtonumber( L, s ) \
    apilog_stringtonumber( apilog_func, __FILE__, __LINE__, (L), (s) )
#endif




APILOG_API int apilog_getinfo( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               char const* what,
                               lua_Debug* ar ) {
    int result = lua_getinfo( L, what, ar );
    apilog_print( L, func, filename, lineno, "lua_getinfo" );
    return result;
}
#undef lua_getinfo
#define lua_getinfo( L, what, ar ) \
    apilog_getinfo( apilog_func, __FILE__, __LINE__, (L), (what), (ar) )


APILOG_API char const* apilog_getlocal( char const* func,
                                        char const* filename,
                                        int lineno,
                                        lua_State* L,
#if LUA_VERSION_NUM >= 503
                                        lua_Debug const* ar,
#else
                                        lua_Debug* ar,
#endif
                                        int n ) {
    char const* result = lua_getlocal( L, ar, n );
    apilog_print( L, func, filename, lineno, "lua_getlocal" );
    return result;
}
#undef lua_getlocal
#define lua_getlocal( L, ar, n ) \
    apilog_getlocal( apilog_func, __FILE__, __LINE__, (L), (ar), (n) )


APILOG_API char const* apilog_getupvalue( char const* func,
                                          char const* filename,
                                          int lineno,
                                          lua_State* L,
                                          int findex,
                                          int n ) {
    char const* result = lua_getupvalue( L, findex, n );
    apilog_print( L, func, filename, lineno, "lua_getupvalue" );
    return result;
}
#undef lua_getupvalue
#define lua_getupvalue( L, findex, n ) \
    apilog_getupvalue( apilog_func, __FILE__, __LINE__, (L), (findex), (n) )


APILOG_API char const* apilog_setlocal( char const* func,
                                        char const* filename,
                                        int lineno,
                                        lua_State* L,
#if LUA_VERSION_NUM >= 503
                                        lua_Debug const* ar,
#else
                                        lua_Debug* ar,
#endif
                                        int n ) {
    char const* result = lua_setlocal( L, ar, n );
    apilog_print( L, func, filename, lineno, "lua_setlocal" );
    return result;
}
#undef lua_setlocal
#define lua_setlocal( L, ar, n ) \
    apilog_setlocal( apilog_func, __FILE__, __LINE__, (L), (ar), (n) )


APILOG_API char const* apilog_setupvalue( char const* func,
                                          char const* filename,
                                          int lineno,
                                          lua_State* L,
                                          int findex,
                                          int n ) {
    char const* result = lua_setupvalue( L, findex, n );
    apilog_print( L, func, filename, lineno, "lua_setupvalue" );
    return result;
}
#undef lua_setupvalue
#define lua_setupvalue( L, findex, n ) \
    apilog_setupvalue( apilog_func, __FILE__, __LINE__, (L), (findex), (n) )




APILOG_API int apilogL_callmeta( char const* func,
                                 char const* filename,
                                 int lineno,
                                 lua_State* L,
                                 int obj,
                                 char const* e ) {
    int result = luaL_callmeta( L, obj, e );
    apilog_print( L, func, filename, lineno, "luaL_callmeta" );
    return result;
}
#undef luaL_callmeta
#define luaL_callmeta( L, obj, e ) \
    apilogL_callmeta( apilog_func, __FILE__, __LINE__, (L), (obj), (e) )


APILOG_API int apilogL_dofile( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               char const* fname ) {
    int result = luaL_dofile( L, fname );
    apilog_print( L, func, filename, lineno, "luaL_dofile" );
    return result;
}
#undef luaL_dofile
#define luaL_dofile( L, fname ) \
    apilogL_dofile( apilog_func, __FILE__, __LINE__, (L), (fname) )


APILOG_API int apilogL_dostring( char const* func,
                                 char const* filename,
                                 int lineno,
                                 lua_State* L,
                                 char const* s ) {
    int result = luaL_dostring( L, s );
    apilog_print( L, func, filename, lineno, "luaL_dostring" );
    return result;
}
#undef luaL_dostring
#define luaL_dostring( L, s ) \
    apilogL_dostring( apilog_func, __FILE__, __LINE__, (L), (s) )


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API int apilogL_execresult( char const* func,
                                   char const* filename,
                                   int lineno,
                                   lua_State* L,
                                   int stat ) {
    int result = luaL_execresult( L, stat );
    apilog_print( L, func, filename, lineno, "luaL_execresult" );
    return result;
}
#undef luaL_execresult
#define luaL_execresult( L, stat ) \
    apilogL_execresult( apilog_func, __FILE__, __LINE__, (L), (stat) )
#endif


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API int apilogL_fileresult( char const* func,
                                   char const* filename,
                                   int lineno,
                                   lua_State* L,
                                   int stat,
                                   char const* fname ) {
    int result = luaL_fileresult( L, stat, fname );
    apilog_print( L, func, filename, lineno, "luaL_fileresult" );
    return result;
}
#undef luaL_fileresult
#define luaL_fileresult( L, stat, fname ) \
    apilogL_fileresult( apilog_func, __FILE__, __LINE__, (L), (stat), (fname) )
#endif


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API int apilogL_getmetafield( char const* func,
                                     char const* filename,
                                     int lineno,
                                     lua_State* L,
                                     int obj,
                                     char const* e ) {
    int result = luaL_getmetafield( L, obj, e );
    apilog_print( L, func, filename, lineno, "luaL_getmetafield" );
    return result;
}
#undef luaL_getmetafield
#define luaL_getmetafield( L, obj, e ) \
    apilogL_getmetafield( apilog_func, __FILE__, __LINE__, (L), (obj), (e) )
#endif


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
#if LUA_VERSION_NUM >= 503 || defined( COMPAT53_API )
APILOG_API int apilogL_getmetatable( char const* func,
                                     char const* filename,
                                     int lineno,
                                     lua_State* L,
                                     char const* tname ) {
    int result = luaL_getmetatable( L, tname );
    apilog_print( L, func, filename, lineno, "luaL_getmetatable" );
    return result;
}
#else
APILOG_API void apilogL_getmetatable( char const* func,
                                      char const* filename,
                                      int lineno,
                                      lua_State* L,
                                      char const* tname ) {
    luaL_getmetatable( L, tname );
    apilog_print( L, func, filename, lineno, "luaL_getmetatable" );
}
#endif
#undef luaL_getmetatable
#define luaL_getmetatable( L, tname ) \
    apilogL_getmetatable( apilog_func, __FILE__, __LINE__, (L), (tname) )
#endif


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API int apilogL_getsubtable( char const* func,
                                    char const* filename,
                                    int lineno,
                                    lua_State* L,
                                    int idx,
                                    char const* fname ) {
    int result = luaL_getsubtable( L, idx, fname );
    apilog_print( L, func, filename, lineno, "luaL_getsubtable" );
    return result;
}
#undef luaL_getsubtable
#define luaL_getsubtable( L, fname ) \
    apilogL_getsubtable( apilog_func, __FILE__, __LINE__, (L), (fname) )
#endif


#if LUA_VERSION_NUM >= 502
APILOG_API char const* apilogL_gsub( char const* func,
                                     char const* filename,
                                     int lineno,
                                     lua_State* L,
                                     char const* s,
                                     char const* p,
                                     char const* r ) {
    char const* result = luaL_gsub( L, s, p, r );
    apilog_print( L, func, filename, lineno, "luaL_gsub" );
    return result;
}
#undef luaL_gsub
#define luaL_gsub( L, s, p, r ) \
    apilogL_gsub( apilog_func, __FILE__, __LINE__, (L), (s), (p), (r) )
#endif


APILOG_API int apilogL_loadbuffer( char const* func,
                                   char const* filename,
                                   int lineno,
                                   lua_State* L,
                                   char const* buf,
                                   size_t sz,
                                   char const* name ) {
    int result = luaL_loadbuffer( L, buf, sz, name );
    apilog_print( L, func, filename, lineno, "luaL_loadbuffer" );
    return result;
}
#undef luaL_loadbuffer
#define luaL_loadbuffer( L, buf, sz, name ) \
    apilogL_loadbuffer( apilog_func, __FILE__, __LINE__, (L), (buf), (sz), (name) )


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API int apilogL_loadbufferx( char const* func,
                                    char const* filename,
                                    int lineno,
                                    lua_State* L,
                                    char const* buf,
                                    size_t sz,
                                    char const* name,
                                    char const* mode ) {
    int result = luaL_loadbufferx( L, buf, sz, name, mode );
    apilog_print( L, func, filename, lineno, "luaL_loadbufferx" );
    return result;
}
#undef luaL_loadbufferx
#define luaL_loadbufferx( L, buf, sz, name, mode ) \
    apilogL_loadbufferx( apilog_func, __FILE__, __LINE__, (L), (buf), (sz), (name), (mode) )
#endif


APILOG_API int apilogL_loadfile( char const* func,
                                 char const* filename,
                                 int lineno,
                                 lua_State* L,
                                 char const* fname ) {
    int result = luaL_loadfile( L, fname );
    apilog_print( L, func, filename, lineno, "luaL_loadfile" );
    return result;
}
#undef luaL_loadfile
#define luaL_loadfile( L, fname ) \
    apilogL_loadfile( apilog_func, __FILE__, __LINE__, (L), (fname) )


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API int apilogL_loadfilex( char const* func,
                                  char const* filename,
                                  int lineno,
                                  lua_State* L,
                                  char const* fname,
                                  char const* mode ) {
    int result = luaL_loadfilex( L, fname, mode );
    apilog_print( L, func, filename, lineno, "luaL_loadfilex" );
    return result;
}
#undef luaL_loadfilex
#define luaL_loadfilex( L, name, mode ) \
    apilogL_loadfilex( apilog_func, __FILE__, __LINE__, (L), (name), (mode) )
#endif


APILOG_API int apilogL_loadstring( char const* func,
                                   char const* filename,
                                   int lineno,
                                   lua_State* L,
                                   char const* s ) {
    int result = luaL_loadstring( L, s );
    apilog_print( L, func, filename, lineno, "luaL_loadstring" );
    return result;
}
#undef luaL_loadstring
#define luaL_loadstring( L, s ) \
    apilogL_loadstring( apilog_func, __FILE__, __LINE__, (L), (s) )


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API void apilogL_newlib( char const* func,
                                char const* filename,
                                int lineno,
                                lua_State* L,
                                luaL_Reg const* r,
                                size_t n ) {
    (lua_createtable)( L, 0, n );
    (luaL_setfuncs)( L, r, 0 );
    apilog_print( L, func, filename, lineno, "luaL_newlib" );
}
#undef luaL_newlib
#define luaL_newlib( L, r ) \
    apilogL_newlib( apilog_func, __FILE__, __LINE__, (L), (r), (sizeof( (r) )/sizeof( *(r) ))-1 )
#endif


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API void apilogL_newlibtable( char const* func,
                                     char const* filename,
                                     int lineno,
                                     lua_State* L,
                                     size_t n ) {
    (lua_createtable)( L, 0, n );
    apilog_print( L, func, filename, lineno, "luaL_newlibtable" );
}
#undef luaL_newlibtable
#define luaL_newlibtable( L, r ) \
    apilogL_newlibtable( apilog_func, __FILE__, __LINE__, (L), (sizeof( (r) )/sizeof( *(r) ))-1 )
#endif


APILOG_API int apilogL_newmetatable( char const* func,
                                     char const* filename,
                                     int lineno,
                                     lua_State* L,
                                     char const* tname ) {
    int result = luaL_newmetatable( L, tname );
    apilog_print( L, func, filename, lineno, "luaL_newmetatable" );
    return result;
}
#undef luaL_newmetatable
#define luaL_newmetatable( L, tname ) \
    apilogL_newmetatable( apilog_func, __FILE__, __LINE__, (L), (tname) )


APILOG_API int apilogL_ref( char const* func,
                            char const* filename,
                            int lineno,
                            lua_State* L,
                            int t ) {
    int result = luaL_ref( L, t );
    apilog_print( L, func, filename, lineno, "luaL_ref" );
    return result;
}
#undef luaL_ref
#define luaL_ref( L, t ) \
    apilogL_ref( apilog_func, __FILE__, __LINE__, (L), (t) )


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API void apilogL_requiref( char const* func,
                                  char const* filename,
                                  int lineno,
                                  lua_State* L,
                                  char const* modname,
                                  lua_CFunction openf,
                                  int glb ) {
    luaL_requiref( L, modname, openf, glb );
    apilog_print( L, func, filename, lineno, "luaL_requiref" );
}
#undef luaL_requiref
#define luaL_requiref( L, modname, openf, glb ) \
    apilogL_requiref( apilog_func, __FILE__, __LINE__, (L), (modname), (openf), (glb) )
#endif


#if LUA_VERSION_NUM == 501
APILOG_API void apilogL_register( char const* func,
                                  char const* filename,
                                  int lineno,
                                  lua_State* L,
                                  char const* libname,
                                  luaL_Reg const* r ) {
    luaL_register( L, libname, r );
    apilog_print( L, func, filename, lineno, "luaL_register" );
}
#undef luaL_register
#define luaL_register( L, libname, r ) \
    apilogL_register( apilog_func, __FILE__, __LINE__, (L), (libname), (r) )
#endif


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API void apilogL_setfuncs( char const* func,
                                  char const* filename,
                                  int lineno,
                                  lua_State* L,
                                  luaL_Reg const* r,
                                  int nup ) {
    luaL_setfuncs( L, r, nup );
    apilog_print( L, func, filename, lineno, "luaL_setfuncs" );
}
#undef luaL_setfuncs
#define luaL_setfuncs( L, r, nup ) \
    apilogL_setfuncs( apilog_func, __FILE__, __LINE__, (L), (r), (nup) )
#endif


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API char const* apilogL_tolstring( char const* func,
                                          char const* filename,
                                          int lineno,
                                          lua_State* L,
                                          int idx,
                                          size_t* sz ) {
    char const* result = luaL_tolstring( L, idx, sz );
    apilog_print( L, func, filename, lineno, "luaL_tolstring" );
    return result;
}
#undef luaL_tolstring
#define luaL_tolstring( L, idx, sz ) \
    apilogL_tolstring( apilog_func, __FILE__, __LINE__, (L), (idx), (sz) )
#endif


#if LUA_VERSION_NUM >= 502 || defined( COMPAT53_API )
APILOG_API void apilogL_traceback( char const* func,
                                   char const* filename,
                                   int lineno,
                                   lua_State* L,
                                   lua_State* L1,
                                   char const* msg,
                                   int level ) {
    luaL_traceback( L, L1, msg, level );
    apilog_print( L, func, filename, lineno, "luaL_traceback" );
}
#undef luaL_traceback
#define luaL_traceback( L, L1, msg, level ) \
    apilogL_traceback( apilog_func, __FILE__, __LINE__, (L), (L1), (msg), (level) )
#endif


APILOG_API void apilogL_where( char const* func,
                               char const* filename,
                               int lineno,
                               lua_State* L,
                               int lvl ) {
    luaL_where( L, lvl );
    apilog_print( L, func, filename, lineno, "luaL_where" );
}
#undef luaL_where
#define luaL_where( L, lvl ) \
    apilogL_where( apilog_func, __FILE__, __LINE__, (L), (lvl) )


#undef apilog_func
APILOG_API char const* apilog_func = NULL;


/* TODO: check compatibility with compat53 */
/* TODO: disable warning for unused functions in visual c */

#endif /* APILOG_H_ */

