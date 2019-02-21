#pragma once

namespace exo
{

namespace units
{

template <class TYPE, unsigned int NUM, unsigned int DEN>
struct Unit
{
	TYPE value;

	Unit(TYPE v) { value = v; }
	inline operator TYPE() const  { return value; }
	
	inline Unit operator*(Unit u) { return Unit(value * u.value); }
	inline Unit operator/(Unit u) { return Unit(value / u.value); }
	inline Unit operator+(Unit u) { return Unit(value + u.value); }
	inline Unit operator-(Unit u) { return Unit(value - u.value); }

	static inline int numerator()   { return NUM; }
	static inline int denominator() { return DEN; }

	template<class OTHER_TYPE>
	inline TYPE cast() { return static_cast<OTHER_TYPE>(value); }

	template<typename OU>
	inline OU in()
	{
		auto base_units = value * numerator() / (TYPE)denominator();
		auto v = base_units * OU::denominator() / (TYPE)OU::numerator();
		return { v };	
	}
};

template <class TYPE, unsigned int NUM=1, unsigned int DEN=1>
struct Meter : public Unit<TYPE, NUM, DEN>
{
	Meter(TYPE v) : Unit<TYPE, NUM, DEN>(v) { }
};

template <class TYPE>
struct Millimeter : public Meter<TYPE, 1, 1000> { Millimeter(TYPE v) : Meter<TYPE, 1, 1000>(v) { } };

template <class TYPE>
struct Centimeter : public Meter<TYPE, 1, 100> { Centimeter(TYPE v) : Meter<TYPE, 1, 100>(v) { } };

template <class TYPE>
struct Decimeter : public Meter<TYPE, 1, 10> { Decimeter(TYPE v) : Meter<TYPE, 1, 10>(v) { } };

template <class TYPE>
struct Kilometer : public Meter<TYPE, 1000, 1> { Kilometer(TYPE v) : Meter<TYPE, 1000, 1>(v) { } };

using M = Meter<float>;
using Mm = Millimeter<float>;
using Cm = Centimeter<float>;
using Dm = Decimeter<float>;
using Km = Kilometer<float>;

}

}

