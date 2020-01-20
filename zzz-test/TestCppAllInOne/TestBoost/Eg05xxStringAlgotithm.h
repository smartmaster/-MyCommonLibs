#pragma once



#include <string>
#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

namespace
{
	struct animal
	{
		std::string name;
		int legs;
	};

	static std::ostream& operator<<(std::ostream& os, const animal& a)
	{
		return os << "{" << a.name << ", " << a.legs << "}";
	}
}

namespace SmartLib
{
	using namespace ::boost::algorithm;
	using namespace ::std;

	class Eg05xxStringAlgotithm
	{
	public:
		


		static void Case0()
		{
			//////////////////////////////////////////////////////////////////////////
			{
				animal a{ "cat", 4 };
				std::cout << boost::format{ "%1%" } % a << '\n';
			}

			//////////////////////////////////////////////////////////////////////////
			{
				std::cout << boost::format{ "%+s %s %s" } % 1 % 2 % 1 << '\n';

				std::cout << boost::format{ "%+d %d %d" } % 1 % 2 % 1 << '\n';

				std::cout << boost::format{ "%|+| %|| %||" } % 1 % 2 % 1 << '\n';

				try
				{
					std::cout << boost::format{ "%|2$+| %2% %1%" } % 1 % 2 << '\n';
				}
				catch (boost::io::format_error & ex)
				{
					std::cout << ex.what() << '\n';
				}

				cout << boost::format{ "%2%/%1%/%3%" } % 6 % 12 % 2019 << endl;

				std::cout << boost::format{ "%1% %2% %1%" } %
					boost::io::group(std::showpos, 55) % 999 << '\n';

				std::cout << boost::format{ "%|1$+| %2% %1%" } % 100 % 200 << '\n';
			}
			////////////////////////////////////////////////////////////////////////
			{
				try
				{
					int i = boost::lexical_cast<int>("abc");
					std::cout << i << '\n';
				}
				catch (const boost::bad_lexical_cast & e)
				{
					std::cerr << e.what() << '\n';
				}
			}
			//////////////////////////////////////////////////////////////////////////
			{
				std::string s = boost::lexical_cast<std::string>(123);
				std::cout << s << endl;
				double d = boost::lexical_cast<double>(s);
				std::cout << d << endl;
			}
			//////////////////////////////////////////////////////////////////////////
			{
				std::string s = "Boost C++ Libraries";
				boost::iterator_range<std::string::iterator> r = find_regex(s, boost::regex{ "\\w\\+\\+" });
				std::cout << r << endl;
			}
			//////////////////////////////////////////////////////////////////////////
			{
				std::string s = "Boost C++ Libraries";
				std::vector<std::string> v;
				split(v, s, is_space());
				std::cout << v.size() << endl;
			}
			//////////////////////////////////////////////////////////////////////////
			{
				std::string s = "Boost C++ Libraries";
				std::cout.setf(std::ios::boolalpha);
				std::cout << starts_with(s, "Boost") << endl;
				std::cout << ends_with(s, "Libraries") << endl;
				std::cout << contains(s, "C++") << endl;
				std::cout << lexicographical_compare(s, "Boost") << endl;
				std::cout << lexicographical_compare("Boost", s) << endl;
			}
			//////////////////////////////////////////////////////////////////////////
			{
				std::string s = "123456789Boost C++ Libraries123456789";
				std::cout << trim_left_copy_if(s, is_digit()) << endl;
				std::cout << trim_right_copy_if(s, is_digit()) << endl;
				std::cout << trim_copy_if(s, is_digit()) << endl;
			}
			//////////////////////////////////////////////////////////////////////////
			{
				std::string s = "--Boost C++ Libraries--";
				std::cout << trim_left_copy_if(s, is_any_of("-Bo")) << endl;
				std::cout << trim_right_copy_if(s, is_any_of("es-")) << endl;
				std::cout << trim_copy_if(s, is_any_of("es-Bo")) << endl;
			}
			//////////////////////////////////////////////////////////////////////////
			{
				std::string s = "\t Boost C++ Libraries \t";
				std::cout << "_" << trim_left_copy(s) << "_\n";
				std::cout << "_" << trim_right_copy(s) << "_\n";
				std::cout << "_" << trim_copy(s) << "_\n";
			}
			//////////////////////////////////////////////////////////////////////////
			{
				std::string s = "Boost C++ Libraries";
				std::cout << replace_first_copy(s, "+", "-") << endl;
				std::cout << replace_nth_copy(s, "+", 1, "-") << endl;
				std::cout << replace_last_copy(s, "+", "-") << endl;
				std::cout << replace_all_copy(s, "+", "-") << endl;
				std::cout << replace_head_copy(s, 5, "BOOST") << endl;
				std::cout << replace_tail_copy(s, 9, "LIBRARIES") << endl;
			}
			//////////////////////////////////////////////////////////////////////////
			{
				std::vector<std::string> v{ "Boost", "C++", "Libraries" };
				std::cout << join(v, " ") << endl;
			}
			//////////////////////////////////////////////////////////////////////////
			{
				std::string s = "Boost C++ Libraries";
				boost::iterator_range<std::string::iterator> r = find_first(s, "C++");
				std::cout << r << endl;
				r = find_first(s, "xyz");
				std::cout << r << endl;
			}
			//////////////////////////////////////////////////////////////////////////
			{
				std::string s = "Boost C++ Libraries";
				std::cout << erase_first_copy(s, "s") << endl;
				std::cout << erase_nth_copy(s, "s", 0) << endl;
				std::cout << erase_last_copy(s, "s") << endl;
				std::cout << erase_all_copy(s, "s") << endl;
				std::cout << erase_head_copy(s, 5) << endl;
				std::cout << erase_tail_copy(s, 9) << endl;
			}
			//////////////////////////////////////////////////////////////////////////
			{
				std::string s = "Boost C++ k\xfct\xfcphaneleri";
				std::string upper_case1 = to_upper_copy(s);
				std::string upper_case2 = to_upper_copy(s, std::locale{ "Turkish" });
				std::locale::global(std::locale{ "Turkish" });
				std::cout << upper_case1 << endl;
				std::cout << upper_case2 << endl;
			}

			//////////////////////////////////////////////////////////////////////////
			{
				std::string s = "Boost C++ Libraries";
				std::cout << to_upper_copy(s) << endl;
				std::cout << s << endl;
			}
			
		}
	};
}

