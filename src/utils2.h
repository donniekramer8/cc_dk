#ifndef EXAMPLES_UTILS_2_H_
#define EXAMPLES_UTILS_2_H_

#include <fstream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

class Point3f
{
  public:
  Point3f(int x, int y, int z)
      : x(x), y(y), z(z)
  {
  }

  int x, y, z;
};


template <typename PointT, typename ContainerT>
void readPoints_nn(const std::vector<std::vector<int>>& coords, ContainerT& points)
{
  for (const std::vector<int>& row : coords)
  {
    for (size_t i = 0; i < row.size(); i += 3)
    {
      if (i + 2 < row.size())
      {
        int x = row[i];
        int y = row[i + 1];
        int z = row[i + 2];
        points.push_back(PointT(x, y, z));
      }
      else
      {
        std::cerr << "Warning: Incomplete row found in coords." << std::endl;
      }
    }
  }
}

#endif /* EXAMPLES_UTILS_2_H_ */
