from MyLibNN import Tree



import numpy as np

vol_cell = np.zeros((1000, 3)).astype(int)
vol_clas = np.zeros((1000, 1)).astype(int)
# print(vol_cell)

rows, cols = vol_cell.shape

cpp_tree = Tree(vol_cell, vol_clas, rows, cols+1)
cpp_tree.print_num_points()

radius = 1
results = cpp_tree.getNeighbors(radius)

# Print the results or perform further operations as needed
# print(results)
