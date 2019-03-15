#include <vector>
#include <iostream>

#include "BrickBreaker.h"
#include "Transform2D.h"
#include "CreateObject2D.h"
#include <Core/Engine.h>

#include <Mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "Winmm.lib")

using namespace std;

BrickBreaker::BrickBreaker()
{
}

BrickBreaker::~BrickBreaker()
{
}


void BrickBreaker::Init()
{
	//music->
	mciSendString(TEXT("open \"queen.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("open \"rocker.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("open \"berry.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("open \"heart.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("open \"thriller.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("open \"mario.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	//for safety, don't activate music with the maximum ball speed

	//camera and resolution->
	resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400); //gets proper camera view, according to resolution
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false); //stops the motion of the camera

	//sizes->
	bricksWidth = 50, bricksHeight = 25, wallsDimension = 10, platformWidth = 200;
	platformHeight = 15, squareDimension = 25, ballRadius = 12.5f, lifeRadius = 10;

	//colors->
	bricksColor.r = 0.0f, bricksColor.g = 0.2f, bricksColor.b = 0.5f;
	wallsColor.r = 0.05f, wallsColor.g = 0.35f, wallsColor.b = 0.2f;
	squareColor.r = 0.99f, squareColor.g = 0.5f, squareColor.b = 0.35f;
	livesColor.r = 0.9f, livesColor.g = 0.0f, livesColor.b = 0.0f; initialLivesColor = livesColor;
	platformColor.r = 0.4f, platformColor.g = 0.0f, platformColor.b = 0.6f;
	ballColor.r = 0.7f, ballColor.g = 0.0f, ballColor.b = 0.4f;
	fallenBricksColor.r = 0.42f, fallenBricksColor.g = 0.0f, fallenBricksColor.b = 0.21f;
	backgroundColor = glm::vec3(0, 0, 0);

	//positions->
	firstLifePosition = glm::vec3(25, 25, 0);
	secondLifePosition = glm::vec3(50, 25, 0);
	thirdLifePosition = glm::vec3(75, 25, 0);
	ballPosition = glm::vec3(0, 55, 0); initialBallY = ballPosition.y;
	platformPosition = glm::vec3(0, 20, 0);
	powerupPosition = glm::vec3(0, 0, 0);
	leftBottomSquare = glm::vec3(-squareDimension / 2, -squareDimension / 2, 0); //for self-rotation square

	//helpers->
	powerupRotation = 0; powerupExists = 0; powerup = 0, powerupTimer = 0;
	powerupThreshold = 15, powerupSpeed = 150, powerupRotationSpeed = 5;
	ballSpeed = 200, initialBallSpeed = 200, ballDirection.x = 0, ballDirection.y = 1;
	wallError = 75, gameIsRunning = 0, numberOfLives = 3; queenCount = 1;
	platformCollided = true, rocker = false, refresh = false;
	leftCollided = false, rightCollided = false, upperCollided = false, bottomCollided = false;

	//temporarily helpers->
	float distanceOX = 105, distanceOY = 50; //helpers to make the view symmetrically
	int bricksNumber = 0; string bricksName; //helpers for brick names

	//bricks->
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 18; j++) {
			bricksName = "brick" + bricksNumber; //for further use and/or OCD, mostly OCD..
			helperPosition = glm::vec3 (distanceOX + 60 * j, resolution.y - 60 - (35 * i), 0); //calculate position
			Mesh* newBrick = CreateObject2D::CreateBrick(bricksName, helperPosition, bricksWidth, bricksHeight, bricksColor);
			newBrick->isDestroyed = 0;
			currentBricks.push_back(newBrick); //add the new brick in the vector
			bricksNumber++;
		}
	}
	
	//fallen bricks
	fallenBrick = CreateObject2D::CreateBrick(bricksName, glm::vec3(0, 0, 0), bricksWidth, bricksHeight, fallenBricksColor);

	//walls->
	Mesh* leftWall = CreateObject2D::CreateWall("leftWall", 1, wallsDimension, wallsColor, (float)resolution.x, (float) resolution.y);
	AddMeshToList(leftWall);
	Mesh* upperWall = CreateObject2D::CreateWall("upperWall", 2, wallsDimension, wallsColor, (float)resolution.x, (float)resolution.y);
	AddMeshToList(upperWall);
	Mesh* rightWall = CreateObject2D::CreateWall("rightWall", 3, wallsDimension, wallsColor, (float)resolution.x, (float)resolution.y);
	AddMeshToList(rightWall);
	Mesh* powerupWall = CreateObject2D::CreateWall("powerupWall", 13, wallsDimension, wallsColor, (float)resolution.x, (float)resolution.y);
	AddMeshToList(powerupWall);

	//ball, platform and square (powerup)->
	Mesh* ball = CreateObject2D::CreateCircle("ball", ballPosition, ballRadius, ballColor);
	AddMeshToList(ball);
	Mesh* platform = CreateObject2D::CreatePlatform("platform", platformPosition, platformWidth, platformHeight, platformColor);
	AddMeshToList(platform);
	Mesh* square = CreateObject2D::CreateSquare("square", leftBottomSquare, squareDimension, squareColor);
	AddMeshToList(square);

	//surprises->
	surpriseOne = CreateObject2D::CreateSurpriseOne();
	surpriseTwo = CreateObject2D::CreateSurpriseTwo();
	surpriseThree = CreateObject2D::CreateSurpriseThree();
	surpriseFour = CreateObject2D::CreateSurpriseFour();
}

