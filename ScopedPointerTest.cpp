#include <gtest/gtest.h>

#include "ScopedPointer.h"

using namespace ACA;

constexpr int DefaultValue{ 5 };
constexpr int BaseFoo{ 1 };
constexpr int DerivedFoo{ 2 };

struct Base
{
    Base() = default;
    Base(const Base& other) = delete;
    Base& operator=(const Base& other) = delete;
    Base(Base&& other) = delete;
    Base& operator=(Base&& other) = delete;
    virtual int foo() { return BaseFoo; }
    virtual ~Base() = default;
};

struct Derived : public Base
{
    Derived() = default;
    Derived(const Derived& other) = delete;
    Derived& operator=(const Derived& other) = delete;
    Derived(Derived&& other) = delete;
    Derived& operator=(Derived&& other) = delete;
    int foo() { return DerivedFoo; }
    ~Derived() override = default;
};

TEST(ScopedPointer, DefaultCtor) 
{
    ScopedPointer<int> pointer;
    EXPECT_EQ(pointer.get(), nullptr);
}

TEST(ScopedPointer, CtorWithValue) 
{
    int* pInt = new int(DefaultValue);
    ScopedPointer<int> pointer(pInt);
    ASSERT_NE(pointer.get(), nullptr);
    EXPECT_EQ(pointer.get(), pInt);
    EXPECT_EQ(*(pointer.get()), DefaultValue);
}

TEST(ScopedPointer, Dereference) 
{
    ScopedPointer<int> pointer(new int(DefaultValue));
    EXPECT_EQ(*pointer, DefaultValue);
}

TEST(ScopedPointer, ArrowOperator) 
{
    ScopedPointer<Base> pointer(new Base);
    EXPECT_EQ(pointer->foo(), BaseFoo);
}

TEST(ScopedPointer, ReleaseReset) 
{
    int* pInt = new int(DefaultValue);
    ScopedPointer<int> pointer(pInt);
    int* pInt2 = pointer.release();

    EXPECT_EQ(pointer.get(), nullptr);
    EXPECT_EQ(pInt, pInt2);

    pointer.reset(pInt2);
    ASSERT_NE(pointer.get(), nullptr);
    EXPECT_EQ(pointer.get(), pInt2);
    EXPECT_EQ(*pointer, DefaultValue);
}

TEST(ScopedPointer, MoveCtor) 
{
    ScopedPointer<int> pointer1(new int(DefaultValue));
    ScopedPointer<int> pointer2(std::move(pointer1));

    EXPECT_EQ(pointer1.get(), nullptr);
    ASSERT_NE(pointer2.get(), nullptr);
    EXPECT_EQ(*pointer2, DefaultValue);
}

TEST(ScopedPointer, MoveAssign) 
{
    ScopedPointer<int> pointer1(new int(DefaultValue));
    ScopedPointer<int> pointer2;
    pointer2 = std::move(pointer1);

    EXPECT_EQ(pointer1.get(), nullptr);
    ASSERT_NE(pointer2.get(), nullptr);
    EXPECT_EQ(*pointer2, DefaultValue);
}

TEST(ScopedPointer, CtorFromDerivedType) 
{
    ScopedPointer<Base> pointer = []() 
    {
        return ScopedPointer<Base>(new Derived);
    }();

    ASSERT_NE(pointer.get(), nullptr);
    EXPECT_EQ(pointer->foo(), DerivedFoo);
}

const auto CreateDerived = []() 
{
    return ScopedPointer<Derived>(new Derived);
};
TEST(ScopedPointer, MoveCtorFromDerivedType) 
{
    ScopedPointer<Base> pointer = CreateDerived();

    ASSERT_NE(pointer.get(), nullptr);
    EXPECT_EQ(pointer->foo(), DerivedFoo);
}

TEST(ScopedPointer, MoveAssignmentFromDerivedType) 
{
    ScopedPointer<Base> pointer;
    pointer = CreateDerived();

    ASSERT_NE(pointer.get(), nullptr);
    EXPECT_EQ(pointer->foo(), DerivedFoo);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}