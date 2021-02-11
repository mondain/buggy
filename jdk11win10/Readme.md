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

Verification checks on error:

```sh
nm libmain.so
ldd libmain.so
```

## Windows Build

https://stackoverflow.com/questions/1130479/how-to-build-a-dll-from-the-command-line-in-windows-using-msvc

```sh
cl.exe /LD <files-to-compile>
cl.exe /D_USRDLL /D_WINDLL <files-to-compile> <files-to-link> /link /DLL /OUT:<desired-dll-name>.dll

"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat" && cl /O2 /Iall /Iyour /Iincludes /D_USRDLL /D_WINDLL /DOTHER_DEFINES <libs> <source files> /LD /Fe<dll name> /link /DEF:<def name>.def
```

* Build main.dll with ms cl.exe

```sh
$ "c:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
$ cl /I %JDK_REPO%/src/java.base/share/native/include /I %JDK_REPO%/src/java.base/windows/native/include %JDK_REPO%/build/windows-x86_64-normal-server-release/images/jdk/lib/jvm.lib /Z7 /LD main.cpp
```

### Links

[Compiler Options](https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?redirectedfrom=MSDN&view=msvc-160)

[DEF Module](https://docs.microsoft.com/en-us/cpp/build/reference/def-specify-module-definition-file?redirectedfrom=MSDN&view=msvc-160)