#include <gtest/gtest.h>

#include <xar_algo/interval/interval_container.hpp>


namespace xar_algo::interval
{
    bool operator==(const interval::TInterval<int>& left, const interval::TInterval<int>& right)
    {
        return left.begin == right.begin && left.end == right.end;
    }
}

namespace xar_algo
{
    namespace
    {
        using IntervalCollectionType = interval::TIntervalCollection<int>;
        using IntervalType = IntervalCollectionType::IntervalType;


        class IntervalCollectionTest
            : public testing::Test
        {
        };


        std::vector<IntervalType> to_vector(const IntervalCollectionType& collection)
        {
            return {collection.data.begin(), collection.data.end()};
        }


        TEST_F(IntervalCollectionTest, contains_value__empty_container__false)
        {
            IntervalCollectionType interval_collection;

            EXPECT_FALSE(contains(interval_collection, 0));
        }


        TEST_F(IntervalCollectionTest, contains__container_with_one_interval__correct_result)
        {
            IntervalCollectionType interval_collection;
            interval_collection.data.insert({0, 10});

            EXPECT_FALSE(contains(interval_collection, -1));
            EXPECT_TRUE(contains(interval_collection, 0));
            EXPECT_TRUE(contains(interval_collection, 10));
            EXPECT_FALSE(contains(interval_collection, 11));
        }


        TEST_F(IntervalCollectionTest, contains__container_with_two_intervals__correct_result)
        {
            IntervalCollectionType interval_collection;
            interval_collection.data.insert({0, 10});
            interval_collection.data.insert({20, 30});

            EXPECT_FALSE(contains(interval_collection, -1));
            EXPECT_TRUE(contains(interval_collection, 0));
            EXPECT_TRUE(contains(interval_collection, 10));
            EXPECT_FALSE(contains(interval_collection, 11));
            EXPECT_FALSE(contains(interval_collection, 19));
            EXPECT_TRUE(contains(interval_collection, 20));
            EXPECT_TRUE(contains(interval_collection, 30));
            EXPECT_FALSE(contains(interval_collection, 31));
        }


        TEST_F(IntervalCollectionTest, to_vector__empty_container__empty_result)
        {
            IntervalCollectionType interval_collection;

            EXPECT_EQ(interval_collection.data.size(), 0);
        }


        TEST_F(IntervalCollectionTest, to_vector__filled_container__correct_result)
        {
            IntervalCollectionType interval_collection;
            interval_collection.data.insert({10, 20});
            interval_collection.data.insert({0, 5});
            interval_collection.data.insert({30, 35});

            const auto result = to_vector(interval_collection);
            ASSERT_EQ(result.size(), 3);
            EXPECT_EQ(result, (std::vector<IntervalType>{
                {0, 5}, {10, 20}, {30, 35},
            }));
        }


        TEST_F(IntervalCollectionTest, find_overlapping_intervals__empty_container__empty_result)
        {
            IntervalCollectionType interval_collection;
            const auto range = find_overlapping_intervals(interval_collection, {10, 20});
            EXPECT_EQ(range.first, range.second);
            EXPECT_EQ(range.first, interval_collection.data.begin());
        }


