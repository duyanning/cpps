#ifndef PHONYENTITY_H
#define PHONYENTITY_H

#include "Entity.h"

class PhonyEntity : public Entity {
public:
    PhonyEntity(string name);
    bool update() override;
    time_t timestamp() override;
    string name() override;
private:
    string m_name;
};

using PhonyEntityPtr = shared_ptr<PhonyEntity>;

PhonyEntityPtr makePhonyEntity(string name);

#endif // PHONYENTITY_H
