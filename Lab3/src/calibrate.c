void calibrate(int8_t x_acceleration, int8_t y_acceleration, int8_t z_acceleration) {
	acceleration[0] += calibration[0][0]*x_acceleration+calibration[0][1]*y_acceleration+calibration[0][2]*z_acceleration+calibration[3][0];
	acceleration[1] += calibration[1][0]*x_acceleration+calibration[1][1]*y_acceleration+calibration[1][2]*z_acceleration+calibration[3][1];
	acceleration[2] += calibration[2][0]*x_acceleration+calibration[2][1]*y_acceleration+calibration[2][2]*z_acceleration+calibration[3][2];
}

float getPitch(float ax, float ay, float az){

	return atan(ax/sqrt((ay*ay)+(az*az)))* 180 / PI;

}

float getRoll(float ax, float ay, float az){

	return atan(ay/sqrt((ax*ax)+(az*az)))* 180 / PI;

}
