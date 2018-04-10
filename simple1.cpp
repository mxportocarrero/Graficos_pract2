

#include "loadshader.h"
#define windowDIM 512

GLuint VertexArrayID;
GLuint VertexBuffer;
GLuint program1;

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};
/**
static const GLfloat g_vertex_buffer_data[] = {
   0.0f, 0.0f, 0.0f,
   10.0f, 0.0f, 0.0f,
   0.0f, 0.0f, 0.0f,
   0.0f, 10.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};**/

void init(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    program1 = LoadShaders("simple1.vshader","simple1.fshader");

    glGenVertexArrays(1,&VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(1,&VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(g_vertex_buffer_data),g_vertex_buffer_data,GL_STATIC_DRAW);

}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program1);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
    glVertexAttribPointer(
        0, // No particular reason por this, pero debe matchear al Layout en el Shader
        3, // Size
        GL_FLOAT,   //type
        GL_FALSE,   //normalized?
        0,  //stride
        (void*)0    // array buffer offset
    );

    //Dibujamos el triangulo
    glDrawArrays(GL_TRIANGLES,0,3);
    //glDrawArrays(GL_LINES,0,6);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();

    glFlush();
}

int main(int argc, char ** argv){

    /**INICIANDO OPENGL*/
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(windowDIM,windowDIM);
    glutInitContextVersion(3,0);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow("hola");

    if(glewInit()){
        cerr << "Unable to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }

    // funcion init()
    init();

    glutDisplayFunc(display);

    glutMainLoop();
}

