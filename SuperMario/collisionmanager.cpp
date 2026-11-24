bool CollisionManager::checkEnemyCollision(
    Player *player,
    Goomba *enemy,
    const QRectF &previousBounds)
{
    if(player->isDead() || enemy->isDead())
        return false;

    QRectF playerBounds = player->getBounds();
    QRectF enemyBounds = enemy->getBounds();

    if(!playerBounds.intersects(enemyBounds))
        return false;

    if(player->getVelocityY() > 0 &&
        previousBounds.bottom() <= enemyBounds.top())
    {
        enemy->stomp();

        player->setY(player->y() +
                     enemyBounds.top() - playerBounds.bottom());

        player->setOnGround(false);
        player->setVelocityY(-8);

        return true;
    }

    player->takeDamage(enemy->getContactDamage());

    return false;
}