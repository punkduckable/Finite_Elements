#include "Pardiso_Solve.h"

int Pardiso_Solve(const Matrix<double> & K, double* x, double* F) {
    ////////////////////////////////////////////////////////////////////////////
    // Compress K

    /* First, let's determine the number of equations in the system. This is
    simply the number of rows in A. */
    int      n_eqs = K.Get_Num_Rows();

    /* Next, let's compress A. This will allow us to get IA, JA, and A (the
    compressed version of A) */
    class Compressed_Matrix Compressed_K{K};

    int*     IA = Compressed_K.IA;
    int*     JA = Compressed_K.JA;
    double*  A = Compressed_K.A;

    /* Now let's get n_JA. We'll need to pass this to Pardiso */
    int      n_JA = Compressed_K.n_JA;


    ////////////////////////////////////////////////////////////////////////////
    // Pardiso variables

    void*    pt[64];            /* Internal solver memory pointer pt */
    int      iparm[64];
    double   dparm[64];

    int      mtype = 2;         /* K should be a real symmetric positive definite matrix */
    int      nrhs = 1;          /* Number of right hand sides. */
    int      maxfct = 1;        /* Maximum number of numerical factorizations.  */
    int      mnum = 1;	        /* Which factorization to use. */
    int      error = 0;         /* Error code. Initialize to zero. */
    int      msglvl = 0;        /* Print statistical information  */
    int      phase;             /* Phase of the solution (see Pardiso manual) */
    int      solver = 0;        /* use sparse direct solver */
    int      num_procs;         /* Number of processors. */

    /* Auxiliary variables. */
    char*    var;
    double   ddum;              /* Double dummy. Passed as the D and X parameters (Pardiso) in phases 11 and 22. */
    int      idum;              /* Integer dummy. Passed as the PERM parameter (Pardiso). */


    ////////////////////////////////////////////////////////////////////////////
    // Initialize Pardiso.

    pardisoinit (pt,  &mtype, &solver, iparm, dparm, &error);

    if (error != 0) {
      // Report the erorr and then return.
      Report_Pardiso_Error(error);
      return 1;
    } // if (error != 0) {
    #if defined(PARDISO_MONITOR)
    else { printf("[PARDISO]: License check was successful ... \n"); }
    #endif

    /* Numbers of processors, value of OMP_NUM_THREADS */
    var = getenv("OMP_NUM_THREADS");
    if(var != NULL) { sscanf( var, "%d", &num_procs ); }
    else {
        printf("Couldn't find OMP_NUM_THREADS environment variable. Try setting it to 1.\n");
        exit(1);
    } // else {
    iparm[2]  = num_procs;

    error  = 0;         /* Reset error flag. 0 means no error. */


    ////////////////////////////////////////////////////////////////////////////
    // Convert IA and JA from base 0 C notation to base 1 Fortran notation.

    for (int i = 0; i < n_eqs+1; i++) { IA[i] += 1; }
    for (int i = 0; i < n_JA; i++) { JA[i] += 1; }


    ////////////////////////////////////////////////////////////////////////////
    // Step 1: Reoredering and symbolic factorization.
    // This step also allocates all memory that is needed for the factorization

    phase = 11;

    pardiso (pt, &maxfct, &mnum, &mtype, &phase,
	           &n_eqs, A, IA, JA, &idum, &nrhs,
             iparm, &msglvl, &ddum, &ddum, &error, dparm);

    if (error != 0) {
        printf("ERROR during symbolic factorization\n");
        Report_Pardiso_Error(error);
        exit(1);
    } // if (error != 0) {

    #if defined(PARDISO_MONITOR)
    printf("\nReordering completed ... \n");
    printf("Number of nonzeros in factors  = %d\n", iparm[17]);
    printf("Number of factorization MFLOPS = %d\n", iparm[18]);
    #endif


    ////////////////////////////////////////////////////////////////////////////
    // Step 2: Numerical factorization

    phase = 22;
    iparm[32] = 1; /* compute determinant */

    pardiso (pt, &maxfct, &mnum, &mtype, &phase,
             &n_eqs, A, IA, JA, &idum, &nrhs,
             iparm, &msglvl, &ddum, &ddum, &error,  dparm);

    if (error != 0) {
        printf("ERROR during numerical factorization\n");
        Report_Pardiso_Error(error);
        exit(2);
    } // if (error != 0) {

    #if defined(PARDISO_MONITOR)
    printf("\nFactorization completed ...\n");
    #endif

    ////////////////////////////////////////////////////////////////////////////
    // Step 3: Back substitution and iterative refinement.

    phase = 33;
    iparm[7] = 1;       /* Max numbers of iterative refinement steps. */

    pardiso (pt, &maxfct, &mnum, &mtype, &phase,
             &n_eqs, A, IA, JA, &idum, &nrhs,
             iparm, &msglvl, F, x, &error,  dparm);

    if (error != 0) {
        printf("ERROR during solution\n");
        Report_Pardiso_Error(error);
        exit(3);
    } // if (error != 0) {

    #if defined(PARDISO_MONITOR)
    printf("\nSolve completed ... \n");
    printf("The solution of the system is: \n");
    for (int i = 0; i < n_eqs; i++) { printf("x [%d] = % f\n", i, x[i] ); }
    #endif


    ////////////////////////////////////////////////////////////////////////////
    // Termination and release of memory.

    phase = -1;         /* Release internal memory. */

    pardiso (pt, &maxfct, &mnum, &mtype, &phase,
             &n_eqs, &ddum, IA, JA, &idum, &nrhs,
             iparm, &msglvl, &ddum, &ddum, &error,  dparm);

    return 0;
} // int main(void) {
