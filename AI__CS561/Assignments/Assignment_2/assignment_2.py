#============================================================================
# This code is part of Assignment 2 of CS561 (Executive M-Tech -AI Assignment-2)
# Submitted by:
#        IITP001300: Sukhvinder Singh  (email id: sukhvinder.malik13@gmail.com)
#        IITP001316: Manjit Singh Duhan (email id: duhan.manjit@gmail.com)
#        IITP001508: Atul Singh (email id: atulsingh.xcvi@gmail.com)
#============================================================================

import sys
import numpy as np
import heapq
from datetime import datetime

class node_state():
    def __init__(self, matrix, parent=None, move=None):
        self.matrix      = np.array(matrix)
        self.parent      = parent           # The parent to this state. The start state will not have any parrent
        self.move        = move             # What move (Left/Right/Up/Down) has led to this state
        self.max_row     = len(matrix)      # Max rows in the matrix
        self.max_column  = len(matrix[0])   # Max columns in the matrix
        self.hash        = self.get_hash()  # Has of the matrix. Will be used while comparing states.
        self.depth       = self.set_depth() # The depth of this state from the state_state
        self.cost        = self.set_cost()

    
    def get_value(self, i, j):
        ''' Get the value from mattrix at [i][j] location '''
        return self.matrix[i, j]
    
    def get_i_j_of(self, val):
        '''Get the i & j of the value in the matrix '''
        return np.where(self.matrix == val)
    
    def get_copy(self):
        ''' Returns the copy of current matrix '''
        return self.matrix.copy()
    
    def set_depth(self):
        ''' Set the depth of this state. The depth will be parent's depth + 1 '''
        if self.parent is not None:
            return self.parent.get_depth() + 1
        else:
            return 0

    def set_cost(self):
        ''' Set the cost of this state from the root.'''
        if self.parent is not None:
            return self.parent.get_cost() + 1
        else:
            return 0
        
    def get_cost(self):
        ''' Get function to get the cost of the state '''
        return self.cost
        
    def get_depth(self):
        ''' Get function to get the depth of the state '''
        return self.depth

    def __lt__(self, other):
        return self.cost < other.cost
    
    def get_hash(self):
        '''
        Let us create the hash of the matrix so that the we will add the hash of explored state
        in the explored list. Before processing any state, we will check if that state us already
        explored or not.
        To calculate the hash, we are makeing int of the matrix as below
        [[123]
         [456]
         [780]] = 1234567890
        So, every matrix will have a unique hash value
        '''
        multiplier = 100000000
        value = 0
        for i in range(self.max_row):
            for j in range (self.max_column):
                value += self.matrix[i, j] * multiplier
                multiplier //= 10
        return value
    
    def print(self):
        ''' print the matrix '''
        for i in range (self.max_row):
            for j in range (self.max_column):
                val = self.matrix[i, j]
                if val == 0:
                    print("B", end=" ")
                else:
                    print(val, end=" ")
            print(" ")


class Queue():
    def __init__(self):
        self.list = []
        self.max_size = 0

    def current_size(self):
        return len(self.list)
    
    def clear(self):
        self.list.clear()
    
    def max_grown_size(self):
        return self.max_size
    
    def empty(self):
        return self.current_size() == 0

    def add(self, item):
        self.list.append(item)
        self.max_size = max(self.max_size, self.current_size())   

    def remove(self):
        if self.current_size() == 0:
            raise Exception("Queue is empty")
        else:
            return self.list.pop(0)


class Stack(Queue):

    def remove(self):
        if self.current_size() == 0:
            raise Exception("Stack is empty")
        else:
            return self.list.pop(-1)


class Priority_Queue(Queue):

    def add(self, item):
        heapq.heappush(self.list, (item.get_cost(), item))
        self.max_size = max(self.max_size, self.current_size())

    def remove(self):
        if self.current_size() == 0:
            raise Exception("Priority queue is empty")
        cost, item = heapq.heappop(self.list)
        return item

