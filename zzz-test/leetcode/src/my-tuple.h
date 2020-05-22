#pragma once

//#include <string>

namespace SmartLib
{

#if false

	template<typename T>
	class RemoveRef
	{
	public:
		using Type = typename T;
	};

	template<typename T>
	class RemoveRef<T&>
	{
	public:
		using Type = typename T;
	};

	template<typename T>
	class RemoveRef<T&&>
	{
	public:
		using Type = typename T;
	};


	template<typename T>
	using RemoveRefType = typename RemoveRef<T>::Type;


	class Ref
	{
	public:
		template<typename T>
		static constexpr typename RemoveRef<T>::Type&& Move(typename RemoveRef<T>::Type& ref)
		{
			return static_cast<typename RemoveRef<T>::Type&&>(ref);
		}

		template<typename T>
		static constexpr typename RemoveRef<T>::Type&& Move(typename RemoveRef<T>::Type&& ref)
		{
			return static_cast<typename RemoveRef<T>::Type&&>(ref);
		}

		template<typename T>
		static constexpr typename T&& Forward(typename RemoveRef<T>::Type& ref)
		{
			return static_cast<T&&>(ref);
		}


		template<typename T>
		static constexpr typename T&& Forward(typename RemoveRef<T>::Type&& ref)
		{
			return static_cast<T&&>(ref);
		}
	};


#endif

#if true

	template<long INDEX, typename T>
	class SmlTupleData
	{
	private:
		T _data{};
	public:

		template<typename D>
		SmlTupleData(D&& data) :
			_data{static_cast<D&&>(data)}
		{
		}

		SmlTupleData()
		{

		}


		T& Get()
		{
			return _data;
		}
	};

	template<typename T, typename... TARGS>
	class SmlTupleTypeCount
	{
	public:
		static constexpr long Count()
		{
			return 1 + SmlTupleTypeCount<TARGS...>::Count();
		}

	};

	template<typename T>
	class SmlTupleTypeCount<T>
	{
	public:
		static constexpr long Count()
		{
			return 1;
		}
	};

	template<long INDEX, typename T, typename... TARGS>
	class SmlTupleIndexType
	{
	public:
		using Type = typename SmlTupleIndexType<INDEX - 1, TARGS...>::Type;
	};

	template<typename T, typename... TARGS>
	class SmlTupleIndexType<0, T, TARGS...>
	{
	public:
		using Type = /*typename*/ T;
	};

	//template<long INDEX, typename... TARGS>
	//class SmlTupleIndexType<INDEX, TARGS...>
	//{
	//public:
	//	using Type = void;
	//};

	template<long INDEX, typename... TARGS>
	class SmlTupleList
	{
	//public:
	//	SmlTupleList(const SmlTupleList& other)
	//	{
	//	}

		//SmlTupleList()
		//{
		//}

		const SmlTupleList& operator=(SmlTupleList& other)
		{
			return *this;
		}
	};

	template<long INDEX, typename T, typename... TARGS>
	class SmlTupleList<INDEX, T, TARGS...> : 
		public SmlTupleData<INDEX, T>,
		public SmlTupleList<INDEX + 1, TARGS...>
	{
	public:
		template<typename TPARAM, typename... TPARAMS>
		SmlTupleList(TPARAM&& param, TPARAMS&&... params) :
			SmlTupleData<INDEX, T>{static_cast<TPARAM&&>(param)},
			SmlTupleList<INDEX + 1, TARGS...>{ static_cast<TPARAMS&&>(params)... }
		{
		}

		//SmlTupleList()
		//{
		//}

		const SmlTupleList& operator=(SmlTupleList& other)
		{
			Get<INDEX>() = other.Get<INDEX>();
			static_cast<SmlTupleList<INDEX + 1, TARGS...>&>(*this) = static_cast<SmlTupleList<INDEX + 1, TARGS...>&>(other);
			return *this;
		}
		//SmlTupleList(SmlTupleList& other) :
		//	SmlTupleData<INDEX, T>{other.Get<INDEX>()},
		//	SmlTupleList<INDEX + 1, TARGS...>{static_cast<SmlTupleList<INDEX + 1, TARGS...>&>(other)}
		//{

		//}

		//SmlTupleList()
		//{
		//}

		template<long ITEM_INDEX>
		typename SmlTupleIndexType<ITEM_INDEX, T, TARGS...>::Type& Get()
		{
			return (static_cast<SmlTupleData<ITEM_INDEX, typename SmlTupleIndexType<ITEM_INDEX, T, TARGS...>::Type>*>(this))->Get();
		}

		static constexpr long ItemsCount() /*const*/
		{
			return SmlTupleTypeCount<T, TARGS...>::Count();
		}
	};

	template<typename T, typename... TARGS>
	class SmlTuple : 
		public SmlTupleList<0, T, TARGS...>
	{
	public:
		template<typename TPARAM, typename... TPARAMS>
		SmlTuple(TPARAM&& param, TPARAMS&&... params) :
			SmlTupleList<0, T, TARGS...>{static_cast<TPARAM&&>(param), static_cast<TPARAMS&&>(params)... }
		{

		}


		SmlTuple(const SmlTuple& other) :
			SmlTupleList<0, T, TARGS...>{static_cast<const SmlTupleList<0, T, TARGS...>&>(other)}
		{
		}
	};

#endif

#if false


