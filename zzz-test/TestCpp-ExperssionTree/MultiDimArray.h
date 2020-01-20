#pragma once

#include <cassert>
#include <initializer_list>

#include "log.h"

namespace SmartLib
{
	template<class T>
	class MDArray
	{
	private:
		int _dim{ 0 };	
		int * _ranks{ nullptr }; /***INTERNAL***/ /***CLEANUP***/
		int * _weights { nullptr }; /***INTERNAL***/ /***CLEANUP***/	
		T * _data{ nullptr };  /***INTERNAL***/ /***CLEANUP***/ 

	public:
		MDArray(std::initializer_list<int> ranks) :
			_dim((int)ranks.size())
		{
			_ranks = new int[_dim];

			for (int ii = 0; ii < _dim; ++ ii)
			{
				_ranks[ii] = (ranks.begin())[_dim - 1 - ii];
			}

			_weights = new int[_dim + 1];
			_weights[0] = 1;
			for (int ii = 1; ii <= _dim; ++ii)
			{
				_weights[ii] = _ranks[ii - 1] * _weights[ii - 1];
			}


			_data = new T[_weights[_dim]];
		}


		T& operator()(std::initializer_list<int> index)
		{
			assert(index.size() == _dim);
			int offset = 0;
			for (int ii = 0; ii < _dim; ++ ii)
			{
				offset += (index.begin())[_dim - 1 - ii] * _weights[ii];
			}

			return _data[offset];
		}

		int Rank(int ii)
		{
			return _ranks[ii];
		}

		int Dimension()
		{
			return _dim;
		}


		~MDArray()
		{
			delete[] _data;
			_data = nullptr;

			delete[] _ranks;
			_ranks = nullptr;

			delete[] _weights;
			_weights = nullptr;
		}
	};


	class MDArrayTest
	{
	public:
		static void Case0()
		{
			MDArray<int> mda({ 3, 4, 5, 6 });

			for (int ll = 0; ll < mda.Rank(3); ++ll)
			{
				for (int mm = 0; mm < mda.Rank(2); ++mm)
				{
					for (int nn = 0; nn < mda.Rank(1); ++nn)
					{
						for (int kk = 0; kk < mda.Rank(0); ++kk)
						{
							mda({ll, mm, nn, kk }) = 1000*ll + 100 * mm + 10 * nn + kk + 1111;
						}
					}
				}
			}
			

			for (int ll = 0; ll < mda.Rank(3); ++ll)
			{
				for (int mm = 0; mm < mda.Rank(2); ++mm)
				{
					for (int nn = 0; nn < mda.Rank(1); ++nn)
					{
						for (int kk = 0; kk < mda.Rank(0); ++kk)
						{
							assert(mda({ll, mm, nn, kk }) == (1000 * ll + 100 * mm + 10 * nn + kk + 1111));
							LOG_LINE(L"[%d, %d, %d, %d]=%03d", ll + 1,  mm + 1, nn + 1, kk + 1, mda({ll, mm, nn, kk }));
						}
					}
				}
			}

			
				
					
		}
	};
}