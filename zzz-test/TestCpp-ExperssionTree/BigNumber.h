#pragma once

#include <cassert>
#include <vector>
#include <string>

#include "helper.h"

using std::vector;
using std::string;

namespace SmartLib
{
	class BigNum
	{
	private:
		vector<char> _number;


	private:
		static const vector<char> & Zero()
		{
			static vector<char> number{ 0 };
			return number;
		}

		static const vector<char> & One()
		{
			static vector<char> number{ 1 };
			return number;
		}


		static vector<vector<char>> SavdSum(const vector<char> & num)
		{
			vector<vector<char>> saved(11);
			saved[0] = Zero();

			for (int ii = 1; ii < saved.size(); ++ ii)
			{
				saved[ii] = Add(saved[ii - 1], num);
			}

			return saved;
		}


		static void ShiftLeftAndAdd(vector<char> & num, char nn)
		{
			num.push_back(nn);
			Helper::Reverse(&num[0], &num[num.size() - 1]);
			Helper::Reverse(&num[1], &num[num.size() - 1]);
		}

		static int SeqSearch0To9(const vector<vector<char>> & saved, const vector<char> & num)
		{
			assert(11 == saved.size());
			assert(Compare(num, saved[10]) < 0);

			for (int ii = 9; ii >= 0; -- ii)
			{
				if (Compare(num, saved[ii]) >= 0)
				{
					return ii;
				}
			}

			assert(false);
			return -1;
		}

		static int BinSearch0To9(const vector<vector<char>> & saved, const vector<char> & num)
		{
			assert(11 == saved.size());

			int left = 0; 
			int right = saved.size() - 2; //9

			while (left <= right)
			{
				int mid = (left + right) /2 ;		

				int comp = Compare(saved[mid], num);
				if (comp == 0)
				{
					return mid;
				}
				else if (comp < 0)
				{
					if (Compare(saved[mid + 1], num) > 0)
					{
						return mid;
					}
				}


				if (comp > 0)
				{
					right = mid - 1;
				}
				else
				{
					left = mid + 1;
				}
			}

		}

	private:

		static std::pair<vector<char>, vector<char>> Div2(const vector<char> & num, const vector<char> & dd)
		{
			vector<vector<char>> saved = SavdSum(dd);

			vector<char> qq(num.size());
			vector<char> rr = Zero();
			for (int ii =  num.size() - 1; ii >= 0; -- ii)
			{
				ShiftLeftAndAdd(rr, num[ii]);
				if (Compare(rr, dd) < 0)
				{
					qq[ii] = 0;
				}
				else
				{
					//qq[ii] = BinSearch0To9(saved, rr);
					qq[ii] = SeqSearch0To9(saved, rr);
					rr = Sub(rr, saved[qq[ii]] );
				}
			}
			
			return { qq, rr };
		}

		static std::pair<vector<char>, vector<char>> Div(const vector<char> & num1, const vector<char> & num2)
		{
			vector<char> qq;
			vector<char> rr;

			if (Compare(num2, One()) == 0)
			{
				qq = num1;
				rr = Zero();
			}
			else if (Compare(num1, num2) < 0)
			{
				qq = Zero();
				rr = num1;
			}
			else
			{
				vector<char> left = Zero();
				vector<char> right = num1;

				while (Compare(left, right) <= 0)
				{
					vector<char> mid = Half(Add(left, right));

					vector<char> product = Mul(mid, num2);

					int cmp = Compare(product, num1);
					if (cmp == 0)
					{
						qq = mid;
						rr = Zero();
						break;
					}
					else if (cmp < 0)
					{
						vector<char> product2 = Mul(Add(mid, One()), num2);
						if (Compare(product2, num1) > 0)
						{
							qq = mid;
							rr = Sub(num1, product);
							break;
						}
					}


					if (cmp > 0)
					{
						right = Sub(mid, One());
					}
					else
					{
						left = Add(mid, One());
					}
				}
			}

			return { qq, rr };
		}

		static vector<char> Half(const vector<char> & num)
		{
			vector<char> rv(num.size());
			int remainder = 0;
			for (int ii = num.size() - 1; ii >= 0; --ii)
			{
				rv[ii] = (remainder * 10 + num[ii]) / 2;
				remainder = (num[ii] & 1);
			}

			return rv;
		}

