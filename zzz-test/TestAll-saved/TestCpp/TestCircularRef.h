#pragma once


namespace TestCpp
{
	class OuterObj
	{

	public:
		//class ::TestCpp::OuterObj::Inner2;
		class Inner2;

		class  Inner1
		{
		public:
			Inner2 * _friend{nullptr};
		};


		class Inner2
		{
		public:
			Inner1 * _friend{ nullptr };
		};

		static void MakePiar(Inner1 * o1, Inner2* o2)
		{
			if (o1)
			{
				o1->_friend = o2;
			}

			if (o2)
			{
				o2->_friend = o1;
			}
		}

	};


	class OuterObjTest
	{
	public:
		static void Case0()
		{
			OuterObj::Inner1 o1;
			OuterObj::Inner2 o2;
			OuterObj::MakePiar(&o1, &o2);
		}
	};
}