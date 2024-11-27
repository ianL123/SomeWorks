## The commands needed to compile and run the program.
`gcc -o ccp main.c graphics.c`
`./ccp | java -jar .\drawapp-2.0.jar`

## Note
All the positions of home, blocks and markers are typed in the source code. If numbers of blocks or markers are changed, the constant numMark or numBlock should also be changed.  
All the positions finally used of markers and blocks are stored in   
`int grid[9][9]`  
Positions of the home and the robot are stored in the struct directly.  
`struct position robot`  
`struct position home`  
After picked up all the markers, the robot will go back to home according to the former steps.  
`int movementRecord[100]`  
Another integer defined to record the number of steps and it also become a pointer so there is no C-pointer used in the code.  

