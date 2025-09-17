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
        master.geometry("900x600")

        self.serial_port = None
        self.serial_thread = None
        self.running = False
        self.periodic_var = tk.BooleanVar(value=False)

        # --- Connection Settings Frame ---
        self.connection_frame = ttk.LabelFrame(master, text="Connection Settings")
        self.connection_frame.pack(padx=10, pady=5, fill="x")

        ttk.Label(self.connection_frame, text="COM Port:").grid(row=0, column=0, padx=5, pady=5, sticky="w")
        self.port_names = [port.device for port in serial.tools.list_ports.comports()]
        self.com_port_var = tk.StringVar(master)
        if self.port_names:
            self.com_port_var.set(self.port_names[0])
        self.com_port_dropdown = ttk.Combobox(self.connection_frame, textvariable=self.com_port_var, values=self.port_names)
        self.com_port_dropdown.grid(row=0, column=1, padx=5, pady=5, sticky="ew")
        self.com_port_dropdown.bind("<Button-1>", self.update_com_ports)

        ttk.Label(self.connection_frame, text="Baud Rate:").grid(row=1, column=0, padx=5, pady=5, sticky="w")
        self.baud_rate_var = tk.StringVar(master, value="115200")
        self.baud_rate_entry = ttk.Entry(self.connection_frame, textvariable=self.baud_rate_var)
        self.baud_rate_entry.grid(row=1, column=1, padx=5, pady=5, sticky="ew")

        self.connect_button = ttk.Button(self.connection_frame, text="Connect", command=self.connect_serial)
        self.connect_button.grid(row=2, column=0, padx=5, pady=5, sticky="ew")
        self.disconnect_button = ttk.Button(self.connection_frame, text="Disconnect", command=self.disconnect_serial, state=tk.DISABLED)
        self.disconnect_button.grid(row=2, column=1, padx=5, pady=5, sticky="ew")

        self.connection_frame.grid_columnconfigure(1, weight=1)

        # --- Received Data Frame ---
        self.receive_frame = ttk.LabelFrame(master, text="Received Data")
        self.receive_frame.pack(padx=10, pady=5, fill="both", expand=True)

        self.received_text = scrolledtext.ScrolledText(self.receive_frame, width=100, height=20)
        self.received_text.pack(padx=5, pady=5, fill="both", expand=True)
        self.received_text.config(state=tk.DISABLED)

        # --- Send Data Frame ---
        self.send_frame = ttk.LabelFrame(master, text="Send Data")
        self.send_frame.pack(padx=10, pady=5, fill="x")

        ttk.Label(self.send_frame, text="Hex Data:").pack(side="left", padx=5)
        self.send_data_var = tk.StringVar(master)
        self.send_data_entry = ttk.Entry(self.send_frame, textvariable=self.send_data_var)
        self.send_data_entry.pack(side="left", padx=5, pady=5, fill="x", expand=True)

        self.send_button = ttk.Button(self.send_frame, text="Send Once", command=self.send_serial_data, state=tk.DISABLED)
        self.send_button.pack(side="left", padx=5, pady=5)

        self.periodic_check = ttk.Checkbutton(self.send_frame, text="Send Periodic", variable=self.periodic_var, command=self.toggle_periodic)
        self.periodic_check.pack(side="left", padx=5)

        ttk.Label(self.send_frame, text="Interval (ms):").pack(side="left", padx=5)
        self.interval_var = tk.IntVar(value=1000)
        self.interval_entry = ttk.Entry(self.send_frame, textvariable=self.interval_var, width=8)
        self.interval_entry.pack(side="left", padx=5)

        # --- Status Bar ---
        self.status_bar = ttk.Label(master, text="Status: Disconnected", relief="sunken", anchor="w", foreground="red")
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
            self.send_button.config(state=tk.NORMAL)
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
        self.send_button.config(state=tk.DISABLED)
        self.com_port_dropdown.config(state=tk.NORMAL)
        self.baud_rate_entry.config(state=tk.NORMAL)
        self.periodic_var.set(False)

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

    def send_serial_data(self):
        if self.serial_port and self.serial_port.is_open:
            try:
                # Parse hex input: "0x11 0x22 0x33"
                hex_str = self.send_data_var.get().replace("0x", "").split()
                data_bytes = bytes([int(x, 16) for x in hex_str])
                self.serial_port.write(data_bytes)
                self.update_received_text(f"TX: {data_bytes.hex(' ')}")
            except Exception as e:
                self.update_status(f"Send error: {e}", "red")
        else:
            self.update_status("Not connected", "red")

    def toggle_periodic(self):
        if self.periodic_var.get():
            self.send_periodic()

    def send_periodic(self):
        if self.periodic_var.get():
            self.send_serial_data()
            interval = self.interval_var.get()
            self.master.after(interval, self.send_periodic)

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
