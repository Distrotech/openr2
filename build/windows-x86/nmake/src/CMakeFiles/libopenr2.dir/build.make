# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake 2.8\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake 2.8\bin\cmake.exe" -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "C:\Program Files\CMake 2.8\bin\cmake-gui.exe"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake"

# Include any dependencies generated for this target.
include src\CMakeFiles\libopenr2.dir\depend.make

# Include the progress variables for this target.
include src\CMakeFiles\libopenr2.dir\progress.make

# Include the compile flags for this target's objects.
include src\CMakeFiles\libopenr2.dir\flags.make

src\CMakeFiles\libopenr2.dir\r2chan.c.obj: src\CMakeFiles\libopenr2.dir\flags.make
src\CMakeFiles\libopenr2.dir\r2chan.c.obj: ..\..\..\src\r2chan.c
	$(CMAKE_COMMAND) -E cmake_progress_report "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/libopenr2.dir/r2chan.c.obj"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) /FoCMakeFiles\libopenr2.dir\r2chan.c.obj /Fd"C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\src\libopenr2.pdb" -c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2chan.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2chan.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libopenr2.dir/r2chan.c.i"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  > CMakeFiles\libopenr2.dir\r2chan.c.i @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) -E "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2chan.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2chan.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libopenr2.dir/r2chan.c.s"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB /FAs /FoNUL /FaCMakeFiles\libopenr2.dir\r2chan.c.s /c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2chan.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2chan.c.obj.requires:
.PHONY : src\CMakeFiles\libopenr2.dir\r2chan.c.obj.requires

src\CMakeFiles\libopenr2.dir\r2chan.c.obj.provides: src\CMakeFiles\libopenr2.dir\r2chan.c.obj.requires
	$(MAKE) -f src\CMakeFiles\libopenr2.dir\build.make /nologo -$(MAKEFLAGS) src\CMakeFiles\libopenr2.dir\r2chan.c.obj.provides.build
.PHONY : src\CMakeFiles\libopenr2.dir\r2chan.c.obj.provides

src\CMakeFiles\libopenr2.dir\r2chan.c.obj.provides.build: src\CMakeFiles\libopenr2.dir\r2chan.c.obj
.PHONY : src\CMakeFiles\libopenr2.dir\r2chan.c.obj.provides.build

src\CMakeFiles\libopenr2.dir\r2context.c.obj: src\CMakeFiles\libopenr2.dir\flags.make
src\CMakeFiles\libopenr2.dir\r2context.c.obj: ..\..\..\src\r2context.c
	$(CMAKE_COMMAND) -E cmake_progress_report "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\CMakeFiles" $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/libopenr2.dir/r2context.c.obj"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) /FoCMakeFiles\libopenr2.dir\r2context.c.obj /Fd"C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\src\libopenr2.pdb" -c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2context.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2context.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libopenr2.dir/r2context.c.i"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  > CMakeFiles\libopenr2.dir\r2context.c.i @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) -E "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2context.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2context.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libopenr2.dir/r2context.c.s"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB /FAs /FoNUL /FaCMakeFiles\libopenr2.dir\r2context.c.s /c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2context.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2context.c.obj.requires:
.PHONY : src\CMakeFiles\libopenr2.dir\r2context.c.obj.requires

src\CMakeFiles\libopenr2.dir\r2context.c.obj.provides: src\CMakeFiles\libopenr2.dir\r2context.c.obj.requires
	$(MAKE) -f src\CMakeFiles\libopenr2.dir\build.make /nologo -$(MAKEFLAGS) src\CMakeFiles\libopenr2.dir\r2context.c.obj.provides.build
.PHONY : src\CMakeFiles\libopenr2.dir\r2context.c.obj.provides

src\CMakeFiles\libopenr2.dir\r2context.c.obj.provides.build: src\CMakeFiles\libopenr2.dir\r2context.c.obj
.PHONY : src\CMakeFiles\libopenr2.dir\r2context.c.obj.provides.build

