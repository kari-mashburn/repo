#pragma once
#include <wx/colour.h>
#include <fstream>
struct Settings {
	unsigned int liveRed = 128;
	unsigned int liveGreen = 128;
	unsigned int liveBlue = 128;
	unsigned int liveAlpha = 255;

	unsigned int deadRed = 255;
	unsigned int deadGreen = 255;
	unsigned int deadBlue = 255;
	unsigned int deadAlpha = 255;

	unsigned int gridSize = 15;
	unsigned int timeInterval = 50;
	
	bool showNeighborCount = false;
	bool showTorodial = false;
	bool showFinite = false;
	bool showGrid = true;
	bool showThickGrid = false;
	bool showHUD = false;

	wxColor GetLivingColor() const {
		return wxColor(liveRed, liveGreen,liveBlue,liveAlpha);
	}
	wxColor GetDeadColor() const {
		return wxColor(deadRed, deadGreen, deadBlue, deadAlpha);
	}

	void SetLivingColor(wxColor color){
		liveRed = color.GetRed();
		liveGreen = color.GetGreen();
		liveBlue = color.GetBlue();
		liveAlpha = color.GetAlpha();
	}
	void SetDeadColor(wxColor color){
		deadRed = color.GetRed();
		deadGreen = color.GetGreen();
		deadBlue = color.GetBlue();
		deadAlpha = color.GetAlpha();
	}

	void LoadData() {
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		file.read((char*)this, sizeof(Settings));
		file.close();
	}
	void SaveData() {
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		file.write((char*)this, sizeof(Settings));
		file.close();
	}
};