#include "Cmb3D_i.h"

void Cmb3D::setcmb(Cmb* pcmb){
    // setting the CMB
    cmb = pcmb;    
    // setting the ofsets
    for (int i=0; i<3; i++)
        ofs[i] = cmb->L(i) / cmb->LSS();
}

/* general OpenGL initialization function */
int Cmb3D::initGL(  )
{
    /* initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(
            stderr,
            "Video initialization failed: %s\n",
            SDL_GetError()
        );
        Quit(1);
    }

    /* Fetch the video info */
    videoInfo = SDL_GetVideoInfo();

    if (!videoInfo) {
        fprintf(
            stderr,
            "Video query failed: %s\n",
            SDL_GetError()
        );
        Quit(1);
    }

    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          // Enable OpenGL in SDL
    videoFlags |= SDL_GL_DOUBLEBUFFER; // Enable double buffering
    videoFlags |= SDL_HWPALETTE;       // Store the palette in hardware
    videoFlags |= SDL_RESIZABLE;       // Enable window resizing

    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
        videoFlags |= SDL_HWSURFACE;
    else
        videoFlags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if (videoInfo->blit_hw)
        videoFlags |= SDL_HWACCEL; // Is this useful anyway?

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /* Get a SDL surface */
    surface = SDL_SetVideoMode(
        SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
        videoFlags
    );

    if (!surface) {
        fprintf(
            stderr, 
            "Video mode set failed: %s\n",
            SDL_GetError()
        );
        Quit(1);
    }

    /* Enable smooth shading */
    glShadeModel(GL_SMOOTH);

    /* Set the background */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    /* Depth buffer setup */
    glClearDepth(1.0f);

    /* Enables Depth Testing */
    glEnable(GL_DEPTH_TEST);

    /* The Type Of Depth Test To Do */
    glDepthFunc(GL_LEQUAL);

    /* Really Nice Perspective Calculations */
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    return( TRUE );
}

/* function to release/destroy our resources and restoring the old desktop */
void Cmb3D::Quit( int returnCode )
{
    /* clean up the window */
    done = TRUE;
    isActive = FALSE;
    SDL_Quit();

    /* and exit appropriately */
    exit( returnCode );
}

/* function to reset our viewport after a window resize */
int Cmb3D::resizeWindow( int width, int height )
{
    /* Height / width ration */
    GLfloat ratio;

    /* Protect against a divide by zero */
    if (height == 0) height = 1;

    ratio = (GLfloat)width / (GLfloat)height;

    /* Setup our viewport. */
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* Set our perspective */
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode(GL_MODELVIEW);

    /* Reset The View */
    glLoadIdentity();

    return(TRUE);
}

Cmb3D::Cmb3D()
{
    // Pointers to be initialized later.
    cmb = 0;
    surface = 0;
    videoFlags = 0;
    videoInfo = 0;

    // We're not yet done.
    done = FALSE;
    isActive = FALSE;

    // rotate so yz plane is parallel to the screen
    blorot[0]=0.0;
    blorot[1]=0.0;
    blorot[2]=0.25;
    
    ofs[0]=0.64102564102564097;
    ofs[1]=0.64102564102564097;
    ofs[2]=2*0.64102564102564097;

    // show CMB copies in the z direction (often most interresting)
    show[0]=0;
    show[1]=0;
    show[2]=1;

    distance = -3.3;

    // Show Aitoff projection
    mFlat = 0;
}

