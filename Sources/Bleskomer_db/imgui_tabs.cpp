
#include "stdafx.h"


void		IMGUITAB::Init(){
	selectedTab = 0;
	TabCounts	= 0;
}

void		IMGUITAB::AddTab(PConstStr Name, TabFunctCall Funct){
	TabLists[TabCounts].guiHandler = Funct;
	TabLists[TabCounts].tabName = Name;
	TabCounts++;
}


void		IMGUITAB::DrawAuto(f32 Weig){
	
    ImVec2 tabBarMin;

	f32 posW = 0;

    for (int i = 0; i < TabCounts; ++i) {
        TabList *tab = &TabLists[i];
        const char *tabName = tab->tabName;

		ImVec2 sizd = ImGui::CalcTextSize(tabName);

		posW += sizd.x + 20;

		if (posW > Weig)  { 
			ImGui::NewLine();
			ImGui::Separator();
			posW = sizd.x + 20;	
		}

		//sizd.y += 5;

        if (ImGui::Selectable(tabName, selectedTab == i, 0, sizd)) {  selectedTab = i;  }
        if (!i) { tabBarMin = ImGui::GetItemRectMin(); }
        if ((i + 1) < TabCounts)  {
            ImGui::SameLine(); 
			ImGui::Text("|"); 
			ImGui::SameLine();
        }

    }
    ImVec2 tabBarLastTabTopRight = ImGui::GetItemRectMax();
    ImGui::Separator();
    ImVec2 tabBarLowerRight = ImGui::GetItemRectMax();
    ImVec2 tabBarMax(tabBarLowerRight.x, tabBarLastTabTopRight.y);
	/*
    if (ImGui::IsMouseHoveringRect(tabBarMin, tabBarMax) && ImGui::IsMouseClicked(1))
    {
        ImGui::OpenPopup("tab menu");
    }

    if (ImGui::BeginPopup("tab menu"))
    {
        for (int i = 0; i < TabCounts; ++i)
        {
            TabList *tab =  &TabLists[i];
            const char *tabName = tab->tabName;

            if (ImGui::Selectable(tabName, selectedTab == i))
            {
                selectedTab = i;
            }
        }

        ImGui::EndPopup();
    }
	*/
    selectedTab = max( 0, min (selectedTab, (int)TabCounts - 1));
    
    TabList *selected =  &TabLists[selectedTab];
    ImGui::BeginChild(selected->tabName);

    if (selected->guiHandler) selected->guiHandler();

    ImGui::EndChild();


	
}