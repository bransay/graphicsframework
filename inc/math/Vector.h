#ifndef MATH_VECTOR_H_
#define MATH_VECTOR_H_

namespace Math
{
	template <typename T, int N>
	class Vector
	{
	public:
		// TODO: expose as traits
		using ValueType = T;
		const int Dimension = N;

	public:
		Vector();
		~Vector();

		ValueType& operator[](int index);
		const ValueType& operator[](int index) const;

	private:
		T m_V[N];
	};
};

#include "Vector.inl"

#endif