/* function to handle key press events */
/* Commented out are Dvorak key-codes. */
void Cmb3D::handleKeyPress( SDL_keysym *keysym )
{
    switch ( keysym->sym )
    {
    case SDLK_ESCAPE:
        /* ESC key was pressed */
        Quit(0);
        break;
    case SDLK_F1:
        /* F1 key was pressed
         * this toggles fullscreen mode
         */
        SDL_WM_ToggleFullScreen(surface);
        break;
    // case '\'':
    case 'q':
        /* Rotate along x-axis */
        blorot[0] += 0.01;
        break;
    // case 'a':
    case 'a':
        /* Rotate back along x-axis */
        blorot[0] -= 0.01;
        break;
    // case ',':
    case 'w':
        /* Rotate along y-axis */
        blorot[1] += 0.01;
        break;
    // case 'o':
    case 's':
        /* Rotate back along y-axis */
        blorot[1] -= 0.01;
        break;
    // case '.':
    case 'e':
        /* Rotate along z-axis */
        blorot[2] += 0.01;
        break;
    // case 'e':
    case 'd':
        /* Rotate back along y-axis */
        blorot[2] -= 0.01;
        break;
    // case 'p':
    case 'r':
        /* Increase distance */
        distance += 0.1;
        break;
    // case 'u':
    case 'f':
        /* Decrease distance */
        distance -= 0.1;
        break;

    // case 'g':
    case 'u':
        /* Change x-axis offset */
        ofs[0] += 0.02;
        break;
    // case 'h':
    case 'j':
        /* Change x-axis offset */
        ofs[0] -= 0.02;
        break;
    // case 'm':
    case 'm':
        /* Toggle showing CMB copies along x-axis */
        show[0] = !show[0];
        break;

    // case 'c':
    case 'i':
        /* Change y-axis offset */
        ofs[1] += 0.02;
        break;
    // case 't':
    case 'k':
        /* Change y-axis offset */
        ofs[1] -= 0.02;
        break;
    // case 'w':
    case ',':
        /* Toggle showing CMB copies along y-axis */
        show[1] = !show[1];
        break;

    // case 'r':
    case 'o':
        /* Change z-axis offset */
        ofs[2] += 0.02;
        break;
    // case 'n':
    case 'l':
        /* Change z-axis offset */
        ofs[2] -= 0.02;
        break;
    // case 'v':
    case '.':
        /* Toggle showing CMB copies along z-axis */
        show[2] = !show[2];
        break;

    // case 'f':
    case 'y':
        /* Toggle showing Aitoff projection */
        cout << "setting mFlat from " << mFlat;
        mFlat = !mFlat;
        cout << " to " << mFlat << endl;
        break;

    // case ';':
    case 'z':
        // print values:
        for (int i=0; i<3; i++)
            cout << "show[" << i << "]=" << show[i] << endl;
        for (int i=0; i<3; i++)
            cout << "ofs[" << i << "]=" << ofs[i] << endl;
        for (int i=0; i<3; i++)
            cout << "blorot[" << i << "]=" << blorot[i] << endl;
        cout << "distance=" << distance << endl;
        break;
    
    case ' ':
        done = TRUE;
        break;
    case 'x':
        done = TRUE;
        break;
    
    default:
        break;
    }

    return;
}

