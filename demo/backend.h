#ifndef _backend
#define _backend
class pendulum
{
private:
    double theta, velocity, length, step;
    static const double gravity;
public:
    pendulum(double t = 0, double v = 0, double l = 1, double s=0.1):\
        theta(t), velocity(v), length(l), step(s){}
    void setStatus(double t, double v){
        theta = t;
        velocity = v;
    }
    double getAngle() const{
        return theta;
    }
    double getVelocity() const{
        return velocity;
    }
    void run();
};
#endif

