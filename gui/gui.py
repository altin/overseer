import os
import tkinter as tk

class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        self.processes_widget()

    def processes_widget(self):
        self.processes_listbox = tk.Listbox(self, height=30, selectmode=tk.SINGLE)
        self.processes_listbox.grid(row=0, column=0)
        for process in running_processes():
            self.processes_listbox.insert(tk.END, process)
        self.processes_listbox.bind('<Double-Button-1>', self.blacklist_process)

        self.blacklisted_processes_listbox = tk.Listbox(self, height=30, selectmode=tk.SINGLE)
        self.blacklisted_processes_listbox.grid(row=0, column=1)
        self.blacklisted_processes_listbox.bind('<Double-Button-1>', self.unblacklist_process)

    def blacklist_process(self, event):
        widget = event.widget
        value  = widget.get(widget.curselection())
        self.processes_listbox.delete(self.processes_listbox.curselection()[0])
        self.blacklisted_processes_listbox.insert(tk.END, value)

    def unblacklist_process(self, event):
        widget = event.widget
        value  = widget.get(widget.curselection())
        self.blacklisted_processes_listbox.delete(self.blacklisted_processes_listbox.curselection()[0])
        self.processes_listbox.insert(tk.END, value)

def running_processes():
    pids      = [pid for pid in os.listdir('/proc') if pid.isdigit()]
    processes = set()
    
    for pid in pids:
        try:
            with open(os.path.join('/proc', pid, 'comm'), 'rb') as o:
                processes.add(o.read().decode('utf-8').strip("\n"))
        except IOError:
            continue

    return processes

def main():
    root = tk.Tk()
    root.title = "overseer GUI"
    app = Application(master=root)
    app.mainloop()

if __name__ == '__main__':
    main()
