#ifndef _commontypes
#define _commontypes

class Node{
private:
    double zeta;  /* vorticity */
    double psi;   /* stream */ 
    double xi;    /* X-position in the transformed cordination */
    double eta;   /* Y-position in the transformed cordination */
    double x;     /* X-position in the physics world */
    double y;     /* Y-position in the physics world */
    bool isBoundary;
public:
    Node(double z, double p, double x,double e, bool b):zeta(z), psi(p), xi(x), eta(e), isBoundary(b)
    {}

    void setZeta(double z){
        zeta = z;
        return;
    }
    void setPsi(double p){
        psi = p;
        return;
    }
    void setXi(double x){
        xi = x;
        return;
    }
    void setEta(double e){
        eta = e;
        return;
    }
    double getZeta(){
        return zeta;
    }
    double getPsi(){
        return psi;
    }
    double getXi(){
        return xi;
    }
    double getEta(){
        return eta;
    }
    double getX(){ /* Do not use before calculate X! */
        return x;
    }
    double getY(){ /* Do not use before calculate Y! */
        return y;
    }

    virtual void run() = 0; /* Do one recursive calculation */
    virtual void calculateX() = 0; /* Calculate X position from xi and eta */ 
    virtual void calculateY() = 0; /* Calculate Y position from xi and eta */ 
 
};

#endif /*_commontypes*/
