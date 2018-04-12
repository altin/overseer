import json
import os
import tkinter as tk

class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.grid(sticky=tk.N+tk.S+tk.E+tk.W)
        self.create_widgets()

    def create_widgets(self):
        self.processes_widget()
        self.save_widget()

    def processes_widget(self):
        self.processes_listbox = tk.Listbox(self, height=15, selectmode=tk.SINGLE)
        for process in running_processes():
            self.processes_listbox.insert(tk.END, process)
        self.processes_listbox.bind('<Double-Button-1>', self.blacklist_process)

        self.blacklisted_processes_listbox = tk.Listbox(self, height=15, selectmode=tk.SINGLE)
        self.blacklisted_processes_listbox.bind('<Double-Button-1>', self.unblacklist_process)

        processes_listbox_scrollbar = tk.Scrollbar(self, orient=tk.VERTICAL)
        self.processes_listbox.config(yscrollcommand=processes_listbox_scrollbar.set)
        processes_listbox_scrollbar.config(command=self.processes_listbox.yview)

        blacklisted_processes_listbox_scrollbar = tk.Scrollbar(self, orient=tk.VERTICAL)
        self.blacklisted_processes_listbox.config(yscrollcommand=blacklisted_processes_listbox_scrollbar.set)
        blacklisted_processes_listbox_scrollbar.config(command=self.blacklisted_processes_listbox.yview)

        self.processes_listbox.grid(row=0, column=0)
        self.blacklisted_processes_listbox.grid(row=0, column=1, sticky=tk.NS)
        processes_listbox_scrollbar.grid(row=0, column=0, sticky=tk.N + tk.E + tk.S)
        blacklisted_processes_listbox_scrollbar.grid(row=0, column=1, sticky=tk.N + tk.E + tk.S)

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

    def save_widget(self):
        self.save_button = tk.Button(self, text="Save", command=self.save)
        self.save_button.grid(row=1, column=2)

    def save(self):
        for idx in range(self.blacklisted_processes_listbox.size()):
            blacklisted_processes = self.blacklisted_processes_listbox.get(idx)

        data = { 'blacklisted_processes'  : blacklisted_processes
               , 'process_kill_wait_time' : 0
               }

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
