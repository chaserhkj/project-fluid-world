#ifndef _commontypes
#define _commontypes

class Node
{
public:
    double zeta;  /* vorticity */
    double psi;   /* stream */
    double zetat; /* vorticity after one recursion */
    double newzetat; /* used for recursive calculation */
    double newpsi;
    double xi;    /* X-position in the transformed cordination */
    double eta;   /* Y-position in the transformed cordination */
    double x;     /* X-position in the physics world */
    double y;     /* Y-position in the physics world */

    virtual void calculateXY() = 0; /* Calculate X and Y position from xi and eta */

    /*
        Node(double z=0, double p=0, double x=0,double e=0):zeta(z), psi(p), xi(x), eta(e)
        {}

        void setZeta(double z){
            zeta = z;
            return;
        }
        void setPsi(double p){
            psi = p;
            return;
        }
        void newZeta(double z){
            newzeta = z;
            return;
        }
        void newPsi(double p){
            newpsi = p;
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
        double getX() const{  Do not use before calculate X!
            return x;
        }
        double getY() const{  Do not use before calculate Y!
            return y;
        }


        void flush() flush zeta and psi {
            zeta = newzeta;
            psi = newpsi;
        }
    */
};

#endif /*_commontypes*/
