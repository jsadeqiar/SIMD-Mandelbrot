#include "Application.h"

void main(int argc, char** argv)
{
    SM::CreateImGuiContext();
    SM::Application((ImGuiIO&)ImGui::GetIO());
    return;
}