#include <stddef.h>
#include <umfpack.h>

/* i is along columns and j is along rows, start from (0,0) */
class Solver
{
private:
    int n; /* must be square */
    int nz;
    int *Ti, *Tj;
    double *b;
    double *Tx;
    double *solution;

    int counter; /* so eachtime we input to a different position */
public:
    Solver(int num, int nonzero, double* right):n(num), nz(nonzero),b(right) {
        Ti = new int[nz];
        Tj = new int[nz];
        Tx = new double[nz];
        solution = new double[n];
        counter = 0;
    }
    ~Solver(){
        delete [] Ti;
        delete [] Tj;
        delete [] Tx;
        delete [] solution;
    }
    bool input(int i, int j, double x) {
        if (counter >= nz ){
            return false;
        }
        Ti[counter] = i;
        Tj[counter] = j;
        Tx[counter] = x;

        counter += 1;
        return true;
    }
    void solve() {
        int *Ap, *Ai;
        double *Ax;
        void *Symbolic, *Numeric;
        Ap = new int[nz];
        Ai = new int[nz];
        Ax = new double[nz];

        umfpack_di_triplet_to_col(n, n, counter, Ti, Tj, Tx, Ap, Ai, Ax, NULL);
        umfpack_di_symbolic(n, n, Ap, Ai, Ax, &Symbolic, NULL, NULL);
        umfpack_di_numeric(Ap, Ai, Ax, Symbolic, &Numeric, NULL, NULL);
        umfpack_di_free_symbolic(&Symbolic);
        umfpack_di_solve(UMFPACK_A, Ap, Ai, Ax, solution, b, Numeric, NULL, NULL);
        umfpack_di_free_numeric(&Numeric);
        delete [] Ap;
        delete [] Ai;
        delete [] Ax;
    }
    double* getSolution() {
        return solution;
    }
};

    
