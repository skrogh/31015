//=====================================================================================================
// MadgwickAHRS.h
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
// 11/06/2014	Søren Andersen	Encaptulated and changed to work on doubles
//
//=====================================================================================================
#ifndef MadgwickAHRS_h
#define MadgwickAHRS_h

//----------------------------------------------------------------------------------------------------
// Variable declaration


typedef struct {
	double sampleFreq;		// sample frequency in hertz
	double beta;	// algorithm gain
	double q[4];	// quaternion of sensor frame relative to auxiliary frame
} AHRS_t;
//---------------------------------------------------------------------------------------------------
// Function declarations

void MadgwickAHRSinit( AHRS_t* AHRS, double qInit[4], double beta, double sampleFreq );
void MadgwickAHRSupdate( AHRS_t* AHRS, double gx, double gy, double gz, double ax, double ay, double az, double mx, double my, double mz);
void MadgwickAHRSupdateIMU( AHRS_t* AHRS, double gx, double gy, double gz, double ax, double ay, double az);

#endif
//=====================================================================================================
// End of file
//=====================================================================================================
