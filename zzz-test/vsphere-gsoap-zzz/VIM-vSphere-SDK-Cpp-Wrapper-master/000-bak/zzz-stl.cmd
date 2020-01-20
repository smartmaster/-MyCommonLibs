set path=%path%;E:\MyProjects\gsoap-vmware\vim25

set targetDir=E:\MyProjects\gsoap-vmware\VIM-vSphere-SDK-Cpp-Wrapper-master\wrapper

md %targetDir%

vim-stl.pl -p Q:\VMWare-install\older-vmware-sdk\VMware-vSphere-SDK-5.5.0-1284541\SDK\vsphere-ws\wsdl\vim25\vimService.wsdl %targetDir%

vim-stl.pl -e Q:\VMWare-install\older-vmware-sdk\VMware-vSphere-SDK-5.5.0-1284541\SDK\vsphere-ws\docs\ReferenceGuide

vim-stl.pl -g %targetDir%


