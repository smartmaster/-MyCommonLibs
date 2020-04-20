#pragma once


namespace SmartLib
{
	class Logger
	{
	public:
		template<typename TOUT, typename TARG, typename... TARGS>
		static TOUT& Print(TOUT& out, TARG arg, TARGS... args)
		{
			out << static_cast<TARG&&>(arg);
			return Print(out, static_cast<TARGS&&>(args)...);
		}


		template<typename TOUT>
		static TOUT& Print(TOUT& out)
		{
			return out;
		}
	};
}