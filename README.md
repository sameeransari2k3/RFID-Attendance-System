# RFID-Attendance-System
A real-time RFID-based employee attendance system using UART communication and C language with CSV logging and admin management features

This project is a **microcontroller-interfaced Attendance Management System** developed in C. It uses **RFID cards**, **UART communication**, and system time to mark IN/OUT attendance for each user and stores data in `.csv` and `.txt` files for further tracking and analysis.

---

## 🚀 Features

- ✅ RFID-based user identification  
- ✅ Real-time attendance tracking (IN/OUT logic with working hours calculation)  
- ✅ UART-based communication with external devices  
- ✅ Dynamic memory allocation for storing user and attendance data  
- ✅ Admin menu (Add, Edit, Delete, View Users)  
- ✅ Daily CSV attendance logging with system time  
- ✅ File-based sync of data on program exit (via signal interrupt)

---

## 🛠️ Technologies Used

- **Language**: C (GCC Compiler)  
- **Platform**: Linux (Ubuntu / Raspberry Pi supported)  
- **Microcontroller**: LPC2148 (ARM7)  
- **Communication**: UART  
- **File Formats**: `.csv` for attendance, `.txt` for database

---

## 📂 File Structure

