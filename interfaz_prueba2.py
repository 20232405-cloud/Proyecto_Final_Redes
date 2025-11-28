
import tkinter as tk
from tkinter import ttk, messagebox
import json, os

class InterfazMetricas(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Métricas de Red")
        self.geometry("450x380")
        self.configure(bg="#f4f4f4")

        frame = ttk.Frame(self, padding=10)
        frame.pack(expand=True, fill="both")

        ttk.Label(frame, text="📊 Métricas registradas", font=("Arial", 16, "bold")).pack(pady=10)

        # Tabla con dos columnas
        self.tree = ttk.Treeview(frame, columns=("Métrica", "Valor"), show="headings", height=8)
        self.tree.heading("Métrica", text="Métrica")
        self.tree.heading("Valor", text="Valor")
        self.tree.column("Métrica", width=200)
        self.tree.column("Valor", width=150)
        self.tree.pack(expand=True, fill="both", pady=10)

        ttk.Button(frame, text="🔄 Actualizar", command=self.cargar).pack(pady=10)

        self.cargar()

    def cargar(self):
        if not os.path.exists("metricas.json"):
            messagebox.showwarning("Advertencia", "Primero ejecuta prueba_main.py")
            return

        with open("metricas.json", "r") as f:
            data = json.load(f)

        # Limpiar tabla
        for row in self.tree.get_children():
            self.tree.delete(row)

        # Insertar valores
        for key, value in data.items():
            self.tree.insert("", tk.END, values=(key, f"{value:.2f}"))

if __name__ == "__main__":
    InterfazMetricas().mainloop()