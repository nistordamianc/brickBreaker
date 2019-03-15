#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>

class BrickBreaker : public SimpleScene
{
public:
	BrickBreaker();
	~BrickBreaker();

	void Init() override;

private:

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

protected:
	//colors->
	glm::vec3 bricksColor, wallsColor, livesColor, platformColor, ballColor, squareColor, backgroundColor, fallenBricksColor, initialLivesColor;
	//positions->
	glm::vec3 firstLifePosition, secondLifePosition, thirdLifePosition, ballPosition, platformPosition, leftBottomSquare, powerupPosition;
	//sizes->
	float bricksWidth, bricksHeight, wallsDimension, platformWidth, platformHeight, squareDimension, ballRadius, lifeRadius;
	//helpers->	
	float ballSpeedX, powerupRotationSpeed, powerupThreshold, initialBallY, wallError, initialBallSpeed,
		powerupTimer, powerupRotation, powerupExists, ballSpeed, powerupSpeed, queenCount;
	int powerup, gameIsRunning, numberOfLives, powerupChance;
	bool refresh, platformCollided, rocker, queenColor, leftCollided, rightCollided, upperCollided, bottomCollided, gameOver;
	//extended helpers->
	std::vector<Mesh*> currentBricks; glm::ivec2 resolution; glm::mat3 modelMatrix; Mesh* fallenBrick; glm::vec3 ballDirection, helperPosition;
	//surprises->
	Mesh* surpriseOne, *surpriseTwo, *surpriseThree, *surpriseFour;
};