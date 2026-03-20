# HexGameOfLifeProject
A cellular automata on an hexagonal grid I made with the intent to find a rule that can mimic a glider.


this project uses the raylib library to function

to change the settings like the rules or the starter seed, simply modify the argumentsPatternDetect.txt

to compile use the command "bash patternDetectVisual.bash" if you're on linux, if you're on Windows, un-comment the start of the file and comment de second part

I did not do a final clean because this was supposed to be a personal projet, so I left it in a dirty state, but it works

Also it was my first big project, so I didn't use git, so no history, for my next projets I will be using git


the TIPEvisu.out is compiled for linux, if you put it in the same folder as argumentsPatternDetect.txt it should work out of the box

controls :
1 - go back a step
2 - go forward a step
3 - toggle so that 1 and 2 are continous and not single step
4 - run the simulation, press again to pause
0 - reset camera zoom and position
left click - switch a cell state
right click - move camera
mouse wheel - zoom
e - switch cell state in a gear pattern, was used to test things

