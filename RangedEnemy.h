#pragma once  
#ifndef __RANGED_ENEMY_H__  
#define __RANGED_ENEMY_H__  

#include "Enemy.h"  
#include "Projectile.h"  

class RangedEnemy : public Enemy  
{  
private:  
   float m_projectileSpeed;  
   float m_projectileRange;  
   std::string m_projectileType;  
   void shootProjectile();  
   std::shared_ptr<Projectile> createProjectile();  
   void fireProjectile(); // Add declaration for fireProjectile  
   void die();

public:  
   virtual bool init() override;  
   virtual void updateAI(float delta) override;  
   virtual void attack() override;  

public:  
   CREATE_FUNC(RangedEnemy);  
};  

 
#endif // __RANGED_ENEMY_H__   