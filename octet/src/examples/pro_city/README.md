# Procedural City
![Alt text](https://dl.dropboxusercontent.com/u/4405475/proc_city.jpg?raw=true "Procedural City image")
### Introduction
In this assignment is planned a procedural city to be created as a scenery playground where player have to push a ball to other side avoiding buildings.
Buildings position and heigth will be randomly assigned according to a seed.  
A score will be recorded in Hall of Fame. This Hall of Fame is recorded in a text file and updated every time game ends. 
### Gameplay
After the city construction a ball is launched which is controlled by player force avoiding to collide with buildings and try to reach portal on city opposite side.
Every time player collides ball with a building initial score will be deducted by 100 points. If player reaches zero game ends.  
If player, by the other hand, passes under a portal game finishes and Hall of Fame is displayed with player score if he proofed worthy.  
### Development
#### 1. Engine class  
Keeps track of game machine state with Enumerated Type <enum State>. Also have a score variable that keeps an updated score until end of game.
#### 2. Sound class
Created a special class where all sounds used in the game are configurated. After sound initialization (<game_sounds.init_sound();>) sounds can easily be accessed as <game_sounds.playSoundEnd();>.
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
#### 7. Text
Is a simple implementation from "example_text.h" and allows on the left instructions for gameplay and on the left current score. When game ends Hall of Fame is shown in the middle.
#### 8. Player movement
Player movement is a simple implementation from "example_fps.h". Object was changed to <btCapsuleShape> because originates better collisions with ball.  
### Conclusion  
Initial approach of Voronoi for city creation was largely studied, but after some dead ends was abandoned due deadline approach. Unfortunately a lot of time was spent on this particular research that did not allowed other aspects to be developed.  
One of those aspects is collision. Despite logic is fully implemented when objects collision happens nothing was done in object properties to handle this information provided by Bullet library. A data structure of type <struct my_objects> was created getting different kind of data elementes under same name and stored via at <dynarray<my_objects> objects>. Later tried to move to a class <CollisionObjects.h> properties of bullet variables but was unfinished.  
Highscore is saved into file but its display in the end of the game is erratic.  
Compared with other works done on Octet this was still a progress due use of Enumareted Types on "engine.h" and a special class for sound play "sound.h".  
Enumerated type are very usefull to store state machine phases and allows a more readible code.  
For this work was done a video from gameplay that can be seen at https://youtu.be/YwrdltPwJLE and some pictures taken available in next chapter.  

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