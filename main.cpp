/**
PRACTICA NRO - 2: Animaciones y efectos con Shaders
Autor: Max W. Portocarrero
**/

/**
MATRICES PARA SITUAR LA CAMARA
VRP -> (7,3,-10,1)file:///home/maxito911/Desktop/Graficos/practica-2/Source-Files/rotate-cube-new.cpp

VPN -> (-7,-3,10,0)
VUP -> (0,1,0,0)
**/



#include "sphere.h"

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

/**VARIABLES GLOBALES**/
const char *filename;
Sphere *sphere;

/**Variables OpenGL*/
enum VAO_IDs{mySphere,extras,NumVAOs};;
enum Buffer_IDs{ArrayBuffer,NormalBuffer,extraBuffer,NumBuffers};
enum Attrib_IDs{vPos = 0,nPos = 1};
enum eAttrib_IDs{eColor = 0, ePos = 1};

GLuint program1,program2;       /* shader program object id */

/**VARIABLES PARA LA ANIMACION**/
enum Path_Points{A,B,C};
enum Paths_IDs{A_to_B,B_to_C,C_to_A}; int currentPath = 0;
vec3 Paths[3] = {vec3(3.0f,1.0f,5.0f),
            vec3(-1.0f,1.0f,-4.0f),
            vec3(3.5f,1.0f,-2.5f),
            };
float t = 0.0f;
float angle = 0.0f;

/**VARIABLES PARA ILUMINACION**/
mat4 Model,View,Projection,MVP,VP,M,V,mv;
mat3 NormalMatrix;

glm::vec4  LightPos;
vec3 Ld =  vec3(1.0f, 1.0f, 1.0f);
vec3 La = vec3(0.4f, 0.4f, 0.4f);
vec3 Ls = vec3(1.0f, 1.0f, 1.0f);

vec3 Kd = vec3(0.9f,0.5f,0.3f);
vec3 Ks = vec3(0.8f,0.8f,0.8f);
vec3 Ka = vec3(0.9f,0.5f,0.3f);

GLfloat Shininess = 2000.f;

/**VARIABLES PARA LOS MENUS**/
bool isWireframeEnabled = false;
int isLightingEnabled = 1;
bool isIdling = false;
int _x=0,_y=0,_z=0;


GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];


/**----MVP()-----**/
void MVP_handle(){
    //Combinación afín para el desplazamiento
    vec3 displacement = (1.0f - t) * Paths[A] + t * Paths[B];
    vec3 dirVectors[3] = {
        Paths[B]-Paths[A],
        Paths[C]-Paths[B],
        Paths[A]-Paths[C],
    };
    float dirAngles[3] = { atan2(dirVectors[A_to_B].z , dirVectors[A_to_B].x) * 180.0f /PI,
                            atan2(dirVectors[B_to_C].z , dirVectors[B_to_C].x)* 180.0f /PI,
                            atan2(dirVectors[C_to_A].z , dirVectors[C_to_A].x)* 180.0f /PI};
    mat4 extraRotation;
    switch(currentPath){
        case A_to_B:
            displacement = (1.0f - t) * Paths[A] + t * Paths[B];
            extraRotation = glm::rotate(mat4(),dirAngles[A_to_B] ,vec3(0.0,-1.0,0.0));
            break;
        case B_to_C:
            displacement = (1.0f - t) * Paths[B] + t * Paths[C];
            extraRotation = glm::rotate(mat4(),dirAngles[B_to_C],vec3(0.0,-1.0,0.0));
            break;
        case C_to_A:
            displacement = (1.0f - t) * Paths[C] + t * Paths[A];
            extraRotation = glm::rotate(mat4(),dirAngles[C_to_A],vec3(0.0,-1.0,0.0));
            break;
        default:
            break;
    }

    //cout << displacement.x << "y";
    //Model Matrix: Nuestro Modelo estara en el centro
    //mat4 Model = mat4(1.0f); //Matriz identidad
    Model = glm::translate(mat4(),displacement) *
                            //extraRotation *
                            glm::rotate(mat4(),angle,vec3(1.0,0.0,0.0));

    //Camera matrix
    View = glm::lookAt(
        vec3(7+_x,3+_y,-10+_z), //Posicion de la Camara en el World Space
        vec3(0,0,0), //Mira hacia el Origen
        vec3(0,1,0) //VUP hacia arriba paralelo al eje y
        );

    //Matriz de Proyección: Campo de vision 45°, Ratio 4:3, display range: 0.1 unit<->100 units
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    Projection = glm::perspective(glm::radians(50.0f), 4.0f / 4.0f, 0.1f, 100.0f);
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // Multiplicacmos las matrices
    MVP = Projection * View * Model; //Observar el orden de multiplicación
    VP = Projection * View; //Para los objetos de referencia en el centro
    mv = View * Model;
    NormalMatrix = mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]));
}


