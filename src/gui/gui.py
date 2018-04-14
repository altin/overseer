import json
import os
import tkinter as tk
import tkinter.messagebox as messagebox
from tkinter import ttk

class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)

        self.blacklisted_processes = []

        self.notebook = ttk.Notebook(master)
        self.notebook.grid(sticky=tk.N + tk.S + tk.E + tk.W)
        self.pwatch_frame = ttk.Frame(self.notebook)
        self.notebook.add(self.pwatch_frame, text='Process Killer')

        self.create_widgets()

    def create_widgets(self):
        """Create the widgets."""
        self.processes_tab()

    def processes_tab(self):
        """Create the widgets for the Process Killer tab and draw them."""
        # Process Killer checkbox.
        self.process_killer_checkbox = ttk.Checkbutton( self.pwatch_frame
                                                      , text='Enable Process Killer'
                                                      , command=self.change_widgets_state
                                                      , onvalue=True
                                                      , offvalue=False
                                                      )

        processes_label = tk.Label(self.pwatch_frame, text='Processes: ')
        # Processes listbox.
        self.processes_listbox = tk.Listbox( self.pwatch_frame
                                           , height=15
                                           , selectmode=tk.SINGLE
                                           , state=tk.DISABLED
                                           )
        self.processes_listbox.bind('<Double-Button-1>', self.blacklist_process)

        blacklisted_processes_label = tk.Label(self.pwatch_frame, text='Blacklisted Processes: ')
        # Blacklisted processes listbox.
        self.blacklisted_processes_listbox = tk.Listbox( self.pwatch_frame
                                                       , height=15
                                                       , selectmode=tk.SINGLE
                                                       , state=tk.DISABLED
                                                       )
        self.blacklisted_processes_listbox.bind('<Double-Button-1>', self.unblacklist_process)

        # Processes listbox scrollbar.
        processes_listbox_scrollbar = tk.Scrollbar(self.pwatch_frame, orient=tk.VERTICAL)
        self.processes_listbox.config( state=tk.DISABLED
                                     , yscrollcommand=processes_listbox_scrollbar.set
                                     )
        processes_listbox_scrollbar.config(command=self.processes_listbox.yview)

        # Blacklisted processes listbox scrollbar.
        blacklisted_processes_listbox_scrollbar = tk.Scrollbar( self.pwatch_frame
                                                              , orient=tk.VERTICAL
                                                              )
        self.blacklisted_processes_listbox.config( state=tk.DISABLED
                                                 , yscrollcommand=blacklisted_processes_listbox_scrollbar.set
                                                 )
        blacklisted_processes_listbox_scrollbar.config(command=self.blacklisted_processes_listbox.yview)

        # Process kill wait time.
        self.process_kill_wait_time_label = tk.Label( self.pwatch_frame
                                                    , text="Seconds to wait before killing processes:"
                                                    )
        self.wait_time = tk.StringVar()
        self.wait_time.set('1')
        self.process_kill_wait_time_text = tk.Entry( self.pwatch_frame
                                                   , textvariable=self.wait_time
                                                   )

        # Save button.
        self.save_button = tk.Button( self.pwatch_frame
                                    , text="Save"
                                    , command=self.save
                                    , state=tk.DISABLED
                                    )

        # Set grid for all widgets.
        self.process_killer_checkbox.grid(row=0, column=0)
        processes_label.grid(row=1, column=0)
        blacklisted_processes_label.grid(row=1, column=1)
        self.processes_listbox.grid(row=2, column=0)
        self.blacklisted_processes_listbox.grid(row=2, column=1, sticky=tk.NS)
        processes_listbox_scrollbar.grid(row=2, column=0, sticky=tk.N + tk.E + tk.S)
        blacklisted_processes_listbox_scrollbar.grid(row=2, column=1, sticky=tk.N + tk.E + tk.S)
        self.process_kill_wait_time_label.grid(row=3, column=0)
        self.process_kill_wait_time_text.grid(row=3, column=1)
        self.save_button.grid(row=4, column=2)

    def change_widgets_state(self):
        """Change all widgets state based on the Process Killer checkbox."""
        # Enable all widgets if Process Killer is enabled.
        if 'selected' in self.process_killer_checkbox.state():
            self.processes_listbox.config(state=tk.NORMAL)
            self.blacklisted_processes_listbox.config(state=tk.NORMAL)
            self.save_button.config(state=tk.NORMAL)

            for process in running_processes():
                if process not in self.blacklisted_processes:
                    self.processes_listbox.insert(tk.END, process)
        # Otherwise, disable all widgets.
        else:
            # Delete all items in the listbox.
            self.processes_listbox.delete(0, tk.END)
            # Disable all of the widgets.
            self.processes_listbox.config(state=tk.DISABLED)
            self.blacklisted_processes_listbox.config(state=tk.DISABLED)
            self.save_button.config(state=tk.DISABLED)

    def blacklist_process(self, event):
        """Add a process to the blacklist listbox.

        Parameters:
          `event`: `tkinter.Event`. The event of double clicking on a process in the processes
        listbox.

        """
        widget = event.widget
        value  = widget.get(widget.curselection())
        self.processes_listbox.delete(self.processes_listbox.curselection()[0])
        self.blacklisted_processes_listbox.insert(tk.END, value)
        self.blacklisted_processes.append(value)

    def unblacklist_process(self, event):
        """Add a process to the blacklist listbox.

        Parameters:
          `event`: `tkinter.Event`. The event of double clicking a process in the blacklisted
        processes listbox.

        """
        widget = event.widget
        value  = widget.get(widget.curselection())
        self.blacklisted_processes_listbox.delete(self.blacklisted_processes_listbox.curselection()[0])
        self.processes_listbox.insert(tk.END, value)
        self.blacklisted_processes.remove(value)

    def save(self):
        """Save the settings to a JSON file."""
        try:
            wait_time = float(self.wait_time.get())
        except:
            messagebox.showerror("Error", "Seconds to wait textbox only accepts integers.")
            return

        data = { 'enable_process_killer'  : 1
               , 'blacklisted_processes'  : self.blacklisted_processes
               , 'process_kill_wait_time' : wait_time
               }

        with open('../config.json', 'w') as f:
            json.dump( data
                     , f
                     , ensure_ascii=False
                     , indent=4
                     , sort_keys=True
                     )

def running_processes():
    """Return an alphabetically sorted list of all the running processes in the system."""
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
    """The main loop of the program."""
    root = tk.Tk()
    root.title('pcontrol Configuration')
    app = Application(master=root)
    app.mainloop()

if __name__ == '__main__':
    main()
