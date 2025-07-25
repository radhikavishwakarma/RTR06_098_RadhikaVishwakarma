// header files
//#include <GL/GLU.h>
#include <GL/glut.h>
#include <stdbool.h>

// global variable declarations
bool bFullScreen = GL_FALSE;

// Entry-point function
int main(int argc, char* argv[])
{
    // local function declarations
    int initialize(void);
    void resize(int, int);
    void display(void);
    void keyboard(unsigned char, int, int);
    void mouse(int, int, int, int);
    void uninitialize(void);

    // code
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Radhika Vishwakarma");

    initialize();

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    // glutWMCloseFunc(uninitialize);
    glutCloseFunc(uninitialize);

    glutMainLoop();

    return(0);
}

int initialize(void) {
    // code
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return(0);
}

void resize(int width, int height) {
    // code
    glViewport(0, 0, width, height);
}

void display(void) {
    // code
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    // code
    switch (key) {
    case 27:
        glutLeaveMainLoop();
        //  glutLeaveGameMode();
        break;
    case 'F':
    case 'f':
        if (bFullScreen == GL_FALSE) {
            glutFullScreen();
            bFullScreen = GL_TRUE;
        }
        else {
            glutLeaveFullScreen();
            //  glutLeaveGameMode();
            bFullScreen = GL_FALSE;
        }
        break;
    default:
        break;
    }
}

void mouse(int button, int state, int x, int y) {
    // code
    switch (button) {
        case GLUT_LEFT_BUTTON:
            glutLeaveMainLoop();
            // glutLeaveGameMode();
            break;
        default:
            break;
    }
}

void uninitialize(void) {
    // code
}