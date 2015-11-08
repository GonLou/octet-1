# Procedural City
![Alt text](https://dl.dropboxusercontent.com/u/4405475/proc_city.jpg?raw=true "Procedural City image")
### Introduction
In this assignment is planned to create a procedural city as a scenery playground where player have to push a ball to other side avoiding buildings.
Buildings heigth will be placed as random according to seed.  
A score will be recorded in Hall of Fame. This HoF is recorded in a text file and updated every time game ends. 
### Gameplay
After the city construction a ball is launched which is controlled by player force avoiding to collide with buildings and try to reach portal on city opposite side.  
### Development
#### 1. Engine class  
Keeps track of game machine state with Enumerated Type <enum State>.  
#### 2. Sound class
Created a special class where all sounds used in the game are configurated.
#### 3. Game messages
Game messages are holded in <const char* content[200]>. This was early adopted due its simplicity of usage. A more efficient way would be if these messages were in a text file and loaded into memory.  
#### 4. Highscore
When player finishes a game his score is submited and compared with previous scores on database. If score is bigger that any of scores it will add new score on correct position and remove last player name and score which corresponds to last line from text file.
#### 5. Shader 
A tiny change was made on file "default_solid_transparent.fs" where alpha was changed from 1.0f to 0.3f just to make ball seem like a transparent plastic ball.  
#### 6. Screen Ratio 
Window was changed to aspect ratio currently used and added a customized title game name. File modified was "windows_specific.h" and code is as follows:  
{ . . . . } // just to present a window with the same ratio  
void GetDesktopResolution(int &horizontal , int &vertical ) {  
RECT desktop; // Get a handle to the desktop window  
const HWND hDesktop = GetDesktopWindow();  
// Get the size of screen to the variable desktop  
GetWindowRect(hDesktop, &desktop );  
horizontal = desktop. right ;  
vertical = desktop.bottom;   
}  
{ . . . }   
// to center the window with a ratio   
int horizontal_screen = 0; int vertical_screen = 0; const double ratio_screen = 0.80;  
GetDesktopResolution(horizontal_screen , vertical_screen );  
window_handle = CreateWindowW(L"MyClass" , L"Procedural City" , WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, (( int)horizontal_screen*ratio_screen ) , (( int)vertical_screen*ratio_screen ) , NULL, NULL, wndclass.hInstance , (LPVOID)this );  
### Conclusion  
Initial approach of Voronoi for city creation was largely studied, but after some dead ends was abandoned. Unfortunately a lot of time was spent on this particular research that did not allowed other aspects to be approached.  
One of those aspects is collision. Despite logic is implemented for detection of objects collision nothing was done in object properties. A data structure of type <struct my_objects> was created getting different kind of data elementes under same name and stored via at <dynarray<my_objects> objects>. Later tried to move to a class <CollisionObjects.h> properties of bullet variables but was uncompleted.  
Highscore is saving into file but its display is erratic.  
Compared with other works done on Octet this was still a progress due use of Enumareted Types on "engine.h" and a special class for sound play. 
These enumerated type are very usefull in a state machine and allows a more readible code.  
Video can be seen at https://youtu.be/YwrdltPwJLE .  

## Images
![Alt text](https://dl.dropboxusercontent.com/u/4405475/proc_city01.jpg?raw=true "Procedural City early image")
![Alt text](https://dl.dropboxusercontent.com/u/4405475/proc_city02.jpg?raw=true "Procedural City game playing")
![Alt text](https://dl.dropboxusercontent.com/u/4405475/proc_city03.jpg?raw=true "Procedural City highscore")

## Internet Resources
- **Fortune’s algorithm and implementation**, by Algorithms and Stuff, in the http://blog.ivank.net/fortunes-algorithm-and-implementation.html  
- **How to: Draw the Voronoi Diagram**, by sevesixfive, in the http://765.blogspot.co.uk/2009/09/how-to-draw-voronoi-diagram.html  
- **How to check if two given line segments intersect?**, by Geeks for Geeks, in the http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/  
- **C++**, by cplusplus, in the http://www.cplusplus.com/  
- **Stackoverflow**, by multi authors, in the http://stackoverflow.com/

**Goncalo Lourenco**  
October/November 2015

