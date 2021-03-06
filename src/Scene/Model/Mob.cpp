#include "Mob.hpp"

#include "../../util/math.hpp"
#include "../../ui/UiManager.hpp"

MobData::MobData(float radius, float maxLife, float mass, SpriteId spriteId) {
    this->radius = radius;
    this->maxLife = maxLife;
    this->mass = mass;

    this->spriteId = spriteId;
}



Mob::Mob() : Mob(genericMobData, 0, 0) {}
Mob::Mob(const MobData &data, float x, float y)
        : Entity(x, y, 0, 0, data.maxLife)
        , data(&data) {}
Mob::~Mob() {}

float Mob::getRadius() const { return data->radius; }
float Mob::getMaxLife() const { return data->maxLife; }
float Mob::getMass() const { return data->mass; }
Weapon* Mob::getWeapon(WeaponManagerWeaponId id) { return weapons.getWeapon(id); }
const Weapon* Mob::getWeapon(WeaponManagerWeaponId id) const { return weapons.getWeapon(id); }

WeaponManagerWeaponId Mob::addWeapon(const WeaponData &weaponData) {
    return weapons.addWeapon(weaponData);
}
bool Mob::removeWeapon(WeaponManagerWeaponId id) {
    return weapons.removeWeapon(id);
}

void Mob::fireAtPosition(WeaponManagerWeaponId weaponId, GameModelManager &model, float targetX, float targetY) {
    Weapon *weapon = weapons.getWeapon(weaponId);
    if (!weapon) return; // No weapon, abort

    // Get direction vector of target from this Entity
    float dirX = targetX - x,
          dirY = targetY - y;

    // Normalize direction vector
    float dist = getdist(dirX, dirY);
    dirX /= dist;
    dirY /= dist;

    float projectileOffset = getRadius() + weapon->data->projectileData->radius;
    float vel = weapon->data->projectileSpeed;

    weapon->fireUntilNotReady(model,
        x + dirX * projectileOffset,
        y + dirY * projectileOffset,
        dirX * vel,
        dirY * vel);
}
void Mob::fireAtEntity(WeaponManagerWeaponId weaponId, GameModelManager &model, const ImmovableEntity& target) {
    fireAtPosition(weaponId, model, target.x, target.y);
}
void Mob::leadAndFireAtEntity(WeaponManagerWeaponId weaponId, GameModelManager &model, const Entity& target) {
    Weapon *weapon = weapons.getWeapon(weaponId);
    if (!weapon) return; // No weapon, abort
    float projectileSpeed = weapon->data->projectileSpeed;
    float projectileRadius = weapon->data->projectileData->radius;

    // p = projectile
    // e = target's current position
    // t = time till collision
    // .r = radius
    // Have to solve for the new target position e' when t = (dist(this, e') - this->r, p.r, e.r) / p.speed, and e' = e + t*e.vel

    // Approximation only:
    // Find the time taken by the projectile to reach the target's current position (i.e. reach its general area)
    float t = (getdist(target.x - x, target.y - y) - this->getRadius() - target.getRadius() - 2 * projectileRadius) / projectileSpeed;
    // Find where the target will be after that time has elapsed
    float targetX = target.x + target.xvel * t,
          targetY = target.y + target.yvel * t;

    // Fire at that new position
    fireAtPosition(weaponId, model, targetX, targetY);
}

void Mob::doTick() {
    Entity::doTick();

    weapons.doTick();

    // Add drag
    float maxVelChange = .1f;
    float vel = getdist(xvel, yvel);
    if (vel <= maxVelChange) xvel = yvel = 0;
    else {
        float mod = (vel - maxVelChange) / vel;
        xvel *= mod;
        yvel *= mod;
    }
}

void Mob::draw(UiManager &uiManager) {
    float a = getLifeFraction();
    uiManager.setColorMask({1, a, a});
    uiManager.setObjectScale(getRadius());
    uiManager.setAtlasPhase(true, uiManager.getUiTick() * .02f, 0);
    uiManager.drawSprite(x, y, data->spriteId);
}
