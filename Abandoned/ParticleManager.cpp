// ParticleManager.cpp
#include "ParticleManager.h"
#include "cocos2d.h"
#include <vector>
#include <algorithm>
#include <unordered_map>

USING_NS_CC;

ParticleManager* ParticleManager::_instance = nullptr;

ParticleManager* ParticleManager::getInstance() {
    if (!_instance) {
        _instance = new (std::nothrow) ParticleManager();
        if (_instance && _instance->init()) {
            _instance->autorelease();
            _instance->retain();
        } else {
            CC_SAFE_DELETE(_instance);
            _instance = nullptr;
        }
    }
    return _instance;
}

ParticleManager::ParticleManager() {
}

ParticleManager::~ParticleManager() {
    removeAllParticleEffects();
    
    // 锟斤拷锟斤拷锟斤拷锟斤拷模锟斤拷
    for (auto& pair : _particleTemplates) {
        CC_SAFE_RELEASE(pair.second);
    }
    _particleTemplates.clear();
}

bool ParticleManager::init() {
    preloadParticleEffects();
    return true;
}

void ParticleManager::preloadParticleEffects() {
    // 预锟斤拷锟截革拷锟斤拷锟斤拷锟斤拷效锟斤拷模锟斤拷
    _particleTemplates["dash_left"] = createParticleTemplate("dash_left");
    _particleTemplates["dash_right"] = createParticleTemplate("dash_right");
    _particleTemplates["attack"] = createParticleTemplate("attack");
}

ParticleSystem* ParticleManager::createParticleTemplate(const std::string& particleName) {
    ParticleSystem* particle = nullptr;
    
    if (particleName.empty()) {
        CCLOG("Error: Particle template name is empty");
        return nullptr;
    }
    
    if (particleName == "dash_left" || particleName == "dash_right") {
        // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷效锟斤拷 - 使锟斤拷cocos2d-x锟斤拷锟节伙拷跃效锟斤拷
        particle = ParticleMeteor::create();
        if (particle) {
            particle->retain();
            
            // 锟斤拷锟铰的匡拷头位锟斤拷
            particle->setPosition(Vec2::ZERO);
            
            // 锟斤拷锟铰碉拷时锟斤拷 - 璁剧疆涓轰竴娆℃€ф晥鏋?
            particle->setDuration(0.5f); // 璁剧疆绮掑瓙绯荤粺鎸佺画鏃堕棿
            particle->setAutoRemoveOnFinish(true); // 鎾斁瀹屾瘯鍚庤嚜鍔ㄧЩ闄?
            
            // 锟斤拷锟铰的革拷锟斤拷
            particle->setLife(0.5f);
            particle->setLifeVar(0.1f);
            
            // 锟斤拷锟铰碉拷小
            particle->setStartSize(10.0f);
            particle->setEndSize(20.0f);
            
            // 锟斤拷锟铰的憋拷锟斤拷
            if (particleName == "dash_left") {
                particle->setAngle(180.0f); // 锟斤拷锟节伙拷
                particle->setSpeed(300.0f);
            } else {
                particle->setAngle(0.0f); // 锟斤拷锟揭筹拷锟?
                particle->setSpeed(300.0f);
            }
            
            // 锟斤拷锟铰的憋拷锟斤拷围
            particle->setAngleVar(20.0f);
            particle->setSpeedVar(50.0f);
            
            // 锟斤拷锟铰的伙拷锟斤拷
            particle->setGravity(Vec2(0, 0));
            
            // 锟斤拷锟铰的憋拷锟斤拷
            particle->setStartColor(Color4F(0.3f, 0.6f, 1.0f, 1.0f));
            particle->setEndColor(Color4F(0.1f, 0.3f, 0.6f, 0.0f));
            
            // 锟斤拷锟铰碉拷锟斤拷转
            particle->setStartSpin(0.0f);
            particle->setStartSpinVar(360.0f);
            
            // 锟斤拷锟铰碉拷锟斤拷
            particle->setEmissionRate(150.0f);
            
            // 锟斤拷锟矫达拷锟斤拷模式
            particle->setBlendAdditive(true);
        } else {
            CCLOG("Error: Failed to create ParticleMeteor for %s", particleName.c_str());
        }
    } else if (particleName == "attack") {
        // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷效锟斤拷 - 使锟斤拷cocos2d-x锟斤拷锟节伙拷跃效锟斤拷
        particle = ParticleFire::create();
        if (particle) {
            particle->retain();
            
            // 锟斤拷锟铰的匡拷头位锟斤拷
            particle->setPosition(Vec2::ZERO);
            
            // 锟斤拷锟铰的革拷锟斤拷
            particle->setLife(0.3f);
            particle->setLifeVar(0.1f);
            
            // 锟斤拷锟铰碉拷小
            particle->setStartSize(5.0f);
            particle->setEndSize(15.0f);
            
            // 锟斤拷锟铰的憋拷锟斤拷
            particle->setAngle(0.0f);
            particle->setAngleVar(45.0f);
            
            // 锟斤拷锟铰的憋拷锟斤拷
            particle->setSpeed(200.0f);
            particle->setSpeedVar(50.0f);
            
            // 锟斤拷锟铰的伙拷锟斤拷
            particle->setGravity(Vec2(0, 0));
            
            // 锟斤拷锟铰的憋拷锟斤拷
            particle->setStartColor(Color4F(1.0f, 0.5f, 0.0f, 1.0f));
            particle->setEndColor(Color4F(1.0f, 1.0f, 0.0f, 0.0f));
            
            // 锟斤拷锟铰碉拷锟斤拷
            particle->setEmissionRate(100.0f);
            
            // 锟斤拷锟矫达拷锟斤拷模式
            particle->setBlendAdditive(true);
        } else {
            CCLOG("Error: Failed to create ParticleFire for attack particle");
        }
    } else {
        CCLOG("Error: Unknown particle template name: %s", particleName.c_str());
    }
    
    return particle;
}

