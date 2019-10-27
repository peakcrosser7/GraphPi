#include <gtest/gtest.h>
#include <../include/graph.h>
#include <../include/dataloader.h>
#include "../include/pattern.h"
#include "../include/schedule.h"
#include "../include/common.h"


#include <iostream>
#include <string>

TEST(graph_mining_test, test_triangle_counting) {
    Graph *g;
    DataLoader D;
    
    std::string type = "Patents";
    std::string path = "/home/zms/patents_input";
    DataType my_type;
    if(type == "Patents") my_type = DataType::Patents;
    else {
        printf("invalid DataType!\n");
    }
    
    ASSERT_EQ(D.load_data(g,my_type,path.c_str()),true); 

    double t1 = get_wall_time();
    ASSERT_EQ(g->triangle_counting(), 7515023);
    double t2 = get_wall_time();
    printf("brute force single thread TC time: %.6lf\n", t2 - t1);

    t1 = get_wall_time();
    int thread_num = 24;
    ASSERT_EQ(g->triangle_counting_mt(thread_num), 7515023);
    t2 = get_wall_time();
    printf("brute force %d thread TC time: %.6lf\n", thread_num, t2 - t1);

    Pattern tc_pattern(3);
    tc_pattern.add_edge(0, 1);
    tc_pattern.add_edge(0, 2);
    tc_pattern.add_edge(1, 2);
    Schedule tc_schedule(tc_pattern);

    t1 = get_wall_time();
    ASSERT_EQ(g->pattern_matching(tc_schedule, 1), 7515023 * 6);
    t2 = get_wall_time();
    printf("general single thread TC time: %.6lf\n", t2 - t1);

    thread_num = 24;
    t1 = get_wall_time();
    ASSERT_EQ(g->pattern_matching(tc_schedule, thread_num), 7515023 * 6);
    t2 = get_wall_time();
    printf("general %d thread TC time: %.6lf\n", thread_num, t2 - t1);

    delete g;
}