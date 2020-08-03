#include "layers/LayerBase.hpp"
#include "systems/Projection.hpp"
#include "systems/Logging.hpp"

void L_Base::init()
{
        systems.add<S_Projection>();
        systems.add<S_Logging>();
}

void L_Base::handleEvents(float deltaTime)
{

}
void L_Base::update(float deltaTime)
{
        systems.update<S_Projection>(deltaTime, m_Registry);
        systems.update<S_Logging>(deltaTime, m_Registry);
}

void L_Base::render(float deltaTime)
{

}
void L_Base::clean()
{

}

L_Base::L_Base()
{

}
