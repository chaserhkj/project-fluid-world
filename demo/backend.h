#ifndef _backend
#define _backend
class pendulum
{
private:
    double theta, velocity, length, time;
    static const double gravity;
    int recurrenceDepth;
protected:
    double step;
public:
    pendulum(double t = 0, double v = 0, double l = 1, double s=0.1, double ti=0, int rec=1):\
        theta(t), velocity(v), length(l), time(ti), recurrenceDepth(rec), step(s){}
    void setStatus(double t, double v, double ti){
        theta = t;
        velocity = v;
        time = ti;
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