struct VertexData {
        GLfloat color[4];
        GLfloat position[4];
    };

VertexData vertices[10] = {
        {{ 0.0f, 1.0f, 0.0f, 1.0f }, { 5.0f,0.0f,8.0f }}, // x-z plane
        {{ 0.0f, 1.0f, 0.0f, 1.0f }, { -5.0f,0.0f,8.0f }},
        {{ 0.0f, 1.0f, 0.0f, 1.0f }, { 5.0f,0.0f,-4.0f }},
        {{ 0.0f, 1.0f, 0.0f, 1.0f }, { -5.0f,0.0f,-4.0f }},
        {{ 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f,0.0f,0.0f }}, // X -axis
        {{ 1.0f, 0.0f, 0.0f, 1.0f }, { 10.0f,0.0f,0.0f }},
        {{ 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f,0.0f,0.0f }}, // Y -axis
        {{ 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f,10.0f,0.0f }},
        {{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f,0.0f,0.0f }}, // Z -axis
        {{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f,0.0f,10.0f }},
    };


/**----INIT()-----*/
void init(){
    //Color por Defecto Background
    glClearColor(0.529f, 0.807f, 0.92f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    //glLineWidth(2.0);

    program1 = LoadShaders("simple.vshader","simple.fshader");
    program2 = LoadShaders("extra.vshader","extra.fshader");
    glUseProgram(program1);

    glGenVertexArrays(NumVAOs,VAOs);
    glBindVertexArray(VAOs[mySphere]);

    //Generamos nuestros buffers
    glGenBuffers(NumBuffers,Buffers);
    /**EVERY GLM::VEC3 USE 3 FLOATS**/
    glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, 3 * sphere->num_Vertex() * sizeof(float),sphere->data(),GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,Buffers[NormalBuffer]);
    glBufferData(GL_ARRAY_BUFFER, 3 * sphere->num_Vertex() * sizeof(float),sphere->normal_data(),GL_DYNAMIC_DRAW);

    //Funciones adicionales
    MVP_handle();

    //Aqui dibujaremos los ejes y el plano
    /**----EXTRAS()-----**/
    glUseProgram(program2);
    glBindVertexArray(VAOs[extras]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[extraBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_DYNAMIC_DRAW);
}

/**----DISPLAY()-----**/
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(1.0);

    MVP_handle();

    /**VAO 1**/

    glBindVertexArray(VAOs[mySphere]);
    glUseProgram(program1);

    //Enviamos el handle para "MVP"
    GLuint MatrixID1 = glGetUniformLocation(program1,"MVP");
    GLuint ViewMatrixID = glGetUniformLocation(program1, "View");
    GLuint ModelMatrixID = glGetUniformLocation(program1, "Model");
    GLuint ProjectionMatrixID = glGetUniformLocation(program1, "Projection");
    GLuint mvID = glGetUniformLocation(program1, "mv");
    GLuint normalMatrixID = glGetUniformLocation(program1, "NormalMatrix");

    glUniformMatrix4fv(MatrixID1,1,GL_FALSE,&MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &Projection[0][0]);
    glUniformMatrix4fv(mvID, 1, GL_FALSE, &mv[0][0]);
    glUniformMatrix3fv(normalMatrixID,1,GL_FALSE,&NormalMatrix[0][0]);

    GLuint LdID = glGetUniformLocation(program1, "Light.Ld");
    GLuint LaID = glGetUniformLocation(program1, "Light.La");
    GLuint LsID = glGetUniformLocation(program1, "Light.Ls");
    glUniform3f(LdID, Ld.x, Ld.y, Ld.z);
    glUniform3f(LaID, La.x, La.y, La.z);
    glUniform3f(LsID, Ls.x, Ls.y, Ls.z);

    GLuint LightID = glGetUniformLocation(program1, "Light.Position");
    LightPos = View * vec4(5.0f, 5.0f, 2.0f, 1.0f);
    glUniform3f(LightID, LightPos.x, LightPos.y, LightPos.z);

    GLuint KdID = glGetUniformLocation(program1, "Material.Kd");
    GLuint KsID = glGetUniformLocation(program1, "Material.Ks");
    GLuint KaID = glGetUniformLocation(program1, "Material.Ka");
    GLuint ShininessID = glGetUniformLocation(program1,"Material.Shininess");
    GLuint flagID = glGetUniformLocation(program1,"LightEnabled");

    glUniform3f(KdID, Kd.x,Kd.y,Kd.z);
    glUniform3f(KsID, Ks.x,Ks.y,Ks.z);
    glUniform3f(KaID, Ka.x,Ka.y,Ka.z);
    glUniform1f(ShininessID,Shininess);
    glUniform1i(flagID,isLightingEnabled);

    glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
    glEnableVertexAttribArray(vPos);
    glVertexAttribPointer(vPos,3,GL_FLOAT,GL_FALSE,sizeof(vec3),BUFFER_OFFSET(0));

    glBindBuffer(GL_ARRAY_BUFFER,Buffers[NormalBuffer]);
    glEnableVertexAttribArray(nPos);
    glVertexAttribPointer(nPos,3,GL_FLOAT,GL_FALSE,sizeof(vec3),BUFFER_OFFSET(0));

    if(isWireframeEnabled)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // Funcion para activar e wireframe
                glColor4f(1.0, 0.84, 0.0,1.0);
                glDrawArrays(GL_TRIANGLES,0,sphere->num_Vertex());

    glDisableVertexAttribArray(vPos);
    glDisableVertexAttribArray(nPos);

    /*====================================================*/
    /*====================================================*/

    /**inter**/
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    /**VAO 2**/

    glBindVertexArray(VAOs[extras]);
    //glBindBuffer(GL_ARRAY_BUFFER,Buffers[extraBuffer]);
    glUseProgram(program2);

    GLuint MatrixID2 = glGetUniformLocation(program2,"VP");
    glUniformMatrix4fv(MatrixID2,1,GL_FALSE,&VP[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER,Buffers[extraBuffer]);
    glEnableVertexAttribArray(eColor);
    glVertexAttribPointer(eColor,4,GL_FLOAT, GL_FALSE, sizeof(VertexData),BUFFER_OFFSET(0));

    glEnableVertexAttribArray(ePos);
    glVertexAttribPointer(ePos,3,GL_FLOAT, GL_FALSE,
                          sizeof(VertexData),BUFFER_OFFSET( sizeof(vertices[0].color) ));

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glLineWidth(1.5);
    glDrawArrays(GL_LINES,4,6);

    glDisableVertexAttribArray(eColor);
    glDisableVertexAttribArray(ePos);

    glutSwapBuffers();

    glFlush();
}

/**FUNCIONES GLUT**/
//---------------------------------------------------------------------------
void idle (void)
{
    angle += 0.01;
    //t += 0.001;
    if(t > 1.0f){
        currentPath++;
        t = 0.0f;
        if(currentPath > C_to_A){
            currentPath = A_to_B;
            angle = 0.0;
        }

    }

    //cout << "angle:" << angle << endl;
    glutPostRedisplay();
}
//----------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y){
    switch(key) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit(EXIT_SUCCESS );
	    break;
    case 'b': case 'B':
	    if(!isIdling){
            glutIdleFunc(idle);
            isIdling = true;
	    }else{
            glutIdleFunc(NULL);
            isIdling = false;
        }
        break;
     //LOS VALORES DE X Y Z DEBEN CAMBIAR LA POSICION DE LA CAMARA
    case 'x':
        _x--;
        break;
    case 'X':
	    _x++;
	    break;
    case 'y':
        _y--;
        break;
    case 'Y':
	    _y++;
	    break;
    case 'z':
        _z--;
        break;
    case 'Z':
	    _z++;
	    break;
    }
    glutPostRedisplay();
}
//----------------------------------------------------------------------------
void reshape(int width, int height)
{
    glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        exit(0);
}

