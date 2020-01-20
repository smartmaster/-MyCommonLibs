set source=E:\MyProjects\~MyCommonLibs
set dest=E:\MyProjects\~MyCommonLibs-backup

md %dest%

robocopy "%source%" "%dest%" *.wxs *.wixproj *.rar *.zip *.ini *.png *.config *.csproj *.cs *.svcmap *.sln *.vcxproj *.vcxproj.filters *.h *.cpp *.c *.cc *.hpp *.hxx *.hh *.inl  *.idl *.asm  *.inc *.def *.rc *.resx *.cmd *.bat *.txt  *.pem *.xml *.xaml *.pl *.nsmap *.cab /s /e /is >> "%dest%\backup-logs.log"

rem xcopy "%source%\*.lib" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
rem xcopy "%source%\*.exe" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
rem xcopy "%source%\*.dll" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"


