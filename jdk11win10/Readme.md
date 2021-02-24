# Test Code for JDK 11 Issue on Windows 10

The test code here is for determining and displaying an issue present in JDK 11 on Windows 10. The working Linux version is here as a check-point.

## Expected Output

After compiling the native library and the Java class, execute it:

```sh
java JavaMain
```

The expected successful output:

```sh
Opening main
Main new: 140494085752560
Main open
JMain opened: 140494085752560
Opened main for id: 140494085752560; now attempting close
JMain close: 140494085752560
Main remove: 140494085752560
Main close
Main closed
Main destruct
JMain closed
Closed main
```

Failed output on Windows 10: 

```sh
Opening main
Main new: 2675066083424
Main open
JMain opened: 2675066083424
Opened main for id: 2675066083424; now attempting close
JMain close: 2675066083424
Main remove: -698541984
Main close
#
# A fatal error has been detected by the Java Runtime Environment:
#
#  EXCEPTION_ACCESS_VIOLATION (0xc0000005) at pc=0x00007ffa3cef1360, pid=11012, tid=5204
#
# JRE version: Java(TM) SE Runtime Environment 18.9 (11.0.10+8) (build 11.0.10+8-LTS-162)
# Java VM: Java HotSpot(TM) 64-Bit Server VM 18.9 (11.0.10+8-LTS-162, mixed mode, tiered, compressed oops, g1 gc, windows-amd64)
# Problematic frame:
# C  [main.dll+0x1360]
#
# No core dump will be written. Minidumps are not enabled by default on client versions of Windows
#
# An error report file with more information is saved as:
# C:\Users\monda\workspace\github\buggy\jdk11win10\hs_err_pid11012.log
#
# If you would like to submit a bug report, please visit:
#   https://bugreport.java.com/bugreport/crash.jsp
# The crash happened outside the Java Virtual Machine in native code.
# See problematic frame for where to report the bug.
#
```

_Note_ pointer numbers will change per execution

If the JVM crashes, this displays the issue!

_Note_ Tested with Oracle JDK 11, AdoptJDK, and Corretto JDK 11.

## Linux Build

* Compile the object file

```sh
g++ -std=c++17 -c -shared-libgcc -fPIC -Wl,-export-dynamic -Wl,-rpath=$$ORIGIN -I /usr/lib/jvm/java-11-amazon-corretto/include -I /usr/lib/jvm/java-11-amazon-corretto/include/linux main.cpp -o libmain.o
```

* Compile object file into shared library `gcc -shared -o libmain.so libmain.o`
* Compile java `javac JavaMain.java`
* Export path to the so file `export LD_LIBRARY_PATH=<path to .SO file>`
* Export libstdc++ so we don't get the undefined symbol: `_ZTVN10__cxxabiv117__class_type_infoE` error `export LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libstdc++.so.6`
* Run it: `java JavaMain`

Verification checks on error:

```sh
nm libmain.so
ldd libmain.so
```

## Windows Build

* Setup build environment:

```sh
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
```

* Build main.dll with ms cl.exe

```sh
cl /I "%JAVA_HOME%\include" /I "%JAVA_HOME%\include\win32" main.cpp /LD
```

Oracle JDK example

```sh
cl /I "C:\Program Files\Java\jdk-11.0.10\include" /I "C:\Program Files\Java\jdk-11.0.10\include\win32" main.cpp /LD
```

_Note_ update paths as needed to fit your environment

* Compile java `javac JavaMain.java`
* Run it: `java JavaMain`

### Links

[Compiler Options](https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?redirectedfrom=MSDN&view=msvc-160)

[DEF Module](https://docs.microsoft.com/en-us/cpp/build/reference/def-specify-module-definition-file?redirectedfrom=MSDN&view=msvc-160)

[SO Question - howto build dll from cmdline](https://stackoverflow.com/questions/1130479/how-to-build-a-dll-from-the-command-line-in-windows-using-msvc)
