//=====================================================================================================
// quaternion.h
//=====================================================================================================
//
//
//
//=====================================================================================================
#ifndef QUATERNION_h
#define QUATERNION_h

//----------------------------------------------------------------------------------------------------
// Variable declaration


//---------------------------------------------------------------------------------------------------
// Function declarations

// Multiply quaternion  res = q1*q2
void  quaternionMult( double res[3], double q1[4], double q2[4] );
// Rotate q1 by q2 and place in res
void quaternionRotate( double res[4], double q1[4], double q2[4] );
// Clone q in res
void quaternionClone( double res[4], double q[4] );
// Rotate q1 by q2 and place in res
void quaternionConjugate( double res[4], double q1[4] );

// Rotate v by q2 and place in res
void quaternionVectorRotate( double res[3], double v[3], double q[4] );



#endif
//=====================================================================================================
// End of file
//=====================================================================================================
