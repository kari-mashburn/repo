#include "MainWindow.h"
#include "wx/numdlg.h"
#include "wx/filedlg.h"
#include "wx/textfile.h"
wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::OnResize)
EVT_MENU(10000, MainWindow::OnPlay)
EVT_MENU(10001, MainWindow::OnPause)
EVT_MENU(10002, MainWindow::OnNext)
EVT_MENU(10003, MainWindow::OnTrash)
EVT_TIMER(10004,MainWindow::OnTimer)
EVT_MENU(10009,MainWindow::OnSettings)
EVT_MENU(10010, MainWindow::OnDisplayNeighbors)
EVT_MENU(10018, MainWindow::OnDisplayFinite)
EVT_MENU(10019, MainWindow::OnDisplayTorodial)
EVT_MENU(10021, MainWindow::OnDisplayGrid)
EVT_MENU(10022, MainWindow::OnDisplayThickGrid)
EVT_MENU(10023, MainWindow::OnDisplayHUD)
EVT_MENU(10011, MainWindow::OnRandomize)
EVT_MENU(10012, MainWindow::OnRandomizeWithSeed)
EVT_MENU(10020, MainWindow::OnResetSettings)
EVT_MENU(10013, MainWindow::OnNew)
EVT_MENU(10014, MainWindow::OnOpen)
EVT_MENU(10021, MainWindow::OnImport)
EVT_MENU(10015, MainWindow::OnSave)
EVT_MENU(10016, MainWindow::OnSaveAs)
EVT_MENU(10017, MainWindow::OnExitApp)
wxEND_EVENT_TABLE()
MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(600, 600)) {
	settings.LoadData();
	drawingPanel = new DrawingPanel(this, board, neighborCount);
	drawingPanel->SetSettings(&settings);
	wxMenuBar* menuBar = new wxMenuBar();
	SetMenuBar(menuBar);
	wxMenu* menu = new wxMenu();
	wxMenu* viewMenu = new wxMenu();
	wxMenu* fileMenu = new wxMenu();
	displayNeighbors = new wxMenuItem(viewMenu,10010,"Display Neighbors","",wxITEM_CHECK);
	displayNeighbors->SetCheckable(true);
	finite = new wxMenuItem(viewMenu, 10018, "Finite", "", wxITEM_CHECK);
	finite->SetCheckable(true);
	torodial = new wxMenuItem(viewMenu, 10019, "Torodial", "", wxITEM_CHECK);
	torodial->SetCheckable(true);
	grid = new wxMenuItem(viewMenu, 10021, "Display Grid", "", wxITEM_CHECK);
	grid->SetCheckable(true);
	thickGrid = new wxMenuItem(viewMenu, 10022, "Display Thick Grid", "", wxITEM_CHECK);
	thickGrid->SetCheckable(true);
	hUD = new wxMenuItem(viewMenu, 10023, "Display HUD", "", wxITEM_CHECK);
	hUD->SetCheckable(true);

	randomize = new wxMenuItem(menu,10011,"Randomize","");
	randomizeWithSeed = new wxMenuItem(menu,10012,"Randomize With Seed", "");
	resetSettings = new wxMenuItem(menu, 10020, "Reset Settings", "");

	newFile = new wxMenuItem(fileMenu,10013,"New","");
	openFile = new wxMenuItem(fileMenu, 10014, "Open", "");
	saveFile = new wxMenuItem(fileMenu, 10015, "Save", "");
	saveAs = new wxMenuItem(fileMenu, 10016, "Save As", "");
	exit = new wxMenuItem(fileMenu, 10017, "Exit", "");
	importFile = new wxMenuItem(fileMenu, 10021, "Import", "");
	viewMenu->Append(finite);
	viewMenu->Append(torodial);
	viewMenu->Append(displayNeighbors);
	viewMenu->Append(grid);
	viewMenu->Append(thickGrid);
	viewMenu->Append(hUD);


	menu->Append(randomize);
	menu->Append(randomizeWithSeed);
	menu->Append(10009,"Settings");
	menu->Append(resetSettings);
	
	fileMenu->Append(newFile);
	fileMenu->Append(openFile);
	fileMenu->Append(importFile);
	fileMenu->Append(saveFile);
	fileMenu->Append(saveAs);
	fileMenu->Append(exit);

	menuBar->Append(menu,"Options");
	menuBar->Append(viewMenu, "View");
	menuBar->Append(fileMenu, "File");

	displayNeighbors->Check(settings.showNeighborCount);	
	grid->Check(settings.showGrid);
	thickGrid->Check(settings.showThickGrid);
	if (settings.showFinite == false && settings.showTorodial == false)
	{
		settings.showFinite = true;
	}
	finite->Check(settings.showFinite);
	torodial->Check(settings.showTorodial);
	hUD->Check(settings.showHUD);
	statusBar = CreateStatusBar();
	UpdateStatusText();
	timer = new wxTimer(this,10004);
	toolBar = CreateToolBar();
	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap trashIcon(trash_xpm);
	toolBar->AddTool(10000, "Play", playIcon);
	toolBar->AddTool(10001, "Pause", pauseIcon);
	toolBar->AddTool(10002, "Next", nextIcon);
	toolBar->AddTool(10003, "Trash", trashIcon);
	toolBar->Realize();
	sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(drawingPanel, 1, wxEXPAND | wxALL);
	this->SetSizer(sizer);
	InitializeGrid();
	this->Layout();
}
MainWindow::~MainWindow() 
{
	delete drawingPanel;
	delete timer;
	if (settingsWindow)
		delete settingsWindow;
}
void MainWindow::OnResize(wxSizeEvent& evt)
{
	wxSize size = GetSize();
	drawingPanel->SetSize(size);
	evt.Skip();
}
void MainWindow::OnPlay(wxCommandEvent& evt) {
	timer->Start(settings.timeInterval);
}
void MainWindow::OnPause(wxCommandEvent& evt) {
	timer->Stop();
}
void MainWindow::OnNext(wxCommandEvent& evt) 
{
	CalculateNextGenerationCells();
}
void MainWindow::OnTrash(wxCommandEvent& evt) {
	board.clear();
	board.resize(settings.gridSize, std::vector<bool>(settings.gridSize, false));
	neighborCount.clear();
	neighborCount.resize(settings.gridSize, std::vector<int>(settings.gridSize, 0));
	livingCells = 0;
	generation = 0;
	UpdateStatusText();
	drawingPanel->Refresh();
	timer->Stop();
}
void MainWindow::OnTimer(wxTimerEvent& evt) {
	CalculateNextGenerationCells();
}
void MainWindow::OnSettings(wxCommandEvent& evt) {
	if (settingsWindow != nullptr)
		delete settingsWindow;
	settingsWindow = new SettingsWindow(this, &settings);
	if (settingsWindow->ShowModal() == wxID_OK) {
		InitializeGrid();
		drawingPanel->Refresh();
	}
}
void MainWindow::OnDisplayNeighbors(wxCommandEvent& evt) {
	if (settings.showNeighborCount)
		settings.showNeighborCount = false;
	else
		settings.showNeighborCount = true;
	settings.SaveData();
	drawingPanel->Refresh();
	evt.Skip();
}
void MainWindow::OnDisplayFinite(wxCommandEvent& evt) {
	if (settings.showTorodial == true)
	{
		settings.showTorodial = false;
	}
	settings.showFinite = true;
	finite->Check(settings.showFinite);
	torodial->Check(settings.showTorodial);
	settings.SaveData();
	drawingPanel->Refresh();
}
void MainWindow::OnDisplayTorodial(wxCommandEvent& evt) {
	if (settings.showFinite == true)
	{
		settings.showFinite = false;
	}
	settings.showTorodial = true;
	finite->Check(settings.showFinite);
	torodial->Check(settings.showTorodial);
	settings.SaveData();
	drawingPanel->Refresh();
}
void MainWindow::OnDisplayGrid(wxCommandEvent& evt){
	if (settings.showGrid)
		settings.showGrid = false;
	else
		settings.showGrid = true;
	settings.SaveData();
	drawingPanel->Refresh();
}
void MainWindow::OnDisplayThickGrid(wxCommandEvent& evt){
	if (settings.showThickGrid)
		settings.showThickGrid = false;
	else
		settings.showThickGrid = true;
	settings.SaveData();
	drawingPanel->Refresh();
}
void MainWindow::OnDisplayHUD(wxCommandEvent& evt) {
	if (settings.showHUD)
		settings.showHUD = false;
	else
		settings.showHUD = true;
	settings.SaveData();
	drawingPanel->Refresh();
}
void MainWindow::OnRandomize(wxCommandEvent& evt){
	RandomizeGrid(time(NULL));
}
void MainWindow::OnRandomizeWithSeed(wxCommandEvent& evt) {
	long int seed = wxGetNumberFromUser("Enter an number to seed","","",0L,0L,LONG_MAX);
	if (seed != -1)
		RandomizeGrid(seed);
}
void MainWindow::RandomizeGrid(unsigned int seed) {
	srand(seed);
	for (int i = 0; i < settings.gridSize; i++)
	{
		for (int j = 0; j < settings.gridSize; j++) {
			int randomValue = rand();
			board[i][j] = (randomValue % 2) == 0;
		}
	}
	drawingPanel->Refresh();
}
void MainWindow::InitializeGrid() {
	neighborCount.clear();
	neighborCount.resize(settings.gridSize, std::vector<int>(settings.gridSize, 0));
	board.clear();
	board.resize(settings.gridSize, std::vector<bool>(settings.gridSize, false));
	drawingPanel->SetGridSize(settings.gridSize);
}
void MainWindow::UpdateStatusText()
{
	wxString text = "Generation: " + std::to_string(generation) + " Living cell count: " + std::to_string(livingCells);
	statusBar->SetStatusText(text);

}
int MainWindow::CalculateLivingNeighborCount(const int& row, const int& col) {
	int count = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;
			if (settings.showTorodial) {
				int tempRow = row + i;
				int tempCol = col + j;
				if (tempRow < 0) {
					tempRow = settings.gridSize - 1;
				}
				else if (tempRow >= settings.gridSize) {
					tempRow = 0;
				}
				if (tempCol < 0) {
					tempCol = settings.gridSize - 1;
				}
				else if (tempCol >= settings.gridSize) {
					tempCol = 0;
				}

				if (board[tempRow][tempCol]) {
					count++;
				}
			}
			else 
			{
				if ((row + i >= 0 && row + i < settings.gridSize && col + j >= 0 && col + j < settings.gridSize) && board[row + i][col + j])
				count++;
			}
		}
	}
	return count;
}
void MainWindow::CalculateNextGenerationCells()
{	
	sandBox.clear();
	neighborCount.clear();
	sandBox.resize(settings.gridSize, std::vector<bool>(settings.gridSize, false));
	neighborCount.resize(settings.gridSize, std::vector<int>(settings.gridSize, 0));

	livingCells = 0;
	for (int i = 0; i < settings.gridSize; i++)
	{
		for (int j = 0; j < settings.gridSize; j++) {
			int count = CalculateLivingNeighborCount(i,j);		
			if ((board[i][j] == true && (count == 3 || count == 2)) || (board[i][j] == false && count == 3))
			{			
				sandBox[i][j] = true;
				livingCells++;
			}
		}
	}
	board.swap(sandBox);
	for (int i = 0; i < settings.gridSize; i++)
	{
		for (int j = 0; j < settings.gridSize; j++) {
			int count = CalculateLivingNeighborCount(i, j);
			neighborCount[i][j] = count;
		}
	}
	generation++;
	UpdateStatusText();
	drawingPanel->Refresh();
}
void MainWindow::OnNew(wxCommandEvent& evt){
	currentFileName.Clear();
	OnTrash(evt);
}
void MainWindow::OnOpen(wxCommandEvent& evt) {
	wxFileDialog dialog(this, _("Open CELLS file"), wxEmptyString, wxEmptyString, "CELLS files (*.cells)|*.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_CANCEL) {
		return;
	}

		currentFileName = dialog.GetPath();
		OnTrash(evt);
		std::vector<std::vector<bool>> tempGrid(ReadGridFromFile());
		board.clear();
		board = tempGrid;
		drawingPanel->SetGridSize(board.size());
		drawingPanel->Refresh();
}
void MainWindow::OnImport(wxCommandEvent& evt) {
	wxFileDialog dialog(this, _("Open CELLS file"), wxEmptyString, wxEmptyString, "CELLS files (*.cells)|*.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_CANCEL) {
		return;
	}

	currentFileName = dialog.GetPath();
	OnTrash(evt);
	std::vector<std::vector<bool>> tempGrid(ReadGridFromFile());
	tempGrid.resize(settings.gridSize, std::vector<bool>(settings.gridSize, false));
	for (auto& row : tempGrid) {
		row.resize(settings.gridSize, false);
	}
	board.clear();
	board = tempGrid;
	drawingPanel->Refresh();
}
void MainWindow::OnSave(wxCommandEvent& evt) {
	if (currentFileName.IsEmpty()) {
		OnSaveAs(evt);
	}
	else {
		WriteGridToFile();
	}
}
void MainWindow::OnSaveAs(wxCommandEvent& evt) {
	wxFileDialog
		dialog(this, _("Save CELLS file"), wxEmptyString, wxEmptyString, "CELLS files (*.cells)|*.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (dialog.ShowModal() == wxID_CANCEL) {
		return;
	}	
	
		currentFileName = dialog.GetPath();
		WriteGridToFile();
	
}
void MainWindow::OnExitApp(wxCommandEvent& evt) {
	this->Close();
}
void MainWindow::OnResetSettings(wxCommandEvent& evt) {
	settings.SetDeadColor(*wxBLUE);
	settings.SetLivingColor(*wxGREEN);
	settings.gridSize = 15;
	settings.timeInterval = 50;
	displayNeighbors->Check(false);
	grid->Check(true);
	thickGrid->Check(false);
	finite->Check(true);
	torodial->Check(false);
	hUD->Check(false);
	settings.showNeighborCount = false;
	settings.showFinite = true;
	settings.showTorodial = false;
	settings.showGrid = true;
	settings.showThickGrid = false;
	settings.showHUD = false;
	settings.SaveData();
	OnTrash(evt);
}
void MainWindow::WriteGridToFile() {
	wxTextFile file(currentFileName);

	if (!file.Exists())
		file.Create();
	else file.Open();

	file.Clear();

	for (const auto& row : board)
	{
		wxString line;
		for (bool cell : row)
		{
			if (cell) {
				line += '*';
			}
			else {
				line += '.';
			}
		}
		file.AddLine(line);
	}
	file.Write();
	file.Close();
}
std::vector<std::vector<bool>> MainWindow::ReadGridFromFile() {
	wxTextFile file(currentFileName);
	std::vector<std::vector<bool>> tempBoard;

	file.Open();
	wxString line;

	for (line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine()) {
		std::vector<bool> row;
		for (wxChar letter : line) {
			if (letter == '*')
				row.push_back(true);
			else if (letter == '.')
				row.push_back(false);
		}
		tempBoard.push_back(row);
	}

	file.Close();
	return tempBoard;
}
unsigned int MainWindow::generation = 0;
unsigned int MainWindow::livingCells = 0;