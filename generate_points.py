## 
# @brief A code to generate a polygon with given number of vertices
#
import sys
import numpy as np
from matplotlib import pyplot as plt


def draw_polygon(ax, n,lst):

    x = np.random.randint(0,1000,n)
    y = np.random.randint(0,1000,n)

    center_point = [np.sum(x)/n, np.sum(y)/n]

    angles = np.arctan2(x-center_point[0],y-center_point[1])
    sort_tups = sorted([(i,j,k) for i,j,k in zip(x,y,angles)], key = lambda t: t[2])

    if len(sort_tups) != len(set(sort_tups)):
        lst=[]
        draw_polygon(ax, n,lst)

    x,y,angles = zip(*sort_tups)
    x = list(x)
    y = list(y)
    x.append(x[0])
    y.append(y[0])

    for i in range(len(x)-1):
        lst.append([x[i],y[i]])
    ax.plot(x,y, label = '{}'.format(n))

if __name__ == '__main__':

    fig,ax = plt.subplots()
    lst=[]
    k = int(sys.argv[1])
    draw_polygon(ax,k,lst)
    print(k)
    for i in lst:
        print(i[0],i[1])

    with open('input.txt', 'w') as f:
        f.write(str(k) + '\n')
        for i in lst:
            f.write(str(i[0]) + ' ' + str(i[1]) + '\n')
    

    ax.legend()
    plt.axis('equal')
    # show coordinates
    for i in range(len(lst)):
        ax.annotate(f'{lst[i][0]},{lst[i][1]}', (lst[i][0], lst[i][1]), textcoords="offset points", xytext=(0,10), ha='center')
    plt.grid()
    plt.show()