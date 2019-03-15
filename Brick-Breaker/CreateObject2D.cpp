#include "CreateObject2D.h"

#include <iostream>
#include <Core/Engine.h>

Mesh* CreateObject2D::CreateBrick(std::string name, glm::vec3 startPosition, float lengthX, float lengthY, glm::vec3 color)
{
	std::vector<VertexFormat> vertices = //order matters, take care!
	{
		VertexFormat(glm::vec3(0), color), //startPosition means bottomLeft
		VertexFormat(glm::vec3(lengthX, 0, 0), color),
		VertexFormat(glm::vec3(lengthX, lengthY, 0), color),
		VertexFormat(glm::vec3(0, lengthY, 0), color)
	};

	Mesh* brick = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 }; //last 0, 2 for filling

	brick->startingPosition = startPosition;
	brick->InitFromData(vertices, indices);
	return brick;
}
Mesh* CreateObject2D::CreateWall(std::string name, int number, float length, glm::vec3 color, float resolutionX, float resolutionY)
{
	std::vector<VertexFormat> vertices;
	float wallError = 75;

	if (number == 1) {
		vertices = 
		{
			VertexFormat(glm::vec3(0, wallError, 0), color),
			VertexFormat(glm::vec3(length, wallError, 0), color),
			VertexFormat(glm::vec3(length, resolutionY, 0), color),
			VertexFormat(glm::vec3(0, resolutionY, 0), color)
		};
	}
	else if (number == 2) {
		vertices =
		{
			VertexFormat(glm::vec3(0, resolutionY, 0), color),
			VertexFormat(glm::vec3(0, resolutionY - length, 0), color),
			VertexFormat(glm::vec3(resolutionX, resolutionY - length, 0), color),
			VertexFormat(glm::vec3(resolutionX, resolutionY, 0), color)
		};
	}
	else if (number == 3) {
		vertices =
		{
			VertexFormat(glm::vec3(resolutionX, resolutionY, 0), color),
			VertexFormat(glm::vec3(resolutionX, wallError, 0), color),
			VertexFormat(glm::vec3(resolutionX - length, wallError, 0), color),
			VertexFormat(glm::vec3(resolutionX - length, resolutionY, 0), color)
		};
	}
	else { //POWERUP
		vertices =
		{
			VertexFormat(glm::vec3(0, 0, 0), color),
			VertexFormat(glm::vec3(resolutionX, 0, 0), color),
			VertexFormat(glm::vec3(resolutionX, length, 0), color),
			VertexFormat(glm::vec3(0, length, 0), color)
		};
	}

	Mesh* wall = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	wall->InitFromData(vertices, indices);
	return wall;
}
Mesh* CreateObject2D::CreatePlatform(std::string name, glm::vec3 startPosition, float lengthX, float lengthY, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(startPosition + glm::vec3(-lengthX / 2, 0, 0), color),
		VertexFormat(startPosition + glm::vec3(lengthX / 2, 0, 0), color),
		VertexFormat(startPosition + glm::vec3(lengthX / 2, lengthY, 0), color),
		VertexFormat(startPosition + glm::vec3(-lengthX / 2, lengthY, 0), color)
	};

	Mesh* platform = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	platform->InitFromData(vertices, indices);
	return platform;
}
Mesh* CreateObject2D::CreateSquare(std::string name, glm::vec3 startPosition, float length, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(startPosition, color),
		VertexFormat(startPosition + glm::vec3(length, 0, 0), color),
		VertexFormat(startPosition + glm::vec3(length, length, 0), color),
		VertexFormat(startPosition + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	square->InitFromData(vertices, indices);
	return square;
}
Mesh* CreateObject2D::CreateCircle(std::string name, glm::vec3 centerPosition, float radius, glm::vec3 color)
{
	std::vector<VertexFormat> vertices;
	float pi = 3.1415926f;
	float limit = 2.0f * pi;
	float step = limit / 30; //accuracy 30

	for (float i = 0.0f; i <= limit; i += step) {
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(i), radius * sin(i), 0), color));
	}

	Mesh* circle = new Mesh(name);
	circle->startingPosition = centerPosition;
	circle->SetDrawMode(GL_TRIANGLE_FAN); //GL_TRIANGLE_FAN for filling, GL_LINE_LOOP for a disk (hollow circle)
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
	circle->InitFromData(vertices, indices);
	return circle;
}

