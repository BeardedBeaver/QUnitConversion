#include <gtest/gtest.h>

#include "qlinearfunction.h"

TEST(QLinearFunction, Basis)
{
    QLinearFunction function;
    ASSERT_FALSE(function.isValid());
    ASSERT_DOUBLE_EQ(function.k(), 0);
    ASSERT_DOUBLE_EQ(function.b(), 0);

    function.setK(15);
    function.setB(5);
    ASSERT_TRUE(function.isValid());
    ASSERT_DOUBLE_EQ(function.k(), 15);
    ASSERT_DOUBLE_EQ(function.b(), 5);

    QLinearFunction secondFunction(1, 2);
    ASSERT_TRUE(secondFunction.isValid());
    ASSERT_DOUBLE_EQ(secondFunction.k(), 1);
    ASSERT_DOUBLE_EQ(secondFunction.b(), 2);
}

TEST(QLinearFunction, Value)
{
    QLinearFunction function(5.5, 3);
    ASSERT_TRUE(function.isValid());
    ASSERT_DOUBLE_EQ(function.y(0), 3);
    ASSERT_DOUBLE_EQ(function.y(-5), -24.5);
    ASSERT_DOUBLE_EQ(function.y(1), 8.5);
    ASSERT_DOUBLE_EQ(function.y(0.7659), 7.21245);
    ASSERT_DOUBLE_EQ(function.y(35.6), 198.8);
}

TEST(QLinearFunction, Inverted)
{
    // inverted() of an invalid function (k=0) returns a default (invalid) function
    QLinearFunction invalid;
    ASSERT_FALSE(invalid.inverted().isValid());

    // f(x) = 3x - 2  =>  f⁻¹(x) = x/3 + 2/3
    QLinearFunction function(3, -2);
    function = function.inverted();
    ASSERT_DOUBLE_EQ(function.k(), double(1) / 3);
    ASSERT_DOUBLE_EQ(function.b(), double(2) / 3);

    // f(x) = -2x - 16  =>  f⁻¹(x) = -0.5x - 8
    function.setK(-2);
    function.setB(-16);
    function = function.inverted();
    ASSERT_DOUBLE_EQ(function.k(), -0.5);
    ASSERT_DOUBLE_EQ(function.b(), -8);

    // applying a function then its inverse round-trips to the original value: f⁻¹(f(x)) == x
    QLinearFunction f(3, -2);
    ASSERT_DOUBLE_EQ(f.inverted().y(f.y(0)), 0);
    ASSERT_DOUBLE_EQ(f.inverted().y(f.y(7)), 7);
    ASSERT_DOUBLE_EQ(f.inverted().y(f.y(-4.5)), -4.5);
}

TEST(QLinearFunction, Combined)
{
    QLinearFunction cToK(1, 273.15);
    QLinearFunction KToF(1.8, -459.67);
    QLinearFunction combined = QLinearFunction::combined(cToK, KToF);
    
    double eps = 0.00001;
    ASSERT_NEAR(combined.y(-273.15), -459.67, eps);
    ASSERT_NEAR(combined.y(-45.56), -50.008, eps);
    ASSERT_NEAR(combined.y(-40), -40, eps);
    ASSERT_NEAR(combined.y(-34.44), -29.992, eps);
    ASSERT_NEAR(combined.y(-28.89), -20.002, eps);
    ASSERT_NEAR(combined.y(-23.33), -9.994, eps);
    ASSERT_NEAR(combined.y(-12.22), 10.004, eps);
    ASSERT_NEAR(combined.y(-6.67), 19.994, eps);
    ASSERT_NEAR(combined.y(-1.11), 30.002, eps);
    ASSERT_NEAR(combined.y(0), 32, eps);
    ASSERT_NEAR(combined.y(4.44), 39.992, eps);
    ASSERT_NEAR(combined.y(10), 50, eps);
    ASSERT_NEAR(combined.y(60), 140, eps);
    ASSERT_NEAR(combined.y(65.56), 150.008, eps);
    ASSERT_NEAR(combined.y(100), 212, eps);
    ASSERT_NEAR(combined.y(260), 500, eps);
    ASSERT_NEAR(combined.y(537.78), 1000.004, eps);
}
