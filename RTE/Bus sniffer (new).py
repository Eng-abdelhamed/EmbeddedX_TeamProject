import tkinter as tk
from tkinter import ttk, scrolledtext
import serial
import serial.tools.list_ports
import threading
import time


class SerialApp:
    def __init__(self, master):
        self.master = master
        master.title("Bus Sniffer Application")
        master.geometry("1000x800")

        self.serial_port = None
        self.serial_thread = None
        self.running = False

        # Periodic control variables
        self.periodic_vars = [tk.BooleanVar(value=False) for _ in range(3)]
        self.interval_vars = [tk.IntVar(value=1000) for _ in range(3)]

        # --- Connection Settings Frame ---
        self.connection_frame = ttk.LabelFrame(master, text="Connection Settings")
        self.connection_frame.pack(padx=10, pady=5, fill="x")

        ttk.Label(self.connection_frame, text="COM Port:").grid(row=0, column=0, padx=5, pady=5, sticky="w")
        self.port_names = [port.device for port in serial.tools.list_ports.comports()]
        self.com_port_var = tk.StringVar(master)
        if self.port_names:
            self.com_port_var.set(self.port_names[0])
        self.com_port_dropdown = ttk.Combobox(
            self.connection_frame, textvariable=self.com_port_var, values=self.port_names
        )
        self.com_port_dropdown.grid(row=0, column=1, padx=5, pady=5, sticky="ew")
        self.com_port_dropdown.bind("<Button-1>", self.update_com_ports)

        ttk.Label(self.connection_frame, text="Baud Rate:").grid(row=1, column=0, padx=5, pady=5, sticky="w")
        self.baud_rate_var = tk.StringVar(master, value="115200")
        self.baud_rate_entry = ttk.Entry(self.connection_frame, textvariable=self.baud_rate_var)
        self.baud_rate_entry.grid(row=1, column=1, padx=5, pady=5, sticky="ew")

        self.connect_button = ttk.Button(self.connection_frame, text="Connect", command=self.connect_serial)
        self.connect_button.grid(row=2, column=0, padx=5, pady=5, sticky="ew")
        self.disconnect_button = ttk.Button(
            self.connection_frame, text="Disconnect", command=self.disconnect_serial, state=tk.DISABLED
        )
        self.disconnect_button.grid(row=2, column=1, padx=5, pady=5, sticky="ew")

        self.connection_frame.grid_columnconfigure(1, weight=1)

        # --- Received Data Frame ---
        self.receive_frame = ttk.LabelFrame(master, text="Received Data")
        self.receive_frame.pack(padx=10, pady=5, fill="both", expand=True)

        self.received_text = scrolledtext.ScrolledText(self.receive_frame, width=110, height=20)
        self.received_text.pack(padx=5, pady=5, fill="both", expand=True)
        self.received_text.config(state=tk.DISABLED)

        # --- Send Data Frame ---
        self.send_frame = ttk.LabelFrame(master, text="Send Data")
        self.send_frame.pack(padx=10, pady=5, fill="x")

        # Create 3 command rows
        self.send_data_vars = []
        for i in range(3):
            ttk.Label(self.send_frame, text=f"Hex Data {i+1}:").grid(row=i, column=0, padx=5, pady=5)
            var = tk.StringVar(master)
            self.send_data_vars.append(var)
            entry = ttk.Entry(self.send_frame, textvariable=var)
            entry.grid(row=i, column=1, padx=5, pady=5, sticky="ew")

            send_button = ttk.Button(
                self.send_frame, text=f"Send {i+1}", command=lambda v=var: self.send_serial_data(v)
            )
            send_button.grid(row=i, column=2, padx=5, pady=5)

            chk = ttk.Checkbutton(
                self.send_frame,
                text="Periodic",
                variable=self.periodic_vars[i],
                command=lambda idx=i: self.toggle_periodic(idx),
            )
            chk.grid(row=i, column=3, padx=5, pady=5)

            ttk.Label(self.send_frame, text="Interval (ms):").grid(row=i, column=4, padx=5)
            interval_entry = ttk.Entry(self.send_frame, textvariable=self.interval_vars[i], width=8)
            interval_entry.grid(row=i, column=5, padx=5)

        # Send All button
        self.send_all_button = ttk.Button(self.send_frame, text="Send All", command=self.send_all_commands)
        self.send_all_button.grid(row=3, column=0, columnspan=6, pady=10)

        self.send_frame.grid_columnconfigure(1, weight=1)

        # --- Status Bar ---
        self.status_bar = ttk.Label(
            master, text="Status: Disconnected", relief="sunken", anchor="w", foreground="red"
        )
        self.status_bar.pack(side="bottom", fill="x")

    # --------------------------
    # Connection
    # --------------------------
    def update_com_ports(self, event=None):
        self.port_names = [port.device for port in serial.tools.list_ports.comports()]
        self.com_port_dropdown['values'] = self.port_names
        if self.port_names and self.com_port_var.get() not in self.port_names:
            self.com_port_var.set(self.port_names[0])
        elif not self.port_names:
            self.com_port_var.set("")

    def connect_serial(self):
        port = self.com_port_var.get()
        try:
            baud_rate = int(self.baud_rate_var.get())
        except ValueError:
            self.update_status("Error: Invalid baud rate", "red")
            return

        if not port:
            self.update_status("Error: No COM port selected", "red")
            return

        try:
            self.serial_port = serial.Serial(port, baud_rate, timeout=0.1)
            self.running = True
            self.serial_thread = threading.Thread(target=self.read_from_port, daemon=True)
            self.serial_thread.start()

            self.update_status(f"Connected to {port} @ {baud_rate}", "green")
            self.connect_button.config(state=tk.DISABLED)
            self.disconnect_button.config(state=tk.NORMAL)
            self.com_port_dropdown.config(state=tk.DISABLED)
            self.baud_rate_entry.config(state=tk.DISABLED)
        except serial.SerialException as e:
            self.update_status(f"Error: {e}", "red")

    def disconnect_serial(self):
        self.running = False
        if self.serial_thread and self.serial_thread.is_alive():
            self.serial_thread.join(timeout=1)
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.close()

        self.update_status("Disconnected", "red")
        self.connect_button.config(state=tk.NORMAL)
        self.disconnect_button.config(state=tk.DISABLED)
        self.com_port_dropdown.config(state=tk.NORMAL)
        self.baud_rate_entry.config(state=tk.NORMAL)

        # Stop all periodic sends
        for var in self.periodic_vars:
            var.set(False)

    # --------------------------
    # Data Handling
    # --------------------------
    def read_from_port(self):
        while self.running and self.serial_port and self.serial_port.is_open:
            try:
                if self.serial_port.in_waiting > 0:
                    data = self.serial_port.readline().decode('utf-8', errors='ignore').strip()
                    if data:
                        self.master.after(0, self.update_received_text, data)
            except (serial.SerialException, OSError) as e:
                self.master.after(0, self.update_status, f"Error: {e}", "red")
                self.master.after(0, self.disconnect_serial)
                break
            time.sleep(0.01)

    def update_received_text(self, data):
        self.received_text.config(state=tk.NORMAL)
        self.received_text.insert(tk.END, data + "\n")
        self.received_text.see(tk.END)
        self.received_text.config(state=tk.DISABLED)

    def send_serial_data(self, data_var):
        if self.serial_port and self.serial_port.is_open:
            try:
                hex_str = data_var.get().replace("0x", "").split()
                if not hex_str:
                    return
                data_bytes = bytes([int(x, 16) for x in hex_str])
                self.serial_port.write(data_bytes)
                self.update_received_text(f"TX: {data_bytes.hex(' ')}")
            except Exception as e:
                self.update_status(f"Send error: {e}", "red")
        else:
            self.update_status("Not connected", "red")

    def send_all_commands(self):
        for var in self.send_data_vars:
            self.send_serial_data(var)

    # --------------------------
    # Periodic Sending
    # --------------------------
    def toggle_periodic(self, idx):
        if self.periodic_vars[idx].get():
            self.send_periodic(idx)

    def send_periodic(self, idx):
        if self.periodic_vars[idx].get():
            self.send_serial_data(self.send_data_vars[idx])
            interval = self.interval_vars[idx].get()
            self.master.after(interval, lambda: self.send_periodic(idx))

    # --------------------------
    # Status Bar
    # --------------------------
    def update_status(self, message, color="black"):
        self.status_bar.config(text=f"Status: {message}", foreground=color)


# --------------------------
# Main Program
# --------------------------
if __name__ == "__main__":
    root = tk.Tk()
    app = SerialApp(root)
    root.mainloop()

