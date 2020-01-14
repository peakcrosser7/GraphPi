#include "../include/dataloader.h"
#include "../include/graph.h"
#include "../include/vertex_set.h"
#include <cstdlib>
#include <cstring>

bool DataLoader::load_data(Graph* &g, DataType type, const char* path, int oriented_type) {
    if(type == Patents || type == Orkut || type == complete8 || type == LiveJournal) {
        if (freopen(path, "r", stdin) == NULL)
        {
            printf("File not found. %s\n", path);
            return false;
        }
        printf("Load begin in %s\n",path);
        g = new Graph();
        scanf("%d%d",&g->v_cnt,&g->e_cnt);
        int* degree = new int[g->v_cnt];
        memset(degree, 0, g->v_cnt * sizeof(int));
        g->e_cnt *= 2;
        std::pair<int,int> *e = new std::pair<int,int>[g->e_cnt];
        id.clear();
        int x,y;
        int tmp_v,tmp_e;
        tmp_v = 0;
        tmp_e = 0;
        while(scanf("%d%d",&x,&y)!=EOF) {
            if(x == y) {
                printf("find self circle\n");
                g->e_cnt -=2;
                continue;
                //return false;
            }
            if(!id.count(x)) id[x] = tmp_v ++;
            if(!id.count(y)) id[y] = tmp_v ++;
            x = id[x];
            y = id[y];
            e[tmp_e++] = std::make_pair(x,y);
            e[tmp_e++] = std::make_pair(y,x);
            ++degree[x];
            ++degree[y];
            if(tmp_e % 1000000 == 0) printf("load %d edges\n",tmp_e);
        }
        if ( oriented_type != 0 ) {
            std::pair<int,int> *rank = new std::pair<int,int>[g->v_cnt];
            int *new_id = new int[g->v_cnt];
            for(int i = 0; i < g->v_cnt; ++i) rank[i] = std::make_pair(i,degree[i]);
            if( oriented_type == 1) std::sort(rank, rank + g->v_cnt, cmp_degree_gt);
            if( oriented_type == 2) std::sort(rank, rank + g->v_cnt, cmp_degree_lt);
            for(int i = 0; i < g->v_cnt; ++i) new_id[rank[i].first] = i;
//            for(int i = 0; i < g->v_cnt; ++i) 
//                if( rank[i].second < rank[i + 1].second ) puts("wrong");
            for(int i = 0; i < g->e_cnt; ++i) {
                e[i].first = new_id[e[i].first];
                e[i].second = new_id[e[i].second];
            }
            delete[] rank;
            delete[] new_id;
        }
        std::sort(degree, degree + g->v_cnt);
        // The max size of intersections is the second largest degree.
        VertexSet::max_intersection_size = degree[g->v_cnt - 2];
        delete[] degree;
        if(tmp_v != g->v_cnt) {
            printf("vertex number error!\n");
        }
        if(tmp_e != g->e_cnt) {
            printf("edge number error!\n");
        }
        if(tmp_v != g->v_cnt || tmp_e != g->e_cnt) {
            fclose(stdin);
            delete g;
            delete[] e;
            return false;
        }
        std::sort(e,e+tmp_e,cmp_pair);
        g->e_cnt = unique(e,e+tmp_e) - e;
        for(int i = 0; i < g->e_cnt - 1; ++i)
            if(e[i] == e[i+1]) {
                printf("have same edge\n");
                fclose(stdin);
                delete g;
                delete[] e;
                return false;
            }
        g->edge = new int[g->e_cnt];
        g->vertex = new int[g->v_cnt + 1];
        int lst_v = -1;
        for(int i = 0; i < g->v_cnt; ++i) g->vertex[i] = -1;
        for(int i = 0; i < g->e_cnt; ++i) {
            if(e[i].first != lst_v)
                g->vertex[e[i].first] = i;
            lst_v = e[i].first;
            g->edge[i] = e[i].second;
        }
        delete[] e;
        printf("Success! There are %d nodes and %d edges.\n",g->v_cnt,g->e_cnt);
        if(g->vertex[g->v_cnt - 1] == -1)
            g->vertex[g->v_cnt - 1] = g->e_cnt;
        g->vertex[g->v_cnt] = g->e_cnt;
        for(int i = g->v_cnt - 2; i >= 0; --i)
            if(g->vertex[i] == -1) {
                g->vertex[i] = g->vertex[i+1];
            }
        return true;
    }
    printf("invalid DataType!\n");
    return false;
}

bool DataLoader::cmp_pair(std::pair<int,int>a, std::pair<int,int>b) {
    return a.first < b.first || (a.first == b.first && a.second < b.second);
}

bool DataLoader::cmp_degree_gt(std::pair<int,int> a,std::pair<int,int> b) {
    return a.second > b.second;
}

bool DataLoader::cmp_degree_lt(std::pair<int,int> a,std::pair<int,int> b) {
    return a.second < b.second;
}