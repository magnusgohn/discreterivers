#ifndef GRAPH_TOOLS_H
#define GRAPH_TOOLS_H

#include <queue>
#include <assert.h>
#include <functional>

namespace graphtools
{

template<class node_type, class index_type>
class graph
{
public:
    graph(const std::vector<node_type> &_nodes,
          const std::vector<std::vector<index_type>> &_adjacency) :
        nodes(_nodes), adjacency(_adjacency) {} // makes copies of the lists

    template<class heuristic_type>
    struct compare_functor;

    template<class heuristic_type>
    class iterator
    {
    public:
        iterator(index_type _index, heuristic_type _heuristic,
                 const std::vector<node_type> &_nodes, const std::vector<std::vector<index_type>> &_adjacency) :
            index(_index), heuristic(_heuristic), nodes(_nodes), adjacency(_adjacency),
            open_points_queue(compare_functor<heuristic_type>(_heuristic))
        {
            already_visited = std::vector<bool>(_nodes.size(), false);

            // start;
            visit(_index);
            ++(*this);
        }

        float heuristic_eval()
        {
           if (open_points_queue.empty())
           {
               return std::numeric_limits<float>::max();
           }
           else
           {
               heuristic(index);
           }
        }

        bool search_end() {return open_points_queue.empty();}

        iterator& operator++()
        {
            assert((!open_points_queue.empty()));

            // actual increment takes place here
//            if (!open_points_queue.empty())
//            {

                index = open_points_queue.top();
                open_points_queue.pop();

                for (auto const &i_adj : adjacency[int(index)])
                {
                    if (already_visited[int(i_adj)]==false)
                    {
                        visit(i_adj);
                    }
                }
//            }
            return *this;
        }

        // Iterator copying is expensive, therefore avoided
        // iterator& operator++(int)

        const node_type& operator*() const { return nodes[int(index)]; }
        const node_type* operator->() const { return &nodes[int(index)]; }

        const index_type get_index() const { return index; }
        const index_type get_parent_index() const { return parent_index; }

    private:
        std::priority_queue<index_type, std::vector<index_type>, compare_functor<heuristic_type>> open_points_queue;

        heuristic_type heuristic;

        index_type index;

        const std::vector<node_type> &nodes;
        const std::vector<std::vector<index_type>> &adjacency; // maps to nodes

        std::vector<bool> already_visited; // maps to nodes

        // next up
        index_type parent_index;
        //size_t distance;

        void visit(index_type _index)
        {
            already_visited[int(_index)] = true;
            open_points_queue.push(_index);
        }
    };

    // second order compare function created from the heuristic function
    template<class heuristic_type>
    struct compare_functor
    {
        compare_functor(heuristic_type _heuristic) : heuristic(_heuristic) {}
        bool operator()(index_type i1, index_type i2) {return heuristic(i1) > heuristic(i2);}

    private:
        heuristic_type heuristic;
    };

    template<class heuristic_type>
    iterator<heuristic_type> search(  index_type start_index,
                                      heuristic_type heuristic)
    {

        return iterator<heuristic_type>(start_index, heuristic,
                                        nodes, adjacency);
    }


private:
    const std::vector<node_type> nodes;
    const std::vector<std::vector<index_type>> adjacency;
};

template<class node_type, class index_type>
graph<node_type, index_type> make_graph(const std::vector<node_type> &_nodes,
                                        const std::vector<std::vector<index_type>> &_adjacency)
{
    return graph<node_type, index_type>(_nodes, _adjacency);
}

} // namespace graphtools

#endif // GRAPH_TOOLS_H
