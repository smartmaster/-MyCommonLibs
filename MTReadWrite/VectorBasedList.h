#pragma once

#include <vector>
#include <functional>

namespace VectorBasedList_NS
{
	template<typename T>
	class VectorBasedList
	{
	private:
		int m_first;
		int m_last;

		std::vector<T> m_data;
		std::vector<int> m_next;
		std::vector<int> m_prev;
		std::vector<int> m_deleted;
	
		static const int _NULL = -1;
		static const int _DELETED = -2;

	public:
		VectorBasedList() :
			m_first(_NULL),
			m_last(_NULL)
		{

		}

		void GetLogicalPhysicalIndexMap(std::vector<int> & lpm)
		{
			int ii = m_first;
			while (ii >= 0)
			{
				lpm.push_back(ii);
				ii = m_next[ii];
			}
		}

		void Delete(int indexPhysical)
		{
			if (indexPhysical >= 0 && indexPhysical < m_data.size())
			{
				if (m_next[indexPhysical] == _DELETED || m_prev[indexPhysical] == _DELETED)
				{
					//already deleted
				}
				else
				{
					int prev = m_prev[indexPhysical];
					int next = m_next[indexPhysical];
					m_deleted.push_back(indexPhysical);
					
					Connect(prev, next);
					
					if (indexPhysical == m_first)
					{
						m_first = next;
					}
					if (indexPhysical == m_last)
					{
						m_last = prev;
					}

					m_data[indexPhysical] = T{};
					m_prev[indexPhysical] = _DELETED;
					m_next[indexPhysical] = _DELETED;
				}
			}
			
		}

		int InsertLast(const T & value)
		{
			return InsertAfter(m_last, value);
		}

		int InsertFirst(const T & value)
		{
			return InsertBefore(m_first, value);
		}


		int InsertAfter(int indexPhysical, const T & val)
		{
			int newIndex  = -1;
			if (m_deleted.size())
			{
				newIndex = m_deleted.back();
				m_deleted.pop_back();
				m_data[newIndex] = val;
				m_next[newIndex] = _NULL;
				m_prev[newIndex] = _NULL;
			}
			else
			{
				m_data.push_back(val);
				m_next.push_back(_NULL);
				m_prev.push_back(_NULL);
				newIndex = m_data.size() - 1;
			}
			


			if (1 == m_data.size() - m_deleted.size())
			{
				indexPhysical = _NULL;
			}

			if (m_data.size() - m_deleted.size() > 1)
			{
				Connect(newIndex, m_next[indexPhysical]);
				Connect(indexPhysical, newIndex);
			}

			if (1 == m_data.size() - m_deleted.size())
			{
				m_first = newIndex;
				m_last = newIndex;
			}
			else if (indexPhysical == m_last)
			{
				m_last = newIndex;
			}

			return newIndex;
		}

		int InsertBefore(int indexPhysical, const T & val)
		{
			int newIndex = -1;
			if (m_deleted.size())
			{
				newIndex = m_deleted.back();
				m_deleted.pop_back();
				m_data[newIndex] = val;
				m_next[newIndex] = _NULL;
				m_prev[newIndex] = _NULL;
			}
			else
			{
				m_data.push_back(val);
				m_next.push_back(_NULL);
				m_prev.push_back(_NULL);
				newIndex = m_data.size() - 1;
			}

			if (1 == m_data.size() - m_deleted.size())
			{
				indexPhysical = _NULL;
			}

			if (m_data.size() - m_deleted.size() > 1)
			{
				Connect(m_prev[indexPhysical], newIndex);
				Connect(newIndex, indexPhysical);
			}

			if (1 == m_data.size() - m_deleted.size())
			{
				m_first = newIndex;
				m_last = newIndex;
			}
			else if (indexPhysical == m_first)
			{
				m_first = newIndex;
			}

			return newIndex;
		}

		void Iterate(std::function<void(const T&)> func)
		{
			int ii = m_first;
			while (ii >= 0)
			{
				func(m_data[ii]);
				ii = m_next[ii];
			}
		}

		void IterateReverse(std::function<void(const T&)> func)
		{
			int ii = m_last;
			while (ii >= 0)
			{
				func(m_data[ii]);
				ii = m_prev[ii];
			}
		}


	private:
		void Connect(int ii, int jj)
		{
			if (ii >= 0)
			{
				m_next[ii] = jj;
			}

			if (jj >= 0)
			{
				m_prev[jj] = ii;
			}
		}
	};

	struct UT
	{
		static void test()
		{
			VectorBasedList<int> lst;
			int newindex = -1;
			for (int ii = 1; ii <= 5; ++ii)
			{
				//newindex = lst.InsertFirst(ii);
				newindex = lst.InsertBefore(0, ii);
			}


			for (int ii = 1; ii <= 5; ++ii)
			{
				//newindex = lst.InsertLast(ii * 10);
				newindex = lst.InsertAfter(0, ii * 10);
			}

			std::function<void(const int &)> print_data = [](const int & value) 
			{
				printf("%d, ", value);
			};

			//////////////////////////////////////////////////////////////////////////
			lst.Iterate(print_data);
			printf("\r\n");

			lst.IterateReverse(print_data);
			printf("\r\n");
			printf("\r\n");

			//////////////////////////////////////////////////////////////////////////
			lst.Delete(0);
			lst.Delete(5);

			lst.Iterate(print_data);
			printf("\r\n");

			lst.IterateReverse(print_data);
			printf("\r\n");
			printf("\r\n");

			//////////////////////////////////////////////////////////////////////////
			newindex = lst.InsertBefore(9, 1);
			newindex = lst.InsertAfter(6, 10);

			lst.Iterate(print_data);
			printf("\r\n");

			lst.IterateReverse(print_data);
			printf("\r\n");
			printf("\r\n");

			//////////////////////////////////////////////////////////////////////////
			for (int ii = 0; ii < 100; ++ ii)
			{
				lst.Delete(ii);
			}

			for (int ii = 1; ii <= 5; ++ii)
			{
				newindex = lst.InsertFirst(ii);
				//newindex = lst.InsertBefore(0, ii);
			}


			for (int ii = 1; ii <= 5; ++ii)
			{
				newindex = lst.InsertLast(ii * 10);
				//newindex = lst.InsertAfter(0, ii * 10);
			}

			lst.Iterate(print_data);
			printf("\r\n");

			lst.IterateReverse(print_data);
			printf("\r\n");
			printf("\r\n");

			//////////////////////////////////////////////////////////////////////////
		}
	};

} //end namespace VectorBasedList_NS