	template<long INDEX, typename T>
	class SmlTupleData
	{
	private:
		T _data{};

	public:
		T& Get()
		{
			return _data;
		}

		SmlTupleData(const T& data) :
			_data{data}
		{
		}

		SmlTupleData(T&& data) :
			_data{static_cast<T&&>(data)}
		{
		}

		SmlTupleData()
		{
		}
	};

	template<typename T, typename... TARGS>
	class SmlTupleDataCount
	{
	public:
		static constexpr long Count()
		{
			return 1 + SmlTupleDataCount<TARGS...>::Count();
		}

	};

	template<typename T>
	class SmlTupleDataCount<T>
	{
	public:
		static constexpr long Count()
		{
			return 1;
		}
	};

	template<long INDEX, typename T, typename... TARGS>
	class SmlTupleDataType
	{
	public:
		using Type = typename SmlTupleDataType<INDEX - 1, TARGS...>::Type;
	};

	template<typename T, typename... TARGS>
	class SmlTupleDataType<0, T, TARGS...>
	{
	public:
		using Type = typename T;
	};


	template<long INDEX, typename... TARGS>
	class TupeBase
	{
	};

	template<long INDEX, typename T, typename... TARGS>
	class TupeBase<INDEX, T, TARGS...> :
		public SmlTupleData<INDEX, T>,
		public TupeBase<INDEX + 1, TARGS...>
	{
	public:
		template<typename TPARAM, typename... TPARAMS>
		TupeBase(TPARAM&& param, TPARAMS&&... params) :
			SmlTupleData<INDEX, T>{static_cast<TPARAM&&>(param)},
			TupeBase<INDEX + 1, TARGS...>{ static_cast<TPARAMS&&>(params)...}
		{
		}

		template<long INDEX>
		auto& Get()
		{
			return (static_cast<SmlTupleData<INDEX, SmlTupleDataType<INDEX, T, TARGS...>::Type>*>(this))->Get();
		}


		constexpr long ItemsCount() const
		{
			return SmlTupleDataCount<T, TARGS...>::Count();
		}
	};

	template<typename T, typename... TARGS>
	class SmlTuple : public TupeBase<0, T, TARGS...>
	{
	public:
		template<typename TPARAM, typename... TPARAMS>
		SmlTuple(TPARAM&& param, TPARAMS&&... params) :
			TupeBase<0, T, TARGS...>{static_cast<TPARAM&&>(param), static_cast<TPARAMS&&>(params)...}
		{
		}
	};

#endif

#if false

	//////////////////////////////////////////////////////////////////////////
	template<long INDEX, typename T>
	class SmlTupleData
	{
	private:
		T _data;
	public:
		T& Get()
		{
			return _data;
		}

		SmlTupleData(const T& data) :
			_data{ data }
		{
		}

		SmlTupleData(T&& data) :
			_data{ static_cast<T&&>(data) }
		{
		}
	};

	//////////////////////////////////////////////////////////////////////////
	template<typename T, typename... TARGS>
	class SmlTupleDataCount
	{
	public:
		static constexpr long Count()
		{
			return 1 + SmlTupleDataCount<TARGS...>::Count();
		}
	};

	template<typename T>
	class SmlTupleDataCount<T>
	{
	public:
		static constexpr long Count()
		{
			return 1L;
		}
	};


	template<long INDEX, typename T, typename ... TARGS>
	class SmlTupleDataType
	{
	public:
		using Type = typename SmlTupleDataType<INDEX - 1, TARGS...>::Type;
	};

	template<typename T, typename... TARGS>
	class SmlTupleDataType<0, T, TARGS...>
	{
	public:
		using Type = typename T;
	};


	//////////////////////////////////////////////////////////////////////////
	template<long INDEX, typename ... TARGS>
	class SmlTupleBase
	{
	};

	template<long INDEX, typename T, typename... TARGS>
	class SmlTupleBase<INDEX, T, TARGS...> :
		public SmlTupleData<INDEX, T>,
		public SmlTupleBase<INDEX + 1, TARGS...>
	{
	public:
		SmlTupleBase(T&& data, TARGS&& ... args) :
			SmlTupleData<INDEX, T>{ static_cast<T&&>(data) },
			SmlTupleBase<INDEX + 1, TARGS...>{ static_cast<TARGS&&>(args)... }
		{
		}
	};



	template<typename T, typename... TARGS>
	class SmlTuple : public SmlTupleBase<0, T, TARGS...>
	{
	public:
		SmlTuple(T&& data, TARGS&&... args) :
			SmlTupleBase<0, T, TARGS...>{ static_cast<T&&>(data), static_cast<TARGS&&>(args)... }
		{
		}

		template<long INDEX>
		auto& Get()
		{
			return (static_cast<SmlTupleData<INDEX, SmlTupleDataType<INDEX, T, TARGS...>::Type>*>(this))->Get();
		}


		constexpr long ItemsCount() const
		{
			return SmlTupleDataCount<T, TARGS...>::Count();
		}

	};




#endif

	
	

}