void Cmb3D::drawCMB(Cmb * pcmb)
{
    glBegin(GL_TRIANGLES);

    float cmax = pcmb->cmbdata().maximum();
    float cmin = pcmb->cmbdata().minimum();
    float cdif = cmax-cmin;
    

    for (Cmbdata::iterator cmbEl = pcmb->cmbdata().begin();
         cmbEl != pcmb->cmbdata().end();
         ++cmbEl)
    {
        float x1,x2,x3,x4,y1,y2,y3,y4,z1,z2,z3,z4,c1,c2,c3,c4;
        c1 = real(cmbEl->temperature);
        x1 = cmbEl->coordinate.cartesianX();
        y1 = cmbEl->coordinate.cartesianY();
        z1 = cmbEl->coordinate.cartesianZ();

        c2 = real(cmbEl->triangle[0]->temperature);
        x2 = cmbEl->triangle[0]->coordinate.cartesianX();
        y2 = cmbEl->triangle[0]->coordinate.cartesianY();
        z2 = cmbEl->triangle[0]->coordinate.cartesianZ();

        c3 = real(cmbEl->triangle[1]->temperature);
        x3 = cmbEl->triangle[1]->coordinate.cartesianX();
        y3 = cmbEl->triangle[1]->coordinate.cartesianY();
        z3 = cmbEl->triangle[1]->coordinate.cartesianZ();

        c4 = real(cmbEl->triangle[2]->temperature);
        x4 = cmbEl->triangle[2]->coordinate.cartesianX();
        y4 = cmbEl->triangle[2]->coordinate.cartesianY();
        z4 = cmbEl->triangle[2]->coordinate.cartesianZ();

        glColor3f( (c1-cmin)/cdif, 0, 1-(c1-cmin)/cdif); 
        glVertex3f(x1, y1, z1);
        glColor3f( (c2-cmin)/cdif, 0, 1-(c2-cmin)/cdif); 
        glVertex3f(x2, y2, z2);
        glColor3f( (c3-cmin)/cdif, 0, 1-(c3-cmin)/cdif); 
        glVertex3f(x3, y3, z3);

        glColor3f( 0,1,1 );
        glColor3f( (c1-cmin)/cdif, 0, 1-(c1-cmin)/cdif); 
        glVertex3f(x1, y1, z1);
        glColor3f( (c4-cmin)/cdif, 0, 1-(c4-cmin)/cdif); 
        glVertex3f(x4, y4, z4);
        glColor3f( (c3-cmin)/cdif, 0, 1-(c3-cmin)/cdif); 
        glVertex3f(x3, y3, z3);

    }
    glEnd();
}

void Cmb3D::drawCMBFlat(Cmb * pcmb)
{
    glPushMatrix();
    glTranslatef(0.9, 0, 0);

    glBegin(GL_TRIANGLES);

    float cmax = pcmb->cmbdata().maximum();
    float cmin = pcmb->cmbdata().minimum();
    float cdif = cmax-cmin;
    

    for (Cmbdata::iterator cmbEl = pcmb->cmbdata().begin();
        cmbEl != pcmb->cmbdata().end();
        ++cmbEl)
    {
        float x1,x2,x3,x4,y1,y2,y3,y4,z1,z2,z3,z4,c1,c2,c3,c4;
        c1 = real(cmbEl->temperature);
        x1 = 0;
        //z1 = cmbEl->coordinate.sphericalAzimuth();
        //y1 = cmbEl->coordinate.sphericalZenith();
        z1 = cmbEl->coordinate.sphericalAitoffX();
        y1 = cmbEl->coordinate.sphericalAitoffY();
        
        c2 = real(cmbEl->triangle[0]->temperature);
        x2 = 0;
        //z2 = cmbEl->triangle[0]->coordinate.sphericalAzimuth();
        //y2 = cmbEl->triangle[0]->coordinate.sphericalZenith();
        z2 = cmbEl->triangle[0]->coordinate.sphericalAitoffX();
        y2 = cmbEl->triangle[0]->coordinate.sphericalAitoffY();

        c3 = real(cmbEl->triangle[1]->temperature);
        x3 = 0;
        //z3 = cmbEl->triangle[1]->coordinate.sphericalAzimuth();
        //y3 = cmbEl->triangle[1]->coordinate.sphericalZenith();
        z3 = cmbEl->triangle[1]->coordinate.sphericalAitoffX();
        y3 = cmbEl->triangle[1]->coordinate.sphericalAitoffY();

        c4 = real(cmbEl->triangle[2]->temperature);
        x4 = 0;
        //z4 = cmbEl->triangle[2]->coordinate.sphericalAzimuth();
        //y4 = cmbEl->triangle[2]->coordinate.sphericalZenith();
        z4 = cmbEl->triangle[2]->coordinate.sphericalAitoffX();
        y4 = cmbEl->triangle[2]->coordinate.sphericalAitoffY();

        // some wrap arround, screwing the whole thing up, only draw small triangles
        if( (abs(y1-y2) < 0.1) && (abs(y2-y3) < 0.1) && (abs(y3-y4) < 0.1) &&
            (abs(z1-z2) < 0.1) && (abs(z2-z3) < 0.1) && (abs(z3-z4) < 0.1) &&
            1) { // HACK
            glColor3f( (c1-cmin)/cdif, 0, 1-(c1-cmin)/cdif); 
            glVertex3f(x1, y1, z1);
            glColor3f( (c2-cmin)/cdif, 0, 1-(c2-cmin)/cdif); 
            glVertex3f(x2, y2, z2);
            glColor3f( (c3-cmin)/cdif, 0, 1-(c3-cmin)/cdif); 
            glVertex3f(x3, y3, z3);

            glColor3f( 0,1,1 );
            glColor3f( (c1-cmin)/cdif, 0, 1-(c1-cmin)/cdif); 
            glVertex3f(x1, y1, z1);
            glColor3f( (c4-cmin)/cdif, 0, 1-(c4-cmin)/cdif); 
            glVertex3f(x4, y4, z4);
            glColor3f( (c3-cmin)/cdif, 0, 1-(c3-cmin)/cdif); 
            glVertex3f(x3, y3, z3);
        }

    }
    glEnd();

    glPopMatrix();
}

