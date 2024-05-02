#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include "MainWindow.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()
DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& board, std::vector<std::vector<int>>& neighborCount)
	: wxPanel(parent, wxID_ANY), boardRef(board), neighborCountRef(neighborCount)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}
DrawingPanel::~DrawingPanel()
{

}
void DrawingPanel::OnPaint(wxPaintEvent& evt) {
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* context = wxGraphicsContext::Create(dc);
	if (!context) return;
	if (mSettings->showGrid) {
		context->SetPen(wxColor(0,0,0,64));
	}
	else {
		context->SetPen(wxTransparentColor);
	}
	
	wxSize size = GetSize();
	wxDouble cellHeight = size.GetHeight() / mSettings->gridSize;
	wxDouble cellWidth = size.GetWidth() / mSettings->gridSize;
	context->SetFont(wxFontInfo(16), *wxBLACK);

	for (int i = 0; i < mSettings->gridSize; i++)
	{
		for (int j = 0; j < mSettings->gridSize; j++)
		{
			wxString displayText = std::to_string(neighborCountRef[i][j]);
			if (boardRef[i][j] == true)
			{
				context->SetBrush(mSettings->GetLivingColor());
			}
			else {
				context->SetBrush(mSettings->GetDeadColor());
			}
			wxDouble calculatedX = i * cellWidth;
			wxDouble calculatedY = j * cellHeight;
			context->DrawRectangle(calculatedX, calculatedY, cellWidth, cellHeight);
			double textWidth, textHeight;
			if (mSettings->showNeighborCount == true)
			{
				context->GetTextExtent(displayText, &textWidth, &textHeight);
				context->DrawText(displayText, calculatedX + cellWidth/2-textWidth/2, calculatedY+cellHeight/2-textHeight/2);
			}
		}
	}
	if (mSettings->showThickGrid)
	{
		int lineCount = mSettings->gridSize / 10;
		if (lineCount > 0)
		{
			context->SetPen(*wxBLACK);
			for (int i = 0; i < lineCount; i++) {
				wxPoint vStart(cellWidth * 10 *(i + 1), 0);
				wxPoint vEnd(cellWidth * 10 * (i + 1), this->GetSize().y);
				dc.DrawLine(vStart, vEnd);
			}
			for (int i = 0; i < lineCount; i++) {
				wxPoint hStart(0, cellHeight * 10 * (i + 1));
				wxPoint hEnd(this->GetSize().x, cellHeight * 10 * (i + 1));
				dc.DrawLine(hStart, hEnd);
			}
		}

	}
	if (mSettings->showHUD) {
		context->SetFont(wxFontInfo(16),*wxRED);
		std::string boundary;
		if (mSettings->showFinite)
			boundary = "finite";
		else
			boundary = "torodial";
		wxString displayText = "Generation: " + std::to_string(MainWindow::generation) + "\nLiving cell count: " + std::to_string(MainWindow::livingCells) + "\nBoundary Type: "+boundary+"\nUniverse Size: " + std::to_string(mSettings->gridSize)+"x"+ std::to_string(mSettings->gridSize);
		wxDouble textWidth, textHeight;
		
		context->GetTextExtent(displayText,&textWidth,&textHeight);

		context->DrawText(displayText,10,size.y-textHeight-10);
	}
	delete context;
}
void DrawingPanel::OnMouseUp(wxMouseEvent& evt) {
	int x = evt.GetX();
	int y = evt.GetY();

	wxSize size = GetSize();
	double cellHeight = static_cast<double>(size.GetHeight()) / mSettings->gridSize;
	double cellWidth = static_cast<double>(size.GetWidth()) / mSettings->gridSize;

	int row = static_cast<int>(x / cellWidth);
	int col = static_cast<int>(y / cellHeight);

	if (row >= 0 && row < mSettings->gridSize && col >= 0 && col < mSettings->gridSize)
		boardRef[row][col] = !boardRef[row][col];

	Refresh();
}
void DrawingPanel::SetSize(wxSize& size) {
	wxPanel::SetSize(size);
	wxPanel::Refresh();
}
void DrawingPanel::SetGridSize(unsigned int size) {
	mSettings->gridSize = size;
}
void DrawingPanel::SetSettings(Settings* settings)
{
	mSettings = settings;
}