void BrickBreaker::FrameStart()
{
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1); //sets the color buffer to white
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears the color buffer using the set color

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y); //sets the screen area where to draw
}

void BrickBreaker::Update(float deltaTimeSeconds)
{
	//lives and game over->
	Mesh* firstLife = CreateObject2D::CreateCircle("firstLife", firstLifePosition, lifeRadius, livesColor);
	Mesh* secondLife = CreateObject2D::CreateCircle("secondLife", secondLifePosition, lifeRadius, livesColor);
	Mesh* thirdLife = CreateObject2D::CreateCircle("thirdLife", thirdLifePosition, lifeRadius, livesColor);

	if (numberOfLives > 0) {
		RenderMesh2D(firstLife, shaders["VertexColor"], Transform2D::Translate(firstLifePosition.x, firstLifePosition.y));
		if (numberOfLives > 1) {
			RenderMesh2D(secondLife, shaders["VertexColor"], Transform2D::Translate(secondLifePosition.x, secondLifePosition.y));
			if (numberOfLives > 2) {
				RenderMesh2D(thirdLife, shaders["VertexColor"], Transform2D::Translate(thirdLifePosition.x, thirdLifePosition.y));
			}
		}
	}
	else {
		numberOfLives = 3;
		for (auto elem : currentBricks) { //reload the bricks
			elem->isDestroyed = 0;
			elem->scaleFactor = 1;
		}
		ballDirection.x = 0; 
		ballDirection.y = 1;
	}

	//walls and powerups->
	RenderMesh2D(meshes["leftWall"], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes["upperWall"], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes["rightWall"], shaders["VertexColor"], glm::mat3(1));

	if (powerupExists == 1) {
		if (powerupPosition.y < -squareDimension / 2) { //the powerup exits the map
			powerupExists = 0;
			powerup = 0;
		}
		if (CreateObject2D::polygonCollision(platformPosition.x - platformWidth / 2, platformPosition.x + platformWidth / 2,
			platformPosition.y + platformHeight, platformPosition.y, powerupPosition.x - squareDimension / 2, powerupPosition.x + squareDimension / 2,
			powerupPosition.y + squareDimension / 2, powerupPosition.y - squareDimension / 2)) {
			powerup = 1;
			powerupExists = 0;
		}
		else {
			modelMatrix = glm::mat3(1);
			powerupRotation += powerupRotationSpeed * deltaTimeSeconds;
			powerupPosition.y -= powerupSpeed * deltaTimeSeconds;
			modelMatrix *= Transform2D::Translate(powerupPosition.x, powerupPosition.y);
			modelMatrix *= Transform2D::Rotate(powerupRotation);
			RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
		}
	}

	if (powerup == 1) {
		RenderMesh2D(meshes["powerupWall"], shaders["VertexColor"], glm::mat3(1));
		powerupTimer += deltaTimeSeconds;
		if (powerupTimer >= 10) {
			powerupTimer = 0;
			powerup = 0;
		}
	}

	//platform and ball->
	platformPosition.x = window->GetCursorPosition().x;
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(platformPosition.x, 0);
	RenderMesh2D(meshes["platform"], shaders["VertexColor"], modelMatrix);

	if (gameIsRunning == 0) {
		ballPosition.y = initialBallY;
		ballPosition.x = platformPosition.x;
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(ballPosition.x, ballPosition.y);
		RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
	} 
	else { //the ball is moving!
		
		ballPosition.x += ballDirection.x * ballSpeed * deltaTimeSeconds;
		ballPosition.y += ballDirection.y * ballSpeed * deltaTimeSeconds;
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(ballPosition.x, ballPosition.y);
		RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
		
		//the ball exits the map->
		if (ballPosition.x < -ballRadius || ballPosition.x > resolution.x + ballRadius
			|| ballPosition.y < -ballRadius || ballPosition.y > resolution.y + ballRadius) {
			numberOfLives--;
			gameIsRunning = 0;
			ballSpeed = initialBallSpeed;
			powerup = 0; powerupTimer = 0; powerupExists = 0;
			ballDirection.x = 0; ballDirection.y = 1;
			platformCollided = false;
		}

		//platform collision->
		if (CreateObject2D::circleCollision(ballPosition, ballRadius, platformPosition.x - platformWidth / 2,
			platformPosition.x + platformWidth / 2, platformPosition.y + platformHeight, platformPosition.y) &&
			platformCollided == false) {
			ballDirection = CreateObject2D::platformReflexion(ballPosition, ballRadius, platformPosition, platformWidth, platformHeight);
			platformCollided = true;
			upperCollided = false;
			bottomCollided = false;
			leftCollided = false;
			rightCollided = false;
		}

		//wall collisions->
		if (CreateObject2D::circleCollision(ballPosition, ballRadius, (float) 0, wallsDimension, resolution.y, wallError)
			&& leftCollided == false) { //leftWall
			ballDirection = CreateObject2D::normalReflexion(ballPosition, ballRadius, (float)0, wallsDimension , resolution.y, wallError,
				ballDirection.x, ballDirection.y);
			leftCollided = true;
			platformCollided = false;
			upperCollided = false;
			bottomCollided = false;
			rightCollided = false;
		}
		if (CreateObject2D::circleCollision(ballPosition, ballRadius, (float) 0, resolution.x, resolution.y, resolution.y - wallsDimension)
			&& upperCollided == false) { //upperWall
			ballDirection = CreateObject2D::normalReflexion(ballPosition, ballRadius, (float) 0, resolution.x, resolution.y, resolution.y - wallsDimension,
				ballDirection.x, ballDirection.y);
			upperCollided = true;
			platformCollided = false;
			bottomCollided = false;
			leftCollided = false;
			rightCollided = false;
		}
		if (CreateObject2D::circleCollision(ballPosition, ballRadius, resolution.x - wallsDimension, resolution.x, resolution.y, wallError)
			&& rightCollided == false) { //rightWall
			ballDirection = CreateObject2D::normalReflexion(ballPosition, ballRadius, resolution.x - wallsDimension, resolution.x, resolution.y, wallError,
				ballDirection.x, ballDirection.y);
			rightCollided = true;
			platformCollided = false;
			upperCollided = false;
			bottomCollided = false;
			leftCollided = false;
		}
		if (CreateObject2D::circleCollision(ballPosition, ballRadius, (float) 0, resolution.x, wallsDimension, (float) 0) && powerup == 1
			&& bottomCollided == false) { //bottomWall
			ballDirection = CreateObject2D::normalReflexion(ballPosition, ballRadius,  (float) 0, resolution.x, wallsDimension, (float) 0,
				ballDirection.x, ballDirection.y);
			bottomCollided = true;
			platformCollided = false;
			upperCollided = false;
			leftCollided = false;
			rightCollided = false;
		}

		//bricks collisions->
		for (auto elem : currentBricks) {
			if (elem->isDestroyed == 0 && CreateObject2D::circleCollision(ballPosition, ballRadius, elem->startingPosition.x,
				elem->startingPosition.x + bricksWidth, elem->startingPosition.y + bricksHeight, elem->startingPosition.y)) {
				
				platformCollided = false;
				upperCollided = false;
				bottomCollided = false;
				leftCollided = false;
				rightCollided = false;

				elem->isDestroyed = 1;
				elem->scaleFactor = 1;
				ballDirection = CreateObject2D::normalReflexion(ballPosition, ballRadius, elem->startingPosition.x, 
					elem->startingPosition.x + bricksWidth, elem->startingPosition.y + bricksHeight, elem->startingPosition.y,
					ballDirection.x, ballDirection.y);

				powerupChance = rand() % 100;
				if (powerupChance <= powerupThreshold && powerupExists == 0 && powerup == 0) {
					powerupExists = 1;
					powerupPosition = elem->startingPosition;
					powerupPosition.x += bricksWidth / 2;
				}
			}
		}
	}

	//bricks rendering->
	if (refresh == true) {
		for (auto elem : currentBricks)
			elem->isDestroyed = 0;
		refresh = false;
	}

	for (auto elem : currentBricks) {
		if (elem->isDestroyed == 0) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(elem->startingPosition.x, elem->startingPosition.y);
			RenderMesh2D(elem, shaders["VertexColor"], modelMatrix);
		}
		if (elem->isDestroyed == 1) {
			if (elem->scaleFactor >= 0) elem->scaleFactor -= deltaTimeSeconds;
			else elem->scaleFactor = 0; //for approximation errors
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(elem->startingPosition.x, elem->startingPosition.y); //translates to where it belongs
			modelMatrix *= Transform2D::Translate(bricksWidth / 2, bricksHeight / 2); //it goes to the initial position (0, 0, 0)
			modelMatrix *= Transform2D::Scale(elem->scaleFactor, elem->scaleFactor); //beggining the dissolving
			modelMatrix *= Transform2D::Translate(-bricksWidth / 2, -bricksHeight / 2); //it get destroyed relative to its center
			RenderMesh2D(fallenBrick, shaders["VertexColor"], modelMatrix); //to be read bottom-up
		}
	}

	if (queenColor == true) {
		if (livesColor.g > 0.8f) queenCount = -1;
		if (livesColor.g < 0.05f) queenCount = 1;
		livesColor.g += queenCount * deltaTimeSeconds / 2;
	}


	//all the bricks are gone, game finished->
	gameOver = true;
	for (auto elem : currentBricks) {
		if (elem->isDestroyed == 0) {
			gameOver = false;
			break;
		}
	}
	if (gameOver == true) { //or simply press G whenever you want
		ballSpeed = initialBallSpeed;
		platformCollided = false;
		rightCollided = false;
		leftCollided = false;
		upperCollided = false;
		bottomCollided = false;
		powerup = 0; powerupTimer = 0; powerupExists = 0;
		numberOfLives = 0;
		gameIsRunning = 0;
	}

	if (rocker == true) {

		if (backgroundColor.g == 0) backgroundColor = glm::vec3(1, 1, 1);
		else backgroundColor = glm::vec3(0, 0, 0);

		//E->
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1020, 70);
		RenderMesh2D(surpriseOne, shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1030, 70);
		RenderMesh2D(surpriseTwo, shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1030, 90);
		RenderMesh2D(surpriseTwo, shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1030, 110);
		RenderMesh2D(surpriseTwo, shaders["VertexColor"], modelMatrix);

		//G->
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1080, 70);
		RenderMesh2D(surpriseOne, shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1090, 70);
		RenderMesh2D(surpriseTwo, shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1090, 110);
		RenderMesh2D(surpriseTwo, shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1110, 70);
		RenderMesh2D(surpriseThree, shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1110, 80);
		RenderMesh2D(surpriseThree, shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1100, 90);
		RenderMesh2D(surpriseFour, shaders["VertexColor"], modelMatrix);

		//C->
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1140, 70);
		RenderMesh2D(surpriseOne, shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1150, 70);
		RenderMesh2D(surpriseTwo, shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1150, 110);
		RenderMesh2D(surpriseTwo, shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1170, 70);
		RenderMesh2D(surpriseThree, shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1170, 100);
		RenderMesh2D(surpriseThree, shaders["VertexColor"], modelMatrix);
	}
}

