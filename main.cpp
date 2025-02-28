#include <iostream>
#include <math.h>
#include <vector>
#include "glad/glad.h"
#include <GLFW/glfw3.h>


class Vector3{
    public:
        float x,y,z;
        Vector3(){
            this->x=0;
            this->y=0;
            this->z=0;
        }
        Vector3(float x,float y,float z){
            this->x=x;
            this->y=y;
            this->z=z;
        }
        void add(Vector3 v){
            x+=v.x;
            y+=v.y;
            z+=v.z;
        }

        void sub(Vector3 v){
            x-=v.x;
            y-=v.y;
            z-=v.z;
        }
        void neg(){
            x=-x;
            y=-y;
            z=-z;
        }
        float scal(Vector3 v){
            return x*v.x+y*v.y+z*v.z;
        }

        float norm(){
            return pow(pow(x,2)+pow(y,2)+pow(z,2),0.5);
        }

        float dist(Vector3 v){
            return sqrt(pow(x-v.x,2)+pow(y-v.y,2)+pow(z-v.z,2));
        }

        void rotX(float teta){
            float y0 =y;
            float z0=z;
            y=cos(teta)*y0-sin(teta)*z0;
            z=sin(teta)*y0+cos(teta)*z0;
        }

        void rotY(float teta){
            float z0=z;
            float x0=x;
            x=cos(teta)*x0+sin(teta)*z0;
            z=-sin(teta)*x0+cos(teta)*z0;
        }

        void rotZ(float teta){
            float y0=y;
            float x0=x;
            x=cos(teta)*x0-sin(teta)*y0;
            y=sin(teta)*x0+cos(teta)*y0;
        }

        void print(){
            std::cout<< "(" <<this->x << "," <<this->y << "," <<this->z<< ")" << std::endl;
        }
    
};
class Particle{
    public:
        Vector3 pos;
        Vector3 speed;
        Vector3 acceleration;
        float mass;
        Vector3 forces;

        Particle(){
            Vector3 v;
            this->pos=v;
            this->speed=v;
            this->acceleration=v;
            this->mass=0;
            this->forces=v;
        }
        Particle(Vector3 pos){
            Vector3 v;
            this->pos=pos;
            this->speed=v;
            this->acceleration=v;
            this->mass=1;
            this->forces=v;
        }
        Particle(Vector3 pos,float mass){
            Vector3 v;
            this->pos=pos;
            this->speed=v;
            this->acceleration=v;
            this->mass=mass;
            this->forces=v;
        }
        Particle(Vector3 pos,Vector3 speed,float mass){
            Vector3 v;
            this->pos=pos;
            this->speed=speed;
            this->acceleration=v;
            this->mass=mass;
            this->forces=v;
        }
        void applyForce(Vector3 v){
            forces.add(v);
        }
        void updateAcceleration(){
            if(forces.x !=0 || forces.y !=0 || forces.z !=0){
                acceleration.x=forces.x/mass;
                acceleration.y=forces.y/mass;
                acceleration.z=forces.z/mass;
            }
        }

        void updateSpeed(float delta){
            speed.x=speed.x+acceleration.x*delta;
            speed.y=speed.y+acceleration.y*delta;
            speed.z=speed.z+acceleration.z*delta;
        }

        void updatePos(float delta){
            pos.x=pos.x+speed.x*delta;
            pos.y=pos.y+speed.y*delta;
            pos.z=pos.z+speed.z*delta;
        }

        void update(float delta,std::vector<Vector3> forces){
            Vector3 v;
            for(int i=0;i<forces.size();i++){
                this->forces.add(forces.at(i));       
            }
            this->updateAcceleration();
            this->updateSpeed(delta);
            this->updatePos(delta);
            this->forces =v;
        }
};
class SysParticles{
    public:
        std::vector<Particle> tab;

        SysParticles(){}

        void addParticle(Particle p){
            tab.push_back(p);
        }

        void updateParticles(float delta,std::vector<Vector3> forces){
            
            for(int i=0;i<tab.size();i++){
                tab.at(i).update(delta,forces);
                tab.at(i).pos.print();
            }
            
        }
};
const Vector3 g(0,-9.81,0);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main(){
    //initialise glfw
    glfwInit();

    GLfloat vertices[]={
        0,0,0,
        1,0,0,
        0,1,0

    };

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //creer une fenetre 
    GLFWwindow* window = glfwCreateWindow(800, 600, "HELP", NULL, NULL);
    if (!window) {
        std::cerr << "Erreur: Impossible de créer la fenêtre" << std::endl;
        glfwTerminate();
        return -1;
    }
    //assoscie opengGL a la fenetre
    glfwMakeContextCurrent(window); 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //charge les fonctions de openGL
    gladLoadGL();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erreur: Impossible d'initialiser GLAD" << std::endl;
        return -1;
    }

    //precise le Viewport 
    glViewport(0,0,800,600);
   
    GLuint vertexShader= glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram=glCreateProgram();   
    
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    GLuint VAO,VBO;
    glGenVertexArrays(1,&VAO); 
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);
    if (VAO == 0 || VBO == 0) {
    std::cerr << "Erreur lors de la génération des buffers" << std::endl;
    return -1;
    }


    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    //choisi la couleur de fond
    glClearColor(0.5f,0.25f,0.05f,1.0f);
    //clear le buffer fond et attribue la couleur
    glClear(GL_COLOR_BUFFER_BIT);
    //echange le buffer du fond avec celui de lavant
    glfwSwapBuffers(window);
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.2f,0.05f,0.15f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glfwSwapBuffers(window);
        //prend en compte tt les event GLFW
        glfwPollEvents();
    }

    glDeleteBuffers(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteProgram(shaderProgram);
    //supprime la fenetre
    glfwDestroyWindow(window);
    //termine GLFW
    glfwTerminate();
    return 0;
}
