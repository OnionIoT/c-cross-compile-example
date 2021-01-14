# c-cross-compile-example
Example of a C program for the Omega that needs to be cross-compiled

## Purpose

Demonstration project for cross-compiling a C program for the Omega using the LEDE build system. In this case, cross-compilation is required since the C program relies on the `ugpio` library. 

This cannot be compiled directly on the Omega since the header files for libraries that are not regularly part of the C standard library are not included in the Omega's filesystem as a space saving measure. See the [Limitations of Compilation on the Omega](https://docs.onion.io/omega2-docs/c-compiler-on-omega.html#limitations) for more details.

## Companion Example

See the [Cross Compiling for the Omega](https://docs.onion.io/omega2-docs/cross-compiling.html) article in the [Onion Docs](https://docs.onion.io).

Or watch our [video tutorial on C Programs and Cross Compiling for the Omega2](https://www.youtube.com/watch?v=VOs75UsGe_o):

[![](https://onion.io/wp-content/uploads/2018/06/cross-compiling-cover.png)](https://www.youtube.com/watch?v=VOs75UsGe_o)

## Usage and Example Output

Usage:

```
sh xCompile.sh -buildroot <PATH TO BUILD SYSTEM> -lib <REQUIRED LIBRARY>
```

Example Usage:

```
sh xCompile.sh -buildroot /root/source/ -lib ugpio
```

Example Output:

```
root@c1d850940b97:~/c-cross-compile-example# sh xCompile.sh -buildroot /root/source/ -lib ugpio -debug
CC=/root/source//staging_dir/toolchain-mipsel_24kc_gcc-5.4.0_musl-1.1.16/bin/mipsel-openwrt-linux-gcc
CXX=/root/source//staging_dir/toolchain-mipsel_24kc_gcc-5.4.0_musl-1.1.16/bin/mipsel-openwrt-linux-g++
LD=/root/source//staging_dir/toolchain-mipsel_24kc_gcc-5.4.0_musl-1.1.16/bin/mipsel-openwrt-linux-ld
CFLAGS=-Os -pipe -mno-branch-likely -mips32r2 -mtune=24kc -fno-caller-saves -fno-plt -fhonour-copts -Wno-error=unused-but-set-variable -Wno-error=unused-result -msoft-float -mips16 -minterlink-mips16 -Wformat -Werror=format-security -fstack-protector -D_FORTIFY_SOURCE=1 -Wl,-z,now -Wl,-z,relro -I /root/source//staging_dir/toolchain-mipsel_24kc_gcc-5.4.0_musl-1.1.16/usr/include -I /root/source//staging_dir/toolchain-mipsel_24kc_gcc-5.4.0_musl-1.1.16/include -I /root/source//staging_dir/target-mipsel_24kc_musl-1.1.16/usr/include -I /root/source//staging_dir/target-mipsel_24kc_musl-1.1.16/include
LDFLAGS=-L/root/source//staging_dir/toolchain-mipsel_24kc_gcc-5.4.0_musl-1.1.16/usr/lib -L/root/source//staging_dir/toolchain-mipsel_24kc_gcc-5.4.0_musl-1.1.16/lib -L/root/source//staging_dir/target-mipsel_24kc_musl-1.1.16/usr/lib -L/root/source//staging_dir/target-mipsel_24kc_musl-1.1.16/lib
USER_LIBS=ugpio

Compiling C program
/root/source//staging_dir/toolchain-mipsel_24kc_gcc-5.4.0_musl-1.1.16/bin/mipsel-openwrt-linux-gcc -Os -pipe -mno-branch-likely -mips32r2 -mtune=24kc -fno-caller-saves -fno-plt -fhonour-copts -Wno-error=unused-but-set-variable -Wno-error=unused-result -msoft-float -mips16 -minterlink-mips16 -Wformat -Werror=format-security -fstack-protector -D_FORTIFY_SOURCE=1 -Wl,-z,now -Wl,-z,relro -I /root/source//staging_dir/toolchain-mipsel_24kc_gcc-5.4.0_musl-1.1.16/usr/include -I /root/source//staging_dir/toolchain-mipsel_24kc_gcc-5.4.0_musl-1.1.16/include -I /root/source//staging_dir/target-mipsel_24kc_musl-1.1.16/usr/include -I /root/source//staging_dir/target-mipsel_24kc_musl-1.1.16/include gpioRead.c -o gpioRead -L/root/source//staging_dir/toolchain-mipsel_24kc_gcc-5.4.0_musl-1.1.16/usr/lib -L/root/source//staging_dir/toolchain-mipsel_24kc_gcc-5.4.0_musl-1.1.16/lib -L/root/source//staging_dir/target-mipsel_24kc_musl-1.1.16/usr/lib -L/root/source//staging_dir/target-mipsel_24kc_musl-1.1.16/lib -lugpio
```

## Additional Usage Info

### What if I want to cross compile without any libraries?

1. Modify line 10 in the Makefile to remove the `-l$(LIB)`
2. Run the xCompile.sh script without the -lib argument

### What if I want to cross compile with additional libraries?

Add the additional libraries to the `-lib` argument input. Just remember to surround the input with quotes, and to add `-l` to each library after the first one.

For example, say you wanted to use xCompile and link the ugpio, oniondebug, and onioni2c libraries. The command would look like this: 

```
sh xCompile.sh -buildroot /home/ubuntu/source -lib "ugpio -loniondebug -lonioni2c"
```


## Background Info

* The `gpioRead.c` program uses the `ugpio` library

### Cross Compilation

In a nut-shell, cross complation means using your computer's processor (that is likely based on the x86 CPU architecture) to compile a program for a different CPU architecture. In this case, we are cross compiling for the Onion Omega2's MIPS CPU architecture.

For this specific repo, the `xCompile.sh` script takes care of cross-compiling the `gpioRead.c` program into an executable for the Omega's MIPS arcitrecture. To do so it will need to be informed the location of the LEDE Build System and which libraries the program requires. 

* The LEDE Build System is required so the script has access to the LEDE cross-compilers, library headers, and library objects compiled in the LEDE Build System.
* The LEDE Build System will have to be setup and compiled for this to work. See our [article on setting up the LEDE Build System](https://onion.io/2bt-cross-compiling-c-programs-part-1/) for instructions.
  
It ensures that the `makefile` is invoked with arguments that:
  * Specify the compilers to be used by the makefile are the LEDE cross-compilers for the Omega's MIPS CPU architecture
  * Specify the compilation (`CFLAGS` for C programs and `CXXFLAGS` for C++ programs) and linking flags (`LDFLAGS`) to:
    * Ensure all header files that are part of the LEDE Build System can be included in the program. In this case that is  `<ugpio/ugpio.h>`.
      * This is accomplished in `CFLAGS` with the `-I <directory>` statements. Take a look at the specified directories, one of them will contain the `ugpio/gpio.h` header this program requires.
    * Ensure that library shared objects that are part of the LEDE Build System **can** be linked into the final `gpioRead` executable. 
      * This is accomplised in `LDFLAGS` with the `-L<directory>` statements. Take a look at the specified directories, one of them will contain the `libugpio` shared object this program requires.
    * Ensure that the specified library/libraries actually **are** linked in to the final `gpioRead` executable. 
      * This is accomplished with the `-lgpio` statement in the compilation command
