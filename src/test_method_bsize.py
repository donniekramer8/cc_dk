import numpy as np
from sklearn.neighbors import KDTree
import h5py
import pickle
import time
import os

from MyLibNN import Tree



test = Tree([[0,0,0],[0,0,0]], [0,0], 3, 3)
test.print_num_points()