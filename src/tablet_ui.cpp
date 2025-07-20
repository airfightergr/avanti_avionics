#include "tablet_ui.h"
#include "imgui/imgui.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GL/gl.h>
#include <iostream>

void TabletUIStart(void) {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        // Configure ImGui (e.g., enable docking, keyboard navigation)
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        //Initialize OpenGL3 backend
        ImGui_ImplOpenGL3_Init("#version 130"); // Use GLSL 130 for compatibility with X-Plane
        std::cout << "Tablet UI Init" << std::endl;
}


void TabletUIDraw(int x , int y, int  width, int  height , void* refcon) {
        // std::cout << "Draw is called!" << std::endl;
        // Setup OpenGL state
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, width, height, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Tablet UI");
        ImGui::Text("Options");
        if (ImGui::Button("Click Me!")) {

        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Restore OpenGL state
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glPopAttrib();
}

void TabletUIStop(void){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
        std::cout << "Tablet UI Fini" << std::endl;
}
