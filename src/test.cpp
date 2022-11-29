#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
#include "cstack.h"
}

TEST(AllAPITest, BadStackHandler)
{
    stack_free(-1);
    EXPECT_EQ(stack_valid_handler(-1), 1);
    EXPECT_EQ(stack_size(-1), 0u);
    const int data_in = 1;
    stack_push(-1, &data_in, sizeof(data_in));
    int data_out = 0;
    EXPECT_EQ(stack_pop(-1, &data_out, sizeof(data_out)), 0u);
    EXPECT_EQ(data_out, 0);
}

TEST(AllocationTests, SingleAllocation)
{
    const hstack_t stack = stack_new();
    EXPECT_EQ(stack_valid_handler(stack), 0);
    EXPECT_EQ(stack_size(stack), 0u);
    stack_free(stack);
    EXPECT_EQ(stack_valid_handler(stack), 1);
}

TEST(AllocationTests, SeveralAllocations)
{
    const size_t count = 10;
    hstack_t stacks[count] = {-1};
    for (size_t i = 0; i < count; ++i)
    {
        stacks[i] = stack_new();
        EXPECT_EQ(stack_valid_handler(stacks[i]), 0);
        EXPECT_EQ(stack_size(stacks[i]), 0u);
    }
    for (size_t i = 0; i < count; ++i)
    {
        stack_free(stacks[i]);
        EXPECT_EQ(stack_valid_handler(stacks[i]), 1);
    }
}

struct ModifyTests : ::testing::Test
{
    void SetUp()
    {
        stack = stack_new();
        ASSERT_EQ(stack_valid_handler(stack), 0);
    }
    void TearDown()
    {
        stack_free(stack);
        ASSERT_EQ(stack_valid_handler(stack), 1);
    }
    hstack_t stack = -1;
};

TEST_F(ModifyTests, PushBadArgs)
{
    stack_push(stack, nullptr, 0u);
    EXPECT_EQ(stack_size(stack), 0u);

    const int data_out = 1;
    stack_push(stack, &data_out, 0u);
    EXPECT_EQ(stack_size(stack), 0u);

    stack_push(stack, nullptr, sizeof(data_out));
    EXPECT_EQ(stack_size(stack), 0u);
}

TEST_F(ModifyTests, PopBadArgs)
{
    const size_t size = 5;
    const int data_in[size] = {1};
    stack_push(stack, &data_in, sizeof(data_in));
    ASSERT_EQ(stack_size(stack), 1u);

    EXPECT_EQ(stack_pop(stack, nullptr, 0u), 0u);

    int data_out[size - 1] = {0};
    EXPECT_EQ(stack_pop(stack, data_out, sizeof(data_out)), 0u);
    EXPECT_THAT(data_out, ::testing::Each(0));

    EXPECT_EQ(stack_pop(stack, nullptr, sizeof(data_in)), 0u);
    EXPECT_THAT(data_out, ::testing::Each(0));
}

TEST_F(ModifyTests, PopFromEmptyStack)
{
    ASSERT_EQ(stack_size(stack), 0u);
    int data_out = 0;
    EXPECT_EQ(stack_pop(stack, &data_out, sizeof(data_out)), 0u);
}

TEST_F(ModifyTests, SinglePushPop)
{
    const int data_in[3] = {0, 1, 2};
    int data_out[3] = {2, 1, 0};
    stack_push(stack, data_in, sizeof(data_in));
    EXPECT_EQ(stack_size(stack), 1u);
    EXPECT_EQ(stack_pop(stack, data_out, sizeof(data_out)), sizeof(data_out));
    EXPECT_EQ(stack_size(stack), 0u);
    EXPECT_THAT(data_out, ::testing::ElementsAre(0, 1, 2));
}

TEST_F(ModifyTests, SeveralPushPop)
{
    const size_t size = 3;
    const int data_in[size] = {0, 1, 2};
    int data_out[size] = {0, 1, 2};
    for (size_t i = 0; i < size; ++i)
    {
        stack_push(stack, &data_in[i], sizeof(data_in[i]));
        EXPECT_EQ(stack_size(stack), i + 1u);
    }
    for (size_t i = 0; i < size; ++i)
    {
        EXPECT_EQ(stack_pop(stack, &data_out[i], sizeof(data_out[i])), sizeof(data_out[i]));
        EXPECT_EQ(stack_size(stack), size - 1u - i);
    }
    EXPECT_THAT(data_out, ::testing::ElementsAre(2, 1, 0));
}

