/**
 * @file
 * This file contains the implementation of RateMatrix_Jones, which is
 * class that holds a rate matrix in RevBayes.
 *
 * @brief Implementation of RateMatrix_Jones
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-12-11 14:46:24 +0100 (Tue, 11 Dec 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id: RateMatrix_Jones.cpp 1921 2012-12-11 13:46:24Z hoehna $
 */

#include "RateMatrix_Jones.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"
#include "RbVectorUtilities.h"


using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_Jones::RateMatrix_Jones( void ) : RateMatrix_Empirical( 20 ){
    
    MatrixReal &m = *the_rate_matrix;
    
    /* jones */
	m[ 0][ 0] =   0; m[ 0][ 1] =  58; m[ 0][ 2] =  54; m[ 0][ 3] =  81; m[ 0][ 4] =  56; 
	m[ 0][ 5] =  57; m[ 0][ 6] = 105; m[ 0][ 7] = 179; m[ 0][ 8] =  27; m[ 0][ 9] =  36; 
	m[ 0][10] =  30; m[ 0][11] =  35; m[ 0][12] =  54; m[ 0][13] =  15; m[ 0][14] = 194; 
	m[ 0][15] = 378; m[ 0][16] = 475; m[ 0][17] =   9; m[ 0][18] =  11; m[ 0][19] = 298; 
	m[ 1][ 0] =  58; m[ 1][ 1] =   0; m[ 1][ 2] =  45; m[ 1][ 3] =  16; m[ 1][ 4] = 113; 
	m[ 1][ 5] = 310; m[ 1][ 6] =  29; m[ 1][ 7] = 137; m[ 1][ 8] = 328; m[ 1][ 9] =  22; 
	m[ 1][10] =  38; m[ 1][11] = 646; m[ 1][12] =  44; m[ 1][13] =   5; m[ 1][14] =  74; 
	m[ 1][15] = 101; m[ 1][16] =  64; m[ 1][17] = 126; m[ 1][18] =  20; m[ 1][19] =  17; 
	m[ 2][ 0] =  54; m[ 2][ 1] =  45; m[ 2][ 2] =   0; m[ 2][ 3] = 528; m[ 2][ 4] =  34; 
	m[ 2][ 5] =  86; m[ 2][ 6] =  58; m[ 2][ 7] =  81; m[ 2][ 8] = 391; m[ 2][ 9] =  47; 
	m[ 2][10] =  12; m[ 2][11] = 263; m[ 2][12] =  30; m[ 2][13] =  10; m[ 2][14] =  15; 
	m[ 2][15] = 503; m[ 2][16] = 232; m[ 2][17] =   8; m[ 2][18] =  70; m[ 2][19] =  16; 
	m[ 3][ 0] =  81; m[ 3][ 1] =  16; m[ 3][ 2] = 528; m[ 3][ 3] =   0; m[ 3][ 4] =  10; 
	m[ 3][ 5] =  49; m[ 3][ 6] = 767; m[ 3][ 7] = 130; m[ 3][ 8] = 112; m[ 3][ 9] =  11; 
	m[ 3][10] =   7; m[ 3][11] =  26; m[ 3][12] =  15; m[ 3][13] =   4; m[ 3][14] =  15; 
	m[ 3][15] =  59; m[ 3][16] =  38; m[ 3][17] =   4; m[ 3][18] =  46; m[ 3][19] =  31; 
	m[ 4][ 0] =  56; m[ 4][ 1] = 113; m[ 4][ 2] =  34; m[ 4][ 3] =  10; m[ 4][ 4] =   0; 
	m[ 4][ 5] =   9; m[ 4][ 6] =   5; m[ 4][ 7] =  59; m[ 4][ 8] =  69; m[ 4][ 9] =  17; 
	m[ 4][10] =  23; m[ 4][11] =   7; m[ 4][12] =  31; m[ 4][13] =  78; m[ 4][14] =  14; 
	m[ 4][15] = 223; m[ 4][16] =  42; m[ 4][17] = 115; m[ 4][18] = 209; m[ 4][19] =  62; 
	m[ 5][ 0] =  57; m[ 5][ 1] = 310; m[ 5][ 2] =  86; m[ 5][ 3] =  49; m[ 5][ 4] =   9; 
	m[ 5][ 5] =   0; m[ 5][ 6] = 323; m[ 5][ 7] =  26; m[ 5][ 8] = 597; m[ 5][ 9] =   9; 
	m[ 5][10] =  72; m[ 5][11] = 292; m[ 5][12] =  43; m[ 5][13] =   4; m[ 5][14] = 164; 
	m[ 5][15] =  53; m[ 5][16] =  51; m[ 5][17] =  18; m[ 5][18] =  24; m[ 5][19] =  20; 
	m[ 6][ 0] = 105; m[ 6][ 1] =  29; m[ 6][ 2] =  58; m[ 6][ 3] = 767; m[ 6][ 4] =   5; 
	m[ 6][ 5] = 323; m[ 6][ 6] =   0; m[ 6][ 7] = 119; m[ 6][ 8] =  26; m[ 6][ 9] =  12; 
	m[ 6][10] =   9; m[ 6][11] = 181; m[ 6][12] =  18; m[ 6][13] =   5; m[ 6][14] =  18; 
	m[ 6][15] =  30; m[ 6][16] =  32; m[ 6][17] =  10; m[ 6][18] =   7; m[ 6][19] =  45; 
	m[ 7][ 0] = 179; m[ 7][ 1] = 137; m[ 7][ 2] =  81; m[ 7][ 3] = 130; m[ 7][ 4] =  59; 
	m[ 7][ 5] =  26; m[ 7][ 6] = 119; m[ 7][ 7] =   0; m[ 7][ 8] =  23; m[ 7][ 9] =   6; 
	m[ 7][10] =   6; m[ 7][11] =  27; m[ 7][12] =  14; m[ 7][13] =   5; m[ 7][14] =  24; 
	m[ 7][15] = 201; m[ 7][16] =  33; m[ 7][17] =  55; m[ 7][18] =   8; m[ 7][19] =  47; 
	m[ 8][ 0] =  27; m[ 8][ 1] = 328; m[ 8][ 2] = 391; m[ 8][ 3] = 112; m[ 8][ 4] =  69; 
	m[ 8][ 5] = 597; m[ 8][ 6] =  26; m[ 8][ 7] =  23; m[ 8][ 8] =   0; m[ 8][ 9] =  16; 
	m[ 8][10] =  56; m[ 8][11] =  45; m[ 8][12] =  33; m[ 8][13] =  40; m[ 8][14] = 115; 
	m[ 8][15] =  73; m[ 8][16] =  46; m[ 8][17] =   8; m[ 8][18] = 573; m[ 8][19] =  11; 
	m[ 9][ 0] =  36; m[ 9][ 1] =  22; m[ 9][ 2] =  47; m[ 9][ 3] =  11; m[ 9][ 4] =  17; 
	m[ 9][ 5] =   9; m[ 9][ 6] =  12; m[ 9][ 7] =   6; m[ 9][ 8] =  16; m[ 9][ 9] =   0; 
	m[ 9][10] = 229; m[ 9][11] =  21; m[ 9][12] = 479; m[ 9][13] =  89; m[ 9][14] =  10; 
	m[ 9][15] =  40; m[ 9][16] = 245; m[ 9][17] =   9; m[ 9][18] =  32; m[ 9][19] = 961; 
	m[10][ 0] =  30; m[10][ 1] =  38; m[10][ 2] =  12; m[10][ 3] =   7; m[10][ 4] =  23; 
	m[10][ 5] =  72; m[10][ 6] =   9; m[10][ 7] =   6; m[10][ 8] =  56; m[10][ 9] = 229; 
	m[10][10] =   0; m[10][11] =  14; m[10][12] = 388; m[10][13] = 248; m[10][14] = 102; 
	m[10][15] =  59; m[10][16] =  25; m[10][17] =  52; m[10][18] =  24; m[10][19] = 180; 
	m[11][ 0] =  35; m[11][ 1] = 646; m[11][ 2] = 263; m[11][ 3] =  26; m[11][ 4] =   7; 
	m[11][ 5] = 292; m[11][ 6] = 181; m[11][ 7] =  27; m[11][ 8] =  45; m[11][ 9] =  21; 
	m[11][10] =  14; m[11][11] =   0; m[11][12] =  65; m[11][13] =   4; m[11][14] =  21; 
	m[11][15] =  47; m[11][16] = 103; m[11][17] =  10; m[11][18] =   8; m[11][19] =  14; 
	m[12][ 0] =  54; m[12][ 1] =  44; m[12][ 2] =  30; m[12][ 3] =  15; m[12][ 4] =  31; 
	m[12][ 5] =  43; m[12][ 6] =  18; m[12][ 7] =  14; m[12][ 8] =  33; m[12][ 9] = 479; 
	m[12][10] = 388; m[12][11] =  65; m[12][12] =   0; m[12][13] =  43; m[12][14] =  16; 
	m[12][15] =  29; m[12][16] = 226; m[12][17] =  24; m[12][18] =  18; m[12][19] = 323; 
	m[13][ 0] =  15; m[13][ 1] =   5; m[13][ 2] =  10; m[13][ 3] =   4; m[13][ 4] =  78; 
	m[13][ 5] =   4; m[13][ 6] =   5; m[13][ 7] =   5; m[13][ 8] =  40; m[13][ 9] =  89; 
	m[13][10] = 248; m[13][11] =   4; m[13][12] =  43; m[13][13] =   0; m[13][14] =  17; 
	m[13][15] =  92; m[13][16] =  12; m[13][17] =  53; m[13][18] = 536; m[13][19] =  62; 
	m[14][ 0] = 194; m[14][ 1] =  74; m[14][ 2] =  15; m[14][ 3] =  15; m[14][ 4] =  14; 
	m[14][ 5] = 164; m[14][ 6] =  18; m[14][ 7] =  24; m[14][ 8] = 115; m[14][ 9] =  10; 
	m[14][10] = 102; m[14][11] =  21; m[14][12] =  16; m[14][13] =  17; m[14][14] =   0; 
	m[14][15] = 285; m[14][16] = 118; m[14][17] =   6; m[14][18] =  10; m[14][19] =  23; 
	m[15][ 0] = 378; m[15][ 1] = 101; m[15][ 2] = 503; m[15][ 3] =  59; m[15][ 4] = 223; 
	m[15][ 5] =  53; m[15][ 6] =  30; m[15][ 7] = 201; m[15][ 8] =  73; m[15][ 9] =  40; 
	m[15][10] =  59; m[15][11] =  47; m[15][12] =  29; m[15][13] =  92; m[15][14] = 285; 
	m[15][15] =   0; m[15][16] = 477; m[15][17] =  35; m[15][18] =  63; m[15][19] =  38; 
	m[16][ 0] = 475; m[16][ 1] =  64; m[16][ 2] = 232; m[16][ 3] =  38; m[16][ 4] =  42; 
	m[16][ 5] =  51; m[16][ 6] =  32; m[16][ 7] =  33; m[16][ 8] =  46; m[16][ 9] = 245; 
	m[16][10] =  25; m[16][11] = 103; m[16][12] = 226; m[16][13] =  12; m[16][14] = 118; 
	m[16][15] = 477; m[16][16] =   0; m[16][17] =  12; m[16][18] =  21; m[16][19] = 112; 
	m[17][ 0] =   9; m[17][ 1] = 126; m[17][ 2] =   8; m[17][ 3] =   4; m[17][ 4] = 115; 
	m[17][ 5] =  18; m[17][ 6] =  10; m[17][ 7] =  55; m[17][ 8] =   8; m[17][ 9] =   9; 
	m[17][10] =  52; m[17][11] =  10; m[17][12] =  24; m[17][13] =  53; m[17][14] =   6; 
	m[17][15] =  35; m[17][16] =  12; m[17][17] =   0; m[17][18] =  71; m[17][19] =  25; 
	m[18][ 0] =  11; m[18][ 1] =  20; m[18][ 2] =  70; m[18][ 3] =  46; m[18][ 4] = 209; 
	m[18][ 5] =  24; m[18][ 6] =   7; m[18][ 7] =   8; m[18][ 8] = 573; m[18][ 9] =  32; 
	m[18][10] =  24; m[18][11] =   8; m[18][12] =  18; m[18][13] = 536; m[18][14] =  10; 
	m[18][15] =  63; m[18][16] =  21; m[18][17] =  71; m[18][18] =   0; m[18][19] =  16; 
	m[19][ 0] = 298; m[19][ 1] =  17; m[19][ 2] =  16; m[19][ 3] =  31; m[19][ 4] =  62; 
	m[19][ 5] =  20; m[19][ 6] =  45; m[19][ 7] =  47; m[19][ 8] =  11; m[19][ 9] = 961; 
	m[19][10] = 180; m[19][11] =  14; m[19][12] = 323; m[19][13] =  62; m[19][14] =  23; 
	m[19][15] =  38; m[19][16] = 112; m[19][17] =  25; m[19][18] =  16; m[19][19] =   0; 
    
    VectorUtilities::normalize(stationary_freqs);
    
	stationary_freqs[ 0] = 0.076748;
	stationary_freqs[ 1] = 0.051691;
	stationary_freqs[ 2] = 0.042645;
	stationary_freqs[ 3] = 0.051544;
	stationary_freqs[ 4] = 0.019803;
	stationary_freqs[ 5] = 0.040752;
	stationary_freqs[ 6] = 0.061830;
	stationary_freqs[ 7] = 0.073152;
	stationary_freqs[ 8] = 0.022944;
	stationary_freqs[ 9] = 0.053761;
	stationary_freqs[10] = 0.091904;
	stationary_freqs[11] = 0.058676;
	stationary_freqs[12] = 0.023826;
	stationary_freqs[13] = 0.040126;
	stationary_freqs[14] = 0.050901;
	stationary_freqs[15] = 0.068765;
	stationary_freqs[16] = 0.058565;
	stationary_freqs[17] = 0.014261;
	stationary_freqs[18] = 0.032102;
	stationary_freqs[19] = 0.066005;
    
    // multiply stationary frequencies into exchangeability matrix
    for (size_t i = 0; i < 20; i++)
    {
        for (size_t j = 0; j < 20; j++)
        {
            m[i][j] *= stationary_freqs[j];
        }
    }
    
    // set the diagonal values
    setDiagonal();
    
    // rescale 
    rescaleToAverageRate( 1.0 );
    
    // update the eigensystem
    updateEigenSystem();
    
}


/** Destructor */
RateMatrix_Jones::~RateMatrix_Jones(void) {
    
}




RateMatrix_Jones* RateMatrix_Jones::clone( void ) const {
    return new RateMatrix_Jones( *this );
}

