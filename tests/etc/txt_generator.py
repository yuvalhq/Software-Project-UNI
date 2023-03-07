import random
import os
import numpy as np

N = 10

def create_random_mats(create_new):
    for k in range(1, N+1):
        r = random.randint(5, 50)
        c = random.randint(3, 6)
        file_name = "testfiles/test" + str(k) + ".txt"
        if os.path.isfile(file_name):
            if create_new:
                os.remove(file_name)
            else:
                return
        
        with open(file_name, "w") as f:
            for i in range(r):
                for j in range(c):
                    number = random.gauss(0, 0.4) 
                    while number < -11 or number > 11:
                        number = random.gauss(0, 0.4)
                    val = str(format(number, '.4f'))
                    if j + 1 == c:
                       val = val + "\n"
                    else:
                       val = val + ","
                    f.write(val)


def create_random_symmetric_mats(create_new):
    for k in range(1, N+1):
        n = random.randint(3, 12)
        tri = np.random.uniform(low=-300.0, high=300.0, size=(n, n))
        mat = tri + tri.T - np.diag(tri.diagonal())
        
        file_name = "testfiles/test" + str(k) + "_j.txt"
        if os.path.isfile(file_name):
            if create_new:
                os.remove(file_name)
            else:
                return
        
        with open(file_name, "w") as f:
            for i in range(n):
                for j in range(n):
                    val = str(format(mat[i][j], '.4f'))
                    if j + 1 == n:
                       val = val + "\n"
                    else:
                       val = val + ","
                    f.write(val)
    
    

if __name__ == "__main__":
    create_random_mats(True)  
    #create_random_symmetric_mats(True)      