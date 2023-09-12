import numpy as np
from sklearn.neighbors import KDTree
import h5py
import pickle
import time
import math
import threading
from MyLibNN import Tree


print("begin")

test = Tree([[0,0,0],[0,0,0]], [0,0], 3, 3)
test.print_num_points()


# Open mat file
# mat_file = h5py.File(r"\\fatherserverdw\andreex\data\monkey fetus\gestational 40\mat files\vol_all_combined_8_24_2023.mat", 'r')
mat_file = h5py.File("/home/donald/Desktop/vol_all_combined_8_24_2023.mat")

# Access the variables from file
volcell = mat_file['volcell']
volfinal = mat_file['volfinal']

volcell_np = np.array(volcell)
volfinal_np = np.array(volfinal)
max_z, max_y, max_x = volcell_np.shape

nonzero_indices = np.nonzero(volcell_np)

values = volcell_np[nonzero_indices]
classifications = volfinal_np[nonzero_indices].astype(int)

points_np = np.column_stack((nonzero_indices[0], nonzero_indices[1], nonzero_indices[2], classifications)) # try dataframe df

condition_mask = (points_np[:, -1] != 0) & (points_np[:, -1] != 14)
filtered_points_np = points_np[condition_mask]

num_classes = len(np.unique(classifications))
num_cells = len(filtered_points_np)

print(num_cells)
print(num_classes)

del volcell
del volfinal
del mat_file
del volcell_np
del volfinal_np
del classifications
del nonzero_indices
del values
del points_np
del condition_mask

proprotion = 128

half_rows = math.floor(num_cells/proprotion)

rows = half_rows
cols = num_classes

start = time.time()

print('Building  Tree...')

cpp_tree = Tree(filtered_points_np[:half_rows, :3], filtered_points_np[:half_rows, -1], rows, cols)

mid = time.time()
print(f'Building Tree took: {(mid-start)/60} minutes')

radius = 30

print("starting nn_search")

n_threads = 4
break0 = 0
break1 = math.floor(rows/n_threads)
break2 = math.floor(2*rows/n_threads)
break3 = math.floor(3*rows/n_threads)
end = rows

def get_neighbors_thread(radius, start, end):
    cpp_tree.getNeighbors(radius, start, end)

get_neighbors_thread(radius, break0, break1)

t1 = threading.Thread(target=get_neighbors_thread, args=(radius, break0, break1))
t2 = threading.Thread(target=get_neighbors_thread, args=(radius, break1, break2))
t3 = threading.Thread(target=get_neighbors_thread, args=(radius, break2, break3))
t4 = threading.Thread(target=get_neighbors_thread, args=(radius, break3, end))

t1.start()
t2.start()
t3.start()
t4.start()
print("started all threads")

t1.join()
t2.join()
t3.join()
t4.join()

print('Done')
print('Writing pickle file...')

file_name = 'first_half_monkey_30.pickle'

# Open the file in binary write mode and save the data
with open(file_name, 'wb') as file:
    pickle.dump(cpp_tree.return_matrix, file)
    