/* Here goes our drawing code */
int Cmb3D::drawGLScene(  )
{
    /* These are to calculate our fps */
    static GLint T0     = 0;
    static GLint Frames = 0;

    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* Move Left 1.5 Units And Into The Screen 6.0 */
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, distance );

    // rotate cmb for right direction
    glRotatef(360. * blorot[1], 1.0f, 0.0f, 0.0f);
    glRotatef(360. * blorot[2], 0.0f, 1.0f, 0.0f);
    glRotatef(-360. * blorot[0], 0.0f, 0.0f, 1.0f);

    Cmb::topologies topo = cmb->topology();

    if(!mFlat) {
        glBegin(GL_LINES);
        
        // set foreground color, assuming background is black
        glColor3f( 0.0, 0.0, 0.0);
        
        glVertex3f(-2.0f, 0.0f, 0.0f);
        glVertex3f( 2.0f, 0.0f, 0.0f);
        glVertex3f( 0.0f,-2.0f, 0.0f);
        glVertex3f( 0.0f, 2.0f, 0.0f);
        glVertex3f( 0.0f, 0.0f,-2.0f);
        glVertex3f( 0.0f, 0.0f, 2.0f);
        glEnd();
    }

    if(mFlat)
        drawCMBFlat(cmb);
    else
        drawCMB(cmb);
        
    for (int shox=0; shox<=2; shox++)
    {
        if (show[shox]){
            for (int t=-1; t<2; t++) {
                if (t != 0) {
                    glPushMatrix();

                    switch(shox) {
                    case 0:
                        switch (topo) {
                        case Cmb::TORUS:
                        case Cmb::HALFTURN:
                        case Cmb::QUARTERTURN:
                        case Cmb::THIRDTURN:
                        case Cmb::SIXTHTURN:
                            glTranslatef( ofs[0]*t, 0.0f, 0.0f);
                            break;
                        case Cmb::HANTZSCHEWENDT:
                            glTranslatef( ofs[0]/2*t, ofs[1]/2*t, 0.0f);
                            if (t!=0){
                                glRotatef(180.,1.0f,0.0f,0.0f);
                            }
                            break;
                        default:
                            break;
                        } // switch topo
                        break;
                    case 1:
                        switch (topo) {
                        case Cmb::TORUS:
                        case Cmb::HALFTURN:
                        case Cmb::QUARTERTURN:
                            glTranslatef( 0.0f, ofs[1]*t, 0.0f);
                            break;
                        case Cmb::THIRDTURN:
                        case Cmb::SIXTHTURN:
                            glTranslatef( ofs[1]/2*t, ofs[1]*sqrt(3)/2*t, 0.0f);
                            break;
                        case Cmb::HANTZSCHEWENDT:
                            glTranslatef( 0.0f, ofs[1]/2*t, ofs[2]/2*t);
                            if (t!=0){
                                glRotatef(180.,0.0f,1.0f,0.0f);
                            }
                            break;
                        default:
                            break;
                        } // switch topo
                        break;
                    case 2:
                        switch (topo) {
                        case Cmb::TORUS:
                            glTranslatef( 0.0f, 0.0f, ofs[2]*t);
                            break;
                        case Cmb::HALFTURN:
                            glTranslatef( 0.0f, 0.0f, ofs[2]*t/2);
                            if (t!=0){
                                glRotatef(180.,0.0f,0.0f,1.0f);
                            }
                            break;    
                        case Cmb::QUARTERTURN:
                            glTranslatef( 0.0f, 0.0f, ofs[2]*t/4);
                            if (t!=0){
                                glRotatef(t*90.,0.0f,0.0f,1.0f);
                            }
                            break;    
                        case Cmb::THIRDTURN:
                            glTranslatef( 0.0f, 0.0f, ofs[2]*t/3);
                            if (t!=0){
                                glRotatef(t*120.,0.0f,0.0f,1.0f);
                            }
                            break;    
                        case Cmb::SIXTHTURN:
                            glTranslatef( 0.0f, 0.0f, ofs[2]*t/6);
                            if (t!=0){
                                glRotatef(t*60.,0.0f,0.0f,1.0f);
                            }
                            break;    
                        case Cmb::HANTZSCHEWENDT:
                            glTranslatef( ofs[1]/2*t, 0.0f, ofs[2]/2*t);
                            if (t!=0){
                                glRotatef(180.,0.0f,0.0f,1.0f);
                            }
                            break;

                        default:
                            break;
                        } // switch topo
                        break;
                    } // switch shox     

                    if(!mFlat)
                        drawCMB(cmb);
                    glPopMatrix();

                } // if t
            } // for t
        } // if show[shox]
    } // for shox

    /* Draw it to the screen */
    SDL_GL_SwapBuffers();

    return(TRUE);
}

