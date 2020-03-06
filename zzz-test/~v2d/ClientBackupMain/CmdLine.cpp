#include "stdafx.h"
#include "CmdLine.h"

static CONST TCHAR SZ_USAGE[] = TEXT("this.exe -dest <dest path> [-machine <machine name> -user <user> -domain <domain> -password <password>] -volume <volume1>  [-volume <volume2> ...]");

INT CCommandLine::Parse( INT argc, TCHAR ** argv )
{
	INT nStatus = 0;

	for (INT ii=0; ii<argc; ++ii)
	{
		if (0 == _tcsicmp(argv[ii], TEXT("-dest")))
		{
			++ii;
			if (ii >= argc)
			{
				nStatus = -1;
				break;
			}
			m_strDestination = argv[ii];
		}
		else if (0 == _tcsicmp(argv[ii], TEXT("-machine")))
		{
			++ii;
			if (ii >= argc)
			{
				nStatus = -1;
				break;
			}
			m_strMachine = argv[ii];
		}
		else if (0 == _tcsicmp(argv[ii], TEXT("-user")))
		{
			++ii;
			if (ii >= argc)
			{
				nStatus = -1;
				break;
			}
			m_strUser = argv[ii];
		}
		else if (0 == _tcsicmp(argv[ii], TEXT("-domain")))
		{
			++ii;
			if (ii >= argc)
			{
				nStatus = -1;
				break;
			}
			m_strDomain= argv[ii];
		}
		else if (0 == _tcsicmp(argv[ii], TEXT("-password")))
		{
			++ii;
			if (ii >= argc)
			{
				nStatus = -1;
				break;
			}
			m_strPassword = argv[ii];
		}
		else if (0 == _tcsicmp(argv[ii], TEXT("-volume")))
		{
			++ii;
			if (ii >= argc)
			{
				nStatus = -1;
				break;
			}
			m_strVolumes.push_back(argv[ii]);
		}
	}

	if (nStatus)
	{
		_OLD_DINFO((0, DRNPOS, SZ_USAGE));
	}

	return nStatus;
}