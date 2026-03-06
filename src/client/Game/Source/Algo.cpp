
#include "stdafx.h"
#include "Algo.h"
#include "NetIF.h"

#define INL inline
#define NKD __declspec(naked)

int INL NKD big_apple()
    {
    __asm {
        push ebp
        mov ebp, esp 
        sub esp, 20h 
        and esp, 0FFFFFFF0h 
        fld st(0) 
        fst dword ptr [esp + 18h] 
        fistp qword ptr [esp + 10h] 
        fild qword ptr [esp + 10h] 
        mov edx, dword ptr [esp + 18h] 
        mov eax, dword ptr [esp + 10h] 
        test eax, eax 
        je L3

L1:
        fsubp st(1), st 
        test edx, edx 
        jns L2 
        fstp dword ptr [esp] 
        mov ecx, dword ptr [esp] 
        xor ecx, 80000000h 
        add ecx, 7FFFFFFFh 
        adc eax, 0 
        mov edx, dword ptr [esp + 14h] 
        adc edx, 0 
        jmp EXIT

L2:
        fstp dword ptr [esp] 
        mov ecx, dword ptr [esp] 
        add ecx, 7FFFFFFFh 
        sbb eax, 0 
        mov edx, dword ptr [esp + 14h] 
        sbb edx, 0 
        jmp EXIT

L3:
        mov edx, dword ptr [esp + 14h] 
        test edx, 7FFFFFFFh 
        jne L1 
        fstp dword ptr [esp + 18h] 
        fstp dword ptr [esp + 18h] 

EXIT:
        leave
        ret}}

#pragma warning(disable : 4800)
int lua_fox_boff(lua_State* L)
    {
    if ((lua_gettop(L) == 3) && lua_isuserdata(L, 1)
        && lua_isnumber(L, 2) && lua_isnumber(L, 3))
        {
        __asm {
            push 1
            push L
            call lua_touserdata
            add esp, 8
            mov ebx, eax
            push 2
            push L
            call lua_tonumber
            add esp, 8
            call big_apple 
            mov esi, eax
            push 3
            push L
            call lua_tonumber
            add esp, 8
            call big_apple
            mov cl, al
            mov al, byte ptr [ebx + esi]
            xor al, cl
            mov byte ptr [ebx + esi], al
            }}
    return 0;}

int lua_dog_blog(lua_State* L)
    {
    if ((lua_gettop(L) == 3) && lua_isuserdata(L, 1)
        && lua_isnumber(L, 2) && lua_isnumber(L, 3))
        {
        __asm {
            push 1
            push L
            call lua_touserdata
            add esp, 8
            mov ebx, eax
            push 2
            push L
            call lua_tonumber
            add esp, 8
            call big_apple 
            mov esi, eax
            push 3
            push L
            call lua_tonumber
            add esp, 8
            call big_apple
            mov cl, al
            mov al, byte ptr [ebx + esi]
            rol al, cl
            mov byte ptr [ebx + esi], al
            }}
    return 0;}

int lua_dog_brog(lua_State* L)
    {
    if ((lua_gettop(L) == 3) && lua_isuserdata(L, 1)
        && lua_isnumber(L, 2) && lua_isnumber(L, 3))
        {
        __asm {
            push 1
            push L
            call lua_touserdata
            add esp, 8
            mov ebx, eax
            push 2
            push L
            call lua_tonumber
            add esp, 8
            call big_apple 
            mov esi, eax
            push 3
            push L
            call lua_tonumber
            add esp, 8
            call big_apple
            mov cl, al
            mov al, byte ptr [ebx + esi]
            ror al, cl
            mov byte ptr [ebx + esi], al
            }}
    return 0;}

int lua_cat_fish(lua_State* L)
    {
    if ((lua_gettop(L) == 2) && lua_isnumber(L, 1)
        && lua_isnumber(L, 2))
        {
        unsigned int a = (unsigned int)lua_tonumber(L, 1);
        unsigned int b = (unsigned int)lua_tonumber(L, 2);
        lua_pushnumber(L, a / b);
        lua_pushnumber(L, a % b); return 2;}
    else return 0;}

#define REGFN(fn)  { lua_pushstring(L, "" #fn ""); \
	lua_pushcfunction(L, lua_##fn); \
	lua_settable(L, LUA_GLOBALSINDEX); } 

lua_State* init_lua()
    {
    lua_State* L = lua_open();
    luaL_openlibs(L);
    lua_register(L, "lua_fox_boff", lua_fox_boff);
    lua_register(L, "lua_dog_blog", lua_dog_blog);
    lua_register(L, "lua_dog_brog", lua_dog_brog);
    lua_register(L, "lua_cat_fish", lua_cat_fish);

	REGFN(HandleNetMessage);
    return L;}

void exit_lua(lua_State* L) {lua_close(L);}

static void callalert(lua_State* L, int status)
    {
    if (status != 0)
        {
        lua_getglobal(L, "_ALERT");
        if (lua_isfunction(L, -1))
            {
            lua_insert(L, -2);
            lua_call(L, 1, 0);}
        else
            { // no _ALERT function; print it on stderr
            fprintf(stderr, "%s\n", lua_tostring(L, -2));
            lua_pop(L, 2);}}}

int load_luc(lua_State* L, char const* fname)
    {
    int status = luaL_loadfile(L, fname);
    if (status != 0)
        {
        printf("load luc file %s error\n", fname);
        return -1;}
    status = lua_pcall(L, 0, LUA_MULTRET, 0);
    callalert(L, status);
    return status;}
