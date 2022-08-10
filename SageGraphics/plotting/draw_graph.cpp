#include "draw_graph.h" 

DrawGraph::DrawGraph() {
}

DrawGraph::~DrawGraph() {
}

int DrawGraph::createWindow(int argc, char**argv) {
//#ifdef __linux__
    //glfwInit();
    //// std::cout << "GL version : " << glGetString(GL_VERSION) << std::endl;
    //// std::cout << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    //GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); 
    //// FULL SCREEN
    //// GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr);
    //glfwMakeContextCurrent(window);
    //while(!glfwWindowShouldClose(window)) {
    //    glfwSwapBuffers(window);
    //    glfwPollEvents();
    //}
    //if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //    glfwSetWindowShouldClose(window, GL_TRUE);
    //glfwTerminate();
//#endif

    // glutInit(&argc, argv);
    // glutCreateWindow("OpenGL Test"); 
    // glutInitWindowSize(640, 480);  
    // glutInitWindowPosition(50, 50);
    // glutDisplayFunc(display);
    // glutMainLoop(); 
    return 0;
}

void DrawGraph::display() {
#if defined(__linux__) && defined(USE_GLUT)  
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f);   
        glVertex2f( 0.5f, -0.5f);
        glVertex2f( 0.5f,  0.5f);
        glVertex2f(-0.5f,  0.5f);
    glEnd();
    // RENDER
    glFlush();
#endif
}