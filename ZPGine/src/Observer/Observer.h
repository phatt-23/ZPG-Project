#pragma once

namespace ZPG {

class Payload;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void OnNotify(Payload& payload) = 0;
};

}
