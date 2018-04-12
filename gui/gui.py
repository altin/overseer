import json
import os
import tkinter as tk
from tkinter import ttk

class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.notebook = ttk.Notebook(master)
        self.notebook.grid(sticky=tk.N + tk.S + tk.E + tk.W)
        self.pwatch_frame = ttk.Frame(self.notebook)
        self.notebook.add(self.pwatch_frame, text='Process Killer')
        self.create_widgets()

    def create_widgets(self):
        self.processes_tab()

    def processes_tab(self):
        self.process_killer_checkbox = ttk.Checkbutton( self.pwatch_frame
                                                      , text='Enable process killer'
                                                      , command=self.do_stuff
                                                      , onvalue=True
                                                      , offvalue=False
                                                      )

        self.processes_listbox = tk.Listbox( self.pwatch_frame
                                           , height=15
                                           , selectmode=tk.SINGLE
                                           , state=tk.DISABLED
                                           )
        self.processes_listbox.bind('<Double-Button-1>', self.blacklist_process)

        self.blacklisted_processes_listbox = tk.Listbox( self.pwatch_frame
                                                       , height=15
                                                       , selectmode=tk.SINGLE
                                                       , state=tk.DISABLED
                                                       )
        self.blacklisted_processes_listbox.bind('<Double-Button-1>', self.unblacklist_process)

        processes_listbox_scrollbar = tk.Scrollbar(self.pwatch_frame, orient=tk.VERTICAL)
        self.processes_listbox.config( state=tk.DISABLED
                                     , yscrollcommand=processes_listbox_scrollbar.set
                                     )
        processes_listbox_scrollbar.config(command=self.processes_listbox.yview)

        blacklisted_processes_listbox_scrollbar = tk.Scrollbar( self.pwatch_frame
                                                              , orient=tk.VERTICAL
                                                              )
        self.blacklisted_processes_listbox.config( state=tk.DISABLED
                                                 , yscrollcommand=blacklisted_processes_listbox_scrollbar.set
                                                 )
        blacklisted_processes_listbox_scrollbar.config(command=self.blacklisted_processes_listbox.yview)

        self.save_button = tk.Button( self.pwatch_frame
                                    , text="Save"
                                    , command=self.save
                                    , state=tk.DISABLED
                                    )

        self.process_killer_checkbox.grid(row=0, column=0)
        self.processes_listbox.grid(row=1, column=0)
        self.blacklisted_processes_listbox.grid(row=1, column=1, sticky=tk.NS)
        processes_listbox_scrollbar.grid(row=1, column=0, sticky=tk.N + tk.E + tk.S)
        blacklisted_processes_listbox_scrollbar.grid(row=1, column=1, sticky=tk.N + tk.E + tk.S)
        self.save_button.grid(row=2, column=2)

    def do_stuff(self):
        if 'selected' in self.process_killer_checkbox.state():
            self.processes_listbox.config(state=tk.NORMAL)
            for process in running_processes():
                # todo make sure it's not in blacklisted processes
                self.processes_listbox.insert(tk.END, process)
            self.blacklisted_processes_listbox.config(state=tk.NORMAL)
            self.save_button.config(state=tk.NORMAL)
        else:
            self.processes_listbox.config(state=tk.DISABLED)
            self.blacklisted_processes_listbox.config(state=tk.DISABLED)
            self.save_button.config(state=tk.DISABLED)
        print(self.process_killer_checkbox.state())

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

    def save(self):
        if not 'selected' in process_killer_checkbox.state():
            data = { 'enable_process_killer'  : 0
                   , 'blacklisted_processes'  : []
                   , 'process_kill_wait_time' : 1
                   }
        else:
            blacklisted_processes = []
            for idx in range(self.blacklisted_processes_listbox.size()):
                blacklisted_processes.append(self.blacklisted_processes_listbox.get(idx))
                
                data = { 'enable_process_killer'  : 0
                       , 'blacklisted_processes'  : blacklisted_processes
                       , 'process_kill_wait_time' : 1
                       }

        with open('../config.json', 'w') as f:
            json.dump( data
                     , f
                     , ensure_ascii=False
                     , indent=4
                     , sort_keys=True
                     )

def running_processes():
    pids      = [pid for pid in os.listdir('/proc') if pid.isdigit()]
    processes = set()
    
    for pid in pids:
        try:
            with open(os.path.join('/proc', pid, 'comm'), 'rb') as o:
                processes.add(o.read().decode('utf-8').strip("\n"))
        except IOError:
            continue

    # Sort the processes set by the first letter of the word (after making it lowercase).
    return sorted(processes, key=lambda word: word[0].lower())

def main():
    root = tk.Tk()
    root.title = "overseer GUI"
    app = Application(master=root)
    app.mainloop()

if __name__ == '__main__':
    main()
