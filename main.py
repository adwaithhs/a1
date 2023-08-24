import numpy as np

n = 2**20

with open("f3", "w") as f:
    f.write(str(n)+"\n")
    f.write("16\n")
    for i in range(n):
        a = np.random.randint(0, 2**32)
        f.write(str(a)+"\n")
