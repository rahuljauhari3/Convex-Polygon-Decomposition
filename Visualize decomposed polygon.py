import matplotlib.pyplot as plt

file = open("output.txt", "r")

X = []
Y = []
flag = 1

while flag != 0:
    coord = []
    while(1):
        line = file.readline()
        if line == "\n":
            break
        if line == "":
            flag = 0
            break

        coord.append(line.split())

    if flag == 0:
        break
    coord.append(coord[0])

    xs, ys = zip(*coord)
    X.append(list(map(int, xs)))
    Y.append(list(map(int, ys)))

plt.figure()
for i in range(0, len(X)):
    plt.plot(X[i], Y[i])
plt.title("Convex Hull")
for i in range(0, len(X)):
    for j in range(0, len(X[i])):
        plt.annotate(f'{X[i][j]},{Y[i][j]}', (X[i][j], Y[i][j]), textcoords="offset points", xytext=(0,10), ha='center')
plt.show()
