#ifndef MYLIBNN_H
#define MYLIBNN_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include "Octree.hpp"
#include "utils2.h"
#include <time.h>
#include <thread>
#include <cmath>

class Tree
{
  std::vector<std::vector<int>> vol_cell;
  std::vector<int> vol_final;
  std::vector<int> vol_clas;
  std::vector<Point3f> points;
  unibn::Octree<Point3f> octree;
  int num_rows;
  int num_columns;
  

public:
  std::vector<std::vector<uint16_t>> return_matrix;

  Tree(std::vector<std::vector<int>> input_vol_cell, std::vector<int> input_vol_clas, int rows, int columns)
      : vol_cell(input_vol_cell), vol_clas(input_vol_clas), num_rows(rows), num_columns(columns)
  {
    unibn::OctreeParams params;
    readPoints_nn<Point3f>(vol_cell, points);
    octree.initialize(points);

    return_matrix = std::vector<std::vector<uint16_t>>(num_rows, std::vector<uint16_t>(num_columns));
  }


  void print_num_points()
  {
    // just to show bucket size:
    unibn::OctreeParams b_size {};
    std::cout << "Bucket size = " << b_size.bucketSize << '\n';
  }

  void getNeighbors(double radius)
  {
    std::vector<uint32_t> results;

    uint32_t one_percent = (static_cast<double>(points.size()) / 100.);
    if (one_percent == 0)
    {
        one_percent = points.size();  // exception handling
    }

    int64_t begin = clock();

    if (points.size() != 0)
    {
        for (uint32_t i = 0; i < points.size(); ++i)
        {
            if ((i+1) % one_percent == 0)
            {
                int64_t cur_time = clock();
                double search_time = ((double)(cur_time - begin) / CLOCKS_PER_SEC);
                std::cout << (i+1)/one_percent << "% complete, took " << search_time << " seconds." << std::endl;
            }

            const Point3f& p = points[i];

            octree.radiusNeighbors<unibn::L2Distance<Point3f>>(p, radius, results);

            
            // std::cout << "mid: " << i << std::endl;
            if (results.size() != 0)
            {
                for (uint16_t j = 0; j < results.size(); ++j)
                {
                    // std::cout << "j start: " << j << std::endl;
                    auto index = results[j];
                    int q_clas = vol_clas[index];
                    return_matrix[i][q_clas] += 1;
                    // std::cout << "j end: " << j << std::endl;
                }
            }
            // std::cout << "end: " << i << std::endl;
        }
    }
  }
};

#endif // MYLIBNN_H