void BrickBreaker::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_U)) { //speed Up
		if (ballSpeed >= 600) ballSpeed = 600;
		else ballSpeed += 15;
	}
	if (window->KeyHold(GLFW_KEY_D)) { //speed Down
		if (ballSpeed <= 0) ballSpeed = 0;
		else ballSpeed -= 15;
	}
}

void BrickBreaker::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_C) { //Change Color
		if (backgroundColor.g == 0) backgroundColor = glm::vec3(1, 1, 1); //there are two
		else backgroundColor = glm::vec3(0, 0, 0); //types of people
	}
	if (key == GLFW_KEY_R) { //refresh
		refresh = true;
	}
	if (key == GLFW_KEY_G) { //Game over
		ballSpeed = initialBallSpeed;
		platformCollided = false;
		rightCollided = false;
		leftCollided = false;
		upperCollided = false;
		bottomCollided = false;
		powerup = 0; powerupTimer = 0; powerupExists = 0;
		numberOfLives = 0;
		gameIsRunning = 0;
	}
	if (key == GLFW_KEY_P) { //powerup chance, because default might be boring
		if (powerupThreshold == 100) powerupThreshold = 15;
		else powerupThreshold = 100;
	}
	if (key == GLFW_KEY_1) {
		if (queenColor == false) {
			mciSendString(TEXT("play queen.mp3 from 0"), NULL, 0, NULL);
			queenColor = true;
		}
		else {
			queenColor = false;
			livesColor = initialLivesColor;
			mciSendString(TEXT("close queen.mp3"), NULL, 0, NULL);
		}
	}
	if (key == GLFW_KEY_2) {
		if (rocker == false) {
			rocker = true;
			mciSendString(TEXT("play rocker.mp3 from 0"), NULL, 0, NULL);
		}
		else {
			rocker = false;
			backgroundColor = glm::vec3(0);
			mciSendString(TEXT("close rocker.mp3"), NULL, 0, NULL);
		}
	}
	if (key == GLFW_KEY_3) {
		mciSendString(TEXT("play berry.mp3 from 0"), NULL, 0, NULL);
	}
	if (key == GLFW_KEY_4) {
		mciSendString(TEXT("play heart.mp3 from 0"), NULL, 0, NULL);
	}
	if (key == GLFW_KEY_5) {
		mciSendString(TEXT("play thriller.mp3 from 0"), NULL, 0, NULL);
	}
	if (key == GLFW_KEY_6) {
		mciSendString(TEXT("play mario.mp3 from 0"), NULL, 0, NULL);
	}
}

void BrickBreaker::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && gameIsRunning == 0) {
		gameIsRunning = 1;
	}
}