        TEST_F(IntervalCollectionTest, find_overlapping_intervals__container_with_one_interval__correct_results)
        {
            IntervalCollectionType interval_collection;
            interval_collection.data.insert({10, 20});

            {
                SCOPED_TRACE("Range before collection");
                /**
                 * search    : [ ]
                 * collection:      [   ]
                 * result    : empty
                 */

                const auto range = find_overlapping_intervals(interval_collection, {0, 5});
                EXPECT_EQ(range.first, range.second);
            }

            {
                SCOPED_TRACE("Range at the beginning of collection");
                /**
                 * search    : [      ]
                 * collection:      [   ]
                 * result    :      [   ]
                 */

                const auto range = find_overlapping_intervals(interval_collection, {0, 15});
                EXPECT_NE(range.first, range.second);
                EXPECT_EQ(range.first, interval_collection.data.begin());
                EXPECT_EQ(range.second, interval_collection.data.end());
                EXPECT_EQ(*range.first, *interval_collection.data.begin());
            }

            {
                SCOPED_TRACE("Range at the beginning of collection");
                /**
                 * search    : [               ]
                 * collection:      [   ]
                 * result    :      [   ]
                 */

                const auto range = find_overlapping_intervals(interval_collection, {0, 30});
                EXPECT_NE(range.first, range.second);
                EXPECT_EQ(range.first, interval_collection.data.begin());
                EXPECT_EQ(range.second, interval_collection.data.end());
                EXPECT_EQ(*range.first, *interval_collection.data.begin());
            }

            {
                SCOPED_TRACE("Range at the beginning of collection");
                /**
                 * search    :       [ ]
                 * collection:      [   ]
                 * result    :      [   ]
                 */

                const auto range = find_overlapping_intervals(interval_collection, {12, 18});
                EXPECT_NE(range.first, range.second);
                EXPECT_EQ(range.first, interval_collection.data.begin());
                EXPECT_EQ(range.second, interval_collection.data.end());
                EXPECT_EQ(*range.first, *interval_collection.data.begin());
            }

            {
                SCOPED_TRACE("Range at the beginning of collection");
                /**
                 * search    :        [     ]
                 * collection:      [   ]
                 * result    :      [   ]
                 */

                const auto range = find_overlapping_intervals(interval_collection, {18, 25});
                EXPECT_NE(range.first, range.second);
                EXPECT_EQ(range.first, interval_collection.data.begin());
                EXPECT_EQ(range.second, interval_collection.data.end());
                EXPECT_EQ(*range.first, *interval_collection.data.begin());
            }

            {
                SCOPED_TRACE("Range at the beginning of collection");
                /**
                 * search    :              [     ]
                 * collection:      [   ]
                 * result    : empty
                 */

                const auto range = find_overlapping_intervals(interval_collection, {25, 30});
                EXPECT_EQ(range.first, range.second);
            }
        }


        TEST_F(IntervalCollectionTest, find_overlapping_intervals__container_with_two_intervals__correct_results)
        {
            IntervalCollectionType interval_collection;
            interval_collection.data.insert({10, 20});
            interval_collection.data.insert({40, 50});

            {
                SCOPED_TRACE("Range between intervals in collection");
                /**
                 * search    :            [ ]
                 * collection:      [   ]     [   ]
                 * result    : empty
                 */

                const auto range = find_overlapping_intervals(interval_collection, {25, 35});
                EXPECT_EQ(range.first, range.second);
            }

            {
                SCOPED_TRACE("Range at the end of first interval");
                /**
                 * search    :         [    ]
                 * collection:      [   ]     [   ]
                 * result    :      [   ]
                 */

                const auto range = find_overlapping_intervals(interval_collection, {15, 30});
                EXPECT_NE(range.first, range.second);
                EXPECT_EQ(range.first, interval_collection.data.begin());
                EXPECT_EQ(range.second, std::next(interval_collection.data.begin()));
                EXPECT_EQ(std::distance(range.first, range.second), 1);
                EXPECT_EQ(*range.first, *interval_collection.data.begin());
            }

            {
                SCOPED_TRACE("Range at the beginning of second interval");
                /**
                 * search    :             [    ]
                 * collection:      [   ]     [   ]
                 * result    :                [   ]
                 */

                const auto range = find_overlapping_intervals(interval_collection, {30, 45});
                EXPECT_NE(range.first, range.second);
                EXPECT_EQ(range.first, std::next(interval_collection.data.begin()));
                EXPECT_EQ(range.second, interval_collection.data.end());
                EXPECT_EQ(std::distance(range.first, range.second), 1);
                EXPECT_EQ(*range.first, *std::next(interval_collection.data.begin()));
            }

            {
                SCOPED_TRACE("Range at the end of first interval and the beginning of second interval");
                /**
                 * search    :        [         ]
                 * collection:      [   ]     [   ]
                 * result    :      [   ]     [   ]
                 */

                const auto range = find_overlapping_intervals(interval_collection, {15, 45});
                EXPECT_NE(range.first, range.second);
                EXPECT_EQ(range.first, interval_collection.data.begin());
                EXPECT_EQ(range.second, interval_collection.data.end());
                EXPECT_EQ(std::distance(range.first, range.second), 2);
                EXPECT_EQ(*range.first, *interval_collection.data.begin());
                EXPECT_EQ(*std::next(range.first), *std::next(interval_collection.data.begin()));
            }

            {
                SCOPED_TRACE("Range overlap fully both intervals");
                /**
                 * search    :    [                  ]
                 * collection:      [   ]     [   ]
                 * result    :      [   ]     [   ]
                 */

                const auto range = find_overlapping_intervals(interval_collection, {5, 55});
                EXPECT_NE(range.first, range.second);
                EXPECT_EQ(range.first, interval_collection.data.begin());
                EXPECT_EQ(range.second, interval_collection.data.end());
                EXPECT_EQ(std::distance(range.first, range.second), 2);
                EXPECT_EQ(*range.first, *interval_collection.data.begin());
                EXPECT_EQ(*std::next(range.first), *std::next(interval_collection.data.begin()));
            }
        }


