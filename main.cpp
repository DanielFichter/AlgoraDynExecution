#include "algorithm.reachability.ss.es/simpleestree.h"
#include "graph.incidencelist/incidencelistgraph.h"
#include "graph.incidencelist/incidencelistvertex.h"

#include <iostream>

int main()
{
    using namespace Algora;

    SimpleESTree<false> algorithm; 

    IncidenceListGraph graph;
    const auto source = graph.addVertex();
    const auto secondVertex = graph.addVertex();
    const auto thirdVertex = graph.addVertex();
    const auto fourthVertex = graph.addVertex();
    const auto fifthVertex = graph.addVertex();
    const auto firstArc = graph.addArc(source, secondVertex);
    const auto secondArc = graph.addArc(secondVertex, thirdVertex);
    const auto thirdArc = graph.addArc(source, fourthVertex);
    const auto fourthArc = graph.addArc(fifthVertex, source);
    const auto fifthArc = graph.addArc(fifthVertex, fourthVertex);
    algorithm.setAutoUpdate(true);
    algorithm.setGraph(&graph);
    algorithm.setSource(source);
    
    std::cout << std::boolalpha;
    std::cout << "Algorithm is perpared: " << algorithm.prepare() << std::endl;
    
    for (size_t vertexIndex = 0; vertexIndex < graph.getSize(); vertexIndex++) {
        auto vertex = dynamic_cast<Vertex*>(graph.vertexAt(vertexIndex));
        std::cout << "Vertex " << vertexIndex << " is reachable: " << algorithm.query(vertex) << std::endl;
    }

    algorithm.unsetGraph();

    return 0;
}