src\CMakeFiles\libopenr2.dir\r2log.c.obj: src\CMakeFiles\libopenr2.dir\flags.make
src\CMakeFiles\libopenr2.dir\r2log.c.obj: ..\..\..\src\r2log.c
	$(CMAKE_COMMAND) -E cmake_progress_report "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\CMakeFiles" $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/libopenr2.dir/r2log.c.obj"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) /FoCMakeFiles\libopenr2.dir\r2log.c.obj /Fd"C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\src\libopenr2.pdb" -c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2log.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2log.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libopenr2.dir/r2log.c.i"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  > CMakeFiles\libopenr2.dir\r2log.c.i @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) -E "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2log.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2log.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libopenr2.dir/r2log.c.s"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB /FAs /FoNUL /FaCMakeFiles\libopenr2.dir\r2log.c.s /c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2log.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2log.c.obj.requires:
.PHONY : src\CMakeFiles\libopenr2.dir\r2log.c.obj.requires

src\CMakeFiles\libopenr2.dir\r2log.c.obj.provides: src\CMakeFiles\libopenr2.dir\r2log.c.obj.requires
	$(MAKE) -f src\CMakeFiles\libopenr2.dir\build.make /nologo -$(MAKEFLAGS) src\CMakeFiles\libopenr2.dir\r2log.c.obj.provides.build
.PHONY : src\CMakeFiles\libopenr2.dir\r2log.c.obj.provides

src\CMakeFiles\libopenr2.dir\r2log.c.obj.provides.build: src\CMakeFiles\libopenr2.dir\r2log.c.obj
.PHONY : src\CMakeFiles\libopenr2.dir\r2log.c.obj.provides.build

src\CMakeFiles\libopenr2.dir\r2proto.c.obj: src\CMakeFiles\libopenr2.dir\flags.make
src\CMakeFiles\libopenr2.dir\r2proto.c.obj: ..\..\..\src\r2proto.c
	$(CMAKE_COMMAND) -E cmake_progress_report "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\CMakeFiles" $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/libopenr2.dir/r2proto.c.obj"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) /FoCMakeFiles\libopenr2.dir\r2proto.c.obj /Fd"C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\src\libopenr2.pdb" -c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2proto.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2proto.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libopenr2.dir/r2proto.c.i"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  > CMakeFiles\libopenr2.dir\r2proto.c.i @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) -E "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2proto.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2proto.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libopenr2.dir/r2proto.c.s"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB /FAs /FoNUL /FaCMakeFiles\libopenr2.dir\r2proto.c.s /c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2proto.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2proto.c.obj.requires:
.PHONY : src\CMakeFiles\libopenr2.dir\r2proto.c.obj.requires

src\CMakeFiles\libopenr2.dir\r2proto.c.obj.provides: src\CMakeFiles\libopenr2.dir\r2proto.c.obj.requires
	$(MAKE) -f src\CMakeFiles\libopenr2.dir\build.make /nologo -$(MAKEFLAGS) src\CMakeFiles\libopenr2.dir\r2proto.c.obj.provides.build
.PHONY : src\CMakeFiles\libopenr2.dir\r2proto.c.obj.provides

src\CMakeFiles\libopenr2.dir\r2proto.c.obj.provides.build: src\CMakeFiles\libopenr2.dir\r2proto.c.obj
.PHONY : src\CMakeFiles\libopenr2.dir\r2proto.c.obj.provides.build

src\CMakeFiles\libopenr2.dir\r2utils.c.obj: src\CMakeFiles\libopenr2.dir\flags.make
src\CMakeFiles\libopenr2.dir\r2utils.c.obj: ..\..\..\src\r2utils.c
	$(CMAKE_COMMAND) -E cmake_progress_report "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\CMakeFiles" $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/libopenr2.dir/r2utils.c.obj"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) /FoCMakeFiles\libopenr2.dir\r2utils.c.obj /Fd"C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\src\libopenr2.pdb" -c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2utils.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libopenr2.dir/r2utils.c.i"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  > CMakeFiles\libopenr2.dir\r2utils.c.i @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) -E "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2utils.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libopenr2.dir/r2utils.c.s"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB /FAs /FoNUL /FaCMakeFiles\libopenr2.dir\r2utils.c.s /c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2utils.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2utils.c.obj.requires:
