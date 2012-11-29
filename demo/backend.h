#ifndef _backend
#define _backend
class pendulum
{
private:
    double theta, velocity, length, time;
    static const double gravity;
protected:
    double step;
public:
    pendulum(double t = 0, double v = 0, double l = 1, double s=0.1, double t=0):\
        theta(t), velocity(v), length(l), step(s), time(t){}
    void setStatus(double t, double v, double t){
        theta = t;
        velocity = v;
        time = t;
    }
    double getAngle() const{
        return theta;
    }
    double getVelocity() const{
        return velocity;
    }
    double getTime() const{
        return time;
    }
    void run();
};
#endif

