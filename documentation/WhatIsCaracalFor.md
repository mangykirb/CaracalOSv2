# As you might have read the title of this file,

CaracalOSv2 is a new operating system, this time actually better than the older one because i planned stuff out instead of adding crap blindly.
This is still GNU GPL V3, so yeah :D

This got inspired by the Raise a Floppa game on Roblox, especially the new updates like the halloween one! 🎃

*How to use CaracalOS*:
Simple! 
1. Make sure you use WSL or linux!
2. Enter wsl 
3. Run './build.sh' (make sure you use sudo install for the specific libraries)
4. Done!
Reminder: to install wsl on windows, enter the terminal as Admin and type wsl --Install Ubuntu, the rest is up to you.

*How to NOT use CaracalOS*:
Firstly, type in the terminal 'crash'
This divides 1/0, but generally the IDT stops them. To remove IDT, type
'crash noidt'
AND now you know why is this really sussy :0

*How does it work?*:
Great question!
So, it works *specially* because i dont use the .h files every time and one .c file, instead, there are more C files like in kernel/cpu
NO GUYS IT CANT RUN DOOM BTW ITS TOO SMALL TO FUNCTION WELL!!!!!
anyways, im planning to launch it on real hardware somehow... you know, an OLD pc with legacy BIOS and some old linux version and we are good to go
But, getting into the question, there are many folders in kernel.
/asm:
    Assembly crap, C doesnt have enough control although i could use the asm() in C, but it feels cleaner making the files. Specifically display.asm, that does VGA mode 13h, and user.asm for ring 3 (it works :0)
/include/cpu:
            CPU crap again, usual. Such ass: IDT.h, GDT.H, paging.h