.PHONY : src\CMakeFiles\libopenr2.dir\r2utils.c.obj.requires

src\CMakeFiles\libopenr2.dir\r2utils.c.obj.provides: src\CMakeFiles\libopenr2.dir\r2utils.c.obj.requires
	$(MAKE) -f src\CMakeFiles\libopenr2.dir\build.make /nologo -$(MAKEFLAGS) src\CMakeFiles\libopenr2.dir\r2utils.c.obj.provides.build
.PHONY : src\CMakeFiles\libopenr2.dir\r2utils.c.obj.provides

src\CMakeFiles\libopenr2.dir\r2utils.c.obj.provides.build: src\CMakeFiles\libopenr2.dir\r2utils.c.obj
.PHONY : src\CMakeFiles\libopenr2.dir\r2utils.c.obj.provides.build

src\CMakeFiles\libopenr2.dir\r2engine.c.obj: src\CMakeFiles\libopenr2.dir\flags.make
src\CMakeFiles\libopenr2.dir\r2engine.c.obj: ..\..\..\src\r2engine.c
	$(CMAKE_COMMAND) -E cmake_progress_report "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\CMakeFiles" $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/libopenr2.dir/r2engine.c.obj"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) /FoCMakeFiles\libopenr2.dir\r2engine.c.obj /Fd"C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\src\libopenr2.pdb" -c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2engine.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2engine.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libopenr2.dir/r2engine.c.i"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  > CMakeFiles\libopenr2.dir\r2engine.c.i @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) -E "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2engine.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2engine.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libopenr2.dir/r2engine.c.s"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB /FAs /FoNUL /FaCMakeFiles\libopenr2.dir\r2engine.c.s /c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2engine.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2engine.c.obj.requires:
.PHONY : src\CMakeFiles\libopenr2.dir\r2engine.c.obj.requires

src\CMakeFiles\libopenr2.dir\r2engine.c.obj.provides: src\CMakeFiles\libopenr2.dir\r2engine.c.obj.requires
	$(MAKE) -f src\CMakeFiles\libopenr2.dir\build.make /nologo -$(MAKEFLAGS) src\CMakeFiles\libopenr2.dir\r2engine.c.obj.provides.build
.PHONY : src\CMakeFiles\libopenr2.dir\r2engine.c.obj.provides

src\CMakeFiles\libopenr2.dir\r2engine.c.obj.provides.build: src\CMakeFiles\libopenr2.dir\r2engine.c.obj
.PHONY : src\CMakeFiles\libopenr2.dir\r2engine.c.obj.provides.build

src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj: src\CMakeFiles\libopenr2.dir\flags.make
src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj: ..\..\..\src\r2ioabs.c
	$(CMAKE_COMMAND) -E cmake_progress_report "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\CMakeFiles" $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/libopenr2.dir/r2ioabs.c.obj"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) /FoCMakeFiles\libopenr2.dir\r2ioabs.c.obj /Fd"C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\src\libopenr2.pdb" -c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2ioabs.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2ioabs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libopenr2.dir/r2ioabs.c.i"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  > CMakeFiles\libopenr2.dir\r2ioabs.c.i @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) -E "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2ioabs.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2ioabs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libopenr2.dir/r2ioabs.c.s"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB /FAs /FoNUL /FaCMakeFiles\libopenr2.dir\r2ioabs.c.s /c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2ioabs.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj.requires:
.PHONY : src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj.requires

src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj.provides: src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj.requires
	$(MAKE) -f src\CMakeFiles\libopenr2.dir\build.make /nologo -$(MAKEFLAGS) src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj.provides.build
.PHONY : src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj.provides

src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj.provides.build: src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj
.PHONY : src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj.provides.build

