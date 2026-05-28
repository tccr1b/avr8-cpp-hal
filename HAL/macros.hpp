#pragma once


#define __atr_aligned__          __attribute__((aligned))
#define __atr_alloc_size__       __attribute__((alloc_size))
/* ATTRIBUTE: NORETURN
A few standard library functions, such as abort and exit, cannot return. GCC knows this automatically. Some 
programs define their own functions that never return. You can declare them noreturn to tell the compiler this 
fact. For example,
          void fatal () __attribute__ ((noreturn));
          
          void
          fatal (...)
          {
             ...  Print error message. ...
            exit (1);
          }
     
The noreturn keyword tells the compiler to assume that fatal cannot return. It can then optimize without regard to 
what would happen if fatal ever did return. This makes slightly better code. More importantly, it helps avoid 
spurious warnings of uninitialized variables.

The noreturn keyword does not affect the exceptional path when that applies: a noreturn-marked function may still 
return to the caller by throwing an exception or calling longjmp.
Do not assume that registers saved by the calling function are restored before calling the noreturn function.
It does not make sense for a noreturn function to have a return type other than void.
The attribute noreturn is not implemented in GCC versions earlier than 2.5. An alternative way to declare that a 
function does not return, which works in the current version and in some older versions, is as follows:

        typedef void voidfn ();
        volatile voidfn fatal;
     
This approach does not work in GNU C++.*/
#define __atr_no_return__        __attribute__((noreturn))
#define __atr_returns_twice__    __attribute__((returns_twice))
#define __atr_no_clone__         __attribute__((noclone))
/* ATTRIBUTE: PURE
Many functions have no effects except the return value and their return value depends only on the parameters 
and/or global variables. Such a function can be subject to common subexpression elimination and loop optimization 
just as an arithmetic operator would be. These functions should be declared with the attribute pure. For example,
        int square (int) __attribute__ ((pure));
says that the hypothetical function square is safe to call fewer times than the program says.
Some of common examples of pure functions are strlen or memcmp. Interesting non-pure functions are functions with 
infinite loops or those depending on volatile memory or other system resource, that may change between two 
consecutive calls (such as feof in a multithreading environment).
The attribute pure is not implemented in GCC versions earlier than 2.96.*/
#define __atr_pure__             __attribute__((pure))
/* ATTRIBUTE: CONST
Many functions do not examine any values except their arguments, and have no effects except the return value. 
Basically this is just slightly more strict class than the pure attribute below, since function is not allowed to 
read global memory.
Note that a function that has pointer arguments and examines the data pointed to must not be declared const. 
Likewise, a function that calls a non-const function usually must not be const. It does not make sense for a const 
function to return void.
The attribute const is not implemented in GCC versions earlier than 2.5. An alternative way to declare that a 
function has no side effects, which works in the current version and in some older versions, is as follows:

          typedef int intfn ();
          extern const intfn square;
     
This approach does not work in GNU C++ from 2.6.0 on, since the language specifies that the `const' must be 
attached to the return value.*/
#define __atr_const__            __attribute__((const))
#define __atr_no_throw__         __attribute__((nothrow))
#define __atr_sentinel__         __attribute__((sentinel))
/* ATTRIBUTE: ALWAYS_INLINE
Generally, functions are not inlined unless optimization is specified. For functions declared inline, this 
attribute inlines the function even if no optimization level was specified.*/
#define __atr_always_inline__    __attribute__((always_inline))
/* ATTRIBUTE: GNU_INLINE
This attribute should be used with a function which is also declared with the inline keyword. It directs GCC to 
treat the function as if it were defined in gnu90 mode even when compiling in C99 or gnu99 mode.
If the function is declared extern, then this definition of the function is used only for inlining. In no case is 
the function compiled as a standalone function, not even if you take its address explicitly. Such an address 
becomes an external reference, as if you had only declared the function, and had not defined it. This has almost 
the effect of a macro. The way to use this is to put a function definition in a header file with this attribute, 
and put another copy of the function, without extern, in a library file. The definition in the header file will 
cause most calls to the function to be inlined. If any uses of the function remain, they will refer to the single 
copy in the library. Note that the two definitions of the functions need not be precisely the same, although if 
they do not have the same effect your program may behave oddly.

In C, if the function is neither extern nor static, then the function is compiled as a standalone function, as 
well as being inlined where possible.

This is how GCC traditionally handled functions declared inline. Since ISO C99 specifies a different semantics for 
inline, this function attribute is provided as a transition measure and as a useful feature in its own right. This 
attribute is available in GCC 4.1.3 and later. It is available if either of the preprocessor macros 
__GNUC_GNU_INLINE__ or __GNUC_STDC_INLINE__ are defined. See An Inline Function is As Fast As a Macro.

In C++, this attribute does not depend on extern in any way, but it still requires the inline keyword to enable 
its special behavior.*/
#define __atr_gnu_inline__      __attribute__((gnu_inline))
/* ATTRIBUTE: NOINLINE
This function attribute prevents a function from being considered for inlining. If the function does not have 
side-effects, there are optimizations other than inlining that causes function calls to be optimized away, 
although the function call is live. To keep such calls from being optimized away, put
    asm ("");
(see Extended Asm) in the called function, to serve as a special side-effect.*/
#define __atr_no_inline__        __attribute__((noinline))
/* ATTRIBUTE: FLATTEN
Generally, inlining into a function is limited. For a function marked with this attribute, every call inside 
this function will be inlined, if possible. Whether the function itself is considered for inlining depends on its 
size and the current inlining parameters.*/
#define __atr_flatten__          __attribute__((flatten))
/* ATTRIBUTE: NAKED
Use this attribute on the ARM, AVR, MCORE, RX and SPU ports to indicate that the specified function does not need 
prologue/epilogue sequences generated by the compiler. It is up to the programmer to provide these sequences. The 
only statements that can be safely included in naked functions are asm statements that do not have operands. All 
other statements, including declarations of local variables, if statements, and so forth, should be avoided. Naked 
functions should be used to implement the body of an assembly function, while allowing the compiler to construct 
the requisite function declaration for the assembler.*/
#define __atr_naked__            __attribute__((naked))
/* ATTRIBUTE: WEAK
The weak attribute causes the declaration to be emitted as a weak symbol rather than a global. This is primarily 
useful in defining library functions which can be overridden in user code, though it can also be used with 
non-function declarations. Weak symbols are supported for ELF targets, and also for a.out targets when using the 
GNU assembler and linker.*/
#define __atr_weak__             __attribute__((weak))
#define __atr_warning__         __attribute__((warning("message")))
/* ATTRIBUTE: SECTION
Normally, the compiler places the code it generates in the text section. Sometimes, however, you need additional 
sections, or you need certain particular functions to appear in special sections. The section attribute specifies 
that a function lives in a particular section. For example, the declaration:
        extern void foobar (void) __attribute__ ((section ("bar")));
puts the function foobar in the bar section.
Some file formats do not support arbitrary sections so the section attribute is not available on all platforms. 
If you need to map the entire contents of a module to a particular section, consider using the facilities of the 
linker instead.*/
#define __atr_section_progmem__  __attribute__((section(".progmem")))
#define __atr_section_ram__      __attribute__((section(".ram")))
/* Bu veriler cihazın .hex dosyası ile birlikte bir .eep dosyası olarak çıkarılır.Programlayıcı (avrdude) ile cihaza 
yüklenirken EEPROM bölgesine yazılır.*/
#define __atr_section_eeprom__   __attribute__((section(".eeprom")))
/* .noinit bölümüne atılan değişkenlere C startup kodu dokunmaz. Sadece cihazın elektriği tamamen 
kesilirse içindeki veri kaybolur.*/
#define __atr_section_no_init__  __attribute__((section(".noinit")))
/* ATTRIBUTE: UNUSED
This attribute, attached to a variable, means that the variable is meant to be possibly unused. GCC will not 
produce a warning for this variable.*/
#define __atr_unused__              __attribute__((unused))