        TEST_F(IntervalCollectionTest, add_case_1)
        {
            IntervalCollectionType interval_collection;

            add(interval_collection, {0, 5});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {0, 5},
            }));

            add(interval_collection, {10, 15});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {0, 5}, {10, 15},
            }));

            add(interval_collection, {2, 8});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {0, 8}, {10, 15},
            }));

            add(interval_collection, {9, 12});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {0, 8}, {9, 15},
            }));

            add(interval_collection, {8, 9});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {0, 15},
            }));
        }


        TEST_F(IntervalCollectionTest, add_case_2)
        {
            IntervalCollectionType interval_collection;

            add(interval_collection, {10, 20});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {10, 20},
            }));

            add(interval_collection, {5, 25});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {5, 25},
            }));
        }


        TEST_F(IntervalCollectionTest, add_case_3)
        {
            IntervalCollectionType interval_collection;

            add(interval_collection, {5, 25});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {5, 25},
            }));

            add(interval_collection, {10, 20});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {5, 25},
            }));
        }


        TEST_F(IntervalCollectionTest, add_case_4)
        {
            IntervalCollectionType interval_collection;

            add(interval_collection, {10, 20});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {10, 20},
            }));

            add(interval_collection, {5, 15});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {5, 20},
            }));
        }


        TEST_F(IntervalCollectionTest, add_case_5)
        {
            IntervalCollectionType interval_collection;

            add(interval_collection, {10, 20});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {10, 20},
            }));

            add(interval_collection, {15, 25});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {10, 25},
            }));
        }


        TEST_F(IntervalCollectionTest, add_case_6)
        {
            IntervalCollectionType interval_collection;

            add(interval_collection, {10, 20});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {10, 20},
            }));

            add(interval_collection, {20, 30});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {10, 30},
            }));
        }


        TEST_F(IntervalCollectionTest, add_case_7)
        {
            IntervalCollectionType interval_collection;

            add(interval_collection, {10, 20});
            add(interval_collection, {30, 40});
            add(interval_collection, {50, 60});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {10, 20}, {30, 40}, {50, 60}
            }));

            add(interval_collection, {0, 70});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {0, 70},
            }));
        }


        TEST_F(IntervalCollectionTest, add_case_8)
        {
            IntervalCollectionType interval_collection;

            add(interval_collection, {10, 20});
            add(interval_collection, {30, 40});
            add(interval_collection, {50, 60});
            add(interval_collection, {70, 80});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {10, 20}, {30, 40}, {50, 60}, {70, 80}
            }));

            add(interval_collection, {25, 65});
            EXPECT_EQ(to_vector(interval_collection), (std::vector<IntervalType>{
                {10, 20}, {25, 65}, {70, 80}
            }));
        }
    }
}
