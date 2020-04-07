#include <../include/graph.h>
#include <../include/dataloader.h>
#include "../include/pattern.h"
#include "../include/schedule.h"
#include "../include/common.h"


#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

int main(int argc,char *argv[])
{
    if (argc != 4)
    {
        printf("Please give a name of dataset, like Patents, a thread number, and a pattern input filename.");
        return -1;
    }
    Graph *g;
    DataLoader D;
    
    std::string path = std::string(argv[1]);
    int thread_count = atoi(argv[2]);
    DataType my_type = DataType::Patents;
    
    if (D.load_data(g,my_type,path.c_str()) != true)
    {
        printf("Load error.");
        return -1;
    }

    std::fstream fs(argv[3]);
    int t;
    fs >> t;
    for (int z = 0; z < t; z++) {
        int n, m;
        fs >> n >> m;
        Pattern p(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            fs >> u >> v;
            p.add_edge(u, v);
        }
        std::vector < std::pair<int, int> > pairs;
        bool is_pattern_valid;
        Schedule schedule(p, is_pattern_valid, 0, g->v_cnt, g->e_cnt);
        schedule.aggressive_optimize(pairs); // check if the isomorphism_vec size can be deleted to 1 by restriction.("assert(isomorphism_vec.size() == 1);")
        schedule.add_restrict(pairs);
        double start_time = get_wall_time();
        long long ans_aggressive = g->pattern_matching_mpi(schedule, thread_count);
        double end_time = get_wall_time();
        printf("Pattern No. %d, time: %.6lf, ans = %lld\n", z, end_time - start_time, ans_aggressive);
        fflush(stdout);
    }
    return 0;
}