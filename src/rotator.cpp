#include "rotator.h"

void KeyTranslator::init(GLFWwindow *window) {
	horizontal = 0.0;
	zoom = 0.0;
	lastTime = glfwGetTime();
};

void KeyTranslator::poll(GLFWwindow *window) {

	double currentTime, elapsedTime;

	currentTime = glfwGetTime();
	elapsedTime = currentTime - lastTime;
	lastTime = currentTime;

	if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
		horizontal += elapsedTime*5.0; //Move right with speed 5*dt
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT)) {
		horizontal -= elapsedTime*5.0; //Move left with speed 5*dt

	}

	if (glfwGetKey(window, GLFW_KEY_UP)) {
		zoom += elapsedTime*2.0; // Zoom in with speed 3*dt
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN)) {
		zoom -= elapsedTime*2.0; // Zoom out with speed 3*dt
	}
}

void MouseRotator::init(GLFWwindow *window) {
	phi = 0.0;
	theta = 0.0;
	transX = 0.0f;
	transY = 0.0f;
	zoom = 0.0;
	rotStarted = false;

	glfwGetCursorPos(window, &lastX, &lastY);
	lastLeft = GL_FALSE;
	lastRight = GL_FALSE;
}

void MouseRotator::poll(GLFWwindow *window) {

	double currentX;
	double currentY;
	int currentLeft;
	int currentRight;
	int currentMiddle;
	int alt;
	double moveX;
	double moveY;
	int windowWidth;
	int windowHeight;

	// Find out where the mouse pointer is, and which buttons are pressed
	glfwGetCursorPos(window, &currentX, &currentY);
	currentLeft = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	currentRight = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	currentMiddle = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
	alt = true;

	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	if (alt || rotStarted) {
		if (currentLeft && lastLeft) { // If a left button drag is in progress
			moveX = currentX - lastX;
			moveY = currentY - lastY;
			phi += M_PI * moveX / windowWidth; // Longest drag rotates 180 degrees
			if (phi >= M_PI*2.0 + 0.001f) phi = 0.0f;
			if (phi <= 0.0 - 0.001f) phi = M_PI*2.0f;

			theta += M_PI * moveY / windowHeight; // Longest drag rotates 180 deg
			if (theta >= M_PI/2.0f - 0.05f) theta =M_PI/2.0f - 0.05f;
			if (theta <= -M_PI/2.0f + 0.05f) theta =-M_PI/2.0f + 0.05f;
			rotStarted = true;
		}
		else rotStarted = false;

		if (currentMiddle && lastMiddle) { // If a left button drag is in progress
			moveX = currentX - lastX;
			moveY = currentY - lastY;
			transX -= moveX / 500.0f;
			transY += moveY / 500.0f;
		}

		if (currentRight && lastRight) { // If a left button drag is in progress
			moveY = currentY - lastY;
			zoom -= moveY / 1.0f;
		}
	}
	lastLeft = currentLeft;
	lastRight = currentRight;
	lastMiddle = currentMiddle;
	lastX = currentX;
	lastY = currentY;
}