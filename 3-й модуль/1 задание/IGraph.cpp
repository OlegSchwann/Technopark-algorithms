//
// Created by oleg on 25.05.18.
//

#include "IGraph.hpp"
#include <boost/range/irange.hpp>

// вывод графа в виде строки [1 ⟶ 2, 2 ⟶ 4].
std::ostream &operator<<(std::ostream &os, const IGraph &graph) {
    os << "[";
    for (int from : boost::irange(0, (int)graph.VerticesCount())){
        for(int to : graph.GetNextVertices(from)){
            os << from << " ⟶ " << to << ", ";
        }
    }
    os << "\b\b]";
    return os;
};