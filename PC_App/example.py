import glob
import sys
import threading
import time
import tkinter
from tkinter import filedialog
import serial
import os
import tkinter.messagebox
import customtkinter

from asyncio import get_event_loop
from serial_asyncio import open_serial_connection

import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
from matplotlib.figure import Figure

import pandas as pd
import numpy as np

customtkinter.set_appearance_mode("Light")
customtkinter.set_default_color_theme("blue")
pd.options.mode.chained_assignment = None  # default='warn'


class App(customtkinter.CTk):
    def __init__(self):
        super().__init__()

        self.serial = None
        self.received_data = []
        self.loaded_latch = False
        self.drawing_latch = False
        self.saved_latch = False
        self.len = dict()
        # configure window
        self.title("Azure RTOS Load Testing Tool")
        self.geometry(f"{1100}x{800}")
        # configure grid layout (4x4)
        self.grid_columnconfigure(1, weight=1)
        self.grid_columnconfigure((2, 3), weight=0)
        self.grid_rowconfigure((0, 1, 2), weight=1)

        # create sidebar frame with widgets
        self.sidebar_frame = customtkinter.CTkFrame(self, width=140, corner_radius=0)
        self.sidebar_frame.grid(row=0, column=0, rowspan=4, sticky="nsew")
        self.sidebar_frame.grid_rowconfigure(4, weight=1)
        self.logo_label = customtkinter.CTkLabel(self.sidebar_frame, text="ThreadX Load Testing", font=customtkinter.CTkFont(size=20, weight="bold"))
        self.logo_label.grid(row=0, column=0, padx=20, pady=(20, 10))
        self.logo_label = customtkinter.CTkLabel(self.sidebar_frame, text="© R.Kulagin & A.Dorodnyaya, 2022", font=customtkinter.CTkFont(size=14))
        self.logo_label.grid(row=1, column=0, padx=20, pady=(20, 10))

        # com port selector frame
        self.com_port_selector_frame = customtkinter.CTkFrame(self.sidebar_frame)
        self.com_port_selector_frame.grid(row=2, column=0, sticky="nsew")
        # option menu for com port selection
        self.com = customtkinter.CTkOptionMenu(master=self.com_port_selector_frame, values = self.get_available_com_ports(), command=self.com_port_selected)
        self.com.grid(row=0, column=0, padx=20, pady=(20, 10))

        # button to refresh com ports
        self.refresh_button = customtkinter.CTkButton(master=self.com_port_selector_frame, text="Refresh", command=self.refresh_com_ports)
        self.refresh_button.grid(row=1, column=0, padx=20, pady=(20, 10))

        # button to connect to com port
        self.connect_button = customtkinter.CTkButton(master=self.com_port_selector_frame, text="Connect", command=self.connect_to_com)
        self.connect_button.grid(row=2, column=0, padx=20, pady=(20, 10))

        # file selector frame
        self.file_selector_frame = customtkinter.CTkFrame(self.sidebar_frame)
        self.file_selector_frame.grid(row=3, column=0, sticky="nsew")

        # file selector 
        self.file = customtkinter.CTkLabel(master=self.file_selector_frame, text="No File Selected")
        self.file.grid(row=0, column=0, padx=20, pady=(20, 10))

        self.file_loader = customtkinter.CTkButton(master=self.file_selector_frame, text="Select File", command=self.load_file)
        self.file_loader.grid(row=1, column=0, padx=20, pady=(20, 10))

        self.filepath = None

        # create main frame with widgets
        self.main_frame = customtkinter.CTkFrame(self, corner_radius=0, width=700, height=400)
        self.main_frame.grid(row=0, column=1, rowspan=2, sticky="nsew")

        # Text widget for displaying the log filling full available space
        self.log = customtkinter.CTkTextbox(self.main_frame, height=400, width=self.main_frame.cget("width"))
        self.log.grid(row=0, column=0, sticky="nsew")

        # Progress bar widget for displaying the progress of the task
        # self.progress = customtkinter.CTkProgressBar(self.main_frame)
        # self.progress.grid(row=1, column=0, sticky="nsew")

        # Image widget for displaying the graph filling full  available space
        # self.graph = customtkinter.CTkImage(self.main_frame)
        # self.graph.grid(row=0, column=1, sticky="nsew")

        # create right frame with widgets
        self.right_frame = customtkinter.CTkFrame(self, width=140, corner_radius=0)
        self.right_frame.grid(row=0, column=2, rowspan=4, sticky="nsew")
        self.right_frame.grid_rowconfigure(4, weight=1)
        

        # create communcation frame with widgets
        self.communication_frame = customtkinter.CTkFrame(self.right_frame, corner_radius=0)
        self.communication_frame.grid(row=0, column=0, sticky="nsew")
        # create load_data button
        self.load_data_button = customtkinter.CTkButton(self.communication_frame, text="Load Data", command=self.load_data, state=tkinter.DISABLED)
        self.load_data_button.grid(row=0, column=0, padx=20, pady=(20, 10))
        # create run button
        self.run_button = customtkinter.CTkButton(self.communication_frame, text="Run", command=self.run_test, state=tkinter.DISABLED)
        self.run_button.grid(row=1, column=0, padx=20, pady=(20, 10))
        # create save button
        self.save_button = customtkinter.CTkButton(self.communication_frame, text="Save & Open Statistics", command=self.show_statistics_windows, state=tkinter.DISABLED)
        self.save_button.grid(row=2, column=0, padx=20, pady=(20, 10))

        # create status frame with widgets
        self.status_frame = customtkinter.CTkFrame(self.right_frame, corner_radius=0)
        self.status_frame.grid(row=1, column=0, sticky="nsew")
        
        # create status label
        self.status_label = customtkinter.CTkLabel(self.status_frame, text="Status checklist: ", font=customtkinter.CTkFont(size=20, weight="bold"))
        self.status_label.grid(row=0, column=0, padx=20, pady=(20, 10))
        self.status_checkbox = []
        for idx, item in enumerate(["Select Com Port", "Select File", "Connected to Tester", "Load Data To Tester", "Press Run Button", "Receiving Data", "Data Received", "Open Statistics Window"]):
            # create status checkbox
            self.status_checkbox.append(customtkinter.CTkCheckBox(self.status_frame, text=item, font=customtkinter.CTkFont(size=12), state="disabled", text_color_disabled="black",))
            self.status_checkbox[-1].grid(row=idx+1, column=0, padx=20, pady=(10, 5), sticky="w")
        
        # create progress bar
        # self.progress_bar = customtkinter.CTkProgressBar(self.status_frame)
        # self.progress_bar.grid(row=1+len(self.status_checkbox), column=0, padx=20, pady=(20, 10), sticky="w")
        # self.progress_bar.set(0)

    def calculate_statistics(self):
        df = pd.DataFrame(self.received_data)
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
        self.uart4rx = uart4rx
        self.uart5rx = uart5rx
        self.uart6rx = uart6rx

    def show_statistics_windows(self):
        # set checkbox to checked
        self.status_checkbox[7].select()
        self.calculate_statistics()
        self.statistics_window = customtkinter.CTkToplevel(self)
        self.statistics_window.title("Statistics")
        self.statistics_window.geometry("800x600")
        frame_top = tkinter.Frame(self.statistics_window)
        frame_top.pack(fill='both', expand=True)
        fig = Figure(dpi=100) # figsize=(10, 6), 
        ax = fig.add_subplot(111)
        ax.plot(self.uart4rx[0], [0.9] * len(self.uart4rx), 'bo')
        ax.plot(self.uart5rx[0], [0.95] * len(self.uart5rx), 'go')
        ax.plot(self.uart6rx[0], [1] * len(self.uart6rx), 'ro')
        for x, s in zip(self.uart5rx[0].tolist()[:-1], self.uart5rx[0].diff().dropna().abs().tolist()):
            ax.annotate(s, xy=(x+3, 0.955), fontsize=8)
        canvas = FigureCanvasTkAgg(fig, master=frame_top)
        canvas.draw()
        canvas.get_tk_widget().pack(fill='both', expand=True)

        toolbar = NavigationToolbar2Tk(canvas, frame_top)
        toolbar.update()
        self.save_data()


    
    def save_data(self):
        # get filename to save file
        filename = tkinter.filedialog.asksaveasfilename(initialdir = "/",title = "Select file",filetypes = (("csv files","*.csv"),("all files","*.*")))
        with open(filename, "w") as f:
            f.writelines(self.received_data)


    def run_test(self):
        # # run _run_test in a new thread
        # self.thread = threading.Thread(target=self._run_test)
        # self.thread.start()
        
        # self.progress.configure(mode="indeterminnate")
        # self.progress.start()
        # # wait for thread to end
        # self.thread.join()
        
        # self.progress.stop()

        # # check if thread is alive
        # # if self.thread.is_alive():
        self._run_test()


    def _run_test(self):
        # Send "Run" command to tester
        self.serial.write(bytes(b"Run\0"))
        while True:
            # Receive line from tester
            response = self.serial.readline().decode()
            # If line is empty, skip
            if response == '':
                continue
            # If line starts with `UART4:` decrement len['4']
            if response.startswith('UART4:'):
                self.len['4'] -= 1
            # If line starts with `UART6:` decrement len['6']
            elif response.startswith('UART6:'):
                self.len['6'] -= 1
            # Save response to log
            self.print(response)
            self.received_data.append(response)
            # If len['4'] and len['6'] are both 0, break
            if self.len['4'] == 0 and self.len['6'] == 0:
                break


        
    # Load data through serial connection
    def load_data(self):
        if (self.serial is None):
            self.print(text = 'Not connected to tester.')
            return
        if self.filepath == 'No File Selected' or self.filepath == None:
            self.status_label.config(text = 'No file selected.')
            return
        # Load data from file
        with open(self.filepath, 'r') as file:
            data = file.readlines()
        self.len = {"4" : 0, "6": 0}
        # Send data to tester
        for line in data:
            self.serial.write(bytes(line[:-1] + '\0', 'utf-8'))
            self.len[line[0]] += 1
        self.print(text = f"Data loaded to tester. UART4: {self.len['4']}, UART6: {self.len['6']}\n")

    def connect_to_com(self):
        self.print(f"Connecting to {self.com.get()} port.\n")
        # Open connection to COM port

        ser = serial.Serial(
            port = self.com.get(),
            baudrate=115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS
        )
        try:
            if ser.isOpen():
                ser.close()
            ser.open()
            # Send hello to selected COM port
            ser.write(bytes(b"Hello\0"))
            # Receive response from COM port
            ATTEMPTS = 20
            for attempt in range(ATTEMPTS):
                response = ser.readline().decode()
                self.print(response)
                if response == "Azure RTOS Tester v0.1\n":
                    self.print(f"Connected to {self.com.get()} port.\n")   
                    self.serial = ser
                    return    
            self.print(f"Can't connect to {self.com.get()} port. Response: {response}.\n")
        except Exception as e:
            tkinter.messagebox.showerror("Error", e)
            self.print(f"Can't connect to {self.com.get()} port. Error: {e}.\n")


    def get_available_com_ports(self):
        """ Lists serial port names

            :raises EnvironmentError:
                On unsupported or unknown platforms
            :returns:
                A list of the serial ports available on the system
        """
        if sys.platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            ports = glob.glob('/dev/tty[A-Za-z]*')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform')

        result = [""]
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
        return result

    def refresh_com_ports(self):
        self.com.configure(values = self.get_available_com_ports())     

    def com_port_selected(self, event):
        # self.print(event + '\n')
        if event == "":
            tkinter.messagebox.showerror("Error", "No COM port selected")
        else:
            self.print(f"COM port selected: {event}.\n")

    def load_file(self):
        self.filepath = filedialog.askopenfilename(initialdir=os.getcwd(), title='Select a File', filetypes=(('Text Files', '*.txt'), ('All Files', '*.*')))
        self.file.configure(text=self.filepath.split("/")[-1] if self.file else "No File Selected")
        self.print(f"File selected: {self.filepath}.\n")

    def print(self, text):
        self.log.insert(tkinter.END, text)
        self.update_states()

    def update_states(self):
        self.load_data_button.configure(state="disabled")
        self.run_button.configure(state="disabled")
        for checkbox in self.status_checkbox:
            checkbox.deselect()

        if self.filepath != None:
            self.status_checkbox[1].select() # Select File
        # Check that com port is selected
        if self.com.get() != "":
            self.status_checkbox[0].select() # Select Com Port
            if self.serial != None and self.serial.isOpen():
                self.status_checkbox[2].select()
                if self.filepath != None:
                    self.load_data_button.configure(state="normal")
                    self.status_checkbox[1].select() # Select File  
                    if ("4" in self.len and self.len["4"] > 0) or ("6" in self.len and self.len["6"] > 0) or self.loaded_latch:
                        self.loaded_latch = True
                        self.run_button.configure(state="normal")
                        self.status_checkbox[3].select()
                        if len(self.received_data) > 0:
                            self.status_checkbox[4].select()
                            self.save_button.configure(state="normal")
                            self.status_checkbox[5].select()
                            self.status_checkbox[6].select()
                


if __name__ == "__main__":
    app = App()
    app.mainloop()