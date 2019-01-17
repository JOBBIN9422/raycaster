#include "Player.h"

using namespace std;

Player::Player(double startX, double startY) : heading(0.0)
{
    this->XYPosition  = {startX, startY};
}

arma::vec Player::GetXYPosition()
{
    return this->XYPosition;
}

double Player::GetHeading()
{
    return this->heading;
}

double Player::GetMoveSpeed()
{
    return this->moveSpeed;
}

double Player::GetTurnSpeed()
{
    return this->turnSpeed;
}

void Player::SetMoveSpeed(double moveSpeed)
{
    this->moveSpeed = moveSpeed;
}

void Player::SetTurnSpeed(double turnSpeed)
{
    this->turnSpeed = turnSpeed;
}

void Player::TurnRight()
{
    this->heading += this->turnSpeed;
}

void Player::TurnLeft()
{
    this->heading -= this->turnSpeed;
}

void Player::MoveForward()
{
    double dx = this->moveSpeed * cos(this->heading);
    double dy = this->moveSpeed * sin(this->heading);

    this->XYPosition(0) += dx;
    this->XYPosition(1) += dy;
}

void Player::MoveBackward()
{
    double dx = this->moveSpeed * cos(this->heading);
    double dy = this->moveSpeed * sin(this->heading);

    this->XYPosition(0) -= dx;
    this->XYPosition(1) -= dy;
}


arma::vec Player::GetFrontXYPos()
{
    double dx = this->moveSpeed * cos(this->heading);
    double dy = this->moveSpeed * sin(this->heading);

    double x = this->XYPosition(0) + dx;
    double y = this->XYPosition(1) + dy;
    arma::vec frontXYPos = {x, y};
    return frontXYPos;
}