		static vector<char> ShiftLeft(const vector<char> & num, int count)
		{
			vector<char> rv(num.size() + count);

			for (int ii = 0; ii < num.size(); ++ii)
			{
				rv[ii + count] = num[ii];
			}

			return rv;
		}

		static vector<char> TensComplement(const vector<char> & number, int count)
		{
			assert(count >= number.size());

			vector<char> rv = number;
			rv.resize(count);

			for (auto & val : rv)
			{
				val = 9 - val;
			}

			vector<char> result = Add(rv, One());
			result.resize(count);
			return result;
		}

		static vector<char> Sub(const vector<char> & num1, const vector<char> & num2)
		{
			vector<char> nn1 = num1;
			vector<char> nn2 = num2;

			if (nn1.size() < nn2.size())
			{
				nn1.resize(nn2.size());
			}


			if (nn2.size() < nn1.size())
			{
				nn2.resize(nn1.size());
			}

			vector<char> comp = TensComplement(nn2, nn2.size());
			vector<char> result = Add(num1, comp);
			if (result.size() > nn1.size())
			{
				assert(result.size() == nn1.size() + 1);
				result.pop_back();
			}
			return result;
		}

		static vector<char> Add(const vector<char> & num1, const vector<char> & num2)
		{
			vector<char> sum;

			const vector<char> & longer = num1.size() > num2.size() ? num1 : num2;
			const vector<char> & shorter = &longer == &num1 ? num2 : num1;

			int carry = 0;
			sum.resize(longer.size() + 1);
			for (int ii = 0; ii < shorter.size(); ++ii)
			{
				int ss = longer[ii] + shorter[ii] + carry;
				sum[ii] = ss % 10;
				carry = ss / 10;
			}

			for (int ii = shorter.size(); ii < longer.size(); ++ii)
			{
				int ss = longer[ii] + carry;
				sum[ii] = ss % 10;
				carry = ss / 10;
			}

			sum[longer.size()] = carry;

			return sum;
		}

		static int Compare(const vector<char> & num1, const vector<char> & num2) // pass by value, changed to pass by ref
		{

			int rv = 0;
			const vector<char> & longer = num1.size() >= num2.size() ? num1 : num2;
			const vector<char> & shorter = &longer == &num1 ? num2 : num1;

			assert(&longer == &num1 || &longer == &num2);
			assert(&shorter == &num1 || &shorter == &num2);

			int diff = longer.size() - shorter.size();
			for (int ii = longer.size() - 1; ii >= longer.size() - diff; -- ii)
			{
				rv = longer[ii];
				if (rv)
				{
					break;
				}
			}

			if (rv == 0)
			{
				for (int ii = shorter.size() - 1; ii >= 0; -- ii)
				{
					rv = longer[ii] - shorter[ii];
					if (rv)
					{
						break;
					}
				}
			}

			return rv == 0 ? 0 :
				&longer == &num1 ? rv : -rv;

			//if (num1.size() < num2.size())
			//{
			//	num1.resize(num2.size());
			//}

			//if (num2.size() < num1.size())
			//{
			//	num2.resize(num1.size());
			//}


			//int rv = 0;
			//for (int ii = num1.size() - 1; ii >= 0; --ii)
			//{
			//	rv = num1[ii] - num2[ii];
			//	if (rv != 0)
			//	{
			//		break;
			//	}
			//}

			//return rv;
		}


		static vector<char> Mul(const vector<char> & num1, const vector<char> & num2)
		{
			vector<vector<char>> saved =  SavdSum(num1);

			vector<char> rv = Zero();
			for (int ii = 0; ii < num2.size(); ++ii)
			{
				rv = Add(rv, ShiftLeft(saved[num2[ii]], ii));
			}


			return rv;
		}

	private:
		void Normalize()
		{
			while (_number.size() > 1)
			{
				if (0 == _number.back())
				{
					_number.pop_back();
				}
				else
				{
					break;
				}
			}
		}

		BigNum(const vector<char> & another) :
			_number(another)
		{
			Normalize();
		}


		BigNum(vector<char> && another) :
			_number(std::move(another))
		{
			Normalize();
		}

	public:
		BigNum(const char * str)
		{
			while (*str)
			{
				assert(*str >= '0' && *str <= '9');
				_number.push_back(*str - '0');
				++str;
			}

			Helper::Reverse(&_number[0], &_number[_number.size() - 1]);
			Normalize();
		}


		BigNum(const BigNum & another) :
			_number(another._number)
		{
			Normalize();
		}


