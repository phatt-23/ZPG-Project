#pragma once

// In this project, the observer pattern can 
// be useful for decoupled communication between 
// layers. Take for instance, HUD layer and GameLayer. 
// HUD react on changes in the GameLayer. For example 
// when player takes damage HUD displays the current
// health that it received from the GameLayer.

namespace ZPG {

class Payload;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void OnNotify(Payload& payload) = 0;
};

}
