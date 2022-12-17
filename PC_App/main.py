import glob
import sys
import serial
import os
import tkinter as tk
from tkinter import ttk, filedialog


class SerialCommunication:
    def __init__(self):
        pass

    def connect(self):
        pass

    def disconnect(self):
        pass

    def send(self):
        pass

    def receive(self):
        pass


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('Azure RTOS Load Test Tool')
        self.iconbitmap('favicon.ico')
        self.geometry('400x400')
        self.resizable(False, False)
        self.create_widgets()
    
    def create_widgets(self):
        self.label = ttk.Label(self, text='No File Selected')
        self.label.pack()
        # File loader
        self.file_loader = ttk.Button(self, text='Load File', command=self.load_file)
        self.file_loader.pack()
        # Run button
        self.run_button = ttk.Button(self, text='Run')
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


    def load_file(self):
        file = filedialog.askopenfilename(initialdir=os.getcwd(), title='Select a File', filetypes=(('Text Files', '*.txt'), ('All Files', '*.*')))
        self.label.config(text=file.split('/')[-1] if file else 'No File Selected')

    def connect(self):
        print("Trying to connect...")

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


if __name__ == '__main__':
    app = App()
    app.mainloop()