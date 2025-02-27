#include <iostream>
#include <math.h>

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
            x+=v.getX();
            y+=v.getY();
            z+=v.getZ();
        }

        void sub(Vector3 v){
            x-=v.getX();
            y-=v.getY();
            z-=v.getZ();
        }

        float scal(Vector3 v){
            return x*v.getX()+y*v.getY()+z*v.getZ();
        }

        float norm(){
            return pow(pow(x,2)+pow(y,2)+pow(z,2),0.5);
        }

        float dist(Vector3 v){
            return sqrt(pow(x-v.getX(),2)+pow(y-v.getY(),2)+pow(z-v.getZ(),2));
        }

        void rotX(float teta){
            y=cos(teta)*y-sin(teta)*z;
            z=sin(teta)*y+cos(teta)*z;
        }

        void rotY(float teta){
            x=cos(teta)*x+sin(teta)*z;
            z=-sin(teta)*x+cos(teta)*z;
        }

        void rotZ(float teta){
            x=cos(teta)*x-sin(teta)*y;
            y=sin(teta)*x+cos(teta)*y;
        }

        float getX(){
            return x;
        }

        float getY(){
            return y;
        }

        float getZ(){
            return z;
        }

        void print(){
            std::cout<< "(" <<this->x << "," <<this->y << "," <<this->z<< ")" << std::endl;
        }
    
};
class Particles{
    public:
        Vector3 pos;
        Vector3 speed;
        Vector3 acceleration;
        float mass;
        Vector3 forces;

        Particles(){
            Vector3 v;
            this->pos=v;
            this->speed=v;
            this->acceleration=v;
            this->mass=0;
            this->forces=v;
        }
        Particles(Vector3 pos){
            Vector3 v;
            this->pos=pos;
            this->speed=v;
            this->acceleration=v;
            this->mass=1;
            this->forces=v;
        }
        Particles(Vector3 pos,float mass){
            Vector3 v;
            this->pos=pos;
            this->speed=v;
            this->acceleration=v;
            this->mass=mass;
            this->forces=v;
        }
        Particles(Vector3 pos,Vector3 speed,float mass){
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
                acceleration.x=forces.getX()/mass;
                acceleration.y=forces.getY()/mass;
                acceleration.z=forces.getZ()/mass;
            }
        }

        void updateSpeed(float delta){
            speed.x=speed.getX()+acceleration.getX()*delta;
            speed.y=speed.getY()+acceleration.getY()*delta;
            speed.z=speed.getZ()+acceleration.getZ()*delta;
        }

        void updatePos(float delta){
            pos.x=pos.getX()+speed.getX()*delta;
            pos.y=pos.getY()+speed.getY()*delta;
            pos.z=pos.getZ()+speed.getZ()*delta;
        }
};
const Vector3 g(0,-9.81,0);

int main(){
    Vector3 x(0,5,0);
    Vector3 y(2,0.5,0.2);
    //std::cout <<x.dist(y)<<std::endl;
    Particles p(x);
    p.applyForce(g);
    for(int i=0;i<20;i++){
        p.updateAcceleration();
        p.updateSpeed(0.5);
        p.updatePos(0.5);
        p.pos.print();
    }
    
    return 0;
}

