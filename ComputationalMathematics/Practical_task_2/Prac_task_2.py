import numpy as np
import matplotlib.pyplot as plt

fig_size = plt.rcParams["figure.figsize"]
fig_size[1] = 30  # height
fig_size[0] = 15
plt.rcParams["figure.figsize"] = fig_size
fig = plt.figure()
M = 4

# Задача 16
def get_solution_newton(y, x, h, N, a, end, delta):
    while(True):
        y[1] = y[0]+h*(a+h)
        meth_shoot(y, x, h, N)
        F_h = y[N-1] - end
        
        y[1] = y[0]+h*a
        meth_shoot(y, x, h, N)
        F = y[N-1] - end
        
        dF = (F_h - F) / h
        a_new = a - F / dF
        
        if abs(a_new - a) < delta/2:
            return a_new
        if y[N-1] >= end - delta and y[N-1] <= end + delta:
            return a_new
        a = a_new

def get_solution_half(y, x, h, N, left, right, end, delta):
    while(True):
        mid = (left+right)/2
        y[1] = y[0]+h*mid
        meth_shoot(y, x, h, N)
        if right - left < delta/2:
            return mid
        if y[N-1] < end - delta:
            left = mid
        elif y[N-1] > end + delta:
            right = mid
        else:
            return mid
    
def meth_shoot(y, x, h, N):
    for i in range(1, N-1):
        coef_0 = x[i]*x[i]
        coef_1 = x[i]/(2*h) - x[i]*x[i]/(h*h)
        coef_2 = 2*x[i]*x[i]/(h*h)+1
        divider = x[i]*x[i]/(h*h) + x[i]/(2*h)
        y[i+1] = (coef_0+y[i-1]*coef_1+y[i]*coef_2)/divider

# Описание отрезка по оси X
T = 2
h = 2e-4
N = int(T/h + 1)
x_teor = np.linspace(1, 1+T, (N-1)*100+1)
x = np.linspace(1, 1+T, N)

# Задаем альфу
a = 1
delta = 1e-5
end = 3

# Задаем начальные условия для "стрельбы"
# то есть y0 = 4/3 и y1 = a*h + y0
y = np.zeros(N)
y[0] = 4.0/3

# Точное решение задачи
f = lambda x: -x/8+9/(8*x)+x*x/3

# Построение и вызов функции
ax = plt.subplot(2*M-1, 1, 1)
plt.plot(x_teor, f(x_teor), linewidth=5.0)
ax.set_title(f'Точное решение')

# Построим решения при больших дельта
for i in range(2, M + 1):
    ax = plt.subplot(2*M-1, 1, i)
    delta = 10 ** (2-i)
    get_solution_newton(y, x, h, N, -0.5, end, delta)
    #get_solution_half(y, x, h, N, -1, 1, end, delta)
    plt.plot(x_teor, f(x_teor), x, y, '-ro', linewidth=5.0)
    ax.set_title(f'Решение при дельта={delta}, h={h}')

# Построим решения при разных h
for i in range(2, M + 1):
    delta = 1e-1
    h = 10 ** (2-i)
    N = int(T/h + 1)
    y = np.zeros(N)
    y[0] = 4.0/3
    x_teor = np.linspace(1, 1+T, (N-1)*100+1)
    x = np.linspace(1, 1+T, N)
    
    get_solution_newton(y, x, h, N, -0.5, end, delta)
    #get_solution_half(y, x, h, N, -1, 1, end, delta)
    ax = plt.subplot(2*M-1, 1, M+i-1)
    plt.plot(x_teor, f(x_teor), x, y, '-ro', linewidth=5.0)
    ax.set_title(f'Решение при дельта={delta}, h={h}')