		BigNum(BigNum && another) :
			_number(std::move(another._number))
		{
			Normalize();
		}


		string ToString()
		{
			//Normalize();
			string rv;
			for (int ii = _number.size() - 1; ii >= 0; --ii)
			{
				rv += char(_number[ii]) + '0';
			}

			return rv;
		}


	public:
		BigNum operator+(const BigNum & another)
		{
			BigNum bg(Add(_number, another._number));
			//bg.Normalize();
			return bg;
		}

		BigNum operator-(const BigNum & another)
		{
			BigNum bg(Sub(_number, another._number));
			//bg.Normalize();
			return bg;
		}

		BigNum operator*(const BigNum & another)
		{
			BigNum bg(Mul(_number, another._number));
			//bg.Normalize();
			return bg;
		}

		std::pair<BigNum, BigNum> operator/(const BigNum & another)
		{
			auto rv = Div2(_number, another._number);
			//auto rv = Div(_number, another._number);

			return { BigNum(std::move(rv.first)), BigNum(std::move(rv.second)) };
		}


		BigNum Half()
		{
			return BigNum(Half(_number));
		}
	};

	class BigNumTest
	{
	public:


		static void Case5()
		{
			long long ll1 = 128;
			long long ll2 = 32;
			LOG_LINE_A("%I64d / %I64d = %I64d ... %I64d", ll1, ll2, ll1 / ll2, ll1 % ll2);


			BigNum bn1("128");
			BigNum bn2("32");

			auto rv = bn1 / bn2;

			LOG_LINE_A("%s / %s = %s ... %s", bn1.ToString().c_str(), bn2.ToString().c_str(), rv.first.ToString().c_str(), rv.second.ToString().c_str());
		}

		static void Case4()
		{
			long long ll1 = 98749027481074265;
			long long ll2 = 5646731356;
			LOG_LINE_A("%I64d / %I64d = %I64d ... %I64d", ll1, ll2, ll1 / ll2, ll1 % ll2);


			BigNum bn1("98749027481074265");
			BigNum bn2("5646731356");

			auto rv = bn1 / bn2;

			LOG_LINE_A("%s / %s = %s ... %s", bn1.ToString().c_str(), bn2.ToString().c_str(), rv.first.ToString().c_str(), rv.second.ToString().c_str());
		}

		static void Case3()
		{
			long long ll = 919999999;
			LOG_LINE_A("%I64d / 2 = %I64d", ll, ll / 2);

			BigNum bn("919999999");
			BigNum half = bn.Half();
			LOG_LINE_A("%s / 2 = %s", bn.ToString().c_str(), half.ToString().c_str());
		}

		static void Case2()
		{
			long long l1 = 99876599;
			long long l2 = 12341100099;
			long long lrv = l1 * l2;
			LOG_LINE_A("%I64d * %I64d = %I64d", l1, l2, lrv);

			BigNum bn1("99876599");
			BigNum bn2("12341100099");
			BigNum brv = bn1 * bn2;
			LOG_LINE_A("%s * %s = %s", bn1.ToString().c_str(), bn2.ToString().c_str(), brv.ToString().c_str());
		}

		static void Case1()
		{
			//long l1 = 123411000;
			//long l2 =  99876599;

			long l2 = 123411000;
			long l1 = 99876599;

			long ls = l1 - l2;

			LOG_LINE_A("%ld - %ld = %ld", l1, l2, ls);

			char buffer[128] = { 0 };
			BigNum n1(Helper::IntToString(l1, buffer, _countof(buffer)));
			BigNum n2(Helper::IntToString(l2, buffer, _countof(buffer)));
			BigNum ss = n1 - n2;

			LOG_LINE_A("%s - %s = %s", n1.ToString().c_str(), n2.ToString().c_str(), ss.ToString().c_str());

		}

		static void Case0()
		{
			long l1 = 1234561;
			long l2 = 99987654999;
			long ls = l1 + l2;

			LOG_LINE_A("%ld + %ld = %ld", l1, l2, ls);

			char buffer[128] = { 0 };
			BigNum n1(Helper::IntToString(l1, buffer, _countof(buffer)));
			BigNum n2(Helper::IntToString(l2, buffer, _countof(buffer)));
			BigNum sum = n1 + n2;

			LOG_LINE_A("%s + %s = %s", n1.ToString().c_str(), n2.ToString().c_str(), sum.ToString().c_str());

		}
	};

}