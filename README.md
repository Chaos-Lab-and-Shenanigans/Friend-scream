# **Friend Scream Simulator**  

![icon](https://github.com/404errorg6/Friend-scream/blob/main/fs.ico)
## **Introduction**

Ever wanted to test your friend's patience? Or maybe you just enjoy watching chaos unfold on their desktop? **Friend Scream Simulator** is here to make dreams come true!

Simply put:

* Enter your *lucky number*.
* Sit back and watch as your friend's desktop gets *graced* with dozens (or hundreds) of cursed folders ending with a dot `.` — those infamous little demons that Windows hates to delete.
* Oh, and inside each cursed folder? A friendly little note: **"You're totally fked!"**.

Feeling merciful? Enter the **secret code** to **undo the madness**.
Otherwise, *good luck deleting them manually*.

**Disclaimer:** This tool is for **fun and pranks**. Do not use it on people who can’t take a joke. Also, don’t blame us if your friend stops talking to you.

---

## **Easy tweaks**
- Change the limit variable's value to change the max limit easily.
- Change the delay variable's value to change the speed at which folders spawn and delete(It is slow as to not be detetcted as a virus).
### **Cautions**
Spawning too many folders on desktop might delete the existing ones.  


300 is safe limit tested on my side only  
- **Lower** it if you want to be extra **safe**.
- **Increase** it if you want **absolute destruction**.


  
## **How It Works**

* Creates a user-specified number of **cursed folders** on the desktop.
* Each folder name ends with a `.` making it harder to delete through normal means.
* Each folder contains a text file named `amogus.txt` with a message inside.(pretty useless since you can't open folders)
* Creates a file at the end, named "**Delete-Desktop-Folders.txt**" but it doesn't contain the secret code. What does it contain? Only one way to find out! 
* Includes a **GUI prompt** for user input.
* Special code `-143` deletes all previously created cursed folders.

## **Files purpose**

* **fs.ico**: The file containing the exe's icon and also title icon. You can convert any image file to ico format using online converters.
* **resource.h**: This file contains the constant values.
* **resource.rc**: Contains the layout and settings of windows that are displayed.
* **FriendScream**: Contains the whole logic using above files.

You can also easily change icon using this [tool]()


### **Requirements**

* Windows OS
* A sense of humor
* C Compiler (e.g., MSVC or MinGW)

### **Compilation**

Compile with any C compiler that supports Windows API:

```bash
windres resource.rc -O coff -o resource.res
gcc FriendScream.c resource.res -o FriendScream.exe -mwindows
```

### **Usage**

1. Run the executable.
2. Enter a number between `10` and `300` in the prompt.
3. Watch as that many cursed folders appear on your desktop.
4. To remove them, run the program again and enter `-143`.

## **Why This Exists**

Because life is too short not to make your friends question their sanity.

---


## **Extra Fun Prank Ideas**

* **Set Delay Higher**
  Increase the `delay` value in the code so the folders appear slowly… one… by… one… like a horror movie.

* **Change Message Inside Txt File**
  Replace with something spooky like:
  *“I’m watching you…”* or
  *“Run `-143` before midnight or else…”*.

* **Make It a Chain Reaction**
  Instead of deleting with `-143`, make it create **double the folders** when they try to remove them. Pure chaos mode.

* **Rename the Program**
  Name the executable something innocent like `GameSetup.exe` or `FreeRobux.exe` and send it to your friends (with their consent!).

* **Infinite Mode (Extreme)**
  Modify the loop to **never stop creating folders** until system crashes. (Not recommended unless you really hate someone or you’re running in a VM).

* **Desktop Wallpaper Swap**
  Combine this with a script that changes their wallpaper to something like *"AMOGUS IS COMING"* after the folders appear.







