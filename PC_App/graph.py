import tkinter as tk
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

pd.options.mode.chained_assignment = None  # default='warn'



root = tk.Tk()
root.geometry("600x600")
root.title("Graph")

# ---

frame_top = tk.Frame(root)
frame_top.pack(fill='both', expand=True)

fig = Figure(dpi=100) # figsize=(10, 6), 
ax = fig.add_subplot(111)




with open("data.csv", 'r') as file:
    data = file.readlines()
    data = [line[:-1] for line in data]


df = pd.DataFrame(data)
uart4rx = df[df[0].str.startswith('UART4RW:')]
uart5rx = df[df[0].str.startswith('UART5RW:')]
uart6rx = df[df[0].str.startswith('UART6RW:')]
uart4rx[0] = uart4rx[0].str.replace('UART4RW:', '')
uart4rx[0] = uart4rx[0].str.replace(',0','')
uart4rx[0] = uart4rx[0].astype(dtype=np.int64)
uart5rx[0] = uart5rx[0].str.replace('UART5RW:', '')
uart5rx[0] = uart5rx[0].str.replace(',0','')
uart5rx[0] = uart5rx[0].astype(dtype=np.int64)
uart6rx[0] = uart6rx[0].str.replace('UART6RW:', '')
uart6rx[0] = uart6rx[0].str.replace(',0','')
uart6rx[0] = uart6rx[0].astype(dtype=np.int64)

m = max([uart4rx[0].max(), uart5rx[0].max(), uart6rx[0].max()])
uart4rx[0] = m - uart4rx[0] 
uart5rx[0] = m - uart5rx[0] 
uart6rx[0] = m - uart6rx[0] 

ax.plot(uart4rx[0], [0.9] * len(uart4rx), 'bo')
ax.plot(uart5rx[0], [0.95] * len(uart5rx), 'go')
ax.plot(uart6rx[0], [1] * len(uart6rx), 'ro')
for x, s in zip(uart5rx[0].tolist()[:-1], uart5rx[0].diff().dropna().abs().tolist()):
    ax.annotate(s, xy=(x+3, 0.955), fontsize=8)


canvas = FigureCanvasTkAgg(fig, master=frame_top)  # A tk.DrawingArea.
canvas.draw()
canvas.get_tk_widget().pack(fill='both', expand=True)

toolbar = NavigationToolbar2Tk(canvas, frame_top)
toolbar.update()


root.mainloop()

