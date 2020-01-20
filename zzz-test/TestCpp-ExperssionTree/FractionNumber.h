#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <cassert>

#include <sstream>
#include <string>
using std::string;

#include "log.h"
#include "Helper.h"

namespace SmartLib
{
	class FractionNumber // = _denominator / _numerator
	{
		
	private:
		LONGLONG _denominator{ 0 };
		LONGLONG _numerator{ 1 };


	private:
		void Normalize()
		{
			LONGLONG gcd = GCD(ABS(_denominator), _numerator);
			if (gcd > 1)
			{
				_denominator /= gcd;
				_numerator /= gcd;
			}
		}

	public:

		FractionNumber(LONGLONG dd) :
			_denominator(dd),
			_numerator(1)
		{

		}

		FractionNumber(LONGLONG dd, LONGLONG nn) :
			_denominator(dd),
			_numerator(nn)
		{
			assert(nn != 0);

			if (_denominator == 0)
			{
				nn = 1;
			}

			if (_numerator < 0)
			{
				_numerator = -_numerator;
				_denominator = -_denominator;
			}

			Normalize();
		}

		int CompareToZero()
		{
			//if (_denominator == 0)
			//{
			//	return 0;
			//}

			//return _denominator > 0 ? 1 : (-1);
			return _denominator;
		}

		string ToString()
		{
			std::stringstream ss;
			if (1 == _numerator)
			{
				//ss << '(' << _denominator << ')';
				ss << _denominator;
			}
			else
			{
				//ss << '(' << _denominator << '/' << _numerator << ')';
				ss << _denominator << '/' << _numerator;
			}
			
			return ss.str();
		}

		string ToDoubleString()
		{
			return Helper::IntDivString(_denominator, _numerator);
		}

		double ToDouble()
		{
			return ((double)(_denominator)) / ((double)(_numerator));
		}


		FractionNumber operator+(const FractionNumber & rr)
		{
			LONGLONG dd = _denominator * rr._numerator + rr._denominator * _numerator;
			LONGLONG nn = _numerator * rr._numerator;
			return FractionNumber(dd, nn);
		}


		


		FractionNumber operator-(const FractionNumber & rr)
		{
			LONGLONG dd = _denominator * rr._numerator - rr._denominator * _numerator;
			LONGLONG nn = _numerator * rr._numerator;
			return FractionNumber(dd, nn);
		}


		FractionNumber operator*(const FractionNumber & rr)
		{
			LONGLONG dd = _denominator * rr._denominator;
			LONGLONG nn = _numerator * rr._numerator;
			return FractionNumber(dd, nn);
		}


		FractionNumber operator/(const FractionNumber & rr)
		{
			LONGLONG dd = _denominator * rr._numerator;
			LONGLONG nn = _numerator * rr._denominator;
			return FractionNumber(dd, nn);
		}


	private:
		template<typename T>
		static T GCD(T a, T b) 
		{
			return b == 0 ? a : GCD(b, a % b);
		}

		template<typename T>
		static T ABS(T val)
		{
			return val >= 0 ? val : (-val);
		}
	};


	class FractionNumberTest
	{
	public:
		static void Case0()
		{
			//FractionNumber fn1(150000, -1000);
			//FractionNumber fn2(-1000, -100);


			//FractionNumber fn1(150000, -997);
			//FractionNumber fn2(-1000, -73);

			//double dn1{ 150000.0 / (-997.0) };
			//double dn2((-1000.0) / (-73.0));


			FractionNumber fn1(-997, 150000);
			FractionNumber fn2(-73, -1000);

			double dn1{  (-997.0) / 150000.0 };
			double dn2((-73.0) / (-1000.0) );

			FractionNumber sum = fn1 + fn2;
			LOG_LINE_A("%s = %s + %s", sum.ToString().c_str(), fn1.ToString().c_str(), fn2.ToString().c_str());
			LOG_LINE_A("%s = %s + %s", sum.ToDoubleString().c_str(), fn1.ToDoubleString().c_str(), fn2.ToDoubleString().c_str());
			LOG_LINE_A("%.4f = %.4f + %.4f", sum.ToDouble(), fn1.ToDouble(), fn2.ToDouble());
			LOG_LINE_A("%.4f = %.4f + %.4f", dn1 + dn2, dn1, dn2);
			LOG_LINE_A("");

			FractionNumber diff = fn1 - fn2;
			LOG_LINE_A("%s = %s - %s", diff.ToString().c_str(), fn1.ToString().c_str(), fn2.ToString().c_str());
			LOG_LINE_A("%s = %s - %s", diff.ToDoubleString().c_str(), fn1.ToDoubleString().c_str(), fn2.ToDoubleString().c_str());
			LOG_LINE_A("%.4f = %.4f - %.4f", diff.ToDouble(), fn1.ToDouble(), fn2.ToDouble());
			LOG_LINE_A("%.4f = %.4f - %.4f", dn1 - dn2, dn1, dn2);
			LOG_LINE_A("");

			FractionNumber product = fn1 * fn2;
			LOG_LINE_A("%s = %s * %s", product.ToString().c_str(), fn1.ToString().c_str(), fn2.ToString().c_str());
			LOG_LINE_A("%s = %s * %s", product.ToDoubleString().c_str(), fn1.ToDoubleString().c_str(), fn2.ToDoubleString().c_str());
			LOG_LINE_A("%.4f = %.4f * %.4f", product.ToDouble(), fn1.ToDouble(), fn2.ToDouble());
			LOG_LINE_A("%.4f = %.4f * %.4f", dn1 * dn2, dn1, dn2);
			LOG_LINE_A("");

			FractionNumber quotient = fn1 / fn2;
			LOG_LINE_A("%s = %s / %s", quotient.ToString().c_str(), fn1.ToString().c_str(), fn2.ToString().c_str());
			LOG_LINE_A("%s = %s / %s", quotient.ToDoubleString().c_str(), fn1.ToDoubleString().c_str(), fn2.ToDoubleString().c_str());
			LOG_LINE_A("%.4f = %.4f / %.4f", quotient.ToDouble(), fn1.ToDouble(), fn2.ToDouble());
			LOG_LINE_A("%.4f = %.4f / %.4f", dn1 / dn2, dn1, dn2);
			LOG_LINE_A("");


		}

		static void Case1()
		{
			int count = 6;
			FractionNumber fn(1, count);
			FractionNumber fsum(0);
			for (int ii = 0; ii < count; ++ ii)
			{
				fsum = fsum + fn;
			}

			LOG_LINE_A("%s = %s + ...", fsum.ToString().c_str(), fn.ToString().c_str());
			LOG_LINE_A("%s = %s + ...", fsum.ToDoubleString().c_str(), fn.ToDoubleString().c_str());
			LOG_LINE_A("%.32lf = %.32lf + ...", fsum.ToDouble(), fn.ToDouble());
			LOG_LINE_A("");


			double dn{ 1.0 / count };
			double dsum = 0.0;
			for (int ii = 0; ii < count; ++ii)
			{
				dsum = dsum + dn;
			}
			LOG_LINE_A("%.32lf = %.32lf + ...", dsum, dn);
			LOG_LINE_A("");
		}
	};
}