class puzzle():
    def __init__(self, start, goal):
        # Check if the nput Start state and Goal state are valid or not.
        self.validate_input_matrix(start)
        self.validate_input_matrix(goal)
                
        self.start_state   = node_state(start)
        self.goal_state    = node_state(goal)
        self.max_depth     = 0    #This can be used to limit the seach to any depth
        self.max_row, self.max_column = start.shape
        self.explored_list = []   # This list will contain the states that are explored at any moment
        self.solution_steps = []  # This list is used to fill while doing the back-tracing
        self.start_time     = datetime.now()
        self.end_time       = datetime.now()
        self.states_explored = 0  # the number of states explored at any moment of time.
        
    
    def validate_input_matrix(self, matrix):
        ''' Validate the matrix. The matrix should have 0-8 values and no value should be repeated.
            While creating the puzzle object, validation should be done on start_state & goal_state '''
        max_row = len(matrix)
        max_column = len(matrix[0])
        
        for value in range(0, max_row*max_column):
            found = False;
            for i in range(max_row):
                for j in range(max_column):
                    if(value == matrix[i, j]):
                        found = True
            
            if(found == False):
                raise Exception("This is not a valid matrix")

        
    def get_neighbor_states(self, current_state):
        ''' Get the neighbors of the current state. The neighbors will be the states that comes
            after taking Left/Right/Up/Down move.
            No validation is done on the neighbor in this function.
            The called of this function need to do all the required validations'''
        
        move_directions = [("LEFT", 0, -1), ("RIGHT", 0, 1), ("UP", -1, 0), ("DOWN", 1, 0)]
        row, col = current_state.get_i_j_of(B)
        neighbors = []
        
        # if depth is configured then check the depth. If the depth is more then return empty list to the caller 
        if self.max_depth > 0 and current_state.get_depth() >= self.max_depth:
            return neighbors
        
        for move, i_offset, j_offset in move_directions:
            new_i = row + i_offset
            new_j = col + j_offset
            
            # move the blank space Left/Right/Up/Down if it is not crossing the boundries
            if ( 0 <= new_i < self.max_row ) and ( 0 <= new_j < self.max_column) :
                new_state = self.move(current_state, move, row[0], col[0])
                neighbors.append(new_state)

        return neighbors


    def move(self, current_state, move, i, j):
        '''MOve the blank space to Left/Right/Up/Down and return the new state
           The current state will be parent to tthis new state'''
        temp_matrix = current_state.get_copy()
        
        # Store the value of matrix[i, j] in temp varianble. This will be used during swap operation
        temp_val = temp_matrix[i, j]
        
        if move == "RIGHT":
            temp_matrix[i, j] = temp_matrix[i, j+1]
            temp_matrix[i, j+1] = temp_val
        elif move == "LEFT":
            temp_matrix[i, j] = temp_matrix[i, j-1]
            temp_matrix[i, j-1] = temp_val
        elif move == "UP":
            temp_matrix[i, j] = temp_matrix[i-1, j]
            temp_matrix[i-1, j] = temp_val
        elif move == "DOWN":
            temp_matrix[i, j] = temp_matrix[i+1, j]
            temp_matrix[i+1, j] = temp_val

        # Create the state from this new matrix. Assign the current matrix as parent to this new state
        new_state = node_state(temp_matrix, current_state, move)
        return new_state


    def print_solution(self):
        ''' Print the solution '''

        print("\nStart state: ")
        self.start_state.print()

        print("\nGoal state: ")
        self.goal_state.print()

        print("\nSolution steps: \n---------------------")

        while len(self.solution_steps) != 0:
            current_state = self.solution_steps.pop(0)
            print("\nMove: ", current_state.move)
            current_state.print()
    
    def solve(self, my_list, type_of_algorithm):
        '''This fiunction can be used to find the goal state.
            It expect the instance of the Queue/Stack/Priority/Queue in the my_list.
            This function will just call the add & remove function of the list it is
            responsiblity of the called to implement the Queue/Stack/Priority_Queue'''
        
        # Keep looping until solution is not found
        while not my_list.empty():
            # Remove a state from the Queue/Stack
            current_state = my_list.remove()
            self.states_explored += 1
            
            # if hash of current state and Goal state is same then we have reached the goal :D
            if current_state.hash == self.goal_state.hash:
                self.end_time = datetime.now()
                
                item = current_state
                while item.parent is not None:
                    self.solution_steps.insert(0, item)
                    item = item.parent

                return True
            
            # get the valid neighbors of current state. Drop the neighbor that is already explored 
            neighbors = self.get_neighbor_states(current_state)
            for neighbor in neighbors:
                if neighbor.hash not in self.explored_list:
                    self.explored_list.append(neighbor.hash)
                    my_list.add(neighbor)
        
        #Goal state is not found
        return False
    
    def display_statistics(self, type_of_algorithm, solution_found, print_sol):
        self.end_time = datetime.now()
        execution_time = self.end_time - self.start_time
        
        sol_found = " " if solution_found == True else " No "
        
        print(f"{type_of_algorithm}:{sol_found}solution found")
        print(f"{type_of_algorithm}: Time consumed: {execution_time}")
        print(f"{type_of_algorithm}: states explored to reach the goal: {self.states_explored}")

        if (solution_found == True):
            print(f"{type_of_algorithm}: Solution can be reached in steps: {len(self.solution_steps)}")

        if print_sol == True :
            self.print_solution()