src\CMakeFiles\libopenr2.dir\queue.c.obj: src\CMakeFiles\libopenr2.dir\flags.make
src\CMakeFiles\libopenr2.dir\queue.c.obj: ..\..\..\src\queue.c
	$(CMAKE_COMMAND) -E cmake_progress_report "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\CMakeFiles" $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/libopenr2.dir/queue.c.obj"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) /FoCMakeFiles\libopenr2.dir\queue.c.obj /Fd"C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\src\libopenr2.pdb" -c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\queue.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\queue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libopenr2.dir/queue.c.i"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  > CMakeFiles\libopenr2.dir\queue.c.i @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) -E "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\queue.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\queue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libopenr2.dir/queue.c.s"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB /FAs /FoNUL /FaCMakeFiles\libopenr2.dir\queue.c.s /c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\queue.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\queue.c.obj.requires:
.PHONY : src\CMakeFiles\libopenr2.dir\queue.c.obj.requires

src\CMakeFiles\libopenr2.dir\queue.c.obj.provides: src\CMakeFiles\libopenr2.dir\queue.c.obj.requires
	$(MAKE) -f src\CMakeFiles\libopenr2.dir\build.make /nologo -$(MAKEFLAGS) src\CMakeFiles\libopenr2.dir\queue.c.obj.provides.build
.PHONY : src\CMakeFiles\libopenr2.dir\queue.c.obj.provides

src\CMakeFiles\libopenr2.dir\queue.c.obj.provides.build: src\CMakeFiles\libopenr2.dir\queue.c.obj
.PHONY : src\CMakeFiles\libopenr2.dir\queue.c.obj.provides.build

src\CMakeFiles\libopenr2.dir\r2thread.c.obj: src\CMakeFiles\libopenr2.dir\flags.make
src\CMakeFiles\libopenr2.dir\r2thread.c.obj: ..\..\..\src\r2thread.c
	$(CMAKE_COMMAND) -E cmake_progress_report "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\CMakeFiles" $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/libopenr2.dir/r2thread.c.obj"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) /FoCMakeFiles\libopenr2.dir\r2thread.c.obj /Fd"C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\src\libopenr2.pdb" -c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2thread.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2thread.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libopenr2.dir/r2thread.c.i"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  > CMakeFiles\libopenr2.dir\r2thread.c.i @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB $(C_DEFINES) -E "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2thread.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2thread.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libopenr2.dir/r2thread.c.s"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	C:\PROGRA~1\MICROS~1.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) -DHAVE_CONFIG_H -DOR2_EXPORTS -D__OR2_COMPILING_LIBRARY__ -DWIN32_LEAN_AND_MEAN -DWITHOUT_MM_LIB /FAs /FoNUL /FaCMakeFiles\libopenr2.dir\r2thread.c.s /c "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src\r2thread.c"
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

src\CMakeFiles\libopenr2.dir\r2thread.c.obj.requires:
.PHONY : src\CMakeFiles\libopenr2.dir\r2thread.c.obj.requires

src\CMakeFiles\libopenr2.dir\r2thread.c.obj.provides: src\CMakeFiles\libopenr2.dir\r2thread.c.obj.requires
	$(MAKE) -f src\CMakeFiles\libopenr2.dir\build.make /nologo -$(MAKEFLAGS) src\CMakeFiles\libopenr2.dir\r2thread.c.obj.provides.build
.PHONY : src\CMakeFiles\libopenr2.dir\r2thread.c.obj.provides

src\CMakeFiles\libopenr2.dir\r2thread.c.obj.provides.build: src\CMakeFiles\libopenr2.dir\r2thread.c.obj
.PHONY : src\CMakeFiles\libopenr2.dir\r2thread.c.obj.provides.build

# Object files for target libopenr2
libopenr2_OBJECTS = \
"CMakeFiles\libopenr2.dir\r2chan.c.obj" \
"CMakeFiles\libopenr2.dir\r2context.c.obj" \
"CMakeFiles\libopenr2.dir\r2log.c.obj" \
"CMakeFiles\libopenr2.dir\r2proto.c.obj" \
"CMakeFiles\libopenr2.dir\r2utils.c.obj" \
"CMakeFiles\libopenr2.dir\r2engine.c.obj" \
"CMakeFiles\libopenr2.dir\r2ioabs.c.obj" \
"CMakeFiles\libopenr2.dir\queue.c.obj" \
"CMakeFiles\libopenr2.dir\r2thread.c.obj"

