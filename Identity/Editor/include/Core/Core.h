#pragma once
#include <Core/App.h>

namespace Editor::Core
{
    class Core
    {
    public:
        /**
         * @brief Here we will Allocate all pre-required stuff for the editor
         */
        Core();
        /**
         * @brief here we will De-allocate all pre-required stuff for the editor
         */
        ~Core();

        /**
         * @brief Here we initialize all that needs to be initialized (resources, .ini and all)
         */
        void Init();
        /**
         * @brief Here we saved and closes all that needs to be saved and closed and prepare to close the editor
         */
        void Shutdown();
        /**
         * Here we have the editor loop, mainly for what needs to be updated every frame.
         */
        int Update();

        void EditorBase();

        Engine::Core::App m_engine{1920, 1080, "Identity Engine", true};
    };
}
