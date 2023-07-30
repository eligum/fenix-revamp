#pragma once

#include <fenix.hh>

class EditorLayer : public fenix::Layer
{
public:
    EditorLayer() = default;
    ~EditorLayer() = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(fenix::TimeStep ts) override;
    void OnEvent(fenix::Event&) override;
    void OnRenderUI() override;
};
