set CUR_PATH=%cd%\..
set ENGINE_PATH=C:\Program Files\Epic Games\UE_4.27\Engine\Binaries\Win64\
set PROJECT=Framework.uproject

cd %ENGINE_PATH%

UE4Editor.exe %CUR_PATH%\%PROJECT% ^
-nosplash ^
-Unattended ^
-nopause ^
-nosound ^
-NullRHI ^
-nocontentbrowser ^
-ExecCmds="Automation RunTests DrumTest.;quit" ^
-TestExit="Automation Test Queue Empty" ^
-log -log=AllTest.log ^
-ReportOutputPath=%CUR_PATH%\Report\ALL\