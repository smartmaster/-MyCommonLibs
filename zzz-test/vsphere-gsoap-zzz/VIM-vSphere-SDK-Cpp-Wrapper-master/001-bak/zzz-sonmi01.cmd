set gsoap_bin=E:\MyProjects\gsoap-vmware\vim25
set vmware_sdk=Q:\VMWare-install\older-vmware-sdk\VMware-vSphere-SDK-5.5.0-1284541\SDK\vsphere-ws
set targetDir=E:\MyProjects\gsoap-vmware\VIM-vSphere-SDK-Cpp-Wrapper-master\wrapper

echo "############################################"

set path=%path%;%gsoap_bin%

md %targetDir%

vim-sonmi01.pl -p %vmware_sdk%\wsdl\vim25\vimService.wsdl %targetDir%

vim-sonmi01.pl -e %vmware_sdk%\docs\ReferenceGuide

vim-sonmi01.pl -g %targetDir%


echo "############################################"