Cmb3D::~Cmb3D()
{
    Quit(0);
}

void Cmb3D::start()
{
    /* initialize OpenGL */
    initGL( );

    /* resize the initial window */
    resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    /* main loop variable */
    done = FALSE;

    /* whether or not the window is active */
    isActive = TRUE;
    
    /* wait for events */
    while (!done) {
        /* handle the events in the queue */

        while (SDL_PollEvent(&event)) {
            switch( event.type ) {
            case SDL_ACTIVEEVENT:
                /* Something's happend with our focus
                 * If we lost focus or we are iconified, we
                 * shouldn't draw the screen
                 */
                 // does this even work ?
                if ( event.active.gain == 0 )
                    isActive = TRUE; //FALSE;
                else
                    isActive = TRUE;
                break;                
            case SDL_VIDEORESIZE:
                /* handle resize event */
                surface = SDL_SetVideoMode( event.resize.w,
                            event.resize.h,
                            16, videoFlags );
                if ( !surface )
                {
                    fprintf( stderr, "Could not get a surface after resize: %s\n", SDL_GetError( ) );
                    Quit( 1 );
                }
                resizeWindow( event.resize.w, event.resize.h );
                break;
            case SDL_KEYDOWN:
                /* handle key presses */
                handleKeyPress( &event.key.keysym );
                break;
            case SDL_QUIT:
                /* handle quit requests */
                cout << "Got quit event"<<endl;
                done = TRUE;
                break;
            default:
                break;
            } // switch event.type
        } // while poll event
        
        /* draw the scene */
        if (isActive) {
            drawGLScene();
        }
    } // while !done
}
