#ifndef TINY_STL_FUNCTION_H
#define TINT_STL_FUNCTION_H
namespace Tiny_STL
{
template<typename Arg,typename Result>
struct unary_function
{
	typedef Arg argument_type;
	typedef Result result_type;
};

template<typename Arg1,typename Arg2,typename Result>
struct binary_function
{
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};

template<typename T>
struct identity:public unary_function<T,T>
{
	const T& operator()(const T& x) const {return x;}
};

template<typename T>
struct less:public binary_function<T,T,bool>
{
	bool operator()(const T& x,const T& y)const
	{
		return x<y;
	}
};

template<typename T>
struct greater:public binary_function<T,T,bool>
{
	bool operator()(const T& x,const T& y)const
	{
		return x>y;
	}
};
}

#endif