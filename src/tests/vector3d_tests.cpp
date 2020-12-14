#include <core/vector3d.h>

#include "core/dfl.h"

#include "gtest/gtest.h"


TEST(Vector3dTestsSuite, sumTwoVectors)
{
    DFL::Vector3d<double> vector1{ 1.0, 2.0, 3.0 };
    DFL::Vector3d<double> vector2{ 1.0, 2.0, 3.0 };

    DFL::Vector3d<double> vector3{ vector1 + vector2 };

    DFL::Vector3d<double> vector4;
    vector4 = vector1 + vector3;

    EXPECT_EQ(vector3.x, 2.0);
    EXPECT_EQ(vector3.y, 4.0);
    EXPECT_EQ(vector3.z, 6.0);

    EXPECT_EQ(vector4.x, 3.0);
    EXPECT_EQ(vector4.y, 6.0);
    EXPECT_EQ(vector4.z, 9.0);
}

TEST(Vector3dTestsSuite, sumByItselfWithAnotherVector)
{
    DFL::Vector3d<double> vector1{ 1.0, 2.0, 3.0 };
    DFL::Vector3d<double> vector2{ 1.0, 2.0, 3.0 };

    vector2 += vector1;

    EXPECT_EQ(vector2.x, 2.0);
    EXPECT_EQ(vector2.y, 4.0);
    EXPECT_EQ(vector2.z, 6.0);

    EXPECT_EQ(vector1.x, 1.0);
    EXPECT_EQ(vector1.y, 2.0);
    EXPECT_EQ(vector1.z, 3.0);
}

TEST(Vector3dTestsSuite, substractVectors)
{
    DFL::Vector3d<double> vector1{ 5.0, 9.0, 3.0 };
    DFL::Vector3d<double> vector2{ 1.0, 12.0, 3.0 };

    DFL::Vector3d<double> vector3{ vector1 - vector2 };

    DFL::Vector3d<double> vector4;
    vector4 = vector1 - vector3;

    EXPECT_EQ(vector3.x, 4.0);
    EXPECT_EQ(vector3.y, -3.0);
    EXPECT_EQ(vector3.z, 0.0);

    EXPECT_EQ(vector4.x, 1.0);
    EXPECT_EQ(vector4.y, 12.0);
    EXPECT_EQ(vector4.z, 3.0);
}

TEST(Vector3dTestsSuite, substractByItselfWithAnotherVector)
{
    DFL::Vector3d<double> vector1{ 1.0, 2.0, 3.0 };
    DFL::Vector3d<double> vector2{ 11.0, -6.0, 8.0 };

    vector2 -= vector1;

    EXPECT_EQ(vector2.x, 10.0);
    EXPECT_EQ(vector2.y, -8.0);
    EXPECT_EQ(vector2.z, 5.0);

    EXPECT_EQ(vector1.x, 1.0);
    EXPECT_EQ(vector1.y, 2.0);
    EXPECT_EQ(vector1.z, 3.0);
}

TEST(Vector3dTestsSuite, negateAVector)
{
    DFL::Vector3d<double> vector1{ 1.0, 2.0, 3.0 };
    DFL::Vector3d<double> vector2{ 11.0, -6.0, 8.0 };

    vector1 = -vector2;

    EXPECT_EQ(vector1.x, -11.0);
    EXPECT_EQ(vector1.y, 6.0);
    EXPECT_EQ(vector1.z, -8.0);

    EXPECT_EQ(vector2.x, 11.0);
    EXPECT_EQ(vector2.y, -6.0);
    EXPECT_EQ(vector2.z, 8.0);
}

TEST(Vector3dTestsSuite, compareVectors)
{
    DFL::Vector3d<double> vector1{ 1.0, 2.0, 3.0 };
    DFL::Vector3d<double> vector2{ 11.0, -6.0, 8.0 };
    DFL::Vector3d<double> vector3{ 1.0, 2.0, 3.0 };

    bool comparison_result{ false };

    if(vector1 == vector3)
    {
        comparison_result = true;
    }else{
        comparison_result = false;
    }

    EXPECT_EQ(comparison_result, true);

    if(vector1 != vector2)
    {
        comparison_result = true;
    }else{
        comparison_result = false;
    }

    EXPECT_EQ(comparison_result, true);
}

TEST(Vector3dTestsSuite, scaleByANumber)
{
    DFL::Vector3d<double> vector1{ 1.0, 2.0, 3.0 };
    DFL::Vector3d<double> vector2{ 11.0, -6.0, 8.0 };
    DFL::Vector3d<double> vector3{ 1.0, 2.0, 3.0 };

    vector2 = vector1 * 2;

    EXPECT_EQ(vector2.x, 2.0);
    EXPECT_EQ(vector2.y, 4.0);
    EXPECT_EQ(vector2.z, 6.0);

    vector3 = 3 * vector1;

    EXPECT_EQ(vector3.x, 3.0);
    EXPECT_EQ(vector3.y, 6.0);
    EXPECT_EQ(vector3.z, 9.0);
}

TEST(Vector3dTestsSuite, multiplication)
{
    DFL::Vector3d<double> vector3{ 3.0, 6.0, 9.0 };

    vector3 *= 3;

    EXPECT_EQ(vector3.x, 9.0);
    EXPECT_EQ(vector3.y, 18.0);
    EXPECT_EQ(vector3.z, 27.0);
}

TEST(Vector3dTestsSuite, division)
{
    DFL::Vector3d<double> vector2{ 8.0, -6.0, 10.0 };
    DFL::Vector3d<double> vector3{ 6.0, 18.0, 24.0 };

    vector2 = vector3 / 2;

    EXPECT_EQ(vector2.x, 3.0);
    EXPECT_EQ(vector2.y, 9.0);
    EXPECT_EQ(vector2.z, 12.0);

    vector2 /= 3;

    std::cout << vector2 << std::endl;

    EXPECT_EQ(vector2.x, 1.0);
    EXPECT_EQ(vector2.y, 3.0);
    EXPECT_EQ(vector2.z, 4.0);
}






















