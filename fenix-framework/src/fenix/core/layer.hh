#pragma once

#include <string>

#include "fenix/events/event.hh"

namespace fenix {

    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(TimeStep /* dt */) {}
        virtual void OnRenderUI() {}
        virtual void OnEvent(Event& /* event */) {}

        const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };

} // namespace fenix