ParticleSystem* ParticleManager::createParticleEffect(const std::string& particleName, const Vec2& position) {
    // 妫€鏌ョ矑瀛愬悕绉版槸鍚︿负绌?
    if (particleName.empty()) {
        CCLOG("Error: Particle name is empty");
        return nullptr;
    }
    
    // 浠庢ā鏉垮垱寤虹矑瀛?
    auto templateIt = _particleTemplates.find(particleName);
    if (templateIt != _particleTemplates.end() && templateIt->second) {
        // 鐩存帴鍩轰簬妯℃澘鍒涘缓鏂扮殑绮掑瓙绯荤粺锛堜笉浣跨敤clone()锛?
        ParticleSystem* particle = nullptr;
        
        // 鏍规嵁妯℃澘绫诲瀷鍒涘缓鏂扮殑绮掑瓙绯荤粺
        if (particleName == "dash_left" || particleName == "dash_right") {
            particle = ParticleMeteor::create();
        } else if (particleName == "attack") {
            particle = ParticleFire::create();
        } else {
            // 榛樿浣跨敤ParticleMeteor浣滀负鍚庡
            particle = ParticleMeteor::create();
        }
        
        if (particle) {
            // 澶嶅埗妯℃澘鐨勬墍鏈夊睘鎬?
            particle->setLife(templateIt->second->getLife());
            particle->setLifeVar(templateIt->second->getLifeVar());
            particle->setStartSize(templateIt->second->getStartSize());
            particle->setEndSize(templateIt->second->getEndSize());
            particle->setAngle(templateIt->second->getAngle());
            particle->setAngleVar(templateIt->second->getAngleVar());
            particle->setSpeed(templateIt->second->getSpeed());
            particle->setSpeedVar(templateIt->second->getSpeedVar());
            particle->setGravity(templateIt->second->getGravity());
            particle->setStartColor(templateIt->second->getStartColor());
            particle->setEndColor(templateIt->second->getEndColor());
            particle->setStartSpin(templateIt->second->getStartSpin());
            particle->setStartSpinVar(templateIt->second->getStartSpinVar());
            particle->setEmissionRate(templateIt->second->getEmissionRate());
            particle->setBlendAdditive(templateIt->second->isBlendAdditive());
            particle->setDuration(templateIt->second->getDuration());
            particle->setAutoRemoveOnFinish(templateIt->second->isAutoRemoveOnFinish());
            
            // 璁剧疆浣嶇疆
            particle->setPosition(position);
            
            // 璁剧疆绮掑瓙绯荤粺鐨勪綅缃被鍨嬩负鐩稿浜庣埗鑺傜偣
            particle->setPositionType(cocos2d::ParticleSystem::PositionType::GROUPED);
            
            // 閲嶇疆绮掑瓙绯荤粺锛岀‘淇濈矑瀛愭晥鏋滄纭樉绀?
            particle->resetSystem(); // 閲嶇疆绮掑瓙绯荤粺
            
            // 澧炲姞寮曠敤璁℃暟锛屽洜涓烘垜浠皢鍏舵坊鍔犲埌浜哶activeParticles鍒楄〃涓?
            particle->retain();
            _activeParticles.push_back(particle);
            return particle;
        } else {
            CCLOG("Error: Failed to create particle system for name: %s", particleName.c_str());
        }
    } else {
        CCLOG("Error: Particle template not found or is null for name: %s", particleName.c_str());
        // 濡傛灉妯℃澘涓嶅瓨鍦紝灏濊瘯鍒涘缓鏂扮殑绮掑瓙
        auto particle = createParticleTemplate(particleName);
        if (particle) {
            particle->setPosition(position);
            // 娉ㄦ剰锛歝reateParticleTemplate宸茬粡璋冪敤浜唕etain()锛屾墍浠ヨ繖閲屼笉闇€瑕佸啀娆¤皟鐢?
            _activeParticles.push_back(particle);
            return particle;
        }
    }
    
    return nullptr;
}

