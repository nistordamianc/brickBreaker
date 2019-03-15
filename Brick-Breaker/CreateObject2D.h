#pragma once
#define NOMINMAX
#include <string>
#include <algorithm>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace CreateObject2D
{
	Mesh* CreateBrick(std::string name, glm::vec3 startPosition, float lengthX, float lengthY, glm::vec3 color);
	Mesh* CreateWall(std::string name, int number, float length, glm::vec3 color, float resolutionX, float resolutionY);
	Mesh* CreateCircle(std::string name, glm::vec3 centerPosition, float radius, glm::vec3 color);
	Mesh* CreatePlatform(std::string name, glm::vec3 startPosition, float lengthX, float lengthY, glm::vec3 color);
	Mesh* CreateSquare(std::string name, glm::vec3 startPosition, float length, glm::vec3 color);

	Mesh* CreateSurpriseOne();
	Mesh* CreateSurpriseTwo();
	Mesh* CreateSurpriseThree();
	Mesh* CreateSurpriseFour();

	float select(float value, float left, float right);
	bool circleCollision(glm::vec3 ballPosition, float ballRadius, float leftX, float rightX, float upY, float downY);
	bool polygonCollision(float left1X, float right1X, float up1Y, float down1Y, float left2X, float right2X, float up2Y, float down2Y);

	glm::vec3 normalReflexion(glm::vec3 ballPosition, float ballRadius, float leftX, float rightX, float upY, float downY, float ballX, float ballY);
	glm::vec3 platformReflexion(glm::vec3 ballPosition, float ballRadius, glm::vec3 platformPosition, float platformWidth, float platformHeight);

}
#pragma once
