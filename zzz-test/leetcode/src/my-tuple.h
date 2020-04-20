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
	class TupleData
	{
	private:
		T _data{};
	public:

		template<typename D>
		TupleData(D&& data) :
			_data{static_cast<D&&>(data)}
		{
		}

		TupleData()
		{

		}


		T& Get()
		{
			return _data;
		}
	};

	template<typename T, typename... TARGS>
	class TupleTypeCount
	{
	public:
		static constexpr long Count()
		{
			return 1 + TupleTypeCount<TARGS...>::Count();
		}

	};

	template<typename T>
	class TupleTypeCount<T>
	{
	public:
		static constexpr long Count()
		{
			return 1;
		}
	};

	template<long INDEX, typename T, typename... TARGS>
	class TupleIndexType
	{
	public:
		using Type = typename TupleIndexType<INDEX - 1, TARGS...>::Type;
	};

	template<typename T, typename... TARGS>
	class TupleIndexType<0, T, TARGS...>
	{
	public:
		using Type = /*typename*/ T;
	};

	//template<long INDEX, typename... TARGS>
	//class TupleIndexType<INDEX, TARGS...>
	//{
	//public:
	//	using Type = void;
	//};

	template<long INDEX, typename... TARGS>
	class TupleList
	{
	//public:
	//	TupleList(const TupleList& other)
	//	{
	//	}

		//TupleList()
		//{
		//}

		const TupleList& operator=(TupleList& other)
		{
			return *this;
		}
	};

	template<long INDEX, typename T, typename... TARGS>
	class TupleList<INDEX, T, TARGS...> : 
		public TupleData<INDEX, T>,
		public TupleList<INDEX + 1, TARGS...>
	{
	public:
		template<typename TPARAM, typename... TPARAMS>
		TupleList(TPARAM&& param, TPARAMS&&... params) :
			TupleData<INDEX, T>{static_cast<TPARAM&&>(param)},
			TupleList<INDEX + 1, TARGS...>{ static_cast<TPARAMS&&>(params)... }
		{
		}

		//TupleList()
		//{
		//}

		const TupleList& operator=(TupleList& other)
		{
			Get<INDEX>() = other.Get<INDEX>();
			static_cast<TupleList<INDEX + 1, TARGS...>&>(*this) = static_cast<TupleList<INDEX + 1, TARGS...>&>(other);
			return *this;
		}
		//TupleList(TupleList& other) :
		//	TupleData<INDEX, T>{other.Get<INDEX>()},
		//	TupleList<INDEX + 1, TARGS...>{static_cast<TupleList<INDEX + 1, TARGS...>&>(other)}
		//{

		//}

		//TupleList()
		//{
		//}

		template<long ITEM_INDEX>
		typename TupleIndexType<ITEM_INDEX, T, TARGS...>::Type& Get()
		{
			return (static_cast<TupleData<ITEM_INDEX, typename TupleIndexType<ITEM_INDEX, T, TARGS...>::Type>*>(this))->Get();
		}

		static constexpr long ItemsCount() /*const*/
		{
			return TupleTypeCount<T, TARGS...>::Count();
		}
	};

	template<typename T, typename... TARGS>
	class Tuple : 
		public TupleList<0, T, TARGS...>
	{
	public:
		template<typename TPARAM, typename... TPARAMS>
		Tuple(TPARAM&& param, TPARAMS&&... params) :
			TupleList<0, T, TARGS...>{static_cast<TPARAM&&>(param), static_cast<TPARAMS&&>(params)... }
		{

		}


		Tuple(const Tuple& other) :
			TupleList<0, T, TARGS...>{static_cast<const TupleList<0, T, TARGS...>&>(other)}
		{
		}
	};

#endif

