//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: HBKSTUB_CBT_SERVICE_NAME
//
// MessageText:
//
// Servi?o CBT do ^AU_ProductName_HBVB^
//
#define HBKSTUB_CBT_SERVICE_NAME         ((NTSTATUS)0x00000001L)

//
// MessageId: HBKSTUB_CBT_SERVICE_DESCRIPTION
//
// MessageText:
//
// Este servi?o ajuda o ^AU_ProductName_HBVB^ a coletar dados de rastreamento do bloco alterado.
//
#define HBKSTUB_CBT_SERVICE_DESCRIPTION  ((NTSTATUS)0x00000002L)

//
// MessageId: HBKSTUB_CBT_UNDEPLOY_ENTRY_NAME
//
// MessageText:
//
// Servi?o CBT do ^AU_ProductName_HBVB^
//
#define HBKSTUB_CBT_UNDEPLOY_ENTRY_NAME  ((NTSTATUS)0x00000003L)

//
// MessageId: HBKSTUB_CBT_UNDEPLOY_ENTRY_HELP_LINK
//
// MessageText:
//
// http://www.arcserve.com/support
//
#define HBKSTUB_CBT_UNDEPLOY_ENTRY_HELP_LINK ((NTSTATUS)0x00000004L)

//
// MessageId: HBKVM_IC_SERVICE_NAME
//
// MessageText:
//
// Servi?o de integra??o de VMs do ^AU_ProductName_HBVB^
//
#define HBKVM_IC_SERVICE_NAME            ((NTSTATUS)0x00000005L)

//
// MessageId: HBKVM_IC_SERVICE_DESCRIPTION
//
// MessageText:
//
// Este servi?o trabalha com o ^AU_ProductName_HBVB^ para coletar metadados do aplicativo.
//
#define HBKVM_IC_SERVICE_DESCRIPTION     ((NTSTATUS)0x00000006L)

//
// MessageId: HBKVM_IC_SERVICE_UNDEPLOY_ENTRY_NAME
//
// MessageText:
//
// Servi?o de integra??o de VMs do ^AU_ProductName_HBVB^
//
#define HBKVM_IC_SERVICE_UNDEPLOY_ENTRY_NAME ((NTSTATUS)0x00000007L)

//
// MessageId: HBKVM_IC_SERVICE_UNDEPLOY_ENTRY_HELP_LINK
//
// MessageText:
//
// http://www.arcserve.com/support
//
#define HBKVM_IC_SERVICE_UNDEPLOY_ENTRY_HELP_LINK ((NTSTATUS)0x00000008L)

//
// MessageId: HBKVM_IC_SERVICE_HELP_STARTING
//
// MessageText:
//
// Instalador do servi?o de integra??o de VMs do ^AU_ProductName_HBVB^
//
#define HBKVM_IC_SERVICE_HELP_STARTING   ((NTSTATUS)0x00000009L)

//
// MessageId: HBKVM_IC_SERVICE_HELP_GUIDING
//
// MessageText:
//
// --- Comandos suportados ---
//
#define HBKVM_IC_SERVICE_HELP_GUIDING    ((NTSTATUS)0x0000000AL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_INSTALL
//
// MessageText:
//
// Instalar o servi?o de integra??o de VMs do ^AU_ProductName_HBVB^ na m¨¢quina local
//
#define HBKVM_IC_SERVICE_HELP_INSTALL    ((NTSTATUS)0x0000000BL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_UNINSTALL
//
// MessageText:
//
// Desinstalar o servi?o de integra??o de VMs do ^AU_ProductName_HBVB^ da m¨¢quina local
//
#define HBKVM_IC_SERVICE_HELP_UNINSTALL  ((NTSTATUS)0x0000000CL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_RESULT_SUCCEEDED
//
// MessageText:
//
// Instala??o do servi?o de integra??o de VMs do ^AU_ProductName_HBVB^ bem-sucedida
//
#define HBKVM_IC_SERVICE_HELP_RESULT_SUCCEEDED ((NTSTATUS)0x0000000DL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_RESULT_FAILED
//
// MessageText:
//
// Falha ao instalar o servi?o de integra??o de VM do ^AU_ProductName_HBVB^
//
#define HBKVM_IC_SERVICE_HELP_RESULT_FAILED ((NTSTATUS)0x0000000EL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_RESULT_SUCCEEDED_INSTALL
//
// MessageText:
//
// Desinstala??o do servi?o de integra??o de VMs do ^AU_ProductName_HBVB^ bem-sucedida
//
#define HBKVM_IC_SERVICE_HELP_RESULT_SUCCEEDED_INSTALL ((NTSTATUS)0x0000000FL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_RESULT_FAILED_UNINSTALL
//
// MessageText:
//
// Falha ao desinstalar o servi?o de integra??o de VM do ^AU_ProductName_HBVB^
//
#define HBKVM_IC_SERVICE_HELP_RESULT_FAILED_UNINSTALL ((NTSTATUS)0x00000010L)

//