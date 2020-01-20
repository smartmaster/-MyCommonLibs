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
// Service CBT ^AU_ProductName_HBVB^
//
#define HBKSTUB_CBT_SERVICE_NAME         ((NTSTATUS)0x00000001L)

//
// MessageId: HBKSTUB_CBT_SERVICE_DESCRIPTION
//
// MessageText:
//
// Ce service aide ^AU_ProductName_HBVB^ ¨¤ collecter des donn¨¦es de suivi des blocs modifi¨¦s.
//
#define HBKSTUB_CBT_SERVICE_DESCRIPTION  ((NTSTATUS)0x00000002L)

//
// MessageId: HBKSTUB_CBT_UNDEPLOY_ENTRY_NAME
//
// MessageText:
//
// Service CBT ^AU_ProductName_HBVB^
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
// Service d'int¨¦gration de machines virtuelles ^AU_ProductName_HBVB^
//
#define HBKVM_IC_SERVICE_NAME            ((NTSTATUS)0x00000005L)

//
// MessageId: HBKVM_IC_SERVICE_DESCRIPTION
//
// MessageText:
//
// Ce service fonctionne conjointement avec ^AU_ProductName_HBVB^ pour collecter les m¨¦tadonn¨¦es des applications.
//
#define HBKVM_IC_SERVICE_DESCRIPTION     ((NTSTATUS)0x00000006L)

//
// MessageId: HBKVM_IC_SERVICE_UNDEPLOY_ENTRY_NAME
//
// MessageText:
//
// Service d'int¨¦gration de machines virtuelles ^AU_ProductName_HBVB^
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
// Programme d'installation du service d'int¨¦gration de machines virtuelles ^AU_ProductName_HBVB^
//
#define HBKVM_IC_SERVICE_HELP_STARTING   ((NTSTATUS)0x00000009L)

//
// MessageId: HBKVM_IC_SERVICE_HELP_GUIDING
//
// MessageText:
//
// --- Commandes prises en charge ---
//
#define HBKVM_IC_SERVICE_HELP_GUIDING    ((NTSTATUS)0x0000000AL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_INSTALL
//
// MessageText:
//
// Installer le service d'int¨¦gration de machines virtuelles ^AU_ProductName_HBVB^ sur la machine locale
//
#define HBKVM_IC_SERVICE_HELP_INSTALL    ((NTSTATUS)0x0000000BL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_UNINSTALL
//
// MessageText:
//
// D¨¦sinstaller le service d'int¨¦gration de machines virtuelles ^AU_ProductName_HBVB^ de la machine locale
//
#define HBKVM_IC_SERVICE_HELP_UNINSTALL  ((NTSTATUS)0x0000000CL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_RESULT_SUCCEEDED
//
// MessageText:
//
// Le service d'int¨¦gration de machines virtuelles ^AU_ProductName_HBVB^ a ¨¦t¨¦ install¨¦.
//
#define HBKVM_IC_SERVICE_HELP_RESULT_SUCCEEDED ((NTSTATUS)0x0000000DL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_RESULT_FAILED
//
// MessageText:
//
// L'installation du service d'int¨¦gration de machines virtuelles ^AU_ProductName_HBVB^ a ¨¦chou¨¦.
//
#define HBKVM_IC_SERVICE_HELP_RESULT_FAILED ((NTSTATUS)0x0000000EL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_RESULT_SUCCEEDED_INSTALL
//
// MessageText:
//
// Le service d'int¨¦gration de machines virtuelles ^AU_ProductName_HBVB^ a ¨¦t¨¦ d¨¦sinstall¨¦.
//
#define HBKVM_IC_SERVICE_HELP_RESULT_SUCCEEDED_INSTALL ((NTSTATUS)0x0000000FL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_RESULT_FAILED_UNINSTALL
//
// MessageText:
//
// La d¨¦sinstallation du service d'int¨¦gration de machines virtuelles ^AU_ProductName_HBVB^ a ¨¦chou¨¦.
//
#define HBKVM_IC_SERVICE_HELP_RESULT_FAILED_UNINSTALL ((NTSTATUS)0x00000010L)

//