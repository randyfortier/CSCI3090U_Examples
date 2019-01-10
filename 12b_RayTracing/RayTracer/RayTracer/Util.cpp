float clamp(float value) {
	if (value < 0.0f) {
		return 0.0f;
	}
	else if (value > 1.0f) {
		return 1.0f;
	}
	return value;
}

float max(float val1, float val2) {
	if (val1 > val2) {
		return val1;
	}
	return val2;
}