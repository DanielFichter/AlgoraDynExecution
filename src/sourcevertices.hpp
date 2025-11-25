#pragma once

#include <graph.dyn/dynamicdigraph.h>

#include <map>
#include <string>
#include <vector>

using namespace Algora;

const std::map<std::string, std::vector<DynamicDiGraph::VertexIdentifier>>
    sourceVertexIds{
        {"kronecker/csize/answers",
         {0, 2, 8, 16, 64, 256, 2048, 8192, 32768, 65536}},
         {"kronecker/csize/as-newman", {0,2,4,16,32,128,256,512,8192,16384}},
         {"kronecker/csize/as-routeviews", {0,2,8,32,128,256,512,4096,8192,65536}},
         {"kronecker/csize/atp-gr-qc", {0,4,12,34,512,24576,32768,33920,34304,66560}},
         {"kronecker/csize/bio-proteins", {0,1,4,32,128,256,1024,2048,4096,32768}},
         {"kronecker/csize/blog-nat05-6m", {0,1,4,64,128,256,512,16384,32768,65536}},
         {"kronecker/csize/blog-nat06all", {0,1,2,8,16,64,512,4096,8192,65536}},
         {"kronecker/csize/ca-dblp", {0,2,4,24,65,2048,9216,16384,32768,65536}},
         {"kronecker/csize/ca-gr-qc", {0,2,8,16,72,4096,16384,32786,65536,65537}},
         {"kronecker/csize/ca-hep-ph", {0,2,4,8,16,64,256,512,2048,65536}},
         {"kronecker/csize/ca-hep-th", {0,1,8,32,40,65,128,256,512,1024}},
         {"kronecker/csize/cit-hep-ph", {0,2,4,8,64,2048,8192,16384,32768,65536}},
         {"kronecker/csize/cit-hep-th", {0,1,32,256,1024,2048,4096,8192,16384,65536}},
         {"kronecker/csize/delicious", {0,1,4,8,128,256,512,8192,32768,65536}},
         {"kronecker/csize/email-inside", {0,1,4,16,64,1024,4096,8192,16384,32768}},
         {"kronecker/csize/epinions", {0,1,4,8,64,512,2048,4096,16384,32768}},
         {"kronecker/csize/flickr", {0,2,32,64,128,1024,2048,8192,32768,65536}},
         {"kronecker/csize/gnutella-25", {0,1,2,18,20,32,64,128,2048,5120}},
         {"kronecker/csize/gnutella-30", {0,2,64,128,520,8200,16384,32768,40960,65536}},
         {"kronecker/csize/web-notredame", {0,1,4,64,256,512,4096,8192,32768,65536}},

         {"kronecker/growing/answers", {0,2,8,9,10,12,16,17,19,29}},
         {"kronecker/growing/as-newman", {0,1,2,4,6,7,8,9,16,20}},
         {"kronecker/growing/as-routeviews", {0,1,2,5,8,10,12,16,20,24}},
         {"kronecker/growing/atp-gr-qc", {5,8,9,10,17,18,21,23,24,31}},
         {"kronecker/growing/bio-proteins", {0,1,2,4,5,6,7,8,12,16}},
         {"kronecker/growing/blog-nat05-6m", {0,1,2,5,10,12,16,17,19,20}},
         {"kronecker/growing/blog-nat06all", {0,2,3,4,7,8,9,16,17,18}},
         {"kronecker/growing/ca-dblp", {0,2,4,5,8,9,10,16,17,24}},
         {"kronecker/growing/ca-gr-qc", {0,3,4,5,6,8,14,26,28,30}},
         {"kronecker/growing/ca-hep-ph", {0,1,2,3,4,5,9,16,26,31}},
         {"kronecker/growing/ca-hep-th", {0,1,3,4,7,8,12,17,19,30}},
         {"kronecker/growing/cit-hep-ph", {0,1,2,4,5,6,8,16,26,30}},
         {"kronecker/growing/cit-hep-th", {0,1,3,8,9,12,17,20,24,28}},
         {"kronecker/growing/delicious", {0,2,4,5,8,9,10,17,18,20}},
         {"kronecker/growing/email-inside", {0,1,2,4,5,8,16,18,20,24}},
         {"kronecker/growing/epinions", {0,1,2,4,5,6,8,10,15,24}},
         {"kronecker/growing/flickr", {0,1,2,3,4,12,16,17,20,24}},
         {"kronecker/growing/gnutella-25", {0,2,3,8,11,16,17,19,20,28}},
         {"kronecker/growing/gnutella-30", {2,3,4,8,13,16,20,21,28,31}},
         {"kronecker/growing/web-notredame", {0,2,4,5,6,8,16,17,19,20}},

         {"snap/as-caida", {209,701,702,1239,2914,3356,3549,3561,6461,7018}}
    };