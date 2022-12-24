import glob
import sys
import serial
import os
import tkinter as tk
from tkinter import ttk, filedialog

class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('Azure RTOS Load Test Tool')
        self.iconbitmap('favicon.ico')
        self.geometry('400x400')
        self.resizable(False, False)
        self.create_widgets()
        self.serial = None
        self.len = 0
    
    def create_widgets(self):
        self.label = ttk.Label(self, text='No File Selected')
        self.label.pack()
        # File loader
        self.file_loader = ttk.Button(self, text='Load File', command=self.load_file)
        self.file_loader.pack()
        # Run button
        self.run_button = ttk.Button(self, text='Run', command=self.run_test)
        self.run_button.pack()
        # COM port selector
        self.com_port = ttk.Combobox(self, values=self.serial_ports())
        self.com_port.pack()
        # COM port refresh button
        self.refresh_button = ttk.Button(self, text='Refresh', command=lambda: self.com_port.config(values=self.serial_ports()))
        self.refresh_button.pack()
        # Connect button
        self.connect_button = ttk.Button(self, text='Connect', command=self.connect)
        self.connect_button.pack()
        # Quit button
        self.quit_button = ttk.Button(self, text='Quit', command=self.quit)
        self.quit_button.pack()
        self.status_label = ttk.Label(self, text = '')
        self.status_label.pack()
        self.load_data_button = ttk.Button(self, text='Load Data', command=self.load_data)
        self.load_data_button.pack()

    def run_test(self):
        # Send "Run" command to tester
        self.serial.write(bytes(b"Run\0"))
        for i in range(self.len):
            # Receive response from tester
            response = self.serial.readline().decode()
            print(response)

    def load_file(self):
        file = filedialog.askopenfilename(initialdir=os.getcwd(), title='Select a File', filetypes=(('Text Files', '*.txt'), ('All Files', '*.*')))
        self.label.config(text=file.split('/')[-1] if file else 'No File Selected')

    def connect(self):
        # Open connection to COM port
        ser = serial.Serial(
            port = self.com_port.get(),
            baudrate=115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS
        )
        if ser.isOpen():
            ser.close()
        ser.open()
        print("Trying to connect...")
        self.status_label.config(text = ser.isOpen())
        # Send hello to selected COM port
        ser.write(bytes(b"Hello\0"))
        # Receive response from COM port
        response = ser.readline().decode()
        if response == "Azure RTOS Tester v0.1\n":
            self.status_label.config(text='Connected to Tester.')
            self.serial = ser
            return
        self.status_label.config(text = 'Trying to connect...')
        print(response)


    def quit(self):
        self.destroy()

    @staticmethod
    def serial_ports():
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

        result = []
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
        return result

    # Load data through serial connection
    def load_data(self):
        if (self.serial is None):
            self.status_label.config(text = 'Not connected to tester.')
            return
        if (self.label.cget('text') == 'No File Selected'):
            self.status_label.config(text = 'No file selected.')
            return
        # Load data from file
        with open(self.label.cget('text'), 'r') as file:
            data = file.readlines()
        self.len = len(data)
        # Send data to tester
        for line in data:
            self.serial.write(bytes(line[:-1] + '\0', 'utf-8'))


if __name__ == '__main__':
    app = App()
    app.mainloop()