class BFS_puzzle_solver(puzzle):
    
    def find_the_solution(self, print_sol):
        my_list = Queue()
        type_of_algorithm = "BFS"

        # Clear the explored list, solution steps
        self.explored_list.clear()
        self.solution_steps.clear()

        my_list.add(self.start_state)
        self.explored_list.append(self.start_state.hash)
        
        print(f"\n{type_of_algorithm}: Started solving puzzle at {self.start_time}")
        
        self.start_time = datetime.now()
        solution_found = self.solve(my_list, type_of_algorithm)
        
        #Display the statistics
        self.display_statistics(type_of_algorithm, solution_found, print_sol)


class DFS_puzzle_solver(puzzle):
    
    def find_the_solution(self, print_sol):
        my_list = Stack()
        type_of_algorithm = "DFS"

        # Clear the explored list, solution steps
        self.explored_list.clear()
        self.solution_steps.clear()

        my_list.add(self.start_state)
        self.explored_list.append(self.start_state.hash)
        
        print(f"\n{type_of_algorithm}: Started solving puzzle at {self.start_time}")
        
        self.start_time = datetime.now()
        solution_found = self.solve(my_list, type_of_algorithm)
            
        self.display_statistics(type_of_algorithm, solution_found, print_sol)

class UCS_puzzle_solver(puzzle):
    
    def find_the_solution(self, print_sol):
        my_list = Priority_Queue()
        type_of_algorithm = "UCS"

        # Clear the explored list, solution steps
        self.explored_list.clear()
        self.solution_steps.clear()

        my_list.add(self.start_state)
        self.explored_list.append(self.start_state.hash)
        
        print(f"\n{type_of_algorithm}: Started solving puzzle at {self.start_time}")
        
        self.start_time = datetime.now()
        solution_found = self.solve(my_list, type_of_algorithm)
            
        self.display_statistics(type_of_algorithm, solution_found, print_sol)


class IDS_puzzle_solver(puzzle):
    
    def find_the_solution(self, print_sol, max_depth):
        
        my_list = Stack()
        type_of_algorithm = "IDS"
        solution_found = False
        self.max_depth = 0
        
        print(f"\n{type_of_algorithm}: Started solving puzzle at {self.start_time}")
        
        self.solution_steps.clear()
        
        self.start_time = datetime.now()
       
        while True:
            # For each iteration increment the depth untill we do not find the solution.
            self.max_depth += 1 
            if(self.max_depth > max_depth and max_depth != 0):
                print("Reached configured Max Depth({max_depth}) but solution not Fount")
                solution_found = False
                break
                
            # Clear the explored list and the priority Queue
            self.explored_list.clear()
            my_list.clear()
            
            # Add the start state in the priority Queue & in explored list
            my_list.add(self.start_state)
            self.explored_list.append(self.start_state.hash)
            
            if self.solve(my_list, type_of_algorithm) == True:
                solution_found = True
                break

        
        self.display_statistics(type_of_algorithm, solution_found, print_sol)      


print("============================================================================")
print("This code is part of Assignment 2 of CS561 (Executive M-Tech -AI Assignment-2)")
print("Submitted by:")
print("\tIITP001300: Sukhvinder Singh  (email id: sukhvinder.malik13@gmail.com)")
print("\tIITP001316: Manjit Singh Duhan (email id: duhan.manjit@gmail.com)")
print("\tIITP001508: Atul Singh (email id: atulsingh.xcvi@gmail.com)")
print("============================================================================\n\n")


B=0  # Just to make the matrix more readable as eyes will easily catch "B" compared to 0.

#take a random array
# random_array = np.arange(9)
# np.random.shuffle(random_array)

# enter the start state manually. (ToDo: If requirement is to generate random matrix, then neew to write a funtion for that)
# start_state = random_array.reshape(3, 3)
start_state =np.array([
       [3, 2, 1],
       [4, 5, 6],
       [8, 7, B]])


# The goal state. This should be the goal where we need to reach
goal_state= np.array([
       [1, 2, 3],
       [4, 5, 6],
       [7, 8, B]])

# Enabled if want to see the solution
print_solution = False

# Configure the max depth. 0 for unlimited. At present used only for IDS
max_depth = 0

print(f"Start state:")
print(start_state)

print(f"\nGoal state:")
print(goal_state)

print(f"\n=============   BFS    =====================")
bfs = BFS_puzzle_solver(start_state, goal_state)
bfs.find_the_solution(print_solution)

print(f"\n=============   DFS    =====================")
dfs = DFS_puzzle_solver(start_state, goal_state)
dfs.find_the_solution(print_solution)

print(f"\n=============   UCS    =====================")
ucs = UCS_puzzle_solver(start_state, goal_state)
ucs.find_the_solution(print_solution)

print(f"\n=============   IDS    =====================")
ids = IDS_puzzle_solver(start_state, goal_state)
ids.find_the_solution(print_solution, max_depth)



