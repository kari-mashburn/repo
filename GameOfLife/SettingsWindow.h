#pragma once
#include "wx/wx.h"
#include "Settings.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
class SettingsWindow :
    public wxDialog
{
public:
    SettingsWindow(wxWindow* parent, Settings* settings);
    void SetSettings(Settings* settings);
private:
    Settings* mSettings;
    wxBoxSizer* mainBox;
    wxSpinCtrl* gridSizeSpin;
    wxSpinCtrl* timeIntervalSpin;
    wxColourPickerCtrl* deadColorPicker;
    wxColourPickerCtrl* aliveColorPicker;
 
    void OnOk(wxCommandEvent& evt);
    void OnCancel(wxCommandEvent& evt);
    wxDECLARE_EVENT_TABLE();
};

