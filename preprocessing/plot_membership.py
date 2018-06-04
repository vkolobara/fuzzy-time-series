import numpy as np
class MembershipFunction(object):
    def membership(self, x):
        pass

class Lambda(MembershipFunction):
    def __init__(self, a, b, c):
        self.a = a
        self.b = b
        self.c = c

    def membership(self, x):
        if x < self.a:
            return 0
        elif x >= self.a and x < self.b:
            return (x - self.a) / (self.b - self.a)
        elif x >= self.b and x < self.c:
            return (self.c - x) / (self.c - self.b)
        else:
            return 0

class Gamma(MembershipFunction):
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def membership(self, x):
        if x < self.a:
            return 0
        elif x >= self.a and x < self.b:
            return (x - self.a) / (self.b - self.a)
        else:
            return 1

class Pi(MembershipFunction):
    def __init__(self, a, b, c, d):
        self.a = a
        self.b = b
        self.c = c
        self.d = d

    def membership(self, x):
        if x < self.a:
            return 0
        elif x >= self.a and x < self.b:
            return (x - self.a) / (self.b - self.a)
        elif x >= self.b and x < self.c:
            return 1
        elif x >= self.c and x < self.d:
            return (self.d - x) / (self.d - self.c)
        else:
            return 0

class Sigmoid(MembershipFunction):
    def __init__(self, a, c):
        self.a = a
        self.c = c

    def membership(self, x):
        return 1 / (1 + np.exp(-self.a * (x - self.c)))

class Gauss(MembershipFunction):
    def __init__(self, mu, sigma):
        self.mu = mu
        self.sigma = sigma

    def membership(self, x):
        return np.exp(-((x-self.mu)/self.sigma)**2)

def save_plot(xs,ys,file_name):
    from matplotlib import pyplot as plt
    f = plt.figure()
    plt.plot(xs, ys)
    plt.xlabel('x')
    plt.ylabel('$\mu(x)$')
    axes = plt.gca()
    axes = plt.gca()
    axes.set_xlim([0,100])
    axes.set_ylim([-0.1,1.1])
    axes.set_xlim([0,100])
    axes.set_ylim([-0.1,1.1])

    f.savefig(file_name)


if __name__ == '__main__':

    lambdaf  = Lambda(20,50,80)
    gammaf   = Gamma(30,70)
    pif      = Pi(10,20,50,80)
    sigmoidf = Sigmoid(0.1, 50)
    gaussf   = Gauss(50, 10)

    xs = np.linspace(0, 100, num=100)

    lambdav = [lambdaf.membership(x) for x in xs]
    gammav = [gammaf.membership(x) for x in xs]
    lv = [1-gammaf.membership(x) for x in xs]
    piv = [pif.membership(x) for x in xs]
    sigmoidv = [sigmoidf.membership(x) for x in xs]
    gaussv = [gaussf.membership(x) for x in xs]

    save_plot(xs, lambdav, 'lambda.pdf')
    save_plot(xs, gammav, 'gamma.pdf')
    save_plot(xs, lv, 'l.pdf')
    save_plot(xs, piv, 'pi.pdf')
    save_plot(xs, sigmoidv, 'sigmoid.pdf')
    save_plot(xs, gaussv, 'gauss.pdf')

