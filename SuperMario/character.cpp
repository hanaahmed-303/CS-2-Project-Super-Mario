double Character::getVelocityX() const
{
    return velocityX;
}

double Character::getVelocityY() const
{
    return velocityY;
}

void Character::setVelocityX(double velocity)
{
    velocityX = velocity;
}

void Character::setVelocityY(double velocity)
{
    velocityY = velocity;
}

bool Character::isOnGround() const
{
    return onGround;
}

void Character::setOnGround(bool grounded)
{
    onGround = grounded;

    if(grounded && velocityY > 0)
        velocityY = 0;
}