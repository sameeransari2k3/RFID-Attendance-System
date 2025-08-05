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
├── header.h # Common definitions and struct declarations
│
├── sync.c # Loads users and previous attendance from files
├── save.c # Saves attendance and user data to files
├── admin_fun.c # Admin functions: Add, Edit, Delete, View
├── my_AtoF.c # Custom ASCII to Float converter
├── in_out_time.c # IN and OUT time logic + working hours calc
├── project_main.c # Main program: UART receive, IN/OUT logic, signal handler
│
├── lcd.c # LCD display driver for ARM (LPC2148)
├── rtc.c # Real-time clock (RTC) configuration
├── uart0.c # UART0 init and transmit logic (PC communication)
├── uart1.c # UART1 for RFID reader serial communication
├── extint.c # External interrupt logic (optional for RFID)
│
├── database.txt # Stores registered users
├── attendance.csv # Daily attendance data (IN/OUT, working hours)
│
├── Makefile # Compilation instructions for all host-side source files
└── README.md # Project overview and documentation

## 📌 Sample Output (CSV)
ID,Name,Date,Working_hour,IN,OUT

101,Sameer,29-07-2025,8.00,09:00,17:00

102,Gousia,29-07-2025,7.50,09:15,16:45


