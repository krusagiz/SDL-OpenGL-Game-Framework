#include "Projectile.hpp"

#include "../../basicmath.hpp"
#include "../../MainUiManager/MainUiManager.hpp"

ProjectileData::ProjectileData(float radius, float maxLife, float mass, float damage, const ExplosionData* explosionData) {
    this->radius = radius;
    this->maxLife = maxLife;
    this->mass = mass;
    this->damage = damage;
    this->explosionData = explosionData;
}



Projectile::Projectile() : Projectile(genericProjectileData, 0, 0, 0, 0) {}
Projectile::Projectile(const ProjectileData &data, float x, float y, float xvel, float yvel)
        : Entity(x, y, xvel, yvel, data.maxLife) {

    this->data = &data;
}

float Projectile::getRadius() const { return data->radius; }
float Projectile::getMaxLife() const { return data->maxLife; }
float Projectile::getMass() const { return data->mass; }
float Projectile::getDamage() const { return data->damage; }
const ExplosionData* Projectile::getExplosionData() const { return data->explosionData; }

void Projectile::pushEntity(Entity &e) {
    // Add all of this projectile's momentum to e
    e.applyForce(xvel*getMass(), yvel*getMass());
}

void Projectile::damageEntity(ImmovableEntity &e) {
    e.takeDamage(getDamage());
}

void Projectile::doTick() {
    Entity::doTick();
    --life;

    // Drag force is proportional to the square of the velocity
    // (Other factors for the proportion are cross section, shape and fluid density and viscosity)
    // But screw that, too much to balance
    //xvel *= .9f;
    //yvel *= .9f;

    // Add wind
    applyForce(
        (-0.2f - xvel)*.0001f,
        ( 0.0f - yvel)*.0001f);
}

void Projectile::draw(
        std::function<void(int&, int&, float, float)> gameToScreenCoords,
        std::function<float(float)> gameToScreenLength,
        MainUiManager *uiManager) {

    float r = gameToScreenLength(getRadius());
    int sx, sy;
    gameToScreenCoords(sx, sy, x, y);

    float a = getLifeFraction()*0x100;
    if (a < 0) a = 0;
    else if (a > 0xff) a = 0xff;

    uiManager->setDrawColor(0xff, (Uint8)(a), 0x00);
    uiManager->drawLineCircle(sx, sy, r);
}