Mesh* CreateObject2D::CreateSurpriseOne() {

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0), glm::vec3(0)),
		VertexFormat(glm::vec3(10, 0, 0), glm::vec3(0)),
		VertexFormat(glm::vec3(10, 50, 0), glm::vec3(0)),
		VertexFormat(glm::vec3(0, 50, 0), glm::vec3(0))
	};

	Mesh* surprise = new Mesh("surprise");
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	surprise->InitFromData(vertices, indices);
	return surprise;
}
Mesh* CreateObject2D::CreateSurpriseTwo() {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0), glm::vec3(0)),
		VertexFormat(glm::vec3(30, 0, 0), glm::vec3(0)),
		VertexFormat(glm::vec3(30, 10, 0), glm::vec3(0)),
		VertexFormat(glm::vec3(0, 10, 0), glm::vec3(0))
	};

	Mesh* surprise = new Mesh("surprise");
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	surprise->InitFromData(vertices, indices);
	return surprise;
}
Mesh* CreateObject2D::CreateSurpriseThree() {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0), glm::vec3(0)),
		VertexFormat(glm::vec3(10, 0, 0), glm::vec3(0)),
		VertexFormat(glm::vec3(10, 20, 0), glm::vec3(0)),
		VertexFormat(glm::vec3(0, 20, 0), glm::vec3(0))
	};

	Mesh* surprise = new Mesh("surprise");
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	surprise->InitFromData(vertices, indices);
	return surprise;
}
Mesh* CreateObject2D::CreateSurpriseFour() {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0), glm::vec3(0)),
		VertexFormat(glm::vec3(20, 0, 0), glm::vec3(0)),
		VertexFormat(glm::vec3(20, 10, 0), glm::vec3(0)),
		VertexFormat(glm::vec3(0, 10, 0), glm::vec3(0))
	};

	Mesh* surprise = new Mesh("surprise");
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	surprise->InitFromData(vertices, indices);
	return surprise;
}

float CreateObject2D::select(float value, float left, float right) {
	return (float)std::max(left, (float)std::min(right, value));
}
bool CreateObject2D::circleCollision(glm::vec3 ballPosition, float ballRadius, float leftX, float rightX, float upY, float downY)
{
	float closestX = select(ballPosition.x, leftX, rightX); //closest x from the ball center
	float closestY = select(ballPosition.y, downY, upY); //closest y from the ball center

	float distanceX = ballPosition.x - closestX; //actual distance on x axis
	float distanceY = ballPosition.y - closestY; //actual distance on y axis

	float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY); //pitagora
	return (distanceSquared < (ballRadius * ballRadius)); //we don't know the point of collision, but we know that it exists
}
bool CreateObject2D::polygonCollision(float left1X, float right1X, float up1Y, float down1Y, float left2X, float right2X, float up2Y, float down2Y) {
	return (right1X >= left2X && right2X >= left1X && up1Y >= down2Y && up2Y >= down1Y);
}

glm::vec3 CreateObject2D::platformReflexion(glm::vec3 ballPosition, float ballRadius, glm::vec3 platformPosition, float platformWidth, float platformHeight) {

	float beginX = platformPosition.x - platformWidth / 2; //beginning of the platform on Ox axis

	float x = abs(ballPosition.x - beginX - platformWidth / 2); //distance from the platform center to the ball
	float y = platformWidth / 2; //the radius of the imaginated circle, approximated to establish the angle
	float hypotenuse = sqrt((x * x) + (y * y));

	float xCoord = x / hypotenuse; //default, they are
	float yCoord = y / hypotenuse; //all positive

	if (ballPosition.x < platformPosition.x -ballRadius) {
		xCoord = -abs(xCoord); //negative cos
	}
	if (ballPosition.y < platformPosition.y) {
		yCoord = -abs(yCoord); //negative sin
	}

	return glm::vec3(xCoord, yCoord, 0);
}
glm::vec3 CreateObject2D::normalReflexion(glm::vec3 ballPosition, float ballRadius, float leftX, float rightX, float upY, float downY, float ballX, float ballY) {

	//default cases->
	if (ballX == 0.)	return glm::vec3(ballX, -ballY, 0);
	if (ballY == 0)	return glm::vec3(-ballX, ballY, 0);

	float width = rightX - leftX;
	float height = upY - downY;

	glm::vec3 result = glm::vec3(ballX, ballY, 0);
	glm::vec3 objectPosition = glm::vec3(leftX + width / 2, downY + height / 2, 0);
	glm::vec3 differencePosition = abs(ballPosition - objectPosition);
	glm::vec3 scalingVector;

	if (width >= height) scalingVector = glm::vec3((upY - downY) / (rightX - leftX), 1, 1); //scale the width
	else scalingVector = glm::vec3(1, (rightX - leftX) / (upY - downY), 1); //scale the height

	differencePosition *= scalingVector;

	if (abs(differencePosition.x) >= abs(differencePosition.y))	result.x = -ballX;
	if (abs(differencePosition.y) >= abs(differencePosition.x))	result.y = -ballY;

	return result;
}