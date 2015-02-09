#ifndef PHONYENTITY_H
#define PHONYENTITY_H

#include "DependencyGraphEntity.h"

class PhonyEntity : public DependencyGraphEntity {
public:
    PhonyEntity(string name);
    void update();
    time_t timestamp();
    string name();
private:
    string m_name;
};

typedef shared_ptr<PhonyEntity> PhonyEntityPtr;

PhonyEntityPtr makePhonyEntity(string name);

#endif // PHONYENTITY_H
