from plot_membership import *
import numpy as np
from matplotlib import pyplot as plt

if __name__ == '__main__':
    xs = np.linspace(-15, 45, num=1000)

    Pi(-1.58, -0.70, 0.18, 1.05)
    Pi(-0.18, 1.05, 1.93, 2.81)
    Pi(1.93, 2.81, 3.69, 4.57)
    Pi(3.69, 4.57, 5.44, 6.32)
    Pi(5.44, 6.32, 7.20, 8.08)
    Pi(7.20, 8.08, 8.96, 9.83)
    Pi(8.96, 9.83, 10.71, 11.58)

    f = plt.figure()
    plt.plot(xs, [Pi(-1.58, -0.70, 0.18, 1.05).membership(x) for x in xs], label='$L_0$')
    plt.plot(xs, [Pi(-0.18, 1.05, 1.93, 2.81).membership(x) for x in xs], label='$L_1$')
    plt.plot(xs, [Pi(1.93, 2.81, 3.69, 4.57).membership(x) for x in xs], label='$L_2$')
    plt.plot(xs, [Pi(3.69, 4.57, 5.44, 6.32).membership(x) for x in xs], label='$L_3$')
    plt.plot(xs, [Pi(5.44, 6.32, 7.20, 8.08).membership(x) for x in xs], label='$L_4$')
    plt.plot(xs, [Pi(7.20, 8.08, 8.96, 9.83).membership(x) for x in xs], label='$L_5$')
    plt.plot(xs, [Pi(8.96, 9.83, 10.71, 11.58).membership(x) for x in xs], label='$L_6$')
    plt.title('Primjer varijable')
    plt.xlabel('x')
    plt.ylabel('$\mu(x)$')
    axes = plt.gca()
    axes.set_xlim([-2, 12])
    axes.set_ylim([-0.1,1.1])
    axes.legend()

    f.savefig('lang_var_example.pdf') 
