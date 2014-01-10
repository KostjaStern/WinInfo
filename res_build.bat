
set "vc_path=C:\Program Files\Microsoft Visual Studio 11.0\VC\"
set "win_kits_path=C:\Program Files\Windows Kits\8.0\bin\x86\"

call "%vc_path%vcvarsall.bat" x86

set "bin=Debug\"
set "src=WinInfo\"
set "progname=wininfo"

:: http://www.computerhope.com/batch.htm 
:: http://msdn.microsoft.com/en-us/library/36k2cdd4.aspx (#include Directive (C/C++))

:: INCLUDE = C:\Program Files\Microsoft Visual Studio 11.0\VC\INCLUDE;
::           C:\Program Files\Windows Kits\8.0\include\shared;
::           C:\Program Files\Windows Kits\8.0\include\um;
::           C:\Program Files\Windows Kits\8.0\include\winrt;

:: LIB = C:\Program Files\Microsoft Visual Studio 11.0\VC\LIB;
::       C:\Program Files\Windows Kits\8.0\lib\win8\um\x86;

:: LIBPATH = C:\Windows\Microsoft.NET\Framework\v4.0.30319;
::           C:\Windows\Microsoft.NET\Framework\v3.5;
::           C:\Program Files\Microsoft Visual Studio 11.0\VC\LIB;
::           C:\Program Files\Windows Kits\8.0\References\CommonConfiguration\Neutral;
::           C:\Program Files\Microsoft SDKs\Windows\v8.0\ExtensionSDKs\Microsoft.VCLibs\11.0\References\CommonConfiguration\neutral;







:: http://msdn.microsoft.com/ru-ru/library/610ecb4h.aspx (Compiler Command-Line Syntax)
:: /MDd            => Multi-threaded Debug DLL 
:: /GS             => Security Check
:: /RTC1           => Basic Runtime Checks = Both
:: /EHsc           => Enable C++ Exceptions = Yes
:: /Gm             => Enable Minimal Rebuild = Yes
:: /c              => compile only, no link
:: /ZI             => enable Edit and Continue debug info
:: /nologo         => suppress copyright message
:: /W<n>           => set warning level (default n=1)
:: /analyze[:WX-]  => enable code analysis
::                       WX- - code analysis warning should not be treated as error even if /WX is invoked
:: /Od             => disable optimizations (default)
:: /Oy-            => enable frame pointer omission
:: /fp:precise     =>
:: /Zc:wchar_t     =>
:: /Zc:forScope    =>
:: /Fo<file>       => name object file
:: /Fp<file>       => name precomiled header file
:: /Fd[file]       => name .PDB file
:: /errorReport:option => Report internal compiler errors to Microsoft
::             :none   => do not send report
::             :prompt => prompt to immediately send report
::             :queue  =>
::             :send   => 

:: /Yu"stdafx.h" /GS /analyze- /W3 /Zc:wchar_t /ZI /Gm /Od /sdl /Fd"Debug\vc110.pdb" /fp:precise 
:: /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope 
:: /RTC1 /Gd /Oy- /MDd /Fa"Debug\" /EHsc /nologo /Fo"Debug\" /Fp"Debug\TestWinAPI.pch" 

:: "%vc_path%bin\cl.exe" /MDd /GS /RTC1 /EHsc /Gm /Zi /D "_DEBUG" /D "_UNICODE" /D "UNICODE"  ^
::      /Fe%bin%singleton.exe /Fo%bin%singleton.obj %src%Resource.h %src%Singleton.cpp %src%Singleton.rc advapi32.lib user32.lib
	

:: "%vc_path%bin\cl.exe" /c /ZI /W3 /WX- /Od /Oy- /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Gm /EHsc /RTC1 /MDd ^
::                      /I"E:\myWINAPI\Windows-via-CPlusPlus-Code\CommonFiles" ^
::                      /GS /fp:precise /Zc:wchar_t /Zc:forScope /Fo"%bin%%progname%.obj" /Fd"%bin%%progname%.pdb" /Gd /TP /analyze- /errorReport:prompt %src%%progname%.cpp

"%win_kits_path%rc.exe" /D "_UNICODE" /D "UNICODE" /l"0x0409"  /fo"%bin%%progname%.res" %src%%progname%.rc 
:: /SUBSYSTEM:WINDOWS
:: /SAFESEH => Specify that the image will contain a table of safe exception handlers.

:: "%vc_path%bin\link.exe" /ERRORREPORT:PROMPT /OUT:"%bin%%progname%.exe" /INCREMENTAL /NOLOGO kernel32.lib user32.lib gdi32.lib winspool.lib ^
::                        comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /MANIFEST /MANIFESTUAC:"level='asInvoker' uiAccess='false'" ^
::						/manifest:embed /DEBUG /PDB:"%bin%%progname%.pdb" /TLBID:1 /DYNAMICBASE /NXCOMPAT ^
::						/IMPLIB:"%bin%%progname%.lib" /MACHINE:X86 %bin%%progname%.obj %bin%%progname%.res	
         
:: "%vc_path%bin\cl.exe" /Fe%bin%test.exe /Fo%bin%test.obj %src%test.cpp

pause