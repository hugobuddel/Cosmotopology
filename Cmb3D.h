#ifndef CMB3D_H_
#define CMB3D_H_
#include "Cmb.h"
#include "SDL.h"
#include <GL/gl.h>

class Cmb3D
{
public:
    Cmb3D();
    virtual ~Cmb3D();

    void setcmb(Cmb* pcmb);
    void start();

private:
    // distance of scene to the camera
    float distance;
    
    // rotation of the scene
    float blorot[3];
    
    // Cartesian offset of the scene
    float ofs[3];
    
    // Directions in which to show multible copies of the CMB.
    // Last direction is usually the most interesting.
    int   show[3];
    
    // Show aitoff projection?
    int   mFlat;
    
    // the complete cmb
    Cmb * cmb;
    
    /* This is our SDL surface */
    SDL_Surface *surface;

    /* Flags to pass to SDL_SetVideoMode */
    int videoFlags;
    
    /* main loop variable */
    int done;
    
    /* used to collect events */
    SDL_Event event;
    
    /* this holds some info about our display */
    const SDL_VideoInfo *videoInfo;
    
    /* whether or not the window is active */
    int isActive;

    // Handle keyboard events
    void handleKeyPress(SDL_keysym *keysym);
    
    // Draw the CMB as spheres
    void drawCMB(Cmb * pcmb);
    
    // Draw Aitoff projection of CMB
    void drawCMBFlat(Cmb * pcmb);
    
    // Draw the scene
    int drawGLScene();
    
    // Resize hook
    int resizeWindow(int width, int height);
    
    // Start hook
    int initGL();
    
    // Quit hook
    void Quit(int returnCode);

    // Private copy constructor
    Cmb3D(const Cmb3D&);
    
};

#endif /*CMB3D_H_*/
