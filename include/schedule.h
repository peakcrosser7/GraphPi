#pragma once

#include "pattern.h"
#include "prefix.h"
class Schedule
{
public:
    //TODO : more kinds of constructors to construct different Schedules from one Pattern
    Schedule(const Pattern& pattern, bool& is_valid, bool use_performance_modeling, int dataset_vertex_size, int dataset_edge_size);
    ~Schedule();
    inline int get_total_prefix_num() const { return total_prefix_num;}
    inline int get_father_prefix_id(int prefix_id) const { return father_prefix_id[prefix_id];}
    inline int get_loop_set_prefix_id(int loop) const { return loop_set_prefix_id[loop];}
    inline int get_size() const { return size;}
    inline int get_last(int i) const { return last[i];}
    inline int get_next(int i) const { return next[i];}
    void add_restrict(const std::vector< std::pair<int, int> >& restricts);
    inline int get_total_restrict_num() const { return total_restrict_num;}
    inline int get_restrict_last(int i) const { return restrict_last[i];}
    inline int get_restrict_next(int i) const { return restrict_next[i];}
    inline int get_restrict_index(int i) const { return restrict_index[i];}
    int aggresive_optimize(const int *adj_mat, std::vector< std::pair<int,int> >& ordered_pairs) const;
    std::vector< std::vector<int> > get_isomorphism_vec(const int *adj_mat) const;
    static std::vector< std::vector<int> > calc_permutation_group(const std::vector<int> vec, int size);
    inline const int* get_adj_mat_ptr() const {return adj_mat;}
private:
    int* adj_mat;
    int* father_prefix_id;
    int* last;
    int* next;
    int* loop_set_prefix_id;
    Prefix* prefix;
    int* restrict_last;
    int* restrict_next;
    int* restrict_index;
    int size;
    int total_prefix_num;
    int total_restrict_num;
    void build_loop_invariant();
    int find_father_prefix(int data_size, const int* data);
    void get_full_permutation(std::vector< std::vector<int> >& vec, bool use[], std::vector<int> tmp_vec, int depth) const;
    void performance_modeling(const int *adj_mat, int* best_order, int dataset_vertex_size, int dataset_edge_size);
};