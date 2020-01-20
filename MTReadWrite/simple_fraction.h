#pragma once


namespace simple_fraction_ns
{
	template <typename T> //T is integers 
	class CFraction
	{
	private:
		T m_up;
		T m_down;

	public:
		CFraction(T up, T down)
		{
			Assign(up, down);
		}

		CFraction(const CFraction & rhs)
		{
			Assign(rhs.m_up, rhs.m_down);
		}

		const CFraction & operator=(const CFraction & rhs) const
		{
			if (this != &rhs)
			{
				Assign(rhs.m_up, rhs.m_down);
			}
			return *this;
		}

		const CFraction operator+(const CFraction & rhs) const
		{
			return CFraction(m_up * rhs.m_down + rhs.m_up * m_down, m_down * rhs.m_down);
		}

		const CFraction operator-(const CFraction & rhs) const
		{
			return CFraction(m_up * rhs.m_down - rhs.m_up * m_down, m_down * rhs.m_down);
		}

		const CFraction operator*(const CFraction & rhs) const
		{
			return CFraction(m_up * rhs.m_up, m_down * rhs.m_down);
		}

		const CFraction operator/(const CFraction & rhs) const
		{
			return CFraction(m_up * rhs.m_down, m_down * rhs.m_up);
		}

		T GetUp() const { return m_up; }
		T GetDown() const { return m_down; }

	private:
		void Assign(T up, T down)
		{
			m_up = up;
			m_down = down;

			if (0 == m_up)
			{
				m_down = 1;
			}

			if (m_down < 0)
			{
				m_down = -m_down;
				m_up = -m_up;
			}

			CoPrime();
		} //end void Assign(T up, T down)

		void CoPrime()
		{
			T up = m_up < 0 ? -m_up : m_up;
			T down = m_down < 0 ? -m_down : m_down;

			T gcdTemp = gcd(up, down);

			m_up /= gcdTemp;
			m_down /= gcdTemp;
		} //end void MaxCommonDivisor()


		template<typename T>
		static T gcd(T a, T b)
		{
			T c;
			while (a != 0)
			{
				c = a; a = b%a;  b = c;
			}
			return b;
		}

		/* Recursive Standard C Function: Greatest Common Divisor */
		template<typename T>
		static T gcdr(T a, T b)
		{
			if (a == 0) return b;
			return gcdr(b%a, a);
		}

	}; //end class CFraction

} //end namespace simple_fraction_ns