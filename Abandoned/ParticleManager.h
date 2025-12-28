// ParticleManager.h
#pragma once
#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include "cocos2d.h"
#include "2d/CCParticleSystem.h"
#include <string>
#include <unordered_map>

USING_NS_CC;

class ParticleManager : public cocos2d::Ref {
public:
    static ParticleManager* getInstance();
    
    // 锟斤拷锟斤拷锟斤拷锟斤拷效锟斤拷
    cocos2d::ParticleSystem* createParticleEffect(const std::string& particleName, const cocos2d::Vec2& position);
    
    // 为锟斤拷页锟教达拷锟斤拷锟斤拷锟斤拷效锟斤拷
    cocos2d::ParticleSystem* createDashParticle(const cocos2d::Vec2& position, bool isLeft);
    
    // 为锟斤拷夜锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟叫э拷锟?
    cocos2d::ParticleSystem* createAttackParticle(const cocos2d::Vec2& position, bool isRight);
    
    // 锟狡筹拷锟斤拷锟斤拷效锟斤拷
    void removeParticleEffect(cocos2d::ParticleSystem* particle);
    
    // 锟狡筹拷锟斤拷锟斤拷锟斤拷锟斤拷效锟斤拷
    void removeAllParticleEffects();
    
    // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷效锟斤拷
    void update(float delta);
    
private:
    ParticleManager();
    ~ParticleManager();
    
    bool init();
    
    static ParticleManager* _instance;
    
    // 锟芥储锟斤拷锟叫伙拷跃锟斤拷锟斤拷锟斤拷效锟斤拷
    std::vector<cocos2d::ParticleSystem*> _activeParticles;
    
    // 预锟斤拷锟截碉拷锟斤拷锟斤拷效锟斤拷模锟斤拷
    std::unordered_map<std::string, cocos2d::ParticleSystem*> _particleTemplates;
    
    // 预锟斤拷锟斤拷锟斤拷锟斤拷效锟斤拷
    void preloadParticleEffects();
    
    // 锟斤拷锟斤拷锟斤拷锟斤拷模锟斤拷
    cocos2d::ParticleSystem* createParticleTemplate(const std::string& particleName);
};

#endif // PARTICLEMANAGER_H