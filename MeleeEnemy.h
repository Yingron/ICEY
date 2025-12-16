#pragma once  
#ifndef __MELEE_ENEMY_H__  
#define __MELEE_ENEMY_H__  

#include "Enemy.h"  
#include "Hitbox.h" // 确保包含 Hitbox 的声明  
class MeleeEnemy : public Enemy  
{  
private:  
   float m_attackWidth;  
   float m_attackHeight;  

public:  
   virtual bool init() override;  
   virtual void updateAI(float delta) override;  
   virtual void attack() override;

   void die();
   std::shared_ptr<Hitbox> createMeleeHitbox();    
private:  
   std::shared_ptr<Hitbox> createMeleeHitbox();  

public: // Ensure CREATE_FUNC is accessible  
   CREATE_FUNC(MeleeEnemy);  
};  

#endif // __MELEE_ENEMY_H__