import tkinter as tk
from tkinter import messagebox, ttk

# ==============================
# Circular Linked List Classes
# ==============================

class Node:
    def __init__(self, data):
        self.data = data
        self.next = None

class CircularLinkedList:
    def __init__(self):
        self.head = None

    def insert_front(self, value):
        new_node = Node(value)
        if self.head is None:
            self.head = new_node
            new_node.next = self.head
            return

        temp = self.head
        while temp.next != self.head:
            temp = temp.next

        new_node.next = self.head
        temp.next = new_node
        self.head = new_node

    def insert_end(self, value):
        new_node = Node(value)
        if self.head is None:
            self.head = new_node
            new_node.next = self.head
            return

        temp = self.head
        while temp.next != self.head:
            temp = temp.next

        temp.next = new_node
        new_node.next = self.head

    def delete_value(self, value):
        if self.head is None:
            return False

        curr = self.head
        prev = None

        # Delete head
        if curr.data == value:
            if curr.next == self.head:
                self.head = None
                return True

            temp = self.head
            while temp.next != self.head:
                temp = temp.next

            temp.next = self.head.next
            self.head = self.head.next
            return True

        # Delete middle or end
        while curr.next != self.head:
            prev = curr
            curr = curr.next
            if curr.data == value:
                prev.next = curr.next
                return True

        return False

    def search(self, value):
        if self.head is None:
            return False

        temp = self.head
        while True:
            if temp.data == value:
                return True
            temp = temp.next
            if temp == self.head:
                break

        return False

    def clear(self):
        self.head = None

    def display(self):
        if self.head is None:
            return "List is empty."

        result = []
        temp = self.head
        while True:
            result.append(str(temp.data))
            temp = temp.next
            if temp == self.head:
                break

        return " -> ".join(result) + " -> (head)"


# ==============================
# GUI Implementation
# ==============================

cll = CircularLinkedList()

root = tk.Tk()
root.title("Circular Linked List Simulator")
root.geometry("700x500")
root.configure(bg="#1f1f1f")

# ------- Title -------
title_label = tk.Label(
    root, 
    text="Circular Linked List Simulation (GUI)",
    font=("Helvetica", 20, "bold"), 
    fg="#00eaff",
    bg="#1f1f1f"
)
title_label.pack(pady=20)

# ------- Input Frame -------
input_frame = tk.Frame(root, bg="#1f1f1f")
input_frame.pack()

entry_value = tk.Entry(input_frame, font=("Helvetica", 16), width=20)
entry_value.grid(row=0, column=0, padx=10)

# ------- Button Styling -------
button_style = {
    "font": ("Helvetica", 14, "bold"),
    "bg": "#444444",              # visible dark gray
    "fg": "white",
    "activebackground": "#666666",
    "activeforeground": "white",
    "width": 14,
    "height": 1,
    "bd": 3,
    "relief": tk.RAISED,
    "highlightthickness": 2,
    "highlightbackground": "#00eaff"   # neon border for visibility
}


# ------- Functions -------
def update_output():
    output_box.config(state="normal")
    output_box.delete(1.0, tk.END)
    output_box.insert(tk.END, cll.display())
    output_box.config(state="disabled")

def insert_front():
    val = entry_value.get()
    if not val.isdigit():
        messagebox.showwarning("Invalid", "Please enter a number.")
        return
    cll.insert_front(int(val))
    update_output()

def insert_end():
    val = entry_value.get()
    if not val.isdigit():
        messagebox.showwarning("Invalid", "Please enter a number.")
        return
    cll.insert_end(int(val))
    update_output()

def delete_val():
    val = entry_value.get()
    if not val.isdigit():
        messagebox.showwarning("Invalid", "Please enter a number.")
        return
    if cll.delete_value(int(val)):
        update_output()
    else:
        messagebox.showerror("Error", "Value not found.")

def search_val():
    val = entry_value.get()
    if not val.isdigit():
        messagebox.showwarning("Invalid", "Please enter a number.")
        return
    found = cll.search(int(val))
    messagebox.showinfo("Search Result", "Value FOUND!" if found else "Value NOT found.")

def reset_list():
    cll.clear()
    update_output()

# ------- Buttons -------
button_frame = tk.Frame(root, bg="#1f1f1f")
button_frame.pack(pady=20)

btn1 = tk.Button(button_frame, text="Insert Front", command=insert_front, **button_style)
btn1.grid(row=0, column=0, padx=5)

btn2 = tk.Button(button_frame, text="Insert End", command=insert_end, **button_style)
btn2.grid(row=0, column=1, padx=5)

btn3 = tk.Button(button_frame, text="Delete Value", command=delete_val, **button_style)
btn3.grid(row=0, column=2, padx=5)

btn4 = tk.Button(button_frame, text="Search Value", command=search_val, **button_style)
btn4.grid(row=1, column=0, pady=10, padx=5)

btn5 = tk.Button(button_frame, text="Reset List", command=reset_list, **button_style)
btn5.grid(row=1, column=1, pady=10, padx=5)

# ------- Output Box -------
output_box = tk.Text(
    root, height=6, width=60, 
    font=("Courier", 14), 
    bg="#000000",
    fg="#00ff7f",
    state="disabled",
    bd=4,
    relief=tk.SUNKEN
)
output_box.pack(pady=10)

update_output()

root.mainloop()
