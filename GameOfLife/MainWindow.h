#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "Settings.h"
#include "SettingsWindow.h"
class MainWindow :
    public wxFrame
   
{
public:
    MainWindow();
    ~MainWindow();
    void UpdateStatusText();
    static unsigned int generation;
    static unsigned int livingCells;
private:   
    DrawingPanel* drawingPanel;
    wxBoxSizer* sizer;
    wxStatusBar* statusBar;
    wxToolBar* toolBar;
    wxTimer* timer;
    SettingsWindow* settingsWindow;
    wxMenuItem* displayNeighbors;
    wxMenuItem* finite;
    wxMenuItem* torodial;
    wxMenuItem* randomize;
    wxMenuItem* randomizeWithSeed;
    wxMenuItem* resetSettings;
    wxMenuItem* newFile;
    wxMenuItem* openFile;
    wxMenuItem* importFile;
    wxMenuItem* saveFile;
    wxMenuItem* saveAs;
    wxMenuItem* exit;
    wxMenuItem* grid;
    wxMenuItem* thickGrid;
    wxMenuItem* hUD;
    wxString currentFileName;
    Settings settings;
    std::vector<std::vector<bool>> board;
    std::vector<std::vector<bool>> sandBox;
    std::vector<std::vector<int>> neighborCount;
    void OnResize(wxSizeEvent& evt);
    void OnPlay(wxCommandEvent& evt);
    void OnPause(wxCommandEvent& evt);
    void OnNext(wxCommandEvent& evt);
    void OnTrash(wxCommandEvent& evt);
    void OnTimer(wxTimerEvent& evt);
    void OnSettings(wxCommandEvent& evt);
    void OnDisplayNeighbors(wxCommandEvent& evt);
    void OnDisplayFinite(wxCommandEvent& evt);
    void OnDisplayTorodial(wxCommandEvent& evt);
    void OnDisplayGrid(wxCommandEvent& evt);
    void OnDisplayThickGrid(wxCommandEvent& evt);
    void OnDisplayHUD(wxCommandEvent& evt);
    void OnRandomize(wxCommandEvent& evt);
    void OnRandomizeWithSeed(wxCommandEvent& evt);
    void OnResetSettings(wxCommandEvent& evt);
    void OnNew(wxCommandEvent& evt);
    void OnOpen(wxCommandEvent& evt);
    void OnImport(wxCommandEvent& evt);
    void OnSave(wxCommandEvent& evt);
    void OnSaveAs(wxCommandEvent& evt);
    void OnExitApp(wxCommandEvent& evt);
    void RandomizeGrid(unsigned int seed);
    void InitializeGrid();   
    void WriteGridToFile();
    std::vector<std::vector<bool>> ReadGridFromFile();
    int CalculateLivingNeighborCount(const int& row, const int& col);
    void CalculateNextGenerationCells();
    wxDECLARE_EVENT_TABLE();
};

