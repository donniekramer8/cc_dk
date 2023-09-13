import numpy as np
from sklearn.neighbors import KDTree
import h5py
import pickle
import time
import os
import math
from MyLibNN import Tree
import random

range_points = 100000
rows = range_points
cols = 4

points = np.random.randint(range_points, size=(rows, cols))

# print(points)

points = np.array(points)

coords = points[:, :3].tolist()
classes = points[:,-1].tolist()
#print(coords)
# print(classes)

test = Tree(points[:,:3], points[:,-1], rows, range_points)
# print(test.return_matrix)
test.print_num_points()
test.getNeighbors(1.2)