#if false


	template<long INDEX, typename T>
	class TupleData
	{
	private:
		T _data{};

	public:
		T& Get()
		{
			return _data;
		}

		TupleData(const T& data) :
			_data{data}
		{
		}

		TupleData(T&& data) :
			_data{static_cast<T&&>(data)}
		{
		}

		TupleData()
		{
		}
	};

	template<typename T, typename... TARGS>
	class TupleDataCount
	{
	public:
		static constexpr long Count()
		{
			return 1 + TupleDataCount<TARGS...>::Count();
		}

	};

	template<typename T>
	class TupleDataCount<T>
	{
	public:
		static constexpr long Count()
		{
			return 1;
		}
	};

	template<long INDEX, typename T, typename... TARGS>
	class TupleDataType
	{
	public:
		using Type = typename TupleDataType<INDEX - 1, TARGS...>::Type;
	};

	template<typename T, typename... TARGS>
	class TupleDataType<0, T, TARGS...>
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
		public TupleData<INDEX, T>,
		public TupeBase<INDEX + 1, TARGS...>
	{
	public:
		template<typename TPARAM, typename... TPARAMS>
		TupeBase(TPARAM&& param, TPARAMS&&... params) :
			TupleData<INDEX, T>{static_cast<TPARAM&&>(param)},
			TupeBase<INDEX + 1, TARGS...>{ static_cast<TPARAMS&&>(params)...}
		{
		}

		template<long INDEX>
		auto& Get()
		{
			return (static_cast<TupleData<INDEX, TupleDataType<INDEX, T, TARGS...>::Type>*>(this))->Get();
		}


		constexpr long ItemsCount() const
		{
			return TupleDataCount<T, TARGS...>::Count();
		}
	};

	template<typename T, typename... TARGS>
	class Tuple : public TupeBase<0, T, TARGS...>
	{
	public:
		template<typename TPARAM, typename... TPARAMS>
		Tuple(TPARAM&& param, TPARAMS&&... params) :
			TupeBase<0, T, TARGS...>{static_cast<TPARAM&&>(param), static_cast<TPARAMS&&>(params)...}
		{
		}
	};

#endif

#if false

	//////////////////////////////////////////////////////////////////////////
	template<long INDEX, typename T>
	class TupleData
	{
	private:
		T _data;
	public:
		T& Get()
		{
			return _data;
		}

		TupleData(const T& data) :
			_data{ data }
		{
		}

		TupleData(T&& data) :
			_data{ static_cast<T&&>(data) }
		{
		}
	};

	//////////////////////////////////////////////////////////////////////////
	template<typename T, typename... TARGS>
	class TupleDataCount
	{
	public:
		static constexpr long Count()
		{
			return 1 + TupleDataCount<TARGS...>::Count();
		}
	};

	template<typename T>
	class TupleDataCount<T>
	{
	public:
		static constexpr long Count()
		{
			return 1L;
		}
	};


	template<long INDEX, typename T, typename ... TARGS>
	class TupleDataType
	{
	public:
		using Type = typename TupleDataType<INDEX - 1, TARGS...>::Type;
	};

	template<typename T, typename... TARGS>
	class TupleDataType<0, T, TARGS...>
	{
	public:
		using Type = typename T;
	};


	//////////////////////////////////////////////////////////////////////////
	template<long INDEX, typename ... TARGS>
	class TupleBase
	{
	};

	template<long INDEX, typename T, typename... TARGS>
	class TupleBase<INDEX, T, TARGS...> :
		public TupleData<INDEX, T>,
		public TupleBase<INDEX + 1, TARGS...>
	{
	public:
		TupleBase(T&& data, TARGS&& ... args) :
			TupleData<INDEX, T>{ static_cast<T&&>(data) },
			TupleBase<INDEX + 1, TARGS...>{ static_cast<TARGS&&>(args)... }
		{
		}
	};



	template<typename T, typename... TARGS>
	class Tuple : public TupleBase<0, T, TARGS...>
	{
	public:
		Tuple(T&& data, TARGS&&... args) :
			TupleBase<0, T, TARGS...>{ static_cast<T&&>(data), static_cast<TARGS&&>(args)... }
		{
		}

		template<long INDEX>
		auto& Get()
		{
			return (static_cast<TupleData<INDEX, TupleDataType<INDEX, T, TARGS...>::Type>*>(this))->Get();
		}


		constexpr long ItemsCount() const
		{
			return TupleDataCount<T, TARGS...>::Count();
		}

	};




#endif

	
	

}