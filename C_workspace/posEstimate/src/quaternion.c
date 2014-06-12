//=====================================================================================================
// quaternion.c
//=====================================================================================================
//
//
//
//=====================================================================================================

//---------------------------------------------------------------------------------------------------
// Header files

#include "quaternion.h"

//---------------------------------------------------------------------------------------------------
// Function declarations


//====================================================================================================
// Functions
void  quaternionMult( double res[4], double q1[4], double q2[4] ) {
	double _res[4];

	_res[0] = q1[0] * q2[0] - q1[1]*q2[1] - q1[2]*q2[2] - q1[3]*q2[3];
	_res[1] = q1[0] * q2[1] + q1[1]*q2[0] + q1[2]*q2[3] - q1[3]*q2[2];
	_res[2] = q1[0] * q2[2] - q1[1]*q2[3] + q1[2]*q2[0] + q1[3]*q2[1];
	_res[3] = q1[0] * q2[3] + q1[1]*q2[2] - q1[2]*q2[1] + q1[3]*q2[0];

	res[0] = _res[0];
	res[1] = _res[1];
	res[2] = _res[2];
	res[3] = _res[3];
}

// Rotate q1 by q2 and place in res
void quaternionRotate( double res[4], double q1[4], double q2[4] ) {
	double _q2[4]; // q2 conjugated
	quaternionConjugate( _q2, q2 );
	quaternionMult( res, q1, _q2 );
	quaternionMult( res, q2, res );
}
// Clone q in res
void quaternionClone( double res[4], double q[4] ) {
	res[0] = q[0];
	res[1] = q[1];
	res[2] = q[2];
	res[3] = q[3];
}
// Conjugate q, place in res
void quaternionConjugate( double res[4], double q[4] ) {
	res[0] = q[0];
	res[1] = -q[1];
	res[2] = -q[2];
	res[3] = -q[3];
}

// Rotate v by q2 and place in res
void quaternionVectorRotate( double res[3], double v[3], double q[4] ) {
	double _v[4] = { 0, v[0], v[1], v[2] };
	double _res[4];
	quaternionRotate( _res, _v, q );
	res[0] = _res[1];
	res[1] = _res[2];
	res[2] = _res[3];
}



//====================================================================================================
// END OF CODE
//====================================================================================================

