#include <cstddef>

namespace cs540{

	struct BadFunctionCall
	{
		BadFunctionCall()=default;
	};

	template <typename T> class Function;

	template <typename ResultType, typename ... Args>
	class Function<ResultType(Args...)>
	{
		class InvokerI{
		public:
			virtual ~InvokerI() = default;
			virtual InvokerI *clone()=0;
			virtual ResultType invoke(Args...) = 0;
		};

		template <typename FunctionType>
		class Invoker: public InvokerI
		{
			FunctionType func;
		public:
			Invoker(const FunctionType &g):func(g){}

			InvokerI *clone()
			{
				return new Invoker<FunctionType>(func);
			}

			~Invoker() = default;
			
			ResultType invoke(Args...args)
			{
				return func(args...);
			}
		};

		InvokerI *_invoker;

	public:
		Function():_invoker(nullptr){}

		template <typename FunctionType> 
		Function(FunctionType f):_invoker(new Invoker<FunctionType>(f)){}

		Function(const Function &f):_invoker(f._invoker->clone()){}

		Function &operator=(const Function &f)
		{
			if(_invoker!=nullptr)
			{
				delete _invoker;
				_invoker=nullptr;
			}
			_invoker=f._invoker->clone();
			return *this;
		}

		~Function(){
			if(_invoker!=nullptr)
			{
				delete _invoker;
				_invoker=nullptr;
			}
		}

		ResultType operator()(Args...args)
		{
			if(_invoker!=nullptr)
			{
				return _invoker->invoke(args...);
			}
			throw BadFunctionCall();
		}

		explicit operator bool() const
		{
			return (_invoker!=nullptr);
		}
		
	};

	template <typename ResultType, typename ... ArgumentTypes> 
	bool operator==(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t)
	{
		return (!f);		
	}

	template <typename ResultType, typename ... ArgumentTypes> 
	bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &f)
	{
		return (!f);
	}

	template <typename ResultType, typename... ArgumentTypes>
	bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t)
	{
		return bool(f);
	} 
	template <typename ResultType, typename... ArgumentTypes> 
	bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &f)
	{
		return bool(f);
	}

}