#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <string>
#include <iostream>

namespace SmartLib
{
	using namespace ::std;
	using namespace ::boost::multi_index;

	class Eg12BoostContainer
	{
	public:
		static void Case0()
		{
			struct animal
			{
				std::string name;
				int legs;
			};

			typedef multi_index_container
				<
				animal,
				indexed_by
				<
				//hashed_non_unique<member<animal, std::string, &animal::name>>,
				hashed_unique<member<animal, std::string, &animal::name>>,
				//hashed_non_unique<member<animal, int, &animal::legs>>
				hashed_unique<member<animal, int, &animal::legs>>
				>
				> animal_multi;

			//////////////////////////////////////////////////////////////////////////
			{
				animal_multi animals;

				animals.insert({ "cat", 4 });
				animals.insert({ "shark", 0 });
				animals.insert({ "dog", 4 });

				auto& legs_index = animals.get<1>();
				std::cout << legs_index.count(4) << '\n';
			}

			//////////////////////////////////////////////////////////////////////////
			{
				animal_multi animals;
				animals.insert({ "cat", 4 });
				animals.insert({ "shark", 0 });
				animals.insert({ "spider", 8 });

				auto& legIndex = animals.get<1>();
				auto iter = legIndex.find(4);
				legIndex.modify(iter, [](animal& aa) {aa.name="dog"; });

				for (const auto& aa : animals)
				{
					cout << aa.name << ", " << aa.legs << endl;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			{
				animal_multi animals;

				animals.insert({ "cat", 4 });
				animals.insert({ "shark", 0 });
				animals.insert({ "spider", 8 });

				std::cout << animals.count("cat") << '\n';

				const animal_multi::nth_index<1>::type& legs_index = animals.get<1>();
				std::cout << legs_index.count(8) << '\n';
			}

		}
	};

}