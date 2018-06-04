from plot_membership import *
import numpy as np
from matplotlib import pyplot as plt

if __name__ == '__main__':
    xs = np.linspace(-15, 45, num=100)

    low = [1-Gamma(-5, 10).membership(x) for x in xs]
    med = [Lambda(5, 25, 30).membership(x) for x in xs]
    high = [Gamma(25, 35).membership(x) for x in xs]

    f = plt.figure()
    plt.plot(xs, low, label='niska')
    plt.plot(xs, med, label='srednja')
    plt.plot(xs, high, label='visoka')
    plt.title('Temperatura zraka')
    plt.xlabel('x')
    plt.ylabel('$\mu(x)$')
    axes = plt.gca()
    axes.set_xlim([-15, 45])
    axes.set_ylim([-0.1,1.1])
    axes.legend()

    f.savefig('lang_var.pdf') 

    f = plt.figure()
    plt.plot(xs, high, label='visoka')
    plt.title('Visoka temperatura zraka')
    plt.xlabel('x')
    plt.ylabel('$\mu(x)$')
    axes = plt.gca()
    axes.set_xlim([-15, 45])
    axes.set_ylim([-0.1,1.1])

    f.savefig('lang_var_hot.pdf') 
