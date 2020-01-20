#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <string>
#include <iostream>
#include <functional>
#include <algorithm>

#include <boost/regex.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>
#include <boost/tokenizer.hpp>
#include <boost/spirit/include/qi.hpp>


namespace SmartLib
{
	using namespace  ::std;
	using namespace ::boost;
	using namespace boost::spirit;

	class Eg08xx_regex
	{
	public:
		template<typename ITER, typename SKIPPER>
		struct MyGrammar : public
			qi::grammar<
			ITER,
			std::vector<boost::variant<int, bool>>(),
			SKIPPER>
		{
			


			MyGrammar() :
				_value{ qi::int_ | qi::bool_ },
				_values{ _value % ',' },
				MyGrammar::base_type{ _values }
			{
			}
			qi::rule<ITER, boost::variant<int, bool>(), SKIPPER> _value;
			qi::rule<ITER, std::vector<boost::variant<int, bool>>(), SKIPPER> _values;
		};

		static void Case0()
		{
			{
				using namespace boost::spirit;

				string s = "true, false,   12123, 123123 ,45545,  5345345  ,5455 , 345345,  666, true, false";
				std::vector<boost::variant<int, bool>> vec;
				auto iter = s.begin();
				MyGrammar<string::iterator, ascii::space_type> grammar;
				bool match = qi::phrase_parse(iter, s.end(), grammar, ascii::space, vec);
				if (match)
				{
					std::ostream_iterator<boost::variant<int, bool>> out{ cout, "; " };
					std::copy(vec.begin(), vec.end(), out);
				}

			}
			{
				using namespace boost::spirit;
				string s = "2312 , 4234235, 42342345 , 00, 9407290";
				auto it = s.begin();
				qi::rule<std::string::iterator, std::vector<int>(),
					ascii::space_type> values = qi::int_ % ',';
				std::vector<int> v;
				if (qi::phrase_parse(it, s.end(), values, ascii::space, v))
				{
					std::ostream_iterator<int> out{ std::cout, ";" };
					std::copy(v.begin(), v.end(), out);
				}
			}
			{
				using namespace boost::spirit;
				string str = "   1 234 bbb  _";
				auto iter = str.begin();
				//bool match = qi::phrase_parse(iter, str.end(), ascii::digit, ascii::space);
				bool match = qi::phrase_parse(iter, str.end(), ascii::digit, ascii::space, qi::skip_flag::dont_postskip);
				cout << boolalpha << match << endl;
				cout << string{iter, str.end()} << endl;

			}
			{
				using namespace boost::spirit;

				std::string s;
				std::getline(std::cin, s);
				auto it = s.begin();
				bool match = qi::parse(it, s.end(), ascii::digit);
				std::cout << std::boolalpha << match << '\n';
				if (it != s.end())
					std::cout << std::string{ it, s.end() } << '\n';
			}

			{
				typedef boost::tokenizer<boost::offset_separator> tokenizer;
				std::string s = "Boost_C++_Libraries";
				int offsets[] = { 5, 5, 9 };
				boost::offset_separator sep{ offsets, offsets + 3 };
				tokenizer tok{ s, sep };
				for (const auto& t : tok)
					std::cout << t << '\n';
			}
			{
				typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;
				std::string s = "Boost,\"C++ Libraries\"";
				tokenizer tok{ s };
				for (const auto& t : tok)
					std::cout << t << '\n';
			}
			{
				typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
				std::string s = "Boost C++ Libraries";
				boost::char_separator<char> sep{ " ", "+", boost::keep_empty_tokens };
				tokenizer tok{ s, sep };
				for (const auto& t : tok)
					std::cout << t << '\n';
			}

			{
				typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
				std::string s = "Boost C++ Libraries";
				boost::char_separator<char> sep{ " ", "+" };
				tokenizer tok{ s, sep };
				for (const auto& t : tok)
					std::cout << t << '\n';
			}

			{
				typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
				std::string s = "Boost C++ Libraries";
				boost::char_separator<char> sep{ " " };
				tokenizer tok{ s, sep };
				for (const auto& t : tok)
					std::cout << t << '\n';
			}

			{

				typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
				std::string s = "Boost C++ Libraries";
				tokenizer tok{ s };
				for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
				{
					std::cout << *it << '\n';
				}
			}

			{
				ostream_iterator<string> osi{cout, "\n"};
				vector<string> vec{"hello", "boost", "library"};
				for (const auto & str : vec)
				{
					*osi = str;
					++osi;
				}
			}
			{
				using namespace boost::xpressive;

				std::string s = "Boost Libraries";
				std::ostream_iterator<std::string> it{ std::cout, "\n" };
				sregex expr = (+_w)[*boost::xpressive::ref(it) = _] >> _s >> +_w;
				std::cout << std::boolalpha << regex_match(s, expr) << '\n';
			}
			{
				std::string s = "Boost k\xfct\xfcphaneleri";
				boost::basic_regex<char, boost::cpp_regex_traits<char>> expr;
				expr.imbue(std::locale{ "Turkish" });
				expr = "\\w+\\s\\w+";
				std::cout << std::boolalpha << boost::regex_match(s, expr) << '\n';
			}
			{
				std::string s = "Boost Libraries";
				boost::regex expr{ "\\s+" };
				boost::regex_token_iterator<std::string::iterator> it{ s.begin(), s.end(),
				  expr, -1 };
				boost::regex_token_iterator<std::string::iterator> end;
				while (it != end)
					std::cout << *it++ << '\n';
			}


			{
				std::string s = "Boost Libraries";
				boost::regex expr{ "(\\w\\w)\\w+" };
				boost::regex_token_iterator<std::string::iterator> it{ s.begin(), s.end(),
				  expr, 1 };
				boost::regex_token_iterator<std::string::iterator> end;
				while (it != end)
					std::cout << *it++ << '\n';
			}

			{
				std::string s = "Boost Libraries";
				boost::regex expr{ "(\\w+)\\s(\\w+)" };
				std::string fmt{ "\\2 \\1" };
				std::cout << boost::regex_replace(s, expr, fmt,
					boost::regex_constants::format_literal) << '\n';
				std::cout << boost::regex_replace(s, expr, fmt) << '\n';
			}

			{
				std::string s = " Boost Libraries ";
				boost::regex expr{ "\\s" };
				std::string fmt{ "_" };
				std::cout << boost::regex_replace(s, expr, fmt) << '\n';
			}
			{
				std::string s = "Boost Libraries";
				boost::regex expr{ "(\\w+)\\s(\\w+)" };
				boost::smatch what;
				if (boost::regex_search(s, what, expr))
				{
					std::cout << what[0] << '\n';
					std::cout << what[1] << "_" << what[2] << '\n';
				}
			}

			{
				std::string s = "Boost Libraries";
				boost::regex expr{ "\\w+\\s\\w+" };
				std::cout << std::boolalpha << boost::regex_match(s, expr) << '\n';
			}
			
		}
	};
}