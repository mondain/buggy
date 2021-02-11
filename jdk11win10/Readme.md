# Test Code for JDK 11 Issue on Windows 10

The test code here is for determining and displaying an issue present in OpenJDK 11 on Windows 10. The working Linux version is here as a check-point.

## Expected Successful Output

After compiling the native library and the Java class, execute it:

```sh
java JavaMain
```

The expected output:

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
Main new: 2871979580192
Main open
JMain opened: 2871979580192
Opened main for id: 2871979580192; now attempting close
JMain close: 2871979580192
Main remove: -1353540832
Main close
#
# A fatal error has been detected by the Java Runtime Environment:
#
#  EXCEPTION_ACCESS_VIOLATION (0xc0000005) at pc=0x00007ff83cb91360, pid=15272, tid=15336
#
# JRE version: OpenJDK Runtime Environment AdoptOpenJDK (11.0.10+9) (build 11.0.10+9)
# Java VM: OpenJDK 64-Bit Server VM AdoptOpenJDK (11.0.10+9, mixed mode, tiered, compressed oops, g1 gc, windows-amd64)
# Problematic frame:
# C  [main.dll+0x1360]
#
# No core dump will be written. Minidumps are not enabled by default on client versions of Windows
#
# An error report file with more information is saved as:
# C:\Users\Paul\Documents\GitHub\buggy\jdk11win10\hs_err_pid15272.log
#
# If you would like to submit a bug report, please visit:
#   https://github.com/AdoptOpenJDK/openjdk-support/issues
# The crash happened outside the Java Virtual Machine in native code.
# See problematic frame for where to report the bug.
#
```

_Note_ pointer numbers will change per execution

If the JVM crashes, this displays the issue!

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
cl /I "C:\Program Files\AdoptOpenJDK\jdk-11.0.10.9-hotspot\include" /I "C:\Program Files\AdoptOpenJDK\jdk-11.0.10.9-hotspot\include\win32" main.cpp /LD
```

_Note_ update paths as needed to fit your environment

* Compile java `javac JavaMain.java`
* Run it: `java JavaMain`

### Links

[Compiler Options](https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?redirectedfrom=MSDN&view=msvc-160)

[DEF Module](https://docs.microsoft.com/en-us/cpp/build/reference/def-specify-module-definition-file?redirectedfrom=MSDN&view=msvc-160)

[SO Question - howto build dll from cmdline](https://stackoverflow.com/questions/1130479/how-to-build-a-dll-from-the-command-line-in-windows-using-msvc)