# External object files for target libopenr2
libopenr2_EXTERNAL_OBJECTS =

src\libopenr2.dll: src\CMakeFiles\libopenr2.dir\r2chan.c.obj
src\libopenr2.dll: src\CMakeFiles\libopenr2.dir\r2context.c.obj
src\libopenr2.dll: src\CMakeFiles\libopenr2.dir\r2log.c.obj
src\libopenr2.dll: src\CMakeFiles\libopenr2.dir\r2proto.c.obj
src\libopenr2.dll: src\CMakeFiles\libopenr2.dir\r2utils.c.obj
src\libopenr2.dll: src\CMakeFiles\libopenr2.dir\r2engine.c.obj
src\libopenr2.dll: src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj
src\libopenr2.dll: src\CMakeFiles\libopenr2.dir\queue.c.obj
src\libopenr2.dll: src\CMakeFiles\libopenr2.dir\r2thread.c.obj
src\libopenr2.dll: src\CMakeFiles\libopenr2.dir\build.make
src\libopenr2.dll: src\CMakeFiles\libopenr2.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared library libopenr2.dll"
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	"C:\Program Files\CMake 2.8\bin\cmake.exe" -E vs_link_dll C:\PROGRA~1\MICROS~1.0\VC\bin\link.exe /nologo @CMakeFiles\libopenr2.dir\objects1.rsp @<<
 /out:libopenr2.dll /implib:libopenr2.lib /pdb:"C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\src\libopenr2.pdb" /dll /version:0.0  /STACK:10000000 /machine: /INCREMENTAL:NO kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  
<<
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake

# Rule to build all files generated by this target.
src\CMakeFiles\libopenr2.dir\build: src\libopenr2.dll
.PHONY : src\CMakeFiles\libopenr2.dir\build

src\CMakeFiles\libopenr2.dir\requires: src\CMakeFiles\libopenr2.dir\r2chan.c.obj.requires
src\CMakeFiles\libopenr2.dir\requires: src\CMakeFiles\libopenr2.dir\r2context.c.obj.requires
src\CMakeFiles\libopenr2.dir\requires: src\CMakeFiles\libopenr2.dir\r2log.c.obj.requires
src\CMakeFiles\libopenr2.dir\requires: src\CMakeFiles\libopenr2.dir\r2proto.c.obj.requires
src\CMakeFiles\libopenr2.dir\requires: src\CMakeFiles\libopenr2.dir\r2utils.c.obj.requires
src\CMakeFiles\libopenr2.dir\requires: src\CMakeFiles\libopenr2.dir\r2engine.c.obj.requires
src\CMakeFiles\libopenr2.dir\requires: src\CMakeFiles\libopenr2.dir\r2ioabs.c.obj.requires
src\CMakeFiles\libopenr2.dir\requires: src\CMakeFiles\libopenr2.dir\queue.c.obj.requires
src\CMakeFiles\libopenr2.dir\requires: src\CMakeFiles\libopenr2.dir\r2thread.c.obj.requires
.PHONY : src\CMakeFiles\libopenr2.dir\requires

src\CMakeFiles\libopenr2.dir\clean:
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake\src
	$(CMAKE_COMMAND) -P CMakeFiles\libopenr2.dir\cmake_clean.cmake
	cd C:\DOCUME~1\ARNALD~1\Desktop\OPENR2~1.TRU\build\WINDOW~1\nmake
.PHONY : src\CMakeFiles\libopenr2.dir\clean

src\CMakeFiles\libopenr2.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk" "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\src" "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake" "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\src" "C:\Documents and Settings\Arnaldo Pereira\Desktop\openr2.trunk\build\windows-x86\nmake\src\CMakeFiles\libopenr2.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : src\CMakeFiles\libopenr2.dir\depend
