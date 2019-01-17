#include <armadillo>

class Player
{
    private:
        arma::vec XYPosition;   //2D position vector
        double heading;         //direction in radians
        double moveSpeed;         
        double turnSpeed;         


    public:
        Player(double startX, double startY);

        arma::vec GetXYPosition();

        double GetHeading();
        double GetMoveSpeed();
        double GetTurnSpeed();

        void SetMoveSpeed(double moveSpeed);
        void SetTurnSpeed(double turnSpeed);

        void TurnRight();
        void TurnLeft();

        void MoveForward();
        void MoveBackward();

        arma::vec GetFrontXYPos();
};
