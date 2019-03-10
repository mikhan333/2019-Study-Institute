import numpy as np
import matplotlib.pyplot as plt


# explicit Euler method
def ex_euler(u, r, h, N):
    for i in range(0, N-1):
        u[i+1] = (u[i] + r[i]*h)
        r[i+1] = (u[i]*(-100*h) + r[i]*(1-101*h))


# implicit Euler method
def im_euler(u, r, h, N):
    const = 1+101*h+100*h*h
    for i in range(0, N-1):
        u[i+1] = (u[i]*(1+101*h) + r[i]*h)/const
        r[i+1] = (u[i]*(-100*h) + r[i])/const

def build(i, h=2e-2, M=6):
    T = 10
    N = int(T/h + 1)
    x_teor = np.linspace(0,T,(N-1)*100+1) 
    x = np.linspace(0,T,N)
    
    u = np.zeros(N)
    u[0] = 2
    r = np.zeros(N)
    r[0] = -2
    
    ax = plt.subplot(M,1,i+2)
    ex_euler(u, r, h, N)
    plt.plot(x, u, '-r')
    ax.set_title(f'Явный метод - шаг = {h:.3f}')


fig_size = plt.rcParams["figure.figsize"]
fig_size[1] = 30 #height
plt.rcParams["figure.figsize"] = fig_size
fig = plt.figure()

print('Пронаблюдаем явный метод Эйлера при разных шагах h')

M = 8

T = 10
N = int(T/h + 1)
x_teor = np.linspace(0,T,(N-1)*100+1) 

ax = plt.subplot(M+1,1,1)
ex_euler(u, r, h, N)
plt.plot(x_teor, 2*np.exp(-x_teor))
ax.set_title('Точное решение 2e^(-x)')

h_min = 1e-2
h_max = 3e-2
for i in range(M):
    build(i, h_min + i*(h_max-h_min)/(M-1), M+1)
    
print('Как видно устойчивость теряется при шаге, большем чем h = 0.02')