ParticleSystem* ParticleManager::createDashParticle(const Vec2& position, bool isLeft) {
    std::string particleName = isLeft ? "dash_left" : "dash_right";
    ParticleSystem* particle = createParticleEffect(particleName, position);
    if (!particle) {
        CCLOG("Error: Failed to create dash particle, direction: %s", isLeft ? "left" : "right");
    }
    return particle;
}

ParticleSystem* ParticleManager::createAttackParticle(const Vec2& position, bool isRight) {
    ParticleSystem* particle = createParticleEffect("attack", position);
    if (particle) {
        // 鏍规嵁鏀诲嚮鏂瑰悜璋冩暣瑙掑害
        if (!isRight) {
            particle->setAngle(180.0f);
            particle->setAngleVar(45.0f);
        }
    } else {
        CCLOG("Error: Failed to create attack particle");
    }
    return particle;
}

void ParticleManager::removeParticleEffect(ParticleSystem* particle) {
    if (!particle) return;
    
    // 锟接伙拷跃锟斤拷锟斤拷锟叫憋拷锟斤拷锟狡筹拷
    auto it = std::find(_activeParticles.begin(), _activeParticles.end(), particle);
    if (it != _activeParticles.end()) {
        particle->stopSystem();
        _activeParticles.erase(it);
        particle->removeFromParent();
        CC_SAFE_RELEASE(particle);
    }
}

void ParticleManager::removeAllParticleEffects() {
    for (auto particle : _activeParticles) {
        if (particle) {
            particle->stopSystem();
            particle->removeFromParent();
            CC_SAFE_RELEASE(particle);
        }
    }
    _activeParticles.clear();
}

void ParticleManager::update(float delta) {
    // 娓呯悊宸插畬鎴愮殑绮掑瓙鏁堟灉
    for (auto it = _activeParticles.begin(); it != _activeParticles.end();) {
        auto particle = *it;
        if (particle) {
            if (particle->getParticleCount() == 0) {
                particle->removeFromParent();
                CC_SAFE_RELEASE(particle);
                it = _activeParticles.erase(it);
            } else {
                ++it;
            }
        } else {
            // 绉婚櫎绌烘寚閽?
            CCLOG("Warning: Found nullptr in active particles list");
            it = _activeParticles.erase(it);
        }
    }
}