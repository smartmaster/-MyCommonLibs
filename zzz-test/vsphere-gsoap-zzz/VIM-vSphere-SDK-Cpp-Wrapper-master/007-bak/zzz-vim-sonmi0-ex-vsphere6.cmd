set gsoap_bin=F:\MyProjects\~MyCommonLibs\zzz-test\vsphere-gsoap\VIM-vSphere-SDK-Cpp-Wrapper-master
set vmware_sdk=Q:\VMWare-install\vSphere6.0-liuwei\sdk\VMware-vSphere-SDK-6.0.0-1910213\SDK\vsphere-ws
set targetDir=F:\MyProjects\~MyCommonLibs\zzz-test\vsphere-gsoap\VIM-vSphere-SDK-Cpp-Wrapper-master\wrapper

set vimpl=vim-sonmi0-ex.pl
set path=%path%;%gsoap_bin%

echo "############################################"

md %targetDir%

%vimpl% -p %vmware_sdk%\wsdl\vim25\vimService.wsdl %targetDir%

%vimpl% -e %vmware_sdk%\docs\ReferenceGuide

%vimpl% -g %targetDir%


echo "############################################"
