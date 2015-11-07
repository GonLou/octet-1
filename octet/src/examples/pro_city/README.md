# Procedural City

![Alt text](https://dl.dropboxusercontent.com/u/4405475/proc_city.jpg?raw=true "Procedural City image")

In this assignment is planned to create a procedural city. This is a sceneray playground where player have to bring a ball to other side avoiding buildings.
Buildings heigth will be random (a neighbour calculation will ensure not great heigth discrepancies).  
After the city construction a ball is launched and by the force of player (controlled by arrow keys) avoid to collide with buildings.  
A score will be recorded in Hall of Fame. This HoF is recorded in a text file and readed every time game ends.  
### Shader 
A tiny change was made on file "default_solid_transparent.fs" where alpha was changed from 1.0f to 0.3f just to make ball seem like a transparent plastic ball.  
### Screen Ratio 
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


## Internet Resources
- **Fortune’s algorithm and implementation**, by Algorithms and Stuff, in the http://blog.ivank.net/fortunes-algorithm-and-implementation.html
- **How to: Draw the Voronoi Diagram**, by sevesixfive, in the http://765.blogspot.co.uk/2009/09/how-to-draw-voronoi-diagram.html
- **How to check if two given line segments intersect?**, by Geeks for Geeks, in the http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

**Goncalo Lourenco**  
October/November 2015

