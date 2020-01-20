@echo off

IF [%1]==[] GOTO usage
IF [%2]==[] GOTO usage
IF [%3]==[] GOTO usage

set JRE_ROOT="D:\Java\jdk1.8.0_112"
set JAVA_CLASS=com.arcserve.udp.vmwaremanager.wsdl.TrimWsdl
set LIB=.\lib\*
set classpath=%LIB%;

echo --- TrimWsdl begin ---
%JRE_ROOT%\bin\java %JAVA_CLASS% %1 %2 %3 %4
echo --- Done ---
GOTO:end

:usage
echo =====================TrimWsdl========================= 
echo Usage:
echo TrimWsdl source_wsdl_file output_folder whitelist_json_file
echo.
echo Example:
echo TrimWsdl F:\zzz-temp-fff\TrimWsdl\vim25\vim.wsdl F:\zzz-temp-fff\TrimWsdl\wim_wsdl_mini F:\zzz-temp-fff\TrimWsdl\vim_whitelist.json
echo ============================================================ 

:end

