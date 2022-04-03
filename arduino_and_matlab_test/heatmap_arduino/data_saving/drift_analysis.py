# Analyzes the mean values over time, min and max pressure values at 1 node on the grid
# Can run this experiment at various points on the grid to see the drift pattern by changing NODE 
# Can modify range of experiment to visualize with start_idx and col_idx

import csv
import numpy as np
import matplotlib.pyplot as plt

data_file = 'Jan27_6by6.csv'
NUM_MEASUREMENTS = 36 # number of nodes

mean_array = np.zeros(NUM_MEASUREMENTS) # stores NUM_MEASUREMENTS mean values
min_array = np.zeros(NUM_MEASUREMENTS) # Stores NUM_MEASUREMENTS min values
max_array = np.zeros(NUM_MEASUREMENTS) # Stores NUM_MEASUREMENTS max values
sum_array = np.zeros(NUM_MEASUREMENTS) # Stores NUM_MEASUREMENTS sum
val_array = np.zeros(NUM_MEASUREMENTS) # Stores NUM_MEASUREMENTS sum
time_list = list()

# Can change these indices to indicate what time of the experiment want to monitor
start_idx = 0
end_idx = 15

# Node to analyze. Note this ranges from 0 - 35 based on input data given. Need to map 6x6 grid to a value in this range.
NODE = 30

with open(data_file) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    row_idx = 0
    col_idx = 0

    fig = plt.figure()
    fig.suptitle('Mean of pressure measurement over time')
    mean_list = list() # store mean values to plot over time
    val_list = list() # store mean values to plot over time
    min = 0 # min value
    max = 0 # max value


    for row in csv_reader:
        time_entry = True
        for col in row:
            if time_entry:
                # Time entries in first column of each row in data set
                time_entry = False
                time_list.append(col)
            else:
                # Set min array
                if row_idx == 0:
                    # first time initialize min array to whatever is in first data set
                    min_array[col_idx] = col
                else:
                    if int(col) < min_array[col_idx]:
                        min_array[col_idx] = int(col)
                # Set max array
                if int(col) > max_array[col_idx]:
                    max_array[col_idx] = int(col)
                # Set sum array
                sum_array[col_idx] += int(col)
                # Calculate mean array
                mean_array[col_idx] = sum_array[col_idx]/ (row_idx + 1) # row_idx starts from 0
                
                val_array[col_idx] = int(col)


                # Plotting at NODE
                if col_idx == NODE:
                    # mean_list.append(mean_array[col_idx]) # Appending mean values calculated overtime at 1 node
                    val_list.append(val_array[col_idx]) # Appending mean values calculated overtime at 1 node

                    min = min_array[col_idx]
                    max = max_array[col_idx]

                col_idx +=1

        col_idx = 0 # reset column index for next data set
        row_idx +=1
    
    if end_idx > row_idx: # make sure user isn't asking for an end index out of scope
        print("Error! Requesting end index out of bounds")
    else:
        #plt.scatter(time_list[start_idx:end_idx],mean_list[start_idx:end_idx]) # Plot specified range
        # plt.scatter(time_list,mean_list) # Plot specified range
        plt.scatter(time_list,val_list) # Plot specified range

    plt.show()
    print("Min data point over entire cycle: " + str(min))
    print("Max data point over entire cycle: " + str(max))

    print(f'Processed {row_idx*2} minutes of data.') # assume that new data is collected every 30 seconds

