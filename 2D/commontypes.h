#ifndef _commontypes
#define _commontypes

class Node{
private:
    double zeta;  /* vorticity */
    double psi;   /* stream */ 
    double newzeta; /* vorticity after one recursion */
    double newpsi;
    double xi;    /* X-position in the transformed cordination */
    double eta;   /* Y-position in the transformed cordination */
    double x;     /* X-position in the physics world */
    double y;     /* Y-position in the physics world */
public:
    Node(double z, double p, double x,double e):zeta(z), psi(p), xi(x), eta(e)
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
    double getZeta() const{
        return zeta;
    }
    double getPsi() const{
        return psi;
    }
    double getXi() const{
        return xi;
    } 
    double getEta() const{
        return eta;
    }
    double getX() const{ /* Do not use before calculate X! */
        return x;
    }
    double getY() const{ /* Do not use before calculate Y! */
        return y;
    }

    virtual void run(double psi1, double psi2, double psi3, double psi4) = 0; /* Do one recursive calculation, psi1=psi(i+1,j), psi2=psi(i-1,j), psi3=psi(i,j+1), psi4=psi(i,j-1) */
    virtual void calculateXY() = 0; /* Calculate X and Y position from xi and eta */ 
    void flush()/* flush zeta and psi */{
        zeta = newzeta;
        psi = newpsi;
    }
 
};

#endif /*_commontypes*/