/**FUNCIONES MENU**/

void menu(int id){
    switch(id){
        case 1:
            exit(0);
            break;

        case 2:
            glutIdleFunc(idle);
            break;
        case 3:
            glutIdleFunc(idle);
            break;
    }
    glutPostRedisplay();
}

void Lighting_menu(int id){
    switch(id){
        case 1:
            isLightingEnabled = 0;
            break;
        case 2:
            isLightingEnabled = 1;
            break;
    }
    glutPostRedisplay();
}

void Wireframe_menu(int id){
    switch(id){
        case 1:
            isWireframeEnabled = false;
            break;

        case 2:
            isWireframeEnabled = true;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char ** argv){
    filename = argv[1];
    sphere = new Sphere(filename);


    /**Inicio de OpenGL**/
    glutInit(&argc,argv);

    glewExperimental = GL_TRUE;

    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(windowDIM,windowDIM);
    glutInitContextVersion(3,0);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow(filename);

    if(glewInit()){
        cerr << "Unable to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }

    init();

    /**MENU**/
    int sub_menu1 = glutCreateMenu(Lighting_menu);
    glutAddMenuEntry("No",1);
    glutAddMenuEntry("Yes",2);

    int sub_menu2 = glutCreateMenu(Wireframe_menu);
    glutAddMenuEntry("No",1);
    glutAddMenuEntry("Yes",2);

    glutCreateMenu(menu);
    glutAddMenuEntry("Default View Point",1);
    glutAddSubMenu("Enable Lighting",sub_menu1);
    glutAddMenuEntry("Shading",3);
    glutAddMenuEntry("Lighting",4);
    glutAddSubMenu("Wire Frame",sub_menu2);
    glutAddMenuEntry("Quit",6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(myMouse);


    glutMainLoop();
}

//---------------------------------------------------------------------
//
// LoadShaders Function
//

// Create a NULL-terminated string by reading the provided file
static char*
readShaderSource(const char* shaderFile)
{
    FILE* fp = fopen(shaderFile, "r");

    if ( fp == NULL ) { return NULL; }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    char* buf = new char[size + 1];
    fread(buf, 1, size, fp);

    buf[size] = '\0';
    fclose(fp);

    return buf;
}


// Create a GLSL program object from vertex and fragment shader files
GLuint LoadShaders(const char* vShaderFile, const char* fShaderFile)
{
    struct Shader {
	const char*  filename;
	GLenum       type;
	GLchar*      source;
    }  shaders[2] = {
	{ vShaderFile, GL_VERTEX_SHADER, NULL },
	{ fShaderFile, GL_FRAGMENT_SHADER, NULL }
    };

    GLuint program = glCreateProgram();

    for ( int i = 0; i < 2; ++i ) {
	Shader& s = shaders[i];
	s.source = readShaderSource( s.filename );
	if ( shaders[i].source == NULL ) {
	    std::cerr << "Failed to read " << s.filename << std::endl;
	    exit( EXIT_FAILURE );
	   }
        else printf("Successfully read %s\n", s.filename);

	GLuint shader = glCreateShader( s.type );
	glShaderSource( shader, 1, (const GLchar**) &s.source, NULL );
	glCompileShader( shader );

	GLint  compiled;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled ) {
	    std::cerr << s.filename << " failed to compile:" << std::endl;
	    GLint  logSize;
	    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
	    char* logMsg = new char[logSize];
	    glGetShaderInfoLog( shader, logSize, NULL, logMsg );
	    std::cerr << logMsg << std::endl;
	    delete [] logMsg;

	    exit( EXIT_FAILURE );
	   }
        else printf("Successfully compiled %s\n", s.filename);

	delete [] s.source;

	glAttachShader( program, shader );
    }

    /* link and error check */
    glLinkProgram(program);

    GLint  linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked ) {
	std::cerr << "Shader program failed to link" << std::endl;
	GLint  logSize;
	glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
	char* logMsg = new char[logSize];
	glGetProgramInfoLog( program, logSize, NULL, logMsg );
	std::cerr << logMsg << std::endl;
	delete [] logMsg;

	exit( EXIT_FAILURE );
    }
    else printf("Successfully linked program object\n\n");

#if 0 /* YJC: Do NOT use this program obj yet!
              Call glUseProgram() outside, in suitable places inside display(),
              to apply different shading programs on different objects.
      */
    /* use program object */
    glUseProgram(program);
#endif

    return program;
}
