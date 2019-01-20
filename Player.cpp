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

void Player::StrafeLeft()
{
    double pi = 3.141592653589793238463;
    double newHeading = this->heading - pi / 2;
    double dx = this->moveSpeed * cos(newHeading);
    double dy = this->moveSpeed * sin(newHeading);

    this->XYPosition(0) += dx;
    this->XYPosition(1) += dy;
}

void Player::StrafeRight()
{
    double pi = 3.141592653589793238463;
    double newHeading = this->heading + pi / 2;
    double dx = this->moveSpeed * cos(newHeading);
    double dy = this->moveSpeed * sin(newHeading);

    this->XYPosition(0) += dx;
    this->XYPosition(1) += dy;
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
    double pi = 3.141592653589793238463;
    double newHeading = this->heading + pi;
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

arma::vec Player::GetMovePos(MoveType direction)
{
    arma::vec movePos;
    double dx, dy, x, y;
    double newHeading;
    double pi = 3.141592653589793238463;

    switch (direction)
    {
        case MoveType::FORWARD:
            dx = this->moveSpeed * cos(this->heading);
            dy = this->moveSpeed * sin(this->heading);

            x = this->XYPosition(0) + dx;
            y = this->XYPosition(1) + dy;
            movePos = {x, y};
            break;

        case MoveType::BACKWARD:
            newHeading = this->heading + pi;
            dx = this->moveSpeed * cos(newHeading);
            dy = this->moveSpeed * sin(newHeading);

            x = this->XYPosition(0) + dx;
            y = this->XYPosition(1) + dy;
            movePos = {x, y};
            break;

        case MoveType::STRAFE_LEFT:
            newHeading = this->heading - pi / 2;
            dx = this->moveSpeed * cos(newHeading);
            dy = this->moveSpeed * sin(newHeading);

            x = this->XYPosition(0) + dx;
            y = this->XYPosition(1) + dy;
            movePos = {x, y};
            break;

        case MoveType::STRAFE_RIGHT:
            newHeading = this->heading + pi / 2;
            dx = this->moveSpeed * cos(newHeading);
            dy = this->moveSpeed * sin(newHeading);

            x = this->XYPosition(0) + dx;
            y = this->XYPosition(1) + dy;
            movePos = {x, y};
            break;
    }

    return movePos;
}
