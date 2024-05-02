#pragma once
#include "wx/wx.h"
#include "Settings.h"

class DrawingPanel :
    public wxPanel
{
public:
    DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& board, std::vector<std::vector<int>>& neighborCount);
    ~DrawingPanel();
    void SetSize(wxSize& size);
    void SetGridSize(unsigned int size);
    void OnPaint(wxPaintEvent& evt);
    void SetSettings(Settings* settings);

private:
    Settings* mSettings;
    std::vector<std::vector<bool>>& boardRef;
    std::vector<std::vector<int>>& neighborCountRef;
    void OnMouseUp(wxMouseEvent& evt);
    wxDECLARE_EVENT_TABLE(); 
};

