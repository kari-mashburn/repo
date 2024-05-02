#include "SettingsWindow.h"

wxBEGIN_EVENT_TABLE(SettingsWindow, wxDialog)
EVT_BUTTON(wxID_OK,SettingsWindow::OnOk)
EVT_BUTTON(wxID_CANCEL,SettingsWindow::OnCancel)
wxEND_EVENT_TABLE()
SettingsWindow::SettingsWindow(wxWindow* parent, Settings* settings)
	: wxDialog(parent,wxID_ANY,"Settings",wxPoint(100,100))
{
	mSettings = settings;
	mainBox = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainBox);
	
	wxBoxSizer* childBoxOne = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* titleOne = new wxStaticText(this,wxID_ANY,"Dead Cell Color");
	deadColorPicker = new wxColourPickerCtrl(this,10005);	
	childBoxOne->Add(titleOne);
	childBoxOne->Add(deadColorPicker);

	wxBoxSizer* childBoxTwo = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* titleTwo = new wxStaticText(this, wxID_ANY, "Alive Cell Color");
	aliveColorPicker = new wxColourPickerCtrl(this, 10006);
	childBoxTwo->Add(titleTwo);
	childBoxTwo->Add(aliveColorPicker);

	wxBoxSizer* childBoxThree = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* titleThree = new wxStaticText(this, wxID_ANY, "Grid Size");
	gridSizeSpin = new wxSpinCtrl(this,10007);
	childBoxThree->Add(titleThree);
	childBoxThree->Add(gridSizeSpin);

	wxBoxSizer* childBoxFour = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* titleFour = new wxStaticText(this, wxID_ANY, "Timer Interval");
	timeIntervalSpin = new wxSpinCtrl(this, 10008);
	childBoxFour->Add(titleFour);
	childBoxFour->Add(timeIntervalSpin);

	wxSizer* specialControls = CreateButtonSizer(wxOK|wxCANCEL);
	mainBox->Add(childBoxOne);
	mainBox->Add(childBoxTwo);
	mainBox->Add(childBoxThree);
	mainBox->Add(childBoxFour);
	mainBox->Add(specialControls);

	deadColorPicker->SetColour(mSettings->GetDeadColor());
	aliveColorPicker->SetColour(mSettings->GetLivingColor());
	gridSizeSpin->SetValue(mSettings->gridSize);
	timeIntervalSpin->SetValue(mSettings->timeInterval);
}

void  SettingsWindow::SetSettings(Settings* settings)
{
	mSettings = settings;
}
void SettingsWindow::OnOk(wxCommandEvent& evt) {
	mSettings->SetDeadColor(deadColorPicker->GetColour());
    mSettings->SetLivingColor(aliveColorPicker->GetColour());
	mSettings->gridSize = gridSizeSpin->GetValue();
	mSettings->timeInterval = timeIntervalSpin->GetValue();
	mSettings->SaveData();

	EndModal(wxID_OK);
}
void SettingsWindow::OnCancel(wxCommandEvent& evt) {
	EndModal(wxID_CANCEL);
}