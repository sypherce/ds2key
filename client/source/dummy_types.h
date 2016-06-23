#pragma once

//the following typedef's are dummy's
#if defined(_NDS)
/// Circle Pad position.
typedef struct
{
	s16 dx; ///< Pad X
	s16 dy; ///< Pad Y
} circlePosition;

/// Accelerometer vector.
typedef struct
{
	s16 x; ///< Accelerometer X
	s16 y; ///< Accelerometer Y
	s16 z; ///< Accelerometer Z
} accelVector;

/// Gyroscope angular rate.
typedef struct
{
	s16 x; ///< Roll
	s16 z; ///< Yaw
	s16 y; ///< Pitch
} angularRate;
#endif
