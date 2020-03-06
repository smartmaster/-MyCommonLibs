#include "stdafx.h"

#include "..\PublicHeader\PublicSerializeFieldDefine.h"

#include "..\PublicHeader\PublicParameterStreamDef.h"
#include "..\PublicHeader\PublicNameValueStream.h"
#include "..\PublicHeader\PublicParameterStreamHelper.h"

#include "..\PublicHeader\PublicSimpleXml.h"
#include "..\PublicHeader\PublicValueXmlElementConvert.h"

#include "CommandList.h"



#include "CommandList.h.xml.Output.hpp"
#include "CommandList.h.Output.hpp"


CONST LPCTSTR BODY_RESULT_RSP::RootName = MethodString(BODY_RESULT_RSP);

CONST LPCTSTR BODY_FILE_DESCRITION::RootName = MethodString(BODY_FILE_DESCRITION);

CONST LPCTSTR BODY_CREATE_DOWNLOAD_SESSION_REQ::RootName = MethodString(BODY_CREATE_DOWNLOAD_SESSION_REQ);

CONST LPCTSTR BODY_BEGIN_FILE_RSP::RootName = MethodString(BODY_BEGIN_FILE_RSP);

CONST LPCTSTR BODY_BEGIN_ENUM_FILE_REQ::RootName = MethodString(BODY_BEGIN_ENUM_FILE_REQ);

CONST LPCTSTR BODY_ENUM_FILE_DATA_RSP::RootName = MethodString(BODY_ENUM_FILE_DATA_RSP);

CONST LPCTSTR BODY_HANDLER_STATUS_STATISTIC::RootName = MethodString(BODY_HANDLER_STATUS_STATISTIC);

CONST LPCTSTR BODY_HANDLER_STATUS_STATISTIC_TOTAL::RootName = MethodString(BODY_HANDLER_STATUS_STATISTIC_TOTAL);

CONST LPCTSTR BODY_RUN_APP_REQ::RootName = MethodString(BODY_RUN_APP_REQ);

CONST LPCTSTR BODY_RUN_APP_RSP::RootName = MethodString(BODY_RUN_APP_RSP);
