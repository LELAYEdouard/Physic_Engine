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
        0.0,0.0,0.0,
        0.5,0.0,0.0,
        0.0,0.5,0.0,
        0.5,0.5,0.0
    };
    float baseVertices[] = {
        0.0,0.0,0.0,
        0.5,0.0,0.0,
        0.0,0.5,0.0,
        0.5,0.5,0.0
    };

    GLuint indices[]={
        0,1,2,
        2,3,1

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
    //charge les fonctions de openGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //precise le Viewport 
    glViewport(0,0,720,640);
   
    //creer un vertex shader
    GLuint vertexShader= glCreateShader(GL_VERTEX_SHADER);
    //rattache le code source du shader au shader  
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    //compile le code en langage machine
    glCompileShader(vertexShader);

    //creer un fragment shader
    GLuint fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
    //rattache le code source du shader au shader  
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    //compile le code en langage machine
    glCompileShader(fragmentShader);

    //creer une shader programme
    GLuint shaderProgram=glCreateProgram();   
    //rattache les shader au programme de shader
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    //lie les shader au pragramme
    glLinkProgram(shaderProgram);
    //supprime les shader 
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //creer les conteneur pour le vertex array object et le vertex buffer object, et le index array object
    GLuint VAO,VBO,EBO;
    //genere un vertex array
    glGenVertexArrays(1,&VAO); 
    //genere un vertex buffer 
    glGenBuffers(1,&VBO);
    //genere le EBO
    glGenBuffers(1,&EBO);
    
    //bind le VAO
    glBindVertexArray(VAO);

    //bind le VBO en precisant que c'est un GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //met les donné dans le buffer 
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    //bind le EBO en d^precisant que c un GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    //met les donné dans le buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    //configure le Verttex Atttribute pour que opengl comprenne comment lire le VBO
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    //active le Vertex Attribute pour que opengl sais comment lutiliser
    glEnableVertexAttribArray(0);

    //bind le VAO et VBO pour pas les modifier
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //bind le Vertex array pour pas le modifier
    glBindVertexArray(0);
    //bind le EBO pour pas le modif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


    //choisi la couleur de fond
    glClearColor(0.5f,0.25f,0.05f,1.0f);
    //clear le buffer fond et attribue la couleur
    glClear(GL_COLOR_BUFFER_BIT);
    //echange le buffer du fond avec celui de lavant
    glfwSwapBuffers(window);
    float i=0.0;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Affiche le maillage

    while(!glfwWindowShouldClose(window)){
        i += 0.1;

        
        vertices[1] = baseVertices[1] + sin(i)/2;
        vertices[4] = baseVertices[4] + sin(i)/2;
        vertices[7] = baseVertices[7] + sin(i)/2;
        vertices[10] = baseVertices[10] + sin(i)/2;
        
        
        // Met à jour le VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        
        // Nettoyage écran
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 

        glClear(GL_COLOR_BUFFER_BIT);

        // Utilisation du shader et dessin
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        // Mise à jour des buffers et événements
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteBuffers(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteProgram(shaderProgram);
    //supprime la fenetre
    glfwDestroyWindow(window);
    //termine GLFW
    glfwTerminate();
    return 0;
}
