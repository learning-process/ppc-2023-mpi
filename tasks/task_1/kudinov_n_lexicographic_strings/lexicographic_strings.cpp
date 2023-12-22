// Copyright 2023 Kudinov Nikita
#include "task_1/kudinov_n_lexicographic_strings/lexicographic_strings.h"

#include <vector>
#include <utility>
#include <functional>
#include <algorithm>

#include <boost/mpi.hpp>
#include <boost/serialization/utility.hpp>

bool are_strings_lexicographically_ordered_sequential(const std::string& left, const std::string& right) {
    return left <= right;
}

bool are_strings_lexicographically_ordered_parallel(const std::string& left, const std::string& right) {
    boost::mpi::communicator world;

    std::vector<std::pair<std::string, std::string>> segments;
    std::pair<std::string, std::string> local_segment;
    if (world.rank() == 0) {
        std::size_t segment_size = std::min(left.size(), right.size()) / world.size();

        int i;
        std::size_t segment_position;
        for (i = 0, segment_position = 0; i < world.size() - 1; i += 1, segment_position += segment_size) {
            segments.push_back(std::make_pair(
                left.substr(segment_position, segment_size),
                right.substr(segment_position, segment_size)));
        }

        segments.push_back(std::make_pair(
            left.substr(segment_position, left.size() - segment_position),
            right.substr(segment_position, right.size() - segment_position)));
    }

    boost::mpi::scatter(world, segments, local_segment, 0);

    bool local_result = are_strings_lexicographically_ordered_sequential(local_segment.first, local_segment.second);

    bool global_result = true;
    boost::mpi::reduce(world, local_result, global_result, std::logical_and<bool>(), 0);

    return global_result;
}
