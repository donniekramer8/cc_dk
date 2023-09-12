#ifndef MYLIBNN_H
#define MYLIBNN_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include "Octree.hpp"
#include "utils2.h"
#include <time.h>
#include <thread>

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
  std::vector<std::vector<u_int16_t>> return_matrix;

  Tree(std::vector<std::vector<int>> input_vol_cell, std::vector<int> input_vol_clas, int rows, int columns)
      : vol_cell(input_vol_cell), vol_clas(input_vol_clas), num_rows(rows), num_columns(columns)
  {
    unibn::OctreeParams params;
    readPoints_nn<Point3f>(vol_cell, points);
    octree.initialize(points);

    return_matrix = std::vector<std::vector<u_int16_t>>(num_rows, std::vector<u_int16_t>(num_columns));
  }

  void print_num_points()
  {
    // std::cout << points.size() << " points.\n";
    // just to show bucket size:
    unibn::OctreeParams b_size {};
    std::cout << "Bucket size = " << b_size.bucketSize << '\n';
  }

  void getNeighbors(double radius)
  {
    std::vector<uint32_t> results;

    uint32_t one_percent = (static_cast<float>(points.size()) / 100.);
    int64_t begin = clock();

    std::cout << points.size() << " points in tree." << std::endl;

    for (uint32_t i = 0; i < points.size(); ++i)
    {

      if (i % (one_percent) == 0)
      {
        int64_t cur_time = clock();
        double search_time = ((double)(cur_time - begin) / CLOCKS_PER_SEC);
        double percent = static_cast<double>(i)/static_cast<double>(one_percent);
        std::cout << percent << "% complete, took " << search_time << "seconds." << std::endl;
      }

      const Point3f &p = points[i];

      octree.radiusNeighbors<unibn::L2Distance<Point3f>>(p, radius, results);

      for (uint16_t j = 0; j < results.size(); ++j)
      {
        auto index = results[j];
        int q_clas = vol_clas[index];

        return_matrix[i][q_clas] += 1;
      }
    }
  }
};

#endif // MYLIBNN_H
