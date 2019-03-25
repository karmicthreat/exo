#include "exo.hpp"
#include "math.hpp"

#include <iostream>

#define DESCRIPTION "Tests exo math functions"

using namespace exo::math;

#include "test.h"
{
	auto v0 = Vec<float, 3>{ 1, 2, 3 };
	auto v1 = Vec<float, 3>{ 1 };


	{ // test addition
		Vec<float, 3> expected = { 2, 3, 4 };
		assert((v0 + v1) == expected);
	}

	{ // test subtraction
		Vec<float, 3> expected = { 0, 1, 2 };
		assert((v0 - v1) == expected);
	}

	{ // test addition
		Vec<float, 3> expected = { 1, 2, 3 };
		assert((v0 * v1) == expected);
	}

	{ // test addition
		Vec<float, 3> expected = { 1, 2, 3 };
		assert((v0 / v1) == expected);
	}

	// test dot
	assert(NEAR(v0.dot(v0), 14));

	{ // test length and norm
		Vec<float, 3> v = { 1, 2, 3 };
		assert(NEAR(v.norm().len(), 1));
	}

	// test indexing and assignment
	v0[0] = 1000;
	assert(v0[0] == 1000);

	// check scaling
	v0 *= 0.001f;
	assert(v0[0] == 1);

	auto ortho = Vec<float,3>::cross({1, 0, 0}, {0, 1, 0});

	assert(ortho[0] == 0);
	assert(ortho[1] == 0);
	assert(ortho[2] == 1);

	// Test quaternions
	{
		auto q = Quat::from_axis_angle({0, 0, 1}, M_PI / 2);

		bool expect_true = q.rotate({ 1, 0, 0 }).is_near(Vec<float, 3>{ 0, 1, 0 }, 0.1);
		assert(expect_true);
	}

	// Test quaternion to matrix conversion
	{
		auto q = Quat::from_axis_angle({0, 0, 1}, M_PI / 2);
		auto m = q.to_matrix<float>();

		auto v = Vec<float, 4>{ 1, 0, 0, 1 };

		auto vq = q.rotate(v.as_dimension<3>());
		auto vm = (m * v).as_dimension<3>();

		assert(vq.is_near(vm));
	}

	{ // check matrix equality testing
		auto I0 = Mat<float, 3, 3>::I();
		auto I1 = Mat<float, 3, 3>::I();

		assert(I0 == I1);
	}

	{ // check matrix multiplication correctness
		auto R = Mat<float, 3, 3>{
			{ (float)rand(), (float)rand(), (float)rand(), },
			{ (float)rand(), (float)rand(), (float)rand(), },
			{ (float)rand(), (float)rand(), (float)rand(), },
		};
		auto I  = Mat<float, 3, 3>::I();
		auto res = R * I;

		assert(res == R);
	}

	{ // Check 2d rotation #1
		auto v = Vec<float, 2>{ 0, 1 };
		auto rotated = Mat2f::rotation(0) * v;
		assert(v.is_near(rotated));
	}

	{ // Check 2d rotation #2
		auto v = Vec<float, 2>{ 0, 1 };
		auto rotated = Mat2f::rotation(M_PI / 2) * v;
		auto expected = Vec<float, 2>{ 1, 0 };
		assert(expected.is_near(rotated));
	}

	{ // Check 2d rotation #3
		auto v = Vec<float, 2>{ 0, 1 };
		auto rotated = Mat2f::rotation(M_PI) * v;
		auto expected = Vec<float, 2>{ 0, -1 };
		assert(expected.is_near(rotated));
	}

	{ // Check 2d rotation #4
		auto v = Vec<float, 2>{ 0, 1 };
		auto rotated = Mat2f::rotation(3 * (M_PI / 2)) * v;
		auto expected = Vec<float, 2>{ -1, 0 };
		assert(expected.is_near(rotated));
	}

	{ // Check matrix rotation then translation
		auto tm = Mat4f::translate({1, 0, 0});
		auto rm = Mat4f::rotate_z(-M_PI / 2);
		auto v = Vec<float, 4>{ 0, 1, 0, 1 };
		auto trans = tm * rm;
		auto r = trans * v;
		std::cerr << r.to_string() << std::endl;
		assert(r.is_near({ 2, 0, 0, 1 }));
	}

	{ // Check matrix inversion
		auto tm = Mat4f::translate({1, 0, 0});
		auto v = Vec<float, 4>{ 0, 0, 0, 1 };
		auto tv = tm * v;
		assert(tv.is_near({1, 0, 0, 1}));
		auto inv = Mat4f::inverse(tm) * tv;
		assert(inv.is_near(v));
	}

	return 0;
}
