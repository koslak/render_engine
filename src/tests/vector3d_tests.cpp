#include <core/vector3d.h>

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
