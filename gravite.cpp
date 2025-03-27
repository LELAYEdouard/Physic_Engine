#include <iostream>
#include <math.h>
#include <vector>
#include "glad/glad.h"
#include <GLFW/glfw3.h>


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

const float dt=0.016; //tps de une frame en sec
const std::vector<float> gravity={0.0,-9.81}; // N/Kg
const float rebond=0.6; //coef de rebondissement
const float G=6.674*pow(10,-11);

std::vector<float> genVertexCircle(float cx,float cy,float r,int seg); 
void VAOVBOConf(std::vector<float> v,GLuint *vao,GLuint *vbo);
void draw_circle(GLuint shaderProg,std::vector<float> vert,GLuint VAO);


class Circle{
    public:
        GLuint VAO,VBO;
        std::vector<float> pos; // m
        std::vector<float> speed={0.0,0.0}; // m/s
        std::vector<float> acc={0.0,0.0}; // m/s^2
        std::vector<float> applied_forces={0.0,0.0}; // N
        float radius;
        float weight = 1; // Kg
        std::vector<float> vertices;

        Circle(std::vector<float> pos,float weight,float radius){
            this->pos=pos;
            this->weight=weight;
            this->radius=radius;
        }

        void genVert(){
            vertices = genVertexCircle(pos[0],pos[1], radius, 70);
            VAOVBOConf(vertices,&VAO,&VBO);
        }

        void updateVert(){
            vertices = genVertexCircle(pos[0],pos[1], radius, 70);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        void delVAOBVO(){
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }

        void draw(GLuint shaderProgram){
            glUseProgram(shaderProgram); // Utilise le shader compilé
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 3);
            glBindVertexArray(0);
        }

        void updatePos(){
            applied_forces[0]+=gravity[0] * weight; //ajout gravité
            applied_forces[1]+=gravity[1] * weight; //ajout gravité
        
            acc[0] = applied_forces[0] / weight;
            acc[1] = applied_forces[1] / weight;

            

            speed[0]+= acc[0]*dt;
            speed[1]+= acc[1]*dt;

            pos[0]+= speed[0]*dt;
            pos[1]+= speed[1]*dt;

            if (pos[0] - radius < -1 || pos[0] + radius > 1) {
            speed[0] *= -rebond;
            pos[0] = (pos[0] < 0) ? -1 + radius : 1 - radius;
            }

            // Collision avec les bords verticaux
            if (pos[1] - radius < -1 || pos[1] + radius > 1) {
                speed[1] *= -rebond;
                pos[1] = (pos[1] < 0) ? -1 + radius : 1 - radius;
            }
            applied_forces={0.0,0.0};
        };

        void addForces(std::vector<float> f){
            applied_forces[0]+=f[0];
            applied_forces[1]+=f[1];
        }

        
};
void updateForces(std::vector<Circle*> &lst);

int main(){
    //initialise glfw
    glfwInit();

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //creer une fenetre 
    GLFWwindow* window = glfwCreateWindow(720, 640, "HELP", NULL, NULL);
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
    



    std::vector<float> pos={0.0,0.0};
    std::vector<float> pos2={0.0,0.3};
    std::vector<float> pos3={0.0,0.35};
    std::vector<float> pos4={0.0,0.7};

    std::vector<Circle> objs={
        Circle(pos,0.9,0.2)
    };

    for(int i =0; i<objs.size();i++){
        objs[i].genVert();
    }

    std::vector<float> v1={0.0,700.0};

    objs[0].addForces(v1);

    
    std::vector<Circle*> lst;

    for(int i =0; i<objs.size();i++){
        lst.push_back(&objs[i]);
    }

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        updateForces(lst);
        for(int i =0; i<objs.size();i++){
            objs[i].updatePos();
            objs[i].updateVert();
            objs[i].draw(shaderProgram);
        }

        // Mise à jour des buffers et événements
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    for(int i =0; i<objs.size();i++){
        objs[i].delVAOBVO();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

std::vector<float> genVertexCircle(float cx,float cy,float r,int seg){
    std::vector<float> vertices;

    vertices.push_back(cx);
    vertices.push_back(cy);
    vertices.push_back(0.0);

    for(int i=0;i<=seg;i++){
        float theta = 2.0f * 3.1415926f * float(i) / float(seg);
        float x = r * cos(theta);
        float y = r * sin(theta);
        vertices.push_back(cx + x);
        vertices.push_back(cy + y);
        vertices.push_back(0.0);
    }

    return vertices;

}

void VAOVBOConf(std::vector<float> v, GLuint *vao, GLuint *vbo) {
    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);

    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);

    // Allouer la mémoire mais ne pas envoyer les données (elles seront mises à jour avec glBufferSubData)
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void updateForces(std::vector<Circle*> &lst){

    for (int i =0 ;i<lst.size();i++){
        for(int j =0;j<lst.size();j++){
            if(i!=j){
                std::vector<float> direction={lst[j]->pos[0] - lst[i]->pos[0],lst[j]->pos[1] - lst[i]->pos[1]};
                float epsilon = 1e-6;
                float distance =sqrt(direction[0]*direction[0]+direction[1]*direction[1])+epsilon;
                

                float f = (G * lst[i]->weight * lst[j]->weight ) / (distance * distance);
                
                direction[0] /= distance;
                direction[1] /= distance;

                std::vector<float> force = {direction[0] * f, direction[1] * f};

                lst[i]->addForces(force);
            